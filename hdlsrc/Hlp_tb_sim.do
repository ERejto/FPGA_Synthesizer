onbreak resume
onerror resume
vsim -voptargs=+acc work.Hlp_tb
add wave sim:/Hlp_tb/u_firGeneralFilter/clk
add wave sim:/Hlp_tb/u_firGeneralFilter/clk_enable
add wave sim:/Hlp_tb/u_firGeneralFilter/reset
add wave sim:/Hlp_tb/u_firGeneralFilter/filter_in
add wave sim:/Hlp_tb/u_firGeneralFilter/filter_out
add wave sim:/Hlp_tb/filter_out_ref
run -all
