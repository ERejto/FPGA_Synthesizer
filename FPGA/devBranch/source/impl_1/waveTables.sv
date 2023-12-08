module waveTable(input logic [3:0] wave,
				input  logic [7:0] a,
				output logic [7:0] y);
            
  
  typedef enum logic [1:0] {sine = 0, saw=1, sqr=2, triangle=3} waveType;
  logic [7:0] sinOut, sawOut, sqrOut, triOut;
  waveType waveIn;
  assign waveIn  = waveType'(wave[3:2]); // top 2 bits of 6 bit pot value
  
  waveSin sinTable(a, sinOut);
  waveSaw sawTable(a, sawOut);
  waveSqr sqrTable(a, sqrOut);
  waveTri triTable(a, triOut);
  
  always_comb 
  case (waveIn)
	  sine: y = sinOut;
	  saw: y = sawOut;
	  sqr: y = sqrOut;
	  triangle: y = triOut;
	  default: y = sinOut;
  endcase
  
  
	
  
  
endmodule

module waveSin(input  logic [7:0] a,
				output logic [7:0] y);
            
  // wavetable implemented as a ROM
  // This module is combinational and will be inferred using LUTs (logic cells)
  logic [7:0] wave[0:255];

  initial $readmemh("waveSin8.txt", wave);
  assign y = wave[a];
  
endmodule

module waveSaw(input  logic [7:0] a,
				output logic [7:0] y);
            
  // wavetable implemented as a ROM
  // This module is combinational and will be inferred using LUTs (logic cells)
  logic [7:0] wave[0:255];

  initial $readmemh("waveSaw8.txt", wave);
  assign y = wave[a];
  
endmodule

module waveSqr(input  logic [7:0] a,
				output logic [7:0] y);
            
  // wavetable implemented as a ROM
  // This module is combinational and will be inferred using LUTs (logic cells)
  logic [7:0] wave[0:255];

  initial $readmemh("waveSqr8.txt", wave);
  assign y = wave[a];
  
endmodule

module waveTri(input  logic [7:0] a,
				output logic [7:0] y);
            
  // wavetable implemented as a ROM
  // This module is combinational and will be inferred using LUTs (logic cells)
  logic [7:0] wave[0:255];

  initial $readmemh("waveTri8.txt", wave);
  assign y = wave[a];
  
endmodule