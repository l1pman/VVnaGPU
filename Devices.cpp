#include "Devices.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{

	Devices::Devices(cl_platform_id platform)
	{

		printf("\n___Определение OpenCL устройств платформы.___");
		//число OpenCL устройств
		cl_int err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, 0, &m_numd);
		if (err == CL_SUCCESS && m_numd != 0)
			printf("\nОпределено %d OpenCL устройств.", m_numd);
		else
		{
			printf("\n\nОшибка вызова clGetDeviceIDs или отсутствуют OpenCL устройства.\n");
			exit(-1);
		}
		m_platform = platform;

		//массив OpenCL устройств
		m_devices = new cl_device_id[m_numd];
		//получение массива устройств
		err = clGetDeviceIDs(m_platform, CL_DEVICE_TYPE_ALL, m_numd, m_devices, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetDeviceIDs.\n");
			exit(-1);
		}
		printf("\n___Конец___\n");

	}
	Devices::Devices(const Devices &devices)
	{
		m_numd = devices.m_numd;
		m_platform = devices.m_platform;
		m_devices = new cl_device_id[m_numd];

		for(cl_uint i = 0; i < m_numd; i++)
		{
			m_devices[i] = devices.m_devices[i];
		}
	}
	Devices::~Devices()
	{
		delete[m_numd] m_devices;
	}
	
	void Devices::printInfo()
	{
		printf("\n___Информация о доступных устройсвах.___");

		char str[250];
		cl_uint arg_uint;
		for (cl_uint i = 0; i < m_numd; i++) //цикл обхода всех устройств
		{
			printf("\n%d)", i + 1);
			clGetDeviceInfo(m_devices[i], CL_DEVICE_NAME, 250, (void*)str, 0);
			printf("NAME=%s", str);
			clGetDeviceInfo(m_devices[i], CL_DEVICE_OPENCL_C_VERSION, 250, (void*)str, 0);
			printf("\nOPENCL_VERSION=%s", str);
			clGetDeviceInfo(m_devices[i], CL_DEVICE_VENDOR, 250, (void*)str, 0);
			printf("\nVENDOR=%s", str);
			clGetDeviceInfo(m_devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(arg_uint), (void*)&arg_uint, 0);
			printf("\nMAX_CLOCK_FREQUENCY=%d MHz", arg_uint);
			clGetDeviceInfo(m_devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(arg_uint), (void*)&arg_uint, 0);
			printf("\nMAX_COMPUTE_UNITS=%d", arg_uint);
		}
		printf("\n___Конец___\n");
	}
	cl_device_id Devices::getId(cl_uint numd)
	{
		if (m_numd < numd)
		{
			return nullptr;
		}

		return m_devices[numd];
	}
	cl_platform_id Devices::getPlatform()
	{
		return m_platform;
	}
	cl_uint Devices::getNumd()
	{
		return m_numd;
	}
	cl_device_id* Devices::getDevices()
	{
		return m_devices;
	}
}