`timescale 10ns/1ns

module top_tb();
    logic rst, msck,  mCS, msdi, sck, sdi, sdo, LDAC;
	logic [7:0] note, i, j;
	logic [7:0] volVal, savedVol;
	logic [15:0] sendVal;
    // device under test
    top dut(rst, msck,  mCS, msdi, sck, sdo, CS, LDAC, volVal);
    

	
	
 // Create dumpfile
	initial begin
     $dumpfile("top_tb.vcd");
     $dumpvars(0, top_tb);
   end



//pulse reset initialize saving
	initial begin
	 rst = 1; #27; rst = 0;
	 sendVal = 0'b0000_0010_1001_1000;
	 i=0;
	 j=0;
	 savedVol = 0;
   end
   
 
always begin
   if (j==0) begin
		#100; j = 1;
	end
	if (i == 16) begin
		msck = 0; #100; mCS = 0; #1000000; 
		i=0; 
		sendVal = sendVal + 1;
		end
	mCS = 1;
	#1; msdi = sendVal[15-i];
	msck = 0; 
    #1;
    #3; msck = 1; #4
    i = i + 1;
end
   
initial begin
	integer saveFile;
	saveFile = $fopen("output_vol.txt","w");
	forever begin
		$fwrite(saveFile,"%h, ",volVal);
		#50;
		end
end

endmodule