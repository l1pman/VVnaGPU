#include "Image2d.h"


namespace openCLObject
{
	Image2d::Image2d(Context *context, cl_mem_flags flags,
		const cl_image_format* image_format, size_t image_width,
		size_t image_height, size_t image_row_pitch, void* host_ptr)
		: m_image_width(image_width), m_image_height(image_height)
	{
		cl_int arg_int;
		m_image = clCreateImage2D(context->getContext(), flags,
			image_format, image_width, image_height, image_row_pitch, host_ptr, &arg_int);

		if (arg_int != CL_SUCCESS)
		{
			printf("\nОшибка создания изображение");
			exit(-1);
		}
		else printf("\n___Изображение создано___\n");
	}
	Image2d::~Image2d()
	{
		clReleaseMemObject(m_image);
	}

	void Image2d::writeImage2d(CommandQueue *commandQueue, int **mass)
	{
		cl_int err;
		size_t origin[3] = {0, 0, 0};
		size_t region[3] = {1, m_image_height, 1};
		for (int i = 0; i < m_image_width; i++)
		{
			err = clEnqueueWriteImage(commandQueue->getCommandQueue(), m_image, CL_TRUE, origin, region, 0, 0, (void*)mass[i], 0, 0, 0);
			if (err != CL_SUCCESS) {
				printf("\nОшибка clEnqueueWriteImage, Номер строки=%d", i);
				exit(-1);
			}
			origin[0]++;
		}
	}
	void Image2d::readImage2d(CommandQueue *commandQueue, int** mass)
	{
		cl_int err;
		size_t origin[3] = { 0, 0, 0 };
		size_t region[3] = { 1, m_image_height, 1 };

		for (int i = 0; i < m_image_width; i++)
		{
			err = clEnqueueReadImage(commandQueue->getCommandQueue(), m_image, CL_TRUE, origin, region, 0, 0, (void*)mass[i], 0, 0, 0);
			if (err != CL_SUCCESS) {
				printf("\nОшибка clEnqueueReadImage, i = % d", i);
				exit(-1);
			}
			//следующую строку нужно считывать с нового индекса (смещения)
			origin[0]++;
		}
	}

	cl_mem Image2d::getMemImage()
	{
		return m_image;
	}
}