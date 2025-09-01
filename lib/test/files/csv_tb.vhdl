

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library vunit_lib;
context vunit_lib.vunit_context;

entity tb_csv is
	generic (
		runner_cfg : string := "";
        tb_path : string := ""
		);
end entity;

architecture tb of tb_csv is

	constant csv_path_2d : string := "apyfixed_bits_2d.csv";
	constant csv_path_3d : string := "apyfixed_bits_3d.csv";

	constant arr_width_2d : natural := 4;
	constant arr_height_2d : natural := 6;

	constant arr_width_3d : natural := 4;
	constant arr_height_3d : natural := 3;
	constant arr_depth_3d : natural := 2;

	signal val : signed(3 downto 0) := (others => '0');
begin
	main : process
		variable arr: integer_array_t := new_1d(
			length    => 25,
			bit_width => 4,
			is_signed => true
		);
		variable csv_arr: integer_array_t;
	begin
		test_runner_setup(runner, runner_cfg);

		-- Initialize expected array
		for idx in 0 to length(arr) - 1 loop
			set(arr, idx, to_integer(val));
			val <= val + 1;
			wait for 1 ns;
		end loop;

		-- Test 2D case
		reshape(arr, width => arr_width_2d, height => arr_height_2d);
		csv_arr := load_csv(tb_path & csv_path_2d);

		-- Dimension checks
		check_equal(width(csv_arr),  integer(arr_width_2d),  "Unexpected array width");
		check_equal(height(csv_arr), integer(arr_height_2d),  "Unexpected array height");

		-- Check elements
		for y in 0 to height(csv_arr)-1 loop
			for x in 0 to width(csv_arr)-1 loop
				check_equal(
					get(csv_arr, x, y), get(arr, x, y),
					"Mismatch at x=" & to_string(x) & ", y=" & to_string(y)
				);
			end loop;
		end loop;

		-- Test 3D case
		reshape(arr, width => arr_width_3d, height => arr_height_3d, depth => arr_depth_3d);

		csv_arr := load_csv(tb_path & csv_path_3d);
		check_equal(width(csv_arr),  integer(arr_width_3d*arr_height_3d),  "Unexpected array width upon loading CSV");
		check_equal(height(csv_arr), integer(arr_depth_3d),  "Unexpected array height upon loading CSV");
		reshape(csv_arr, width => arr_width_3d, height => arr_height_3d, depth => arr_depth_3d);

		-- Check elements
		for y in 0 to height(csv_arr)-1 loop
			for x in 0 to width(csv_arr)-1 loop
				for z in 0 to depth(csv_arr)-1 loop
					check_equal(
						get(csv_arr, x, y, z), get(arr, x, y, z),
						"Mismatch at x=" & to_string(x) & ", y=" & to_string(y) & ", z=" & to_string(z)
					);
				end loop;
			end loop;
		end loop;

		-- Cleanup
		test_runner_cleanup(runner);
		wait;
	end process;

end architecture;
