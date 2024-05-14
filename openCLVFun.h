#pragma once

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

#include "CL/cl.h"

namespace openCLVFun
{
	void __stdcall func_cl_program(cl_program program, void* user_data)
	{
		printf("\n\tСобытие clBuildProgram");
		//получение параметра типа cl_device_id из void*
		//т.к. в примере устройство одно, то
		//заносим его в одну переменную, а не в массив
		cl_device_id device = *(cl_device_id*)user_data;
		char str[2000]; cl_int err;
		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 2000, (void*)str, NULL);
		if (err != CL_SUCCESS)
		{
			printf("\n\tОшибка вызова clGetProgramBuildInfo"); return;
		}
		//если строка лога сборщика пустая, выйдем из функции
		if (str[0] == '\0' || str[0] == '\n') return;
		printf("\n\tЛОГ OpenCL СБОРЩИКА:");
		printf("\n%s\n", str);
	}

	struct Platforms
	{
		cl_platform_id* platforms;
		cl_uint num;
	};

	Platforms* initPlatforms()
	{
		Platforms* platforms = new Platforms;
		cl_int err;

		printf("\n___Определение OpenCL платформ.___");
		err = clGetPlatformIDs(0, 0, &platforms->num); //запрос числа OpenCL платформ
		if (err == CL_SUCCESS && platforms->num != 0)
			printf("\nОпределено %d OpenCL платформ.", platforms->num);
		else
		{
			printf("\nОшибка вызова clGetPlatformIDs или отсутствуют OpenCL платформы.\n");
			exit(-1);
		}
		//массив на num платформ
		platforms->platforms = new cl_platform_id[platforms->num];
		//запрос массива OpenCL платформ
		err = clGetPlatformIDs(platforms->num, platforms->platforms, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetPlatformIDs.\n");
			exit(-1);
		}
		printf("\n___Конец___\n");

		return platforms;
	}
	void printPlatformsInfo(Platforms* platforms)
	{
		printf("\n___Информация о доступных платформах.___");
		if (platforms == nullptr)
		{
			printf("\n___Ошибка на входе NULL___");
			exit(-1);
		}

		char str[250];
		for (cl_uint i = 0; i < platforms->num; i++) //цикл обхода всех платформ
		{
			printf("\n%d)", i + 1);
			clGetPlatformInfo(platforms->platforms[i], CL_PLATFORM_PROFILE, 250,
				(void*)str, 0);
			printf("PROFILE=%s", str);
			clGetPlatformInfo(platforms->platforms[i], CL_PLATFORM_VERSION, 250,
				(void*)str, 0);
			printf("\nVERSION=%s", str);
			clGetPlatformInfo(platforms->platforms[i], CL_PLATFORM_NAME, 250,
				(void*)str, 0);
			printf("\nNAME=%s", str);
			clGetPlatformInfo(platforms->platforms[i], CL_PLATFORM_VENDOR, 250,
				(void*)str, 0);
			printf("\nVENDOR=%s", str);
			clGetPlatformInfo(platforms->platforms[i], CL_PLATFORM_EXTENSIONS,
				250, (void*)str, 0);
			printf("\nEXTENSIONS=%s", str);
		}
		printf("\n___Конец___\n");
	}
	void dellPlatforms(Platforms* platforms)
	{
		delete[platforms->num] platforms->platforms;
		delete platforms;
		platforms = nullptr;
	}


	struct Devices
	{
		cl_platform_id platform;
		cl_device_id* devices;
		cl_uint numd;
	};

	Devices* initDevices(Platforms* platforms, cl_uint num)
	{
		Devices* devices = new Devices;

		printf("\n___Определение OpenCL устройств платформы %d.___", num);
		//число OpenCL устройств
		cl_int err = clGetDeviceIDs(platforms->platforms[num], CL_DEVICE_TYPE_ALL, 0, 0, &devices->numd);
		if (err == CL_SUCCESS && devices->numd != 0)
			printf("\nОпределено %d OpenCL устройств.", devices->numd);
		else
		{
			printf("\n\nОшибка вызова clGetDeviceIDs или отсутствуют OpenCL устройства.\n");
			exit(-1);
		}
		devices->platform = platforms->platforms[num];

		//массив OpenCL устройств
		devices->devices = new cl_device_id[devices->numd];
		//получение массива устройств
		err = clGetDeviceIDs(devices->platform, CL_DEVICE_TYPE_ALL, devices->numd, devices->devices, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetDeviceIDs.\n");
			exit(-1);
		}
		printf("\n___Конец___\n");


		return devices;
	}
	void printDevicesInfo(Devices* devices)
	{
		printf("\n___Информация о доступных устройсвах.___");
		if (devices == nullptr)
		{
			printf("\n___Ошибка на входе NULL___");
			exit(-1);
		}

		char str[250];
		cl_uint arg_uint;
		for (cl_uint i = 0; i < devices->numd; i++) //цикл обхода всех устройств
		{
			printf("\n%d)", i + 1);
			clGetDeviceInfo(devices->devices[i], CL_DEVICE_NAME, 250, (void*)str, 0);
			printf("NAME=%s", str);
			clGetDeviceInfo(devices->devices[i], CL_DEVICE_OPENCL_C_VERSION, 250,
				(void*)str, 0);
			printf("\nOPENCL_VERSION=%s", str);
			clGetDeviceInfo(devices->devices[i], CL_DEVICE_VENDOR, 250, (void*)str, 0);
			printf("\nVENDOR=%s", str);
			clGetDeviceInfo(devices->devices[i],
				CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(arg_uint), (void*)
				&arg_uint, 0);
			printf("\nMAX_CLOCK_FREQUENCY=%d MHz", arg_uint);
			clGetDeviceInfo(devices->devices[i], CL_DEVICE_MAX_COMPUTE_UNITS,
				sizeof(arg_uint), (void*)&arg_uint, 0);
			printf("\nMAX_COMPUTE_UNITS=%d", arg_uint);
		}
		printf("\n___Конец___\n");
	}
	void dellDevices(Devices* devices)
	{
		delete[devices->numd] devices->devices;
		delete devices;
		devices = nullptr;
	}

	// Создания контекста
	cl_context createContext(Devices* devises/*, cl_context_properties properties[]*/)
	{
		cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)devises->platform, 0 };

		cl_int arg_int;
		//создание контекста context
		cl_context context = clCreateContext(properties, devises->numd, devises->devices, NULL, NULL, &arg_int);
		//передали в контекст платформу (через свойства properties)
		//и массив OpenCL устройств devices количеством numd
		if (arg_int != CL_SUCCESS)
		{
			printf("\n\nОшибка создания контекста");
			exit(-1);
		}
		else printf("\n___Контекст создан___\n");

		return context;
	}

	// Создания командной очереди
	cl_command_queue createCommandQueue(Devices* devises, cl_uint num, cl_context context)
	{
		cl_int arg_int;
		cl_command_queue command_queue = clCreateCommandQueue(context, devises->devices[num], 0, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания очереди команд");
			exit(-1);
		}
		else printf("\n___Очередь команд создана___\n");

		return command_queue;
	}

	// Работа с 
	cl_program createProgramWithSource(cl_context context, cl_uint count, const char** opencl_source)
	{
		cl_int arg_int;
		//создание программного объекта из одной (1) строки opencl_source
		cl_program opencl_program = clCreateProgramWithSource(context, count, opencl_source, NULL, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания программного объекта");
			exit(-1);
		}
		else printf("\n___Программный объект создан___\n");

		return opencl_program;
	}
	void buildProgram(cl_program opencl_program, Devices* devices, cl_uint count)
	{
		cl_int err = clBuildProgram(opencl_program, count, devices->devices, NULL, NULL, NULL /*func_cl_program, (void*)devices*/);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка компиляции или сборки программы");
			exit(-1);
		}
		else printf("\n___Программа скомпилирована, создан исполняемый файл___\n");
	}


	cl_kernel createKernel(cl_program opencl_program, const char* name)
	{
		cl_int arg_int;
		cl_kernel kernel = clCreateKernel(opencl_program, name, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания объекта ядра");
			exit(-1);
		}
		else printf("\n___Объект ядра создан___\n");
		return kernel;
	}
	void printKernelWorkGroupInfo(cl_kernel kernel, Devices* devices, cl_uint num)
	{
		size_t SIZE[3] = {0, 0, 0};
		cl_int err = clGetKernelWorkGroupInfo(kernel, devices->devices[num], CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), (void*)&SIZE[0], NULL);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetKernelWorkGroupInfo");
			exit(-1);
		}
		else printf("\nМаксим-е кол-во рабочих элементов в одной группе=%zu", SIZE[0]);
		err = clGetKernelWorkGroupInfo(kernel, devices->devices[num], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), (void*)&SIZE[0], NULL);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetKernelWorkGroupInfo");
			exit(-1);
		}
		else printf("\nРекомендованное кратное для кол-ва рабочих элементов в группе = %zu \n", SIZE[0]);
	}

	cl_mem createBuffer(cl_context context, cl_mem_flags flags, size_t size, void* host_ptr)
	{
		cl_int arg_int;
		cl_mem mem_for_GPU = clCreateBuffer(context, flags, size, host_ptr, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания объекта памяти");
			exit(-1);
		}
		else printf("\n___Объект памяти создан___\n");

		return mem_for_GPU;
	}
	void rnqueueWriteBuffer(cl_command_queue command_queue,
		cl_mem buffer,
		cl_bool blocking_write,
		size_t offset,
		size_t cb,
		const void* ptr,
		cl_uint num_events_in_wait_list,
		const cl_event* event_wait_list,
		cl_event* event)
	{
		cl_int err = clEnqueueWriteBuffer(command_queue, buffer, blocking_write, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
		if (err != CL_SUCCESS) {
			printf("\nОшибка вызова clEnqueueWriteBuffer (mem_for_CPU->mem_for_GPU)");
			exit(-1);
		}
	}
	void colculeton(cl_command_queue command_queue, cl_kernel kernel, cl_mem mem_for_GPU)
	{
		size_t SIZE[3] = {1, 0, 0};
		cl_int err = clSetKernelArg(kernel, 0, sizeof(mem_for_GPU), (void*)&mem_for_GPU);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clSetKernelArg");
			exit(-1);
		}

		SIZE[0] = 1; SIZE[1] = 0; SIZE[2] = 0;
		err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, SIZE, SIZE, 0, 0, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка запуска ядра");
			exit(-1);
		}
		else printf("\n___Ядро запущено на OpenCL устройстве___");

		err = clFinish(command_queue);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка завершения ядра");
			exit(-1);
		}
		else printf("\n___Вычисление ядра завершено OpenCL устройством___\n");
	}
	void enqueueReadBuffer(cl_command_queue command_queue, cl_mem mem_for_GPU, int* mem_for_CPU)
	{
		printf("\nmem_for_CPU до вычислений ядра = %d", *mem_for_CPU);
		cl_int err = clEnqueueReadBuffer(command_queue, mem_for_GPU, CL_TRUE, 0, sizeof(int), (void*)mem_for_CPU, 0, 0, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка clEnqueueReadBuffer");
			exit(-1);
		}
		else printf("\nРезультат вычислений ядра = %d", *mem_for_CPU);
	}


	void V1()
	{
		Platforms* platforms = initPlatforms();
		printPlatformsInfo(platforms);

		Devices* devices = initDevices(platforms, 0);
		printDevicesInfo(devices);

		cl_context context = createContext(devices);
		cl_command_queue command_queue = createCommandQueue(devices, 0, context);

		const char* opencl_source = "__kernel void Simple (__global int *n){*n = (*n) * 2; } \n";
		cl_program opencl_program = createProgramWithSource(context, 1, &opencl_source);
		buildProgram(opencl_program, devices, 1);
		cl_kernel kernel = createKernel(opencl_program, "Simple");

		int mem_for_CPU = 20;
		cl_mem mem_for_GPU = createBuffer(context, CL_MEM_READ_WRITE, sizeof(mem_for_CPU), NULL);
		rnqueueWriteBuffer(command_queue, mem_for_GPU, CL_TRUE, 0, sizeof(mem_for_CPU), (const void*)&mem_for_CPU, 0, 0, 0);

		printKernelWorkGroupInfo(kernel, devices, 0);
		colculeton(command_queue, kernel, mem_for_GPU);

		enqueueReadBuffer(command_queue, mem_for_GPU, &mem_for_CPU);

		//освобождение OpenCL ресурсов
		clReleaseMemObject(mem_for_GPU);
		clReleaseCommandQueue(command_queue);
		clReleaseKernel(kernel);
		clReleaseProgram(opencl_program);
		clReleaseContext(context);


		dellDevices(devices);
		dellPlatforms(platforms);
	}
}
