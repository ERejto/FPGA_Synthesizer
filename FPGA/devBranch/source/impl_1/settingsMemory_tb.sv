// november 5 2023
// This file contains tesbenches for the final project for MicroPs
//////////////////////////////////////////////////////////////////////////////////

// test bench for the spi module
module memory_tb();
 // Set up test signals
logic clk, rst;
logic [15:0] dataIn, i, j;
logic [3:0] note, car, mod;
logic [5:0] fmBeta, sustain, attack, decay, cornerFreq, bandwidth;
 
 


 // Instantiate the device under test
 memory dut(clk, rst, dataIn, note, car, mod, fmBeta, sustain, attack, decay, cornerFreq, bandwidth);

  
 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line (if applicable)
 initial
   begin
	 
     rst = 1; #27; rst = 0;
	 dataIn = 0; i = 0; j = 0;
 end
 always begin
clk = 0; #5;
clk = 1; #5;
end 

  
 always begin
   if (j==0) begin
		#100; j = 1;
	end
	dataIn = 16'b0000_0000_0000_0000;
	dataIn = dataIn + i;
	#100;
	dataIn = 16'b0001_0000_0100_0000;
	dataIn = dataIn + i;
	#100;
	dataIn = 16'b0010_0100_0000_0000;
	dataIn = dataIn + i;
	#100;
	dataIn = 16'b0011_0001_0100_0100;
	dataIn = dataIn + i;
	#100
    i = i + 1;
end
initial
 begin
   // Create dumpfile for signals
   $dumpfile("memory_tb.vcd");
   $dumpvars(0, memory_tb);
 end

endmodule	