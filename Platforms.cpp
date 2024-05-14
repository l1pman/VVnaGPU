#include "Platforms.h"

#include <cstdio>
#include <locale.h>
#include <stdlib.h>

namespace openCLObject
{

	Platforms::Platforms()
	{
		cl_int err;

		printf("\n___Определение OpenCL платформ.___");
		err = clGetPlatformIDs(0, 0, &m_num); //запрос числа OpenCL платформ
		if (err == CL_SUCCESS && m_num != 0)
			printf("\nОпределено %d OpenCL платформ.", m_num);
		else
		{
			printf("\nОшибка вызова clGetPlatformIDs или отсутствуют OpenCL платформы.\n");
			exit(-1);
		}
		//массив на num платформ
		m_platforms = new cl_platform_id[m_num];
		//запрос массива OpenCL платформ
		err = clGetPlatformIDs(m_num, m_platforms, 0);
		if (err != CL_SUCCESS)
		{
			printf("\nОшибка вызова clGetPlatformIDs.\n");
			exit(-1);
		}
		printf("\n___Конец___\n");
	}
	Platforms::Platforms(const Platforms& platforms)
	{
		m_num = platforms.m_num;
		m_platforms = new cl_platform_id[m_num];

		for (cl_uint i = 0; i < m_num; i++)
		{
			m_platforms[i] = platforms.m_platforms[i];
		}
	}
	Platforms::~Platforms()
	{
		delete[m_num] m_platforms;
	}

	void Platforms::printInfo()
	{
		printf("\n___Информация о доступных платформах.___");

		char str[250];
		for (cl_uint i = 0; i < m_num; i++) //цикл обхода всех платформ
		{
			printf("\n%d)", i + 1);
			clGetPlatformInfo(m_platforms[i], CL_PLATFORM_PROFILE, 250, (void*)str, 0);
			printf("PROFILE=%s", str);
			clGetPlatformInfo(m_platforms[i], CL_PLATFORM_VERSION, 250, (void*)str, 0);
			printf("\nVERSION=%s", str);
			clGetPlatformInfo(m_platforms[i], CL_PLATFORM_NAME, 250, (void*)str, 0);
			printf("\nNAME=%s", str);
			clGetPlatformInfo(m_platforms[i], CL_PLATFORM_VENDOR, 250, (void*)str, 0);
			printf("\nVENDOR=%s", str);
			clGetPlatformInfo(m_platforms[i], CL_PLATFORM_EXTENSIONS, 250, (void*)str, 0);
			printf("\nEXTENSIONS=%s", str);
		}
		printf("\n___Конец___\n");
	}

	cl_platform_id Platforms::getId(cl_uint num)
	{
		if(m_num < num)
		{
			return nullptr;
		}

		return m_platforms[num];
	}
	cl_uint Platforms::getNum()
	{
		return m_num;
	}
}