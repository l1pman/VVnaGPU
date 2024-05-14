#include "Context.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{
	Context::Context(const cl_context_properties *properties, Devices devices)
	{

		cl_int arg_int;
		m_context = clCreateContext(properties, devices.getNumd(), devices.getDevices(), NULL, NULL, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\n\nОшибка создания контекста");
			exit(-1);
		}
		else printf("\n___Контекст создан___\n");

	}
	Context::~Context()
	{
		clReleaseContext(m_context);
	}

	cl_context Context::getContext()
	{
		return m_context;
	}
}