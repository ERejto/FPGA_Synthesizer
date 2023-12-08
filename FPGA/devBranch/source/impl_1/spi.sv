
module spiMcu(input  logic rst,
		   input  logic sck, CS, 
           input  logic sdi,
		   output logic [15:0] recVal,
           output logic done);

    logic       loadCtrl;
	logic [3:0] shiftCount, nextShiftCount;
               
    // when sclk goes shift in 16 bit value
    always_ff @(posedge sck, posedge rst)begin
        if (rst) begin
			recVal <=0;
			shiftCount <= 0;
			end
		else if (CS) begin
			recVal <= {recVal[14:0], sdi};
			shiftCount <= nextShiftCount;
			end
		end
    
    // keep track of shifting for 16 bit 
	always_comb 
	if (shiftCount==16) nextShiftCount = 0;
	else nextShiftCount = shiftCount + 1;
    assign done = ~CS; // 
	
endmodule

module spiDac(input  logic rst,
			   input  logic clk, spiClk, start, 
               output logic sck, sdo, CS, LDAC, // drive ldac low for 100ns 2 cycs
			   input  logic [7:0] voltageVal);
			   
	logic       loadCtrl, running, done;
	logic [3:0] state, nextState; // 0 wait, 1 2 3 4 5 save, 6 running
    logic [15:0] vShiftOut, nextVShiftOut, val2Send;
	logic [4:0] shiftCount, nextShiftCount;
	
	assign val2Send = {4'b0011, 4'b0000, voltageVal}; // opA, 2x gain, active mode, upper 8 bits
	
	// stateReg
	always_ff @(posedge clk, posedge rst) begin
		if (rst)
			state <= 0;
		else
			state <= nextState;
    end
	
	// running couter
	always_ff @(posedge clk, posedge rst) begin
		if (rst) 
			shiftCount <= 0;
		else
			shiftCount <= nextShiftCount;
    end
	
	// shift reg
	always_ff @(negedge clk, posedge rst) begin
		if (rst)
			vShiftOut <= 0;
		else
			vShiftOut <= nextVShiftOut; 
			
    end
	
	//next state logic
	always_comb
	case (state)
		0 :  if (start) nextState = 1;
			else nextState = 0;
		1: nextState = 2;
		2: nextState = 3;
		3: nextState = 4;
		4: nextState = 5;
		5: nextState = 6; // need to hold for 5 cycles? to ensure val2send gets saved into shift register, dependent on clock frequencies
		6: if (done) nextState = 7;
		   else nextState = 6;
		7: nextState = 8;
		8: nextState = 9; // LDAC low
		9: nextState = 0; // LDAC low
		default: nextState = 0;
	endcase
	
	// shift reg logic
	always_comb
	case (state)
		0 : nextVShiftOut = 0;
		1:  nextVShiftOut = val2Send;
		2:  nextVShiftOut = val2Send;
		3:  nextVShiftOut = val2Send;
		4:  nextVShiftOut = val2Send;
		5:  nextVShiftOut = val2Send;
		3:  nextVShiftOut = val2Send;
		6: nextVShiftOut = {vShiftOut[14:0], 1'b0};
		7: nextVShiftOut = 0;
		8: nextVShiftOut = 0; 
		9: nextVShiftOut = 0;
		default: nextVShiftOut = 0;
	endcase
	
	// assigns
	// counter logic
	assign nextShiftCount = (state != 6) ?  0: shiftCount + 1;
	// chip select 
	assign CS = ~(state == 6 || state == 5);
	// sdo
	assign sdo = vShiftOut[15];
	// sck
	assign sck = (state == 6) ? clk : 0;
	// done
	assign done = (shiftCount == 15);
	//LDAC
	assign LDAC = ~(state == 8 || state == 9);
	
	
	
endmodule




