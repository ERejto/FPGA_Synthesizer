onbreak resume
onerror resume
vsim -voptargs=+acc work.Hlp2_tb
add wave sim:/Hlp2_tb/u_lowPassFir/clk
add wave sim:/Hlp2_tb/u_lowPassFir/clk_enable
add wave sim:/Hlp2_tb/u_lowPassFir/reset
add wave sim:/Hlp2_tb/u_lowPassFir/filter_in
add wave sim:/Hlp2_tb/u_lowPassFir/filter_out
add wave sim:/Hlp2_tb/filter_out_ref
run -all
