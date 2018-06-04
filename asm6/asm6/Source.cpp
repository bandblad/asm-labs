#include <iostream>

int main() {
	try {
		/* : 
			[14]:
			(a > b): ((b^3) / a) - 1
			(a == b): 255
			(a < b): ((a^3) - 255) / b
		*/

		const double
			c_255 = 255.0,
			c_1 = 1.0;

		double a, b, X;
		int err = -1;

		// Promt user action
		std::cout
			<< "Type 'a' and 'b'.\n"
			<< "'a' and 'b' is real numbers."
			<< std::endl;

		// Read data from stdin
		std::cin >> a >> b;
		if (!std::cin.good())
			throw std::invalid_argument("Error: Invalid arguments detected in stdin.");

		_asm {
			finit; // Initialize co-processor

			fld qword ptr[b]; // <Stack(0)> == b
			fld qword ptr[a]; // <Stack(0)> == a && <Stack(1)> == b

			fcompp; // a ?? b
			fnstsw ax; // Set flags of <ax> register
			sahf; // Set flags of <ah> register
			
			ja goAGtrB; // (a > b) ? goto goAGtrB
			je goAEqB; // (a == b) ? goto goAEqB
			jb goALsrB; // (a < b) ? goto goALsrB
			
			jmp err_unknown; // (a ?? b) ? goto err_unknown

		goAGtrB:
			fld qword ptr[a]; // <Stack(0)> == a
			
			ftst; // a == 0 ?
			fnstsw ax; // <ax> == (a == 0 ?)
			sahf; // Set flags of <ah> register

			jz err_div_zero; // (a == 0) ? goto err_div_zero

			fld qword ptr[b]; // <Stack(0)> == b && <Stack(1)> == a
			fdivr; // <Stack(0)> == b / a

			fld qword ptr[b]; // <Stack(0)> == b && <Stack(1)> == b / a
			fmul; // <Stack(0)> == (b^2) / a

			fld qword ptr[b]; // <Stack(0)> == b && <Stack(1)> == (b^2) / a
			fmul; // <Stack(0)> == (b^3) / a

			fld qword ptr[c_1]; // <Stack(0)> == 1.0 && <Stack(1)> == (b^3) / a
			fsub; // <Stack(0)> == (b^3) / a - 1

			fstp qword ptr[X]; // X == ((b^3) / a) - 1
			jmp end;
		goAEqB:
			fld qword ptr[c_255]; // <Stack(0)> == 255.0
			fstp qword ptr[X]; // X == 255.0
			jmp end;
		goALsrB:
			fld qword ptr[b]; // <Stack(0)> == b

			ftst; // b == 0 ?
			fnstsw ax; // <ax> == (b == 0 ?)
			sahf; // Set flags of <ah> register

			jz err_div_zero; // (b == 0) ? goto err_div_zero

			fld qword ptr[a]; // <Stack(0)> == a && <Stack(1)> == b
			
			fld qword ptr[a]; // <Stack(0)> == a && <Stack(1)> == a && <Stack(2)> == b
			fmul; // <Stack(0)> == (a^2) && <Stack(1)> == b

			fld qword ptr[a]; // <Stack(0)> == a && <Stack(1)> == a^2 && <Stack(2)> == b
			fmul; // <Stack(0)> == (a^3) && <Stack(1)> == b

			fld qword ptr[c_255]; // <Stack(0)> == 255.0 && <Stack(1)> == (a^3) && <Stack(2)> == b
			fsub; // <Stack(0)> == ((a^3) - 255.0) && <Stack(1)> == b
			fdivr; // <Stack(0)> == ((a^3) - 255.0) / b

			fstp qword ptr[X]; // X == ((a^3) - 255) / b
			jmp end;
		err_div_zero:
			mov err, 0; // err == 0
			jmp end;
		err_unknown:
			mov err, 1; // err == 1		
		end:
		}

		if (err == -1) {
			std::cout << "ASM Result: " << X << std::endl;

			if (a == b) {
				X = 255.0;
			}
			else if (a > b) {
				if (a == 0)
					throw std::domain_error("Error: division by zero.");
				X = ((b * b * b) / a) - c_1;
			}
			else {
				if (b == 0)
					throw std::domain_error("Error: division by zero.");
				X = ((a * a * a) - c_255) / b;
			}

			std::cout << "Regular code result: " << X << std::endl;
		}
		else if (err == 0)
			throw std::domain_error("Error: division by zero.");
		else if (err == 1)
			throw std::runtime_error("Error: undefined behaviour.");
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}