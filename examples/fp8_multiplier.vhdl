library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- A naive FP8 multiplier with:
--  * 4 exponent bits
--  * 3 mantissa bits
--  * Round to nearest, ties to odd

entity fp8_multiplier is
    port ( clk  : in std_logic := '0';
           x_in : in std_logic_vector(7 downto 0) := (others => '0');
           y_in : in std_logic_vector(7 downto 0) := (others => '0');
           z_out : out std_logic_vector(7 downto 0) := (others => '0'));
end fp8_multiplier;

architecture arch of fp8_multiplier is
    -- Input signal x
    signal x : std_logic_vector(7 downto 0) := (others => '0');
    alias x_sign : std_logic is x(7);
    alias x_exp_e4 : std_logic_vector is x(6 downto 3);
    alias x_man_m3 : std_logic_vector is x(2 downto 0);

    -- Input signal y
    signal y : std_logic_vector(7 downto 0) := (others => '0');
    alias y_sign : std_logic is y(7);
    alias y_exp_e4 : std_logic_vector is y(6 downto 3);
    alias y_man_m3 : std_logic_vector is y(2 downto 0);

    -- Extended signals
    signal x_exp : unsigned(3 downto 0) := (others => '0');
    signal x_man : unsigned(3 downto 0) := (others => '0'); -- With leading one
    signal y_exp : unsigned(3 downto 0) := (others => '0');
    signal y_man : unsigned(3 downto 0) := (others => '0'); -- With leading one
    signal z_exp : unsigned(3 downto 0) := (others => '0');
    signal z_man : unsigned(2 downto 0) := (others => '0');

    -- Auxiliary signals
    signal x_zero_exp : std_logic := '0';
    signal y_zero_exp : std_logic := '0';

    signal x_max_exp : std_logic := '0';
    signal y_max_exp : std_logic := '0';

    signal x_man_zero : std_logic := '0';
    signal y_man_zero : std_logic := '0';

    signal x_zero : std_logic := '0';
    signal y_zero : std_logic := '0';

    signal x_nan : std_logic := '0';
    signal y_nan : std_logic := '0';

    signal x_inf : std_logic := '0';
    signal y_inf : std_logic := '0';

    signal z_nan : std_logic := '0';
    signal z_inf : std_logic := '0';

    signal x_normal : std_logic := '0';
    signal y_normal : std_logic := '0';

    -- Intermediate signals for mantissa
    signal x_man_norm : unsigned(3 downto 0) := (others => '0');
    signal y_man_norm : unsigned(3 downto 0) := (others => '0');
    signal x_man_lead_zeros : unsigned(1 downto 0) := (others => '0');
    signal y_man_lead_zeros : unsigned(1 downto 0) := (others => '0');
    signal man_prod : unsigned(7 downto 0) := (others => '0');
    signal man_prod_norm : unsigned(10 downto 0) := (others => '0');
    signal man_prod_shifted : unsigned(10 downto 0) := (others => '0');
    signal man_prod_carry : std_logic := '0';

    -- Signals for quantization
    signal man_prod_trunc : unsigned(2 downto 0) := (others => '0'); -- 0 integer bits
    signal man_prod_rounded : unsigned(3 downto 0) := (others => '0'); -- 1 integer bit
    signal man_round_carry : std_logic := '0';
    signal lsb_after_prod: std_logic := '0'; -- LSB of mantissa after multiplication
    signal guard_bit_G : std_logic := '0';   -- Bit after LSB of mantissa
    signal sticky_bit_T : std_logic := '0';  -- Logical OR of the last mantissa bits
    signal round_bit_B : std_logic := '0';
    signal shift_amount : unsigned(2 downto 0) := (others => '0');

    -- Signals for exponent
    signal z_exp_sum : signed(5 downto 0) := (others => '0');
    signal z_exp_sum2 : signed(5 downto 0) := (others => '0');
    signal z_exp_sum3 : signed(5 downto 0) := (others => '0');
    signal exp_underflow : std_logic := '0';
    signal exp_overflow : std_logic := '0';

    -- Output signals
    signal z : std_logic_vector(7 downto 0);
    signal z_sign : std_logic := '0';

begin

    -- Sign
    z_sign <= x_sign xor y_sign;

    -- Special number signals
    x_zero_exp <= '1' when unsigned(x_exp_e4) = 0 else '0';
    y_zero_exp <= '1' when unsigned(y_exp_e4) = 0 else '0';
    x_man_zero <= '1' when unsigned(x_man_m3) = 0 else '0';
    y_man_zero <= '1' when unsigned(y_man_m3) = 0 else '0';
    x_max_exp <= '1' when unsigned(x_exp_e4) = 15 else '0';
    y_max_exp <= '1' when unsigned(y_exp_e4) = 15 else '0';

    x_normal <= not x_zero_exp;
    y_normal <= not y_zero_exp;

    x_zero <= x_zero_exp and x_man_zero;
    y_zero <= y_zero_exp and y_man_zero;

    x_inf <= x_max_exp and x_man_zero;
    y_inf <= y_max_exp and y_man_zero;

    x_nan <= x_max_exp and not x_man_zero;
    y_nan <= y_max_exp and not y_man_zero;

    z_nan <= x_nan or y_nan or (x_max_exp and y_zero) or (y_max_exp and x_zero);

    -- Computation of exponents
    x_exp <= to_unsigned(1, 4) when x_zero_exp='1' else unsigned(x_exp_e4);
    y_exp <= to_unsigned(1, 4) when y_zero_exp='1' else unsigned(y_exp_e4);

    x_man_lead_zeros <= to_unsigned(0, 2) when  x_man(3)='1' else
                            to_unsigned(1, 2) when x_man(2)='1' else
                            to_unsigned(2, 2) when x_man(1)='1' else
                            to_unsigned(3, 2);

    y_man_lead_zeros <= to_unsigned(0, 2) when y_man(3)='1' else
                            to_unsigned(1, 2) when y_man(2)='1' else
                            to_unsigned(2, 2) when y_man(1)='1' else
                            to_unsigned(3, 2);

    z_exp_sum  <= signed("00" & x_exp) + signed("00" & y_exp) + to_signed(-7, 6);
    z_exp_sum2 <= z_exp_sum + signed'("0" & man_prod_carry) - signed("0" & x_man_lead_zeros) - signed("0" & y_man_lead_zeros);
    z_exp_sum3 <= z_exp_sum2 + signed("0" & shift_amount) + signed'("0" & man_round_carry);

    -- Special handling of exponent
    exp_underflow <= '1' when z_exp_sum2 < to_signed(-3, 6) or x_zero='1' or y_zero='1' else '0';
    exp_overflow <= '1' when z_exp_sum3 >= 15 else '0';
    z_inf <= exp_overflow or x_inf or y_inf; -- Not strict enough criteria but is solved later by mux

    z_exp <= to_unsigned(15, 4) when z_nan='1' or z_inf='1' else
                 to_unsigned(0, 4) when exp_underflow='1' else
                 unsigned(z_exp_sum3(3 downto 0));

    -- Normalization of input mantissas
    x_man <= unsigned(x_normal & x_man_m3);
    y_man <= unsigned(y_normal & y_man_m3);
    x_man_norm <= shift_left(x_man, to_integer(x_man_lead_zeros));
    y_man_norm <= shift_left(y_man, to_integer(y_man_lead_zeros));

    -- Mantissa product
    man_prod <= x_man_norm * y_man_norm;
    man_prod_carry <= man_prod(7);

    -- Denormalization mantissa
    man_prod_norm <= unsigned(man_prod & "000") when man_prod_carry='1' else unsigned(man_prod(6 downto 0) & "0000");
    shift_amount <= unsigned(-resize(z_exp_sum2, 3)) when z_exp_sum2 < 0 else to_unsigned(0, 3);
    man_prod_shifted <= shift_right(man_prod_norm, to_integer(unsigned'("0" & shift_amount))+1) when z_exp_sum2 <= 0 else man_prod_norm;

    -- Quantization of mantissa
    man_prod_trunc <= man_prod_shifted(9 downto 7);

    lsb_after_prod <= man_prod_trunc(0);

    guard_bit_G <= man_prod_shifted(6);

    sticky_bit_T <= '1' when man_prod_shifted(5 downto 0) /= 0 else '0';

    round_bit_B <= guard_bit_G and ((not lsb_after_prod) or sticky_bit_T); -- Round to nearest, ties to odd

    man_prod_rounded <= unsigned('0' & man_prod_trunc) + unsigned'("" & round_bit_B);
    man_round_carry <= '1' when man_prod_rounded(3)='1' else '0';

    z_man <= unsigned'("00" & z_nan) when z_nan='1' or exp_underflow='1' or z_inf='1' else
                 man_prod_rounded(2 downto 0);

    -- Concatenation of result
    z <= std_logic_vector(z_sign & z_exp & z_man);

    process(clk)
    begin
        if rising_edge(clk) then
            x <= x_in;
            y <= y_in;
            z_out <= z;
        end if;
    end process;
end arch;
