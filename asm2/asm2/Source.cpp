#include <iostream>


int main()
{
	try {
		// [14]: (a - b * 4 - 1) / (c / 31 + a * b);

		int
			a = -1,
			b = 1,
			c = 31,
			err = -1;

		_asm {
			mov eax, c; // <eax> == c
			mov ebx, 31; // <ebx> == 31

			cdq; // <eax> -> <edx:eax>
			idiv ebx; // <eax> == c / 31 (remainder in <edx>)
			mov ecx, eax; // <ecx> == c / 31

			mov eax, a; // <eax> == a
			mov ebx, b; // <ebx> == b
			imul ebx; // <edx:eax> == a * b
			jo err_overflow; // Error: value register owerflow

			add eax, ecx; // <eax> == a * b + c / 31
			jo err_overflow; // Error: value register owerflow
			jz err_div_zero; // Error: division by zero

			push eax; // <Stack> == a * b + c / 31

			mov eax, b; // <eax> == b
			mov ebx, 4; // <ebx> == 4

			imul ebx; // <edx:eax> == b * 4
			jo err_overflow; // Error: value register owerflow
			inc eax; // <eax> == b * 4 + 1
			jo err_overflow; // Error: value register owerflow

			mov ebx, eax; // <ebx> == b * 4 + 1
			mov eax, a; // <eax> == a
			sub eax, ebx; // <eax> == a - b * 4 - 1
			jo err_overflow; // Error: value register owerflow

			mov ebx, a; // <ebx> == a
			sub ebx, eax; // <ebx> == a - b * 4 - 1
			jo err_overflow; // Error: value register owerflow

			pop ebx; // <ebx> == <Stack> == a * b + c / 31

			cdq; // <eax> -> <edx:eax>
			idiv ebx; // <eax> == (a - b * 4 - 1) / (c / 31 + a * b) (remainder in <edx>)
			
			mov a, eax; // a == <eax>
			jmp end;
			
		err_div_zero:
			mov err, 0; // err == 0
			jmp end;
		err_overflow:
			mov err, 1; // err == 1
		end:
		}

		switch (err) {
		case -1:
			std::cout << a << std::endl;
			break;
		case 0:
			throw std::domain_error("Error: division by zero.");
			break;
		case 1:
			throw std::overflow_error("Error: value register owerflow.");
		}
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}