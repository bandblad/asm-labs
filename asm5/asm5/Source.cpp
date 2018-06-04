#include <iostream>

int main() {
	try {
		// [14]: (a - b * 4 - 1) / (c / 31 + a * b)

		const double
			c_31 = 31.0,
			c_4 = 4.0,
			c_1 = 1.0;

		double a, b, c, X;
		int err = -1;

		// Promt user action
		std::cout
			<< "Type 'a', 'b', 'c'.\n"
			<< "'a', 'b', 'c' is real numbers."
			<< std::endl;

		// Read data from stdin
		std::cin >> a >> b >> c;
		if (!std::cin.good())
			throw std::invalid_argument("Error: Invalid arguments detected in stdin.");

		_asm {
			finit; // Initialize co-processor

			fld qword ptr[c]; // <Stack(0)> == c
			fdiv qword ptr[c_31]; // <Stack(0)> == c / 31.0
			
			fld qword ptr[a]; // <Stack(0)> == a && <Stack(1)> == c / 31.0
			fmul qword ptr[b]; // <Stack(0)> == a * b
			fadd; // <Stack(0)> == c / 31.0 + a * b

			ftst; // <Stack(0)> == 0 ?
			fnstsw ax; // <ax> == (<Stack(0)> == 0 ?)
			sahf; // Set flags of <ah> register
			jz err_div_zero;

			fld qword ptr[b]; // <Stack(0)> == b && <Stack(1)> == c / 31.0 + a * b
			fmul qword ptr[c_4]; // <Stack(0)> == b * 4.0
			fadd qword ptr[c_1]; // <Stack(0)> == b * 4.0 + 1.0 
			fsub qword ptr[a]; // <Stack(0)> == b * 4.0 + 1.0 - a
			fchs; // <Stack(0)> == a - b * 4.0 - 1.0
			
			fdivr; // <Stack(0)> == (a - b * 4 - 1) / (c / 31.0 + a * b)

			fst qword ptr[X]; // X == <Stack(0)>
			jmp end;
		err_div_zero:
			mov err, 0; // err == 0
		end:
		}

		if (err == -1) {
			std::cout << "ASM Result is: " << X << std::endl;

			double divider = c / c_31 + a * b;
			if (!divider)
				throw std::domain_error("Error: division by zero.");
			double divident = a - b * c_4 - c_1;

			std::cout << "C++ Result is: " << divident / divider << std::endl;
		}
		else if (err == 0)
			throw std::domain_error("Error: division by zero.");
	}
	catch(std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}