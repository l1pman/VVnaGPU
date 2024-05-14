#include "Kernel.h"

namespace openCLObject
{
	Kernel::Kernel(Program *opencl_program, const char* name)
	{
		cl_int arg_int;
		m_kernel = clCreateKernel(opencl_program->getProgram(), name, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания объекта ядра");
			exit(arg_int);
		}
		else printf("\n___Объект ядра создан___\n");
	}
	Kernel::~Kernel()
	{
		clReleaseKernel(m_kernel);
	}

	void Kernel::printInfo(Devices* devices)
	{
		char str[250];
		size_t size_param;
		cl_uint num_args = 0;
		printf("\n___Информация о ядре.___");
		clGetKernelInfo(m_kernel, CL_KERNEL_FUNCTION_NAME, 250, (void*)str, &size_param);
		printf("\nKERNEL_FUNCTION_NAME=%s", str);
		clGetKernelInfo(m_kernel, CL_KERNEL_NUM_ARGS, 250, (void*)&num_args, &size_param);
		printf("\nNUM_ARGS=%x", num_args);

		size_t SIZE[3];
		cl_int err;
		err = clGetKernelWorkGroupInfo(m_kernel, *devices->getDevices(), CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), (void*)&SIZE[0], NULL);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetKernelWorkGroupInfo"); exit(-1);
		}
		else printf("\nМаксим-е кол-во рабочих элементов в одной группе = %d", int(SIZE[0])) ;
		err = clGetKernelWorkGroupInfo(m_kernel, *devices->getDevices(), CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), (void*)&SIZE[0], NULL);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetKernelWorkGroupInfo"); exit(-1);
		}
		else printf("\nРекомендованное кратное для кол-ва рабочих элементов в группе = %d", int(SIZE[0])) ;
		printf("\n___Конец___\n");

	}
	void Kernel::SetKernelArg(Buffer* buffer, cl_uint arg_index)
	{
		cl_mem mem_for_GPU = buffer->getMemForGPU();
		cl_int err = clSetKernelArg(m_kernel, arg_index, sizeof(mem_for_GPU), (void*)&mem_for_GPU);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clSetKernelArg");
			exit(err);
		}
	}
	void Kernel::SetKernelArg(Image2d* image2d, cl_uint arg_index)
	{
		cl_mem mem_for_GPU = image2d->getMemImage();
		cl_int err = clSetKernelArg(m_kernel, arg_index, sizeof(mem_for_GPU), (void*)&mem_for_GPU);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clSetKernelArg imageRead");
			exit(err);
		}
	}
	void Kernel::SetKernelArg(cl_uint arg_index, size_t arg_size, const void* arg_value)
	{
		cl_int err = clSetKernelArg(m_kernel, arg_index, arg_size, arg_value);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clSetKernelArg");
			exit(-1);
		}
	}
	void Kernel::startKernel(CommandQueue* command_queue,
		cl_uint work_dim,
		const size_t* global_work_offset,
		const size_t* global_work_size,
		const size_t* local_work_size,
		cl_uint num_events_in_wait_list,
		const cl_event* event_wait_list,
		cl_event* event)
	{
		cl_int err = clEnqueueNDRangeKernel(command_queue->getCommandQueue(), m_kernel, work_dim,
			global_work_offset, global_work_size, local_work_size,
			num_events_in_wait_list, event_wait_list, event);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка запуска ядра %d", err);
			exit(-1);
		}
		else printf("\n___Ядро запущено на OpenCL устройстве___");

		err = clFinish(command_queue->getCommandQueue());
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка завершения ядра %d", err);
			exit(-1);
		}
		else printf("\n___Вычисление ядра завершено OpenCL устройством___\n");
	}
}