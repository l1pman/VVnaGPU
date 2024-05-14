#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include "CL/cl.h"

#include "Devices.h"
#include "Context.h"

namespace openCLObject
{
	class CommandQueue
	{
	public:
		CommandQueue(Context *context, cl_device_id device, cl_command_queue_properties command_queue_properties = 0);
		CommandQueue(const CommandQueue&) = delete;
		void operator=(const CommandQueue&) = delete;
		~CommandQueue();

		cl_command_queue getCommandQueue();
	private:

	private:
		cl_command_queue m_command_queue;
	};
}

#endif // COMMANDQUEUE_H

