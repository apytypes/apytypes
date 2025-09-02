

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library vunit_lib;
context vunit_lib.vunit_context;

entity tb_csv is
	generic (
		runner_cfg : string := "";
        tb_path : string := "";
		csv_file : string := ""
		);
end entity;

architecture tb of tb_csv is

	constant arr_length_1d : natural := 8;

	-- Transposed to make the indexing the same
	constant arr_T_width_2d : natural := 6;
	constant arr_T_height_2d : natural := 4;

	-- Transposed to make the indexing the same
	constant arr_T_width_3d : natural := 2;
	constant arr_T_height_3d : natural := 3;
	constant arr_T_depth_3d : natural := 4;

begin
	main : process
		variable csv_arr: integer_array_t;
	begin
		test_runner_setup(runner, runner_cfg);

		csv_arr := load_csv(csv_file);

		if run("Case 1D") then
			-- Dimension check.
			check_equal(length(csv_arr),  integer(arr_length_1d),  "Unexpected array length");

			-- Check index order. Also test negative values.
			check_equal(get(csv_arr, 0), 0, "Unexpected value at (0)");
			check_equal(get(csv_arr, 1), 1, "Unexpected value at (1)");
			check_equal(get(csv_arr, 4), -4, "Unexpected value at (4)");
			check_equal(get(csv_arr, 7), -1, "Unexpected value at (7)");

		end if;

		if run("Case 2D") then
			-- Dimension checks. This is now transposed.
			check_equal(width(csv_arr),  integer(arr_T_width_2d),  "Unexpected array width");
			check_equal(height(csv_arr), integer(arr_T_height_2d),  "Unexpected array height");

			-- Check index order. Should match the APyTypes test case.
			check_equal(get(csv_arr, 0, 1), 1, "Unexpected value at (0,1)");
			check_equal(get(csv_arr, 1, 0), 4, "Unexpected value at (1,0)");
			check_equal(get(csv_arr, 1, 1), 5, "Unexpected value at (1,1)");
			check_equal(get(csv_arr, 5, 3), 23, "Unexpected value at (5,3)");

		end if;

		if run("Case 3D") then
			-- Dimension checks. The matrix has been transposed and reshaped into a 2D array for storage.
			check_equal(width(csv_arr),  integer(arr_T_depth_3d * arr_T_width_3d),  "Unexpected array width");
			check_equal(height(csv_arr), integer(arr_T_height_3d),  "Unexpected array height");

			reshape(csv_arr, width => arr_T_width_3d, height => arr_T_height_3d, depth => arr_T_depth_3d);

			-- Check index order. Should match the APyTypes test case.
			check_equal(get(csv_arr, 0, 0, 1), 1, "Unexpected value at (0,0,1)");
			check_equal(get(csv_arr, 0, 1, 0), 4, "Unexpected value at (0,1,0)");
			check_equal(get(csv_arr, 0, 1, 1), 5, "Unexpected value at (0,1,1)");
			check_equal(get(csv_arr, 1, 0, 0), 12, "Unexpected value at (1,0,0)");
			check_equal(get(csv_arr, 1, 0, 1), 13, "Unexpected value at (1,0,1)");
			check_equal(get(csv_arr, 1, 1, 0), 16, "Unexpected value at (1,1,0)");
			check_equal(get(csv_arr, 1, 1, 1), 17, "Unexpected value at (1,1,1)");
			check_equal(get(csv_arr, 1, 2, 3), 23, "Unexpected value at (1,2,3)");

			save_csv(csv_arr, tb_path  & "new_apyfixed_bits_3d.csv");

		end if;

		-- Cleanup
		test_runner_cleanup(runner);
		wait;
	end process;

end architecture;
