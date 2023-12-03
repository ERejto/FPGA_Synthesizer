onbreak resume
onerror resume
vsim -voptargs=+acc work.firHighFreq_tb
add wave sim:/firHighFreq_tb/u_firMidFreq/clk
add wave sim:/firHighFreq_tb/u_firMidFreq/clk_enable
add wave sim:/firHighFreq_tb/u_firMidFreq/reset
add wave sim:/firHighFreq_tb/u_firMidFreq/filter_in
add wave sim:/firHighFreq_tb/u_firMidFreq/filter_out
add wave sim:/firHighFreq_tb/filter_out_ref
run -all
