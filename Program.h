#ifndef PROGRAM_H
#define PROGRAM_H

#include "CL/cl.h"

#include "Devices.h"
#include "Context.h"

namespace openCLObject
{
	class Program
	{
	public:
		Program(Context *context, cl_uint count, const char** opencl_source, const char* startProgramName);
		Program(const Program&) = delete;
		void operator=(const Program&) = delete;
		~Program();

		void buildProgram(Devices* devices, cl_uint count);
		cl_program getProgram();
	private:
		cl_program m_opencl_program;
		const char* m_startProgramName;
	};
}

#endif // PROGRAM_H