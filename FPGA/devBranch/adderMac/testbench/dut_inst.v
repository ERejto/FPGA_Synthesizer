    adderMac u_adderMac(.clk_en_i(clk_en_i),
        .clk_i(clk_i),
        .rst_i(rst_i),
        .data_a_re_i(data_a_re_i),
        .data_b_re_i(data_b_re_i),
        .result_re_o(result_re_o));
