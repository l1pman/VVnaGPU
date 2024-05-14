#include "Program.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{
	Program::Program(Context *context, cl_uint count, const char** opencl_source, const char* startProgramName)
	{
		m_startProgramName = startProgramName;
		cl_int arg_int;
		m_opencl_program = clCreateProgramWithSource(context->getContext(), count, opencl_source, NULL, &arg_int);
		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания программного объекта");
			exit(-1);
		}
		else printf("\n___Программный объект создан___\n");
	}
	Program::~Program()
	{
		clReleaseProgram(m_opencl_program);
	}

	void Program::buildProgram(Devices* devices, cl_uint count)
	{
		cl_int err = clBuildProgram(m_opencl_program, count, devices->getDevices(), NULL, NULL, NULL /*func_cl_program, (void*)devices*/);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка компиляции или сборки программы %d", err);
			exit(-1);
		}
		else printf("\n___Программа скомпилирована, создан исполняемый файл___\n");
	}
	cl_program Program::getProgram()
	{
		return m_opencl_program;
	}
}