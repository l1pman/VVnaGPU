#ifndef KERNEL_H
#define KERNEL_H

#include "CL/cl.h"

#include "Devices.h"
#include "Program.h"
#include "CommandQueue.h"
#include "Buffer.h"
#include "Image2d.h"

namespace openCLObject
{
	class Kernel
	{
	public:
		Kernel(Program *opencl_program, const char* name);
		Kernel(const Kernel&) = delete;
		void operator=(const Kernel&) = delete;
		~Kernel();

		void printInfo(Devices* devices);
		void SetKernelArg(Buffer* buffer, cl_uint arg_index);
		void SetKernelArg(Image2d* image2d, cl_uint arg_index);
		void SetKernelArg(cl_uint arg_index, size_t arg_size, const void* arg_value);
		void startKernel(CommandQueue* command_queue,
			cl_uint work_dim,
			const size_t* global_work_offset,
			const size_t* global_work_size,
			const size_t* local_work_size,
			cl_uint num_events_in_wait_list,
			const cl_event* event_wait_list,
			cl_event*       event);
	private:
		cl_kernel m_kernel;
	};
}

#endif // KERNEL_H