component adderMac is
    port(
        clk_en_i: in std_logic;
        clk_i: in std_logic;
        rst_i: in std_logic;
        data_a_re_i: in std_logic_vector(31 downto 0);
        data_b_re_i: in std_logic_vector(31 downto 0);
        result_re_o: out std_logic_vector(31 downto 0)
    );
end component;

__: adderMac port map(
    clk_en_i=>,
    clk_i=>,
    rst_i=>,
    data_a_re_i=>,
    data_b_re_i=>,
    result_re_o=>
);
