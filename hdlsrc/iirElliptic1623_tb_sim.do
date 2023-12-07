onbreak resume
onerror resume
vsim -voptargs=+acc work.iirElliptic1623_tb
add wave sim:/iirElliptic1623_tb/u_iirElliptic1623/clk
add wave sim:/iirElliptic1623_tb/u_iirElliptic1623/clk_enable
add wave sim:/iirElliptic1623_tb/u_iirElliptic1623/reset
add wave sim:/iirElliptic1623_tb/u_iirElliptic1623/filter_in
add wave sim:/iirElliptic1623_tb/u_iirElliptic1623/filter_out
add wave sim:/iirElliptic1623_tb/filter_out_ref
run -all
