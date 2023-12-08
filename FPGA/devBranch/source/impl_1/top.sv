module top(input logic rst, msck,  mCS, msdi,
           output logic sck, sdo, CS, LDAC,
		   output logic [7:0] volVal
);

	logic clk, start, spiClk, mcuDone;
	logic [15:0] recVal, capturedVal;
	logic [7:0] voltageVal;
	logic [3:0] note, waveCar, waveMod;
	logic [5:0] fmBeta, fmOffset, filterSet, unused;
	
	
	timingCtrl clkGen(rst, clk, spiClk);
	spiMcu readMCU(rst, msck, mCS, msdi, recVal, mcuDone);
	memory setMem(clk, rst, capturedVal, note, waveMod, waveCar , fmBeta, fmOffset, filterSet, unused);
	waveFetcherFM voltageGetter(clk, rst, note, waveMod, waveCar,  fmBeta, fmOffset, start, voltageVal);
	spiDac toSpeak(rst, clk, spiClk, start, sck, sdo, CS, LDAC, voltageVal);
	
	// spi capture reg, should this be in another block? 
	 always_ff @(posedge clk, posedge rst)begin
        if (rst) begin
			capturedVal <=0;
			end
		else if (mcuDone) begin
			capturedVal <= recVal;
			end
		end
	
	//assign testNote = 4'b0010;

	assign volVal = voltageVal; // for simulation purposes
	//assign rst = 0;
endmodule

module timingCtrl(input logic reset,
					output logic clk , spiClk);
		logic [31:0] counter;
	// Internal high-speed oscillator @12MHz
   HSOSC #(.CLKHF_DIV(2'b10)) 
         hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(clk));
		 
		 // Simple clock divider for lower clock freqs, unused.
	always_ff @(posedge clk, posedge reset)
		begin
			if (reset) counter<=0;
			else counter <= counter + 1;
		end
		
		assign spiClk = counter[1];
endmodule