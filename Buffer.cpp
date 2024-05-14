#include "Buffer.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{
	Buffer::Buffer(Context *context, cl_mem_flags flags, size_t size, void* host_ptr)
	{
		cl_int arg_int;
		m_mem_for_GPU = clCreateBuffer(context->getContext(), flags, size, host_ptr, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания объекта памяти");
			exit(arg_int);
		}
		else printf("\n___Объект памяти создан___\n");
	}
	Buffer::~Buffer()
	{
		clReleaseMemObject(m_mem_for_GPU);
	}

	cl_mem Buffer::getMemForGPU()
	{
		return m_mem_for_GPU;
	}
	void Buffer::rnqueueWrite(CommandQueue *command_queue,
		cl_bool blocking_write,
		size_t offset,
		size_t cb,
		const void* ptr,
		cl_uint num_events_in_wait_list,
		const cl_event* event_wait_list,
		cl_event* event)
	{
		cl_int err = clEnqueueWriteBuffer(command_queue->getCommandQueue(), m_mem_for_GPU, blocking_write, offset, cb, ptr, num_events_in_wait_list, event_wait_list, event);
		if (err != CL_SUCCESS) {
			printf("\nОшибка вызова clEnqueueWriteBuffer (mem_for_CPU->mem_for_GPU)");
			exit(err);
		}
	}
	void Buffer::enqueueRead(CommandQueue* command_queue,
		cl_bool             blocking_read,
		size_t              offset,
		size_t              cb,
		void*				ptr,
		cl_uint             num_events_in_wait_list,
		const cl_event* event_wait_list,
		cl_event* event)
	{
		//printf("\nmem_for_CPU до вычислений ядра = %d", *mem_for_CPU);
		cl_int err = clEnqueueReadBuffer(command_queue->getCommandQueue(), m_mem_for_GPU, blocking_read, offset, cb,
			ptr, num_events_in_wait_list, event_wait_list, event);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка clEnqueueReadBuffer");
			exit(-1);
		}
		//else printf("\nРезультат вычислений ядра = %d", *mem_for_CPU);
	}


}