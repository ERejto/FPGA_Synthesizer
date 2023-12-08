module waveFetcher(input logic clk, rst,
				   input logic [3:0] note,
				   input logic [5:0] fmBeta,
				   output logic start,
				   output logic [7:0] voltageVal);
	logic [7:0] carIndex, modIndex, modVol;
	//modulationGenerator modGen(clk, rst, modIndex);
	//waveTable modWaves(modIndex, modVol);
	phaseAccumulator accumPhase(clk, rst, note, fmBeta, modVol, carIndex, start);
	waveTable waves(carIndex, voltageVal);
endmodule


module phaseAccumulator(input logic clk, rst,
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
assign nextAccum = accum + addVal;
//assign betaShifted = beta<<3; // this should be max value of 504 and min of 0 beta = 38 or 37 gives around 300 which is what the matlab uses



	
	
	//assign nextAccum = accum+addVal+betaMulMod; // ToDo put these in a MAC
	// accum register
	always_ff @(posedge clk, posedge rst)
		if (rst) accum <= 0;
		else       accum <= nextAccum;
		
	// depending on note choose phase accumulator value	
	always_comb
	case (noteIn)
		off: addVal = 50000;
		c4: addVal = 46821;
		cS4: addVal = 49603;
		d4:addVal = 52553;
		dS4:addVal = 55673;
		e4:addVal = 58990;
		f4:addVal = 62497;
		fS4:addVal = 66212;
		g4:addVal = 70151;
		gS4:addVal = 74321;
		a4:addVal = 78741;
		aS4:addVal = 83423;
		b4: addVal = 88383;
		default: addVal = 178957; // about 1000 Hz much higher pitch should tell if note in is wrong
	endcase
	
	assign index = accum[31:24];
	assign start = !(accum[31:24] == nextAccum[31:24]);
endmodule

/*
module modulationGenerator(input logic clk, rst,
							output logic [7:0] index);
	logic [31:0] accum, nextAccum;
	logic [23:0] addVal;
	
	assign addVal = 6711; // 50 Hz modulation
	// accum register
	always_ff @(posedge clk, posedge rst)
		if (rst) accum <= 0;
		else       accum <= nextAccum;
	
	assign nextAccum = accum+addVal;
	assign index = accum[31:24];
endmodule
*/

module waveTable(input  logic [7:0] a,
				output logic [7:0] y);
            
  // wavetable implemented as a ROM
  // This module is combinational and will be inferred using LUTs (logic cells)
  logic [7:0] wave[0:255];

  initial $readmemh("wave.txt", wave);
  assign y = wave[a];
  
endmodule

/*
/////////////////////////////////////////////
// sbox
//   Infamous AES byte substitutions with magic numbers
//   Synchronous version which is mapped to embedded block RAMs (EBR)
//   Section 5.1.1, Figure 7
/////////////////////////////////////////////
module waveTable(
	input		logic [7:0] a,
	input	 	logic 			clk,
	output 	logic [7:0] y);
            
  // sbox implemented as a ROM
  // This module is synchronous and will be inferred using BRAMs (Block RAMs)
  logic [7:0] sbox [0:255];

  initial   $readmemh("wave.txt", sbox);
	
	// Synchronous version
	always_ff @(posedge clk) begin
		y <= sbox[a];
	end
endmodule
*/

/*
module mac16x16x32( input logic clk, rst,
			input logic [15:0] mulA, mulB,
		   input logic [31:0] sum,
		   output logic [31:0] res);
		
		
		logic [31:0] mulRes;
		
		`ifdef VERILATOR
        wire [32:0] ab = a * b;
        assign prod = ab + c;
		`else
		SB_MAC16 #(
                .TOPOUTPUT_SELECT(2'b00), // adder, unregistered
                .TOPADDSUB_LOWERINPUT(2'b10), // multiplier hi bits
                .TOPADDSUB_UPPERINPUT(1'b1), // input C
                .TOPADDSUB_CARRYSELECT(2'b11), // top carry in is bottom carry out
                .BOTOUTPUT_SELECT(2'b00), // adder, unregistered
                .BOTADDSUB_LOWERINPUT(2'b10), // multiplier lo bits
                .BOTADDSUB_UPPERINPUT(1'b1), // input D
                .BOTADDSUB_CARRYSELECT(2'b00) // bottom carry in constant 0
        ) mult16x16_add(
                .CLK(clk),
                .A(mulA),
                .B(mulB),
                .C(sum[31:16]),
                .D(sum[15:0]),
                .O(res)
        );
		`endif
/*
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
*/
/*
		pmi_dsp
#(
  .NEG_TRIGGER               (1'b0),                   
  .A_REG                     (1'b0),                   
  .B_REG                     (1'b0),                   
  .C_REG                     (1'b1),                    
  .D_REG                     (1'b1),                   
  .TOP_8x8_MULT_REG          (1'b0),                 
  .BOT_8x8_MULT_REG          (1'b0),                    
  .PIPELINE_16x16_MULT_REG1  (1'b0),                   
  .PIPELINE_16x16_MULT_REG2  (1'b1),                  
  .TOPOUTPUT_SELECT          (2'b00), 
  .TOPADDSUB_LOWERINPUT      (2'b10), 
  .TOPADDSUB_UPPERINPUT      (1'b1),                 
  .TOPADDSUB_CARRYSELECT     (2'b00), 
  .BOTOUTPUT_SELECT          (2'b00), 
  .BOTADDSUB_LOWERINPUT      (2'b10), 
  .BOTADDSUB_UPPERINPUT      (1'b1),                 
  .BOTADDSUB_CARRYSELECT     (2'b00), 
  .MODE_8x8                  (1'b0),                  
  .A_SIGNED                  (1'b0),                    
  .B_SIGNED                  (1'b0)                    
) MAC(
  .CLK        (clk), // I:
  .CE         (1'b1), // I:
  .C          (sum[31:16]), // I:
  .A          (mulA), // I:
  .B          (mulB), // I:
  .D          (sum[15:0]), // I:
  .AHOLD      (1'b0), // I:
  .BHOLD      (1'b0), // I:
  .CHOLD      (1'b0), // I:
  .DHOLD      (1'b0), // I:
  .IRSTTOP    (rst), // I:
  .IRSTBOT    (rst), // I:
  .ORSTTOP    (1'b0), // I:
  .ORSTBOT    (1'b0), // I:
  .OLOADTOP   (1'b0), // I:
  .OLOADBOT   (1'b0), // I:
  .ADDSUBTOP  (1'b0), // I:
  .ADDSUBBOT  (1'b0), // I:
  .OHOLDTOP   (1'b0), // I:
  .OHOLDBOT   (1'b0), // I:
  .CI         (1'b0), // I:
  .ACCUMCI    (1'b0), // I:
  .SIGNEXTIN  (1'b0), // I:
  .O          (res), // O:
  .CO         (CO), // O:
  .ACCUMCO    (ACO), // O:
  .SIGNEXTOUT (SNO)  // O:
);
*/	
// need an if def for this if sim then SB_MAC16 else pmi_dsp	
//endmodule

/*
SB_MAC16 fmLowerMac
(
	.A(mulA),
	.B(mulB),
	.C(sum[31:16]),
	.D(sum[15:0]),
	.O(res),
	.CLK(SYSCLK_i),
	.CE(CE_i),
	.IRSTTOP(RST_i),
	.IRSTBOT(RST_i),
	.ORSTTOP(RST_i),
	.ORSTBOT(RST_i),
	.AHOLD(AHOLD_i),
	.BHOLD(BHOLD_i),
	.CHOLD(CHOLD_i),
	.DHOLD(DHOLD_i),
	.OHOLDTOP(HLDTOPOUT_i),
	.OHOLDBOT(HLDBOTOUT_i),
	.OLOADTOP(LOADTOP_i),
	.OLOADBOT(LOADBOT_i),
	.ADDSUBTOP(ADDSUBTOP_i),
	.ADDSUBBOT(ADDSUBBOT_i),
	.CO(CO),
	.CI(CI),
	//MAC cascading ports.
	.ACCUMCI(1'b0),
	.ACCUMCO(ACO),
	.SIGNEXTIN(1'b0),
	.SIGNEXTOUT(SNO)
);

defparam fmLowerMac. B_SIGNED = 1'b0 ;
defparam fmLowerMac. A_SIGNED = 1'b0 ;
defparam fmLowerMac. MODE_8x8 = 1'b0 ;
defparam fmLowerMac. BOTADDSUB_CARRYSELECT = 2'b00;
defparam fmLowerMac. BOTADDSUB_UPPERINPUT = 1'b0;
defparam fmLowerMac. BOTADDSUB_LOWERINPUT = 2'b10;
defparam fmLowerMac. BOTOUTPUT_SELECT = 2'b01;
defparam fmLowerMac. TOPADDSUB_CARRYSELECT = 2'b10;
defparam fmLowerMac. TOPADDSUB_UPPERINPUT = 1'b0;
defparam fmLowerMac. TOPADDSUB_LOWERINPUT = 2'b10;
defparam fmLowerMac. TOPOUTPUT_SELECT = 2'b01;
defparam fmLowerMac. PIPELINE_16x16_MULT_REG2 = 1'b0;
defparam fmLowerMac. PIPELINE_16x16_MULT_REG1 = 1'b0;
defparam fmLowerMac. BOT_8x8_MULT_REG = 1'b0;
defparam fmLowerMac. TOP_8x8_MULT_REG = 1'b0 ;
defparam fmLowerMac. D_REG = 1'b0 ;
defparam fmLowerMac. B_REG = 1'b0 ;
defparam fmLowerMac. A_REG = 1'b0 ;
defparam fmLowerMac. C_REG = 1'b0 ;

SB_MAC16
#(
  .NEG_TRIGGER               (1'b0),                   
  .A_REG                     (1'b0),                   
  .B_REG                     (1'b0),                   
  .C_REG                     (1'b0),                    
  .D_REG                     (1'b0),                   
  .TOP_8x8_MULT_REG          (1'b0),                 
  .BOT_8x8_MULT_REG          (1'b0),                    
  .PIPELINE_16x16_MULT_REG1  (1'b0),                   
  .PIPELINE_16x16_MULT_REG2  (1'b0),                  
  .TOPOUTPUT_SELECT          (2'b01), 
  .TOPADDSUB_LOWERINPUT      (2'b10), 
  .TOPADDSUB_UPPERINPUT      (1'b0),                 
  .TOPADDSUB_CARRYSELECT     (2'b10), 
  .BOTOUTPUT_SELECT          (2'b01), 
  .BOTADDSUB_LOWERINPUT      (2'b10), 
  .BOTADDSUB_UPPERINPUT      (1'b0),                 
  .BOTADDSUB_CARRYSELECT     (2'b00), 
  .MODE_8x8                  (1'b0),                  
  .A_SIGNED                  (1'b0),                    
  .B_SIGNED                  (1'b0)                    
) MAC(
  .CLK        (clk), // I:
  .CE         (1'b1), // I:
  .C          (sum[31:16]), // I:
  .A          (mulA), // I:
  .B          (mulB), // I:
  .D          (sum[15:0]), // I:
  .AHOLD      (1'b0), // I:
  .BHOLD      (1'b0), // I:
  .CHOLD      (1'b0), // I:
  .DHOLD      (1'b0), // I:
  .IRSTTOP    (1'b0), // I:
  .IRSTBOT    (1'b0), // I:
  .ORSTTOP    (1'b0), // I:
  .ORSTBOT    (1'b0), // I:
  .OLOADTOP   (1'b0), // I:
  .OLOADBOT   (1'b0), // I:
  .ADDSUBTOP  (1'b0), // I:
  .ADDSUBBOT  (1'b0), // I:
  .OHOLDTOP   (1'b0), // I:
  .OHOLDBOT   (1'b0), // I:
  .CI         (1'b0), // I:
  .ACCUMCI    (1'b0), // I:
  .SIGNEXTIN  (1'b0), // I:
  .O          (res), // O:
  .CO         (CO), // O:
  .ACCUMCO    (ACO), // O:
  .SIGNEXTOUT (SNO)  // O:
);
*/

	
