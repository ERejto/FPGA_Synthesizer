/////////////////////////////////////////////////////////////////////////////////
// Kevin Box
// kbox@hmc.edu
// november 5 2023
// This file contains tesbenches for the final project for MicroPs
//////////////////////////////////////////////////////////////////////////////////



`timescale 10us/10us
`default_nettype none



// test bench for the top module
module waveFetcher_tb();
 // Set up test signals
 logic clk, rst;
 logic [7:0] note, voltageVal;
 


 // Instantiate the device under test
 waveFetcher dut(clk, rst, note, voltageVal);

 // Generate clock signal with a period of 10 timesteps.
 always
   begin
     clk = 0; #10;
     clk = 1; #10;
   end
  
 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line (if applicable)
 initial
   begin
	 note = 0; //c4
     rst = 1; #27; rst = 0;
   end
initial
 begin
   // Create dumpfile for signals
   $dumpfile("waveFetcher_tb.vcd");
   $dumpvars(0, waveFetcher_tb);
 end

endmodule	

