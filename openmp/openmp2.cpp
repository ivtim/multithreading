#include <omp.h>
#include <iostream>
#include <thread>
#include <time.h>
#include <chrono>
#define N 10
int C[N] = { 0 }, A[N] = { 1,2,3,4,5,6 }, B[N] = { 7,8,9,10,11,12 };



void function_1()
{
    for (int i = 0; i < N/2; i++)
    {   
        C[i] = A[i] + B[i];
       std::cout << omp_get_thread_num()<<" ";
       // std::cout << "Function 1 (i = " << i << ")" << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void function_2()
{
    for (int i = N/2; i < N; i++)
    {
        C[i] = A[i] + B[i];
        std::cout << omp_get_thread_num()<<" ";
       // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //std::cout << "                   Function 2 "
           // << "(j = " << j << ")" << std::endl;
    }
}

int main()
{
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        A[i] = rand() % 10000;
        B[i] = rand() % 10000;

    }

    auto start = std::chrono::high_resolution_clock::now();
    int min = INT32_MAX;
    #pragma omp parallel sections
    {
        #pragma omp section
        function_1();

        #pragma omp section
        function_2();

        
    }
    int i;
#pragma omp parallel private(i)
    {
        int minl = INT32_MAX;
#pragma omp for
        for ( i = 0;i < N;i++)
        {
            int thread = omp_get_thread_num();
            if (C[i]<minl)
            {
                minl = C[i];
            }
        }
#pragma omp critical
        {
            if (minl < min)
                min = minl;
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time = finish - start;
    std::cout << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << " " << A[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << " " << B[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < N; i++)
    {
        std::cout << " " << C[i]<< " ";
    }
    std::cout << std::endl;
    std::cout << "min(A+B)=" << min<<std::endl;
    std::cout << "time=" << time.count();
}