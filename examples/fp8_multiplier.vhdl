library ieee;
use ieee.std_logic_1164.all;
use ieee.float_pkg.all;

entity fp8_multiplier is
    port ( clk  : in std_logic := '0';
           x_in : in std_logic_vector(7 downto 0) := (others => '0');
           y_in : in std_logic_vector(7 downto 0) := (others => '0');
           z_out : out std_logic_vector(7 downto 0) := (others => '0'));
end fp8_multiplier;

architecture arch of fp8_multiplier is
    signal x, y : float (4 downto -3); -- 4 exponent bits, 3 mantissa bits
begin
    process(clk)
    begin
        if rising_edge(clk) then
            x <= to_float(x_in, x);
            y <= to_float(y_in, y);
            z_out <= to_std_logic_vector(x * y);
        end if;
    end process;
end arch;
