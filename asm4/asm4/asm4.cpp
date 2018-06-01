#include <iostream>

int main() {
	try {
		// [14]: if (a[i] > 0) X *= a[i] * a[i] * a[i];
		
		int X0, X1;
		int err = -1;

		int arr[] = {-1, 2, 0, 5};
		int arr_length = sizeof(arr) / sizeof(*arr);

		_asm {
			mov ecx, arr_length; // <ecx> = arr_length
			jecxz err_zero_len; // <ecx> == 0 -> error

			mov esi, 0; // <esi> == 0
			push 1; // <Stack> == 1
		main_loop:
			mov eax, [arr + esi * 4]; // <eax> == a[i]
			inc esi; // <esi> == <esi> + 1
			jo err_overflow; // Error: value register owerflow

			cmp eax, 0;
			jg val_positive; // <eax> > 0 ? goto val_positive

			loop main_loop; // Continue loop

			pop X; // X == <Stack> == Result
			jmp end; // goto end
		val_positive:
			mov ebx, eax; // <ebx> == a[i]

			imul ebx; // <edx:eax> == (a[i])^2
			jo err_overflow; // Error: value register owerflow

			imul ebx; // <edx:eax> == (a[i])^3
			jo err_overflow; // Error: value register owerflow

			pop ebx; // <ebx> == <Stack> == Result
			xchg ebx, eax; // <ebx> == (a[i])^3 & <eax> == Result

			imul ebx; // <edx:eax> == Result * (a[i])^3
			jo err_overflow; // Error: value register owerflow

			push eax; // <Stack> == Result * (a[i])^3
			loop main_loop; // goto main_loop
		err_zero_len:
			mov err, 0; // err == 0
			jmp end;
		err_overflow:
			mov err, 1; // err == 1
		end:
		}

	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
	
	system("pause");
	return 0;
}