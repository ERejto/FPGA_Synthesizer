module waveFetcherFM(input logic clk, rst,
				   input logic [3:0] note, waveTypeMod, waveTypeCar,
				   input logic [5:0] fmBeta, fmOffset,
				   output logic start,
				   output logic [7:0] voltageVal);
	logic [7:0] carIndex, modIndex, modVol;
	modulationGeneratorFM modGen(clk, rst, fmOffset, modIndex);
	waveTable modWaves(waveTypeMod, modIndex, modVol);
	phaseAccumulatorFM accumPhase(clk, rst, note, fmBeta, modVol, carIndex, start);
	waveTable waves(waveTypeCar, carIndex, voltageVal);
endmodule


module phaseAccumulatorFM(input logic clk, rst,
					input logic [3:0] note,
					input logic [5:0] fmBeta,
				    input logic [7:0] modVal,
				    output logic [7:0] index,
				    output logic start
				  );
	// fetch wave according to given note at given frequency
	// this is the phase accumulator
	// base clock freq is 
	typedef enum logic [3:0] {off = 0, c4=1, cS4=2, d4=3, dS4=4, e4=5, f4=6, fS4=7, g4=8, gS4=9, a4=10, aS4=11, b4=12} noteType;
	noteType noteIn;
	logic[5:0] beta;
	logic [31:0] accum, toAccum, nextAccum;
	logic [23:0] addVal;
	logic [8:0] betaShifted;
	assign noteIn = noteType'(note);
	assign beta = |note ? fmBeta : 0; // turns off modulation by mul by zero with beta, not robust but should do. Can also do this on MCU side
	
	
// MACs for calculating the correct phase accum value with FM
// TODO: make file with different mac types instantiated
assign toAccum = accum + addVal;
assign betaShifted = beta<<3; // this should be max value of 504 and min of 0 beta = 38 or 37 gives around 300 which is what the matlab uses


mac16x16x32FM fmMac(clk, rst, {8'b00000000, modVal}, {7'b0000000, betaShifted}, toAccum, nextAccum);
	
	
	//assign nextAccum = accum+addVal+betaMulMod; // ToDo put these in a MAC
	// accum register
	always_ff @(posedge clk, posedge rst)
		if (rst) accum <= 0;
		else       accum <= nextAccum;
		
	// depending on note choose phase accumulator value	
	always_comb
	case (noteIn)
		off: addVal = 0;
		c4: addVal = 280923;
		cS4: addVal = 297620;
		d4:addVal = 315315;
		dS4:addVal = 334073;
		e4:addVal = 353938;
		f4:addVal = 374983;
		fS4:addVal = 397274;
		g4:addVal = 420907;
		gS4:addVal = 445925;
		a4:addVal = 472446;
		aS4:addVal = 500535;
		b4: addVal = 530300;
		default: addVal = 1073742; // about 1000 Hz much higher pitch should tell if note is wrong
	endcase
	
	assign index = accum[31:24];
	assign start = !(accum[31:24] == nextAccum[31:24]);
endmodule


module modulationGeneratorFM(input logic clk, rst,
							input logic [5:0] fmOffset,
							output logic [7:0] index);
	logic [31:0] accum, nextAccum;
	logic [31:0] addVal;
	
	assign addVal = 10737 + {fmOffset, 10'b0000000000}; // 10-100 w/1.43hz steps Hz modulation fmOffset = 52 is about 50Hz, this is reset value... 
	// accum register
	always_ff @(posedge clk, posedge rst)
		if (rst) accum <= 0;
		else       accum <= nextAccum;
	
	assign nextAccum = accum+addVal;
	assign index = accum[31:24];
endmodule



module mac16x16x32FM( input logic clk, rst,
			input logic [15:0] mulA, mulB,
		   input logic [31:0] sum,
		   output logic [31:0] res);
		
		
		logic [31:0] mulRes;

		multMAC multiply(.clk_i(clk ),
        .clk_en_i(1'b1 ),
        .rst_i(rst),
        .data_a_i(mulA),
        .data_b_i(mulB),
        .result_o(mulRes));
		
		adderMac add(.clk_en_i(1'b1),
        .clk_i(clk),
        .rst_i(rst),
        .data_a_re_i(mulRes),
        .data_b_re_i(sum),
        .result_re_o(res));

endmodule



	
