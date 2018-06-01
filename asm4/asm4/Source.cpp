#include <iostream>

int main() {
	try {
		// [14]: if (a[i] > 0) X *= a[i] * a[i] * a[i];

		int X0, X1 = 1;
		int err = -1;

		int arr[] = { -1, 2, 0, 10 };
		int arr_length = sizeof(arr) / sizeof(*arr);

		_asm {
			mov ecx, arr_length; // <ecx> = arr_length
			jecxz err_zero_len; // <ecx> == 0 ? goto err_zero_len

			xor esi, esi; // <esi> == 0
			push 1; // <Stack> == 1
		main_loop:
			mov eax, [arr + esi * 4]; // <eax> == a[i]
			cmp eax, 0;
			jg val_positive; // (<eax> > 0) ? goto val_positive
		end_loop:
			inc esi; // <esi> == <esi> + 1
			jo err_overflow; // OF ? goto err_overflow

			loop main_loop; // Continue loop
			pop X0; // X0 == Result
			jmp end; // goto end
		val_positive:
			mov ebx, eax; // <ebx> == a[i]

			imul ebx; // <edx:eax> == (a[i])^2
			jo err_overflow; // OF ? goto err_overflow

			imul ebx; // <edx:eax> == (a[i])^3
			jo err_overflow; // OF ? goto err_overflow

			pop ebx; // <ebx> == Result
			imul ebx; // <edx:eax> == (a[i])^3 * Result
			jo err_overflow; // OF ? goto err_overflow

			push eax; // <Stack> == (a[i])^3 * Result
			jmp end_loop; // goto end_loop
		err_zero_len:
			mov err, 0; // err == 0
			jmp end;
		err_overflow:
			mov err, 1; // err == 1
		end:
		}

		switch (err) {
		case -1:
			for (int i = -1, j; ++i < arr_length;) 
				if ((j = arr[i]) > 0)
					X1 *= j * j * j;

			std::cout << "ASM result: " 
				<< X0 
				<< "\nRegular code result: "
				<< X1
				<< std::endl;

			break;
		case 0:
			throw std::invalid_argument("Error: zero length array.");
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