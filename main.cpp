#include <cstdio>
#include <iostream>
#include <locale.h>
#include <stdlib.h>
#include <fstream>

#include "CL/cl.h"

#include "openCLObject.h"
#include "Opencl_source_read.h"

using openCLObject::Image2d;
using openCLObject::Platforms;
using openCLObject::Devices;
using openCLObject::Context;
using openCLObject::CommandQueue;
using openCLObject::Program;
using openCLObject::Kernel;
using openCLObject::Buffer;
using OpenclSource::Opencl_source_read;

template <typename T>
void printMas(T *n, long long size, std::ostream &out, long long i_endl = 0ll)
{
	for (long long i = 0; i < size; i++)
	{
		out << n[i] << " ";
		if (i_endl != 0 && (i + 1) % i_endl == 0)
		{
			out << std::endl;
		}
	}
	out << std::endl;
}

void lab1()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));
	
	Opencl_source_read opencl_source_read("Lab1.txt");
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Simple");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Simple");
	kernel.printInfo(&devices);

	int size = 0;
	printf("\n");
	do
	{
		printf("Введите размер массива:");
		scanf_s("%d", &size);
	} while (size <= 0);
	Buffer buffer1(&context, CL_MEM_READ_WRITE, sizeof(size), NULL);
	buffer1.rnqueueWrite(&command_queue, CL_TRUE, 0, sizeof(size), (const void*)&size, 0, 0, 0);
	int* n = new int[size];
	for (int i = 0; i < size; i++)
	{
		printf("Введите n[%d] = ", i);
		scanf_s("%d", &n[i]);
	}
	Buffer buffer2(&context, CL_MEM_READ_WRITE, sizeof(int) * size, NULL);
	buffer2.rnqueueWrite(&command_queue, CL_TRUE, 0, sizeof(int) * size, (const void*)n, 0, 0, 0);

	size_t GLOBAl_WORK_SIZE[3];
	size_t LOCAL_WORK_SIZE[3];
	printf("\nВвод числа рабочих элементов\n");
	for (int i = 0; i < 3; i++)
	{
		int temp;
		printf("Введите GLOBAl_WORK_SIZE[%d] = ", i);
		scanf_s("%d", &temp);
		GLOBAl_WORK_SIZE[i] = size_t(temp);
	}
	printf("\n");
	for (int i = 0; i < 3; i++)
	{
		int temp;
		printf("Введите GLOBAl_WORK_SIZE[%d] = ", i);
		scanf_s("%d", &temp);
		LOCAL_WORK_SIZE[i] = size_t(temp);
	}
	printf("Конец ввода\n");
	kernel.SetKernelArg(&buffer1, 0);
	kernel.SetKernelArg(&buffer2, 1);
	kernel.startKernel(&command_queue, 1, NULL, GLOBAl_WORK_SIZE, LOCAL_WORK_SIZE, 0, 0, 0);

	printf("\nДо вычислений ядра: ");
	printMas(n, size, std::cout);
	buffer1.enqueueRead(&command_queue, CL_TRUE, 0, sizeof(size), (void*)&size, 0, 0, 0);
	buffer2.enqueueRead(&command_queue, CL_TRUE, 0, sizeof(int) * size, (void*)n, 0, 0, 0);
	printf("\nРезультат вычислений ядра: ");
	printMas(n, size, std::cout);

	delete[size] n;
}

void lab2()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));

	Opencl_source_read opencl_source_read("Lab2.txt");
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Kernel");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Kernel");
	kernel.printInfo(&devices);

	std::ifstream in("inLab2.txt");
	size_t sizeAll = 64, sizeGroup = 32;
	int N;
	in >> N;
	float* A = new float[N];
	float* B = new float[N];
	//определение массива вещественных чисел
	for (int i = 0; i < N; i++) in >> A[i] >> B[i];
	in.close();
	//определение массивов из N подсумм
	float * C = new float[N]; for (int i = 0; i < N; i++) C[i] = 0;
	//ОБЪЕКТЫ ПАМЯТИ
	Buffer buffer1(&context, CL_MEM_READ_ONLY, N * sizeof(float), NULL);
	Buffer buffer2(&context, CL_MEM_READ_ONLY, N * sizeof(float), NULL);
	Buffer buffer3(&context, CL_MEM_WRITE_ONLY, N * sizeof(float), NULL);
	//копируем массивы из оперативной памяти в память OpenCL устройства
	buffer1.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)A, 0, 0, 0);
	buffer2.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)B, 0, 0, 0);
	buffer3.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)C, 0, 0, 0);
	//АРГУМЕНТЫ ЯДРА
	kernel.SetKernelArg(&buffer1, 0);
	kernel.SetKernelArg(&buffer2, 1);
	kernel.SetKernelArg(2, sizeof(N), (void*)&N);
	kernel.SetKernelArg(&buffer3, 3);

	kernel.startKernel(&command_queue, 1, NULL, &sizeAll, &sizeGroup, 0, 0, 0);
	//получение результатов вычислений ядра - МАССИВ
	buffer3.enqueueRead(&command_queue, CL_TRUE, 0, N * sizeof(float), (void*)C, 0, 0, 0);
	//получение результатов вычислений ядра - СУММЫ
	//вывод результатов на экран
	std::ofstream out("outLab2.txt");
	out << "\nМассив C : \n";
	printMas(C, N, out, 5);
	out.close();

	delete[N] A;
	delete[N] B;
	delete[N] C;
}

void lab3()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));

	Opencl_source_read opencl_source_read("Lab3.txt");
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Kernel");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Kernel");
	kernel.printInfo(&devices);

	std::ifstream in("inLab2.txt");
	size_t sizeAll = 64, sizeGroup = 32;
	int N;
	in >> N;
	float* A = new float[N];
	float* B = new float[N];
	//определение массива вещественных чисел
	for (int i = 0; i < N; i++) in >> A[i] >> B[i];
	in.close();
	//определение массивов из N подсумм
	float* C = new float[N]; for (int i = 0; i < N; i++) C[i] = 0;
	//ОБЪЕКТЫ ПАМЯТИ
	Buffer buffer1(&context, CL_MEM_READ_ONLY, N * sizeof(float), NULL);
	Buffer buffer2(&context, CL_MEM_READ_ONLY, N * sizeof(float), NULL);
	Buffer buffer3(&context, CL_MEM_WRITE_ONLY, N * sizeof(float), NULL);
	//копируем массивы из оперативной памяти в память OpenCL устройства
	buffer1.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)A, 0, 0, 0);
	buffer2.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)B, 0, 0, 0);
	buffer3.rnqueueWrite(&command_queue, CL_TRUE, 0, N * sizeof(float), (const void*)C, 0, 0, 0);
	//АРГУМЕНТЫ ЯДРА
	kernel.SetKernelArg(&buffer1, 0);
	kernel.SetKernelArg(&buffer2, 1);
	kernel.SetKernelArg(2, sizeof(N), (void*)&N);
	kernel.SetKernelArg(&buffer3, 3);

	kernel.startKernel(&command_queue, 1, NULL, &sizeAll, &sizeGroup, 0, 0, 0);
	//получение результатов вычислений ядра - МАССИВ
	buffer3.enqueueRead(&command_queue, CL_TRUE, 0, N * sizeof(float), (void*)C, 0, 0, 0);
	//получение результатов вычислений ядра - СУММЫ
	//вывод результатов на экран
	std::ofstream out("outLab2.txt");
	out << "\nМассив C : \n";
	printMas(C, N, out, 1);
	out.close();

	delete[N] A;
	delete[N] B;
	delete[N] C;
}

void lab4_1()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));

	Opencl_source_read opencl_source_read("Lab4_1.txt");
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Kernel");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Kernel");
	kernel.printInfo(&devices);

	size_t sizeAll = 64, sizeGroup = 32;
	std::ifstream in("inLab4.txt");
	int N;
	in >> N;
	if (N % 2 == 0)\
	{
		printf("\nОшибка! N должна быть нечетна!\n");
		exit(-1);
	}
	int* F = new int[N * N];
	int* mx = new int[N * N];
	//определение массива
	for (int i = 0; i < N * N; i++)
	{
		in >> F[i];
		//mx[i] = F[i];
	}
	in.close();
	//ОБЪЕКТЫ ПАМЯТИ
	Buffer buffer1(&context, CL_MEM_READ_WRITE, N * N * sizeof(int), NULL);
	Buffer buffer2(&context, CL_MEM_READ_WRITE, N * N * sizeof(int), NULL);
	//копируем массив из оперативной памяти в память OpenCL устройства
	buffer1.rnqueueWrite(&command_queue, CL_TRUE, 0, N * N * sizeof(int), (const void*)F, 0, 0, 0);
	buffer2.rnqueueWrite(&command_queue, CL_TRUE, 0, N * N * sizeof(int), (const void*)mx, 0, 0, 0);
	//АРГУМЕНТЫ ЯДРА
	kernel.SetKernelArg(&buffer1, 0);
	kernel.SetKernelArg(&buffer2, 1);
	kernel.SetKernelArg(2, sizeof(int), (void*)&N);
	//Старт ядра
	kernel.startKernel(&command_queue, 1, NULL, &sizeAll, &sizeGroup, 0, 0, 0);
	//получение результатов вычислений ядра - МАССИВ
	buffer1.enqueueRead(&command_queue, CL_TRUE, 0, N * N * sizeof(int), (void*)F, 0, 0, 0);
	buffer2.enqueueRead(&command_queue, CL_TRUE, 0, N * N * sizeof(int), (void*)mx, 0, 0, 0);
	//вывод результатов на экран
	std::ofstream out("outLab4.txt");
	out << "\nМассив : \n";
	printMas(F, N * N, out, N);
	out.close();

	delete[N * N] F;
	delete[N * N] mx;
}

void lab4_2()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));

	Opencl_source_read opencl_source_read("Lab4_2.txt");
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Kernel");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Kernel");
	kernel.printInfo(&devices);

	size_t sizeAll = 64, sizeGroup = 32;
	std::ifstream in("inLab4.txt");
	int N;
	in >> N;
	if (N % 2 == 0)\
	{
		printf("\nОшибка! N должна быть нечетна!\n");
		exit(-1);
	}
	int** F = new int* [N];
	int** mx = new int* [N];
	//определение массива
	for (int i = 0; i < N; i++)
	{
		F[i] = new int[N];
		mx[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			in >> F[i][j];
			mx[i][j] = F[i][j];
		}
	}
	in.close();
	//описание формата изображения
	cl_image_format image_format, image_format2;
	image_format.image_channel_data_type = CL_SIGNED_INT32;
	image_format.image_channel_order = CL_INTENSITY;
	image_format2.image_channel_data_type = CL_SIGNED_INT32;
	image_format2.image_channel_order = CL_RGBA;

	Image2d imageWrite1(&context, CL_MEM_READ_WRITE, &image_format, N, N, 0, 0);
	Image2d imageWrite2(&context, CL_MEM_READ_WRITE, &image_format2, N, N, 0, 0);

	imageWrite1.writeImage2d(&command_queue, F); 
	imageWrite2.writeImage2d(&command_queue, mx);
	//АРГУМЕНТЫ ЯДРА
	kernel.SetKernelArg(&imageWrite1, 0);
	kernel.SetKernelArg(&imageWrite2, 1);
	//Старт ядра
	kernel.startKernel(&command_queue, 1, NULL, &sizeAll, &sizeGroup, 0, 0, 0);
	//получение результатов вычислений ядра - МАССИВ
	imageWrite1.readImage2d(&command_queue, F);
	//imageWrite2.readImage2d(&command_queue, mx);
	//вывод результатов на экран
	std::ofstream out("outLab4.txt");
	out << "\nМассив : \n";
	for (int i = 0; i < N; i++)
	{
		printMas(F[i], N, out);
	}
	out.close();

	for (int i = 0; i < N; i++)
	{
		delete[N] F[i];
		delete[N] mx[i];
	}
	delete[N] F;
	delete[N] mx; 
}

void lab4()
{
	setlocale(LC_ALL, "rus");
	int mode;
	do
	{
		printf("0 - без использования изображений. \n");
		printf("1 - с использованием изображений. \n");
		printf("Режим четвертой работы: ");
		scanf_s("%d", &mode);
	} while ((mode != 0) && (mode != 1));
	(mode ? lab4_2() : lab4_1());
}

void lab5()
{
	setlocale(LC_ALL, "rus");
	Platforms platforms;
	platforms.printInfo();

	Devices devices(platforms.getId((cl_uint)0));
	devices.printInfo();

	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devices.getPlatform(), 0 };
	Context context(properties, devices);

	CommandQueue command_queue(&context, devices.getId(0));

	bool ok;
	std::string F_x = "";
	const std::string symbols_F_x = "0123456789/*-+()x ";
	do
	{
		ok = false;
		printf("Введите функцию: ");
		std::cin >> F_x;
		for (auto i : F_x)
		{
			if (symbols_F_x.find(i) == std::string::npos)
			{
				ok = true;
			}
		}
	} while (ok);
	Opencl_source_read opencl_source_read("Lab5.txt");
	size_t sizeAll = 64, sizeGroup = 32;
	opencl_source_read.setTemplates("@\\{0\\}", F_x);
	opencl_source_read.setTemplates("@\\{1\\}", std::to_string(sizeGroup));
	const char* opencl_source = opencl_source_read.getConstChar();
	Program opencl_program(&context, 1, &opencl_source, "Kernel");
	opencl_program.buildProgram(&devices, 1);

	Kernel kernel(&opencl_program, "Kernel");
	kernel.printInfo(&devices);

	float A, B, h;
	do
	{
		printf("Введите левую границу: ");
		scanf_s("%f", &A);
		printf("Введите правую границу: ");
		scanf_s("%f", &B);
		printf("Введите шаг: ");
		scanf_s("%f", &h);
	} while (B - A - h <= 0 || h - 0.0000001 <= 0);

	const int sizeX = sizeAll;
	float* x = new float[sizeX];
	float* dest = new float[sizeX];
	float all_dest = 0;

	cl_event event_wr, wr_finish, finish_rd;
	Buffer buffer1(&context, CL_MEM_READ_WRITE, sizeX * sizeof(float), NULL);

	kernel.SetKernelArg(&buffer1, 0);
	kernel.SetKernelArg(1, sizeof(float), (void*)&h);

	float start = A;
	int leanth;
	do {
		leanth = 0;
		for (float i = start + h; leanth < sizeX && i - B <= 0; i = i + h)
		{
			x[leanth] = i;
			leanth = leanth + 1;
			if (leanth == sizeX)
			{
				break;
			}
		}
		start = x[leanth - 1];

		while (leanth != sizeX)
		{
			x[leanth] = B;
			leanth = leanth + 1;
		}

		buffer1.rnqueueWrite(&command_queue, CL_FALSE, 0, sizeX * sizeof(float), (void*)x, 0, 0, &event_wr);
		kernel.startKernel(&command_queue, 1, NULL, &sizeAll, &sizeGroup, 1, &event_wr, &wr_finish);
		buffer1.enqueueRead(&command_queue, CL_FALSE, 0, sizeX * sizeof(float), (void*)dest, 1, &wr_finish, &finish_rd);

		cl_int arg_int = clWaitForEvents(1, &finish_rd);
		if (arg_int != CL_SUCCESS) {
			printf("\nОшибка clWaitForEvents\n");
			exit(-1);
		}

		for (int i = 0; i < sizeX; i = i + 1)
		{
				all_dest += std::sqrtf(dest[i]);
		}
	} while (B - start - h > 0);

	printf("\nОтвет dest = (%f)\n", all_dest);

	delete[sizeX] x;
	delete[sizeX] dest;
}

int main(int argc, char* argv[])
{
	lab4();
	return 0;
}
