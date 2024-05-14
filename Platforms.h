#ifndef PLATFORMS_H
#define PLATFORMS_H

#include <string>

#include "CL/cl.h"

namespace openCLObject
{
	class Platforms
	{
	public:
		Platforms();
		Platforms(const Platforms& platforms);
		~Platforms();

		void printInfo();
		cl_platform_id getId(cl_uint num);
		cl_uint getNum();
	private:
		cl_platform_id* m_platforms;
		cl_uint m_num;
	};
}

#endif // PLATFORMS_H