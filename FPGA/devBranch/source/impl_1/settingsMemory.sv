module memory(input logic clk, rst,
			  input logic [15:0] dataIn,
			  output logic [3:0] note, mod, car,
			  output logic [5:0] fmBeta, fmOffset, filterSet, unused);
			  
		logic en0, en1, en2;
		// Value Registers
		always_ff @(posedge clk, posedge rst) begin
		if (rst) note <= 0;
		else if (en0) note <= dataIn[11:8]; 	
		end
		
		always_ff @(posedge clk, posedge rst) begin
		if (rst) car <= 0;
		else if (en0) car <= dataIn[7:4]; 	
		end
		
		always_ff @(posedge clk, posedge rst) begin
		if (rst) mod <= 0;
		else if (en0) mod <= dataIn[3:0]; 	
		end
		
		always_ff @(posedge clk, posedge rst) begin
		if (rst) fmBeta <= 0;
		else if (en1) fmBeta <= dataIn[11:6]; 	
		end
		always_ff @(posedge clk, posedge rst) begin
		if (rst) fmOffset <= 6'b100111; // offset of 39 to produce about 50Hz modulation at reset
		else if (en1) fmOffset <= dataIn[5:0]; 	
		end
		
		always_ff @(posedge clk, posedge rst) begin
		if (rst) filterSet <= 0;
		else if (en2) filterSet <= dataIn[11:6]; 	
		end
		
		always_ff @(posedge clk, posedge rst) begin
		if (rst) unused <= 0;
		else if (en2) unused <= dataIn[5:0]; 	
		end
		
		
		
		
		// reg enabling 1 hot?
		assign en0 = dataIn[15:12] == 4'b0000;
		assign en1 = dataIn[15:12] == 4'b0001;
		assign en2 = dataIn[15:12] == 4'b0010;
		
endmodule