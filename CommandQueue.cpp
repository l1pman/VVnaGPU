#include "CommandQueue.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{
	CommandQueue::CommandQueue(Context *context, cl_device_id device, cl_command_queue_properties command_queue_properties)
	{
		cl_int arg_int;
		m_command_queue = clCreateCommandQueue(context->getContext(), device, command_queue_properties, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания очереди команд");
			exit(-1);
		}
		else printf("\n___Очередь команд создана___\n");
	}
	CommandQueue::~CommandQueue()
	{
		clReleaseCommandQueue(m_command_queue);
	}

	cl_command_queue CommandQueue::getCommandQueue()
	{
		return m_command_queue;
	}
}