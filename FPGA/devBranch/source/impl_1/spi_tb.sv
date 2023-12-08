/////////////////////////////////////////////////////////////////////////////////
// Kevin Box
// kbox@hmc.edu
// november 5 2023
// This file contains tesbenches for the final project for MicroPs
//////////////////////////////////////////////////////////////////////////////////

// test bench for the spi module
module spiMcu_tb();
 // Set up test signals
 logic sck, rst, sdi, CS;
 logic [16:0] recVal, sendVal, i, j;
 
 


 // Instantiate the device under test
 spiMcu dut(rst, sck, CS, sdi, recVal, done);

  
 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line (if applicable)
 initial
   begin
	 sendVal = 0;
     rst = 1; #27; rst = 0;
	 i = 0; j=0; CS = 0;
   end
 always begin
   if (j==0) begin
		#100; j = 1;
	end
	if (i == 16) begin
		CS = 0; #100; 
		i=0; 
		sendVal = sendVal + 1;
		end
	CS = 1;
	#1; sdi = sendVal[15-i];
	sck = 1; 
    #1;
    #3; sck = 0; #4
    i = i + 1;
end
initial
 begin
   // Create dumpfile for signals
   $dumpfile("spiMcu_tb.vcd");
   $dumpvars(0, spiMcu_tb);
 end

endmodule	



// test bench for the spi module
module spiDac_tb();
 // Set up test signals
 logic clk, rst, sdo, CS;
 logic [11:0] voltageVal;
 


 // Instantiate the device under test
 spiDac dut(rst, clk, sdo, CS, voltageVal);

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
	 voltageVal = 0;
     rst = 1; #27; rst = 0;
	 #1000; voltageVal = 1;
	 #1000; voltageVal = 5;
	 #1000; voltageVal = 200;
	 #1000; voltageVal = 3000;
   end
initial
 begin
   // Create dumpfile for signals
   $dumpfile("spiDac_tb.vcd");
   $dumpvars(0, spiDac_tb);
 end

endmodule	

