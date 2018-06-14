#include <iostream>

int main()
{
	try {
		// [14]: SUM(1, inf) { ((-1) ^ k) * (cos(k * x) / (k ^ 2)) }

		double eps, x, result;
		const double pi_2 = 1.57079632679489661923;

		// Promt user action
		std::cout
			<< "Type 'x' and 'eps'."
			<< "\n'x' and 'eps' is real numbers."
			<< "\n0 <= x <= PI/2"
			<< std::endl;

		// Read data from stdin
		std::cin >> x >> eps;
		if (!std::cin.good() || x < 0 || x > pi_2)
			throw std::invalid_argument("Error: Invalid arguments detected in stdin.");

		_asm {
			finit; // Initialize co-processor

			fld1; // <Stack(0)> == 1.0
			fchs; // <Stack(0)> == -1.0

			fldz; // <Stack(0)> == 0.0 && <Stack(1)> == -1.0
			fld1; // <Stack(0)> == 1.0 && <Stack(1)> == 0.0 && <Stack(2)> == -1.0
			fld1; // <Stack(0, 1)> == 1.0 && <Stack(2)> == 0.0 && <Stack(3)> == -1.0
		main_loop:
			fld qword ptr[x]; // <Stack(0)> == x
			fmul st, st(2); // <Stack(0)> == k * x
			fcos; // <Stack(0)> == cos(k * x)
			fmul st, st(4); // <Stack(0)> == ((-1) ^ k) * cos(k * x)
			fdiv st, st(2); // <Stack(0)> == ((-1) ^ k) * cos(k * x) / k
			fdiv st, st(2); // <Stack(0)> == ((-1) ^ k) * cos(k * x) / (k ^ 2)

			fld1;
			faddp st(3), st; // k == k + 1

			fxch st(4); 
			fchs; // <Stack(0)> == (-1) ^ k
			fxch st(4); // <Stack(0)> == ((-1) ^ k) * cos(k * x) / (k ^ 2) &&  <Stack(4)> == (-1) ^ k

			fadd st(3), st; // sum = sum + ((-1) ^ k) * cos(k * x) / (k ^ 2)
			fmul;
		end_loop:
			fldz;
			fadd st, st(1);
			fabs;

			fcomp qword ptr[eps];
			fstsw ax;
			sahf;
			ja main_loop;

			fxch st(2);
			fst qword ptr[result];
		}
		std::cout << "ASM Result: " << result << std::endl;

		 /* CPP */
		int k = 1;
		double mul = 1.0, temp;
		result = 0.0;

		do {
			temp = k & 1 ? -1.0 : 1.0;;
			temp *= cos(double(k) * x);
			temp /= double(k * k);
			mul *= temp;

			result += temp;
			++k;
		} while (abs(mul) > eps);

		std::cout << "Regular code result: " << result << std::endl;
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	system("pause");
	return 0;
}

