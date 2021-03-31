// lab1Paralel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>
#include <stdio.h>


int main()
{
	const int N = 8;
	int arr[N];
	omp_set_num_threads(6);
#pragma omp parallel
	{
		int thread = omp_get_thread_num();
		int p = omp_get_num_threads();

		if (thread % 2 == 0) {
			char s[50];
			sprintf_s(s, "Hello! I'm thread - %d\n", thread);
			std::cout << s;
		}
		else {
			int psize = N / (p / 2);
			int size = psize;
			if (thread / 2 < N % (p / 2)) size++;
			int is = 0;
			if (thread != 0)
			{
				is = thread / 2 * psize + fmin(thread / 2, N % (p / 2));
			}

			for (int i = is; i < is + size; i++)
			{
				arr[i] = omp_get_thread_num();
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		std::cout << arr[i] << ' ';
	}
}
