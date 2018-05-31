#include <iostream>

int main() {
	
	try {
		// [14]
		
		int a, b, X;
		int err = -1;

		// Promt user action
		std::cout
			<< "Type 'a' and 'b'.\n"
			<< "'a' and 'b' is integer numbers."
			<< std::endl;

		// Read data from stdin
		std::cin >> a >> b;
		if (!std::cin.good())
			throw std::invalid_argument("Error: Invalid arguments detected in stdin.");

		_asm {
			mov  eax, a; // <eax> == a
			mov  ebx, b; // <ebx> == b
			
			cmp  eax, ebx; // Compare 'a' and 'b'
			
			jg   goAGtrB; // a > b
			jl   goALsrB; // a < b
			je  goAEqB; // a == b
			
			jmp err_unknown; // ???

		// a > b
		// X = (b^3 / a) - 1
		goAGtrB:
			mov eax, a; // <eax> == a
			jz err_div_zero; // Error: division by zero
			
			mov eax, b; // <eax> == b
			mov ebx, b; // <ebx> == b
			
			imul ebx; // <eax> == b^2
			jo err_overflow; // Error: value register owerflow

			imul ebx; // <eax> == b^3
			jo err_overflow; // Error: value register owerflow

			mov ebx, a; // <ebx> == a

			cdq; // <eax> -> <edx:eax>
			idiv ebx; // <eax> == b^3 / a
			jo err_overflow; // Error: value register owerflow

			dec eax; // <eax> == (b^3 / a) - 1
			jo err_overflow; // Error: value register owerflow

			jmp end;
		// a == b
		goAEqB:
			mov X, 255;
			jmp end;
		// a < b
		// X = (a^3 - 255) / b
		goALsrB:
			mov eax, b; // <eax> == b
			jz err_div_zero; // Error: division by zero
			
			mov eax, a; // <eax> == a
			mov ebx, a; // <ebx> == a

			imul ebx; // <eax> == a^2
			jo err_overflow; // Error: value register owerflow

			imul ebx; // <eax> == a^3
			jo err_overflow; // Error: value register owerflow

			mov ebx, 255; // <ebx> == 255
			sub eax, ebx; // <eax> == a^3 - 255
			jo err_overflow; // Error: value register owerflow

			cdq; // <eax> -> <edx:eax>
			mov ebx, b; // <ebx> == b
			idiv ebx; // <eax> == (a^3 - 255) / b
			jo err_overflow; // Error: value register owerflow

			mov X, eax; // X == <eax>
			jmp end;
		
		err_div_zero:
			mov err, 0; // err == 0
			jmp end;
		err_overflow:
			mov err, 1; // err == 1
			jmp end;
		err_unknown:
			mov err, 2; // err == 2
		end:
		}

		switch (err) {
		case -1:
			std::cout << "Result is: " << X << std::endl;
			break;
		case 0:
			throw std::domain_error("Error: division by zero.");
			break;
		case 1:
			throw std::overflow_error("Error: value register owerflow.");
			break;
		case 2:
			throw std::runtime_error("Error: undefined behaviour.");
		}
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}