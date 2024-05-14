#ifndef DEVICES_H
#define DEVICES_H

#include "CL/cl.h"
#include "Platforms.h"


namespace openCLObject
{
	class Devices
	{
	public:
		Devices(cl_platform_id platform);
		Devices(const Devices &devices);
		~Devices();

		void printInfo();
		cl_device_id getId(cl_uint numd);
		cl_platform_id getPlatform();
		cl_uint getNumd();
		cl_device_id* getDevices();
	private:
		cl_platform_id m_platform;
		cl_device_id* m_devices;
		cl_uint m_numd;
	};
}

#endif // DEVICES_H
