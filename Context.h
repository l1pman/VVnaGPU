#ifndef CONTEXT_H
#define CONTEXT_H

#include "CL/cl.h"

#include "Devices.h"

namespace openCLObject
{
	class Context
	{
	public:
		Context(const cl_context_properties* properties, Devices devices);
		Context(const Context&) = delete;
		void operator=(const Context&) = delete;
		~Context();

		cl_context getContext();
	private:
		cl_context m_context;
	};
}
#endif // CONTEXT_H