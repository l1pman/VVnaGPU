#ifndef BUFFER_H
#define BUFFER_H

#include "CL/cl.h"

#include "CommandQueue.h"
#include "Context.h"

namespace openCLObject
{
	class Buffer
	{
	public:
		Buffer(Context *context, cl_mem_flags flags, size_t size, void* host_ptr);
		Buffer(const Buffer&) = delete;
		void operator=(const Buffer&) = delete;
		~Buffer();


		cl_mem getMemForGPU();
		void rnqueueWrite(CommandQueue *command_queue,
			cl_bool blocking_write,
			size_t offset,
			size_t cb,
			const void* ptr,
			cl_uint num_events_in_wait_list,
			const cl_event* event_wait_list,
			cl_event* event);
		void enqueueRead(CommandQueue* command_queue,
					cl_bool             blocking_read,
                    size_t              offset,
                    size_t              cb, 
                    void *              ptr,
                    cl_uint             num_events_in_wait_list,
                    const cl_event *    event_wait_list,
                    cl_event *          event);
	private:
		cl_mem m_mem_for_GPU;
	};
}

#endif // BUFFER_H
