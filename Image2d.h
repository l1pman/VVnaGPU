#ifndef IMAGE2d_H
#define IMAGE2d_H

#include "CL/cl.h"
#include "Context.h"
#include "CommandQueue.h"

namespace openCLObject
{
	class Image2d
	{
	public:
		Image2d(Context *context, cl_mem_flags flags,
			const cl_image_format* image_format, size_t image_width,
			size_t image_height, size_t image_row_pitch, void*  host_ptr);
		Image2d(const Image2d&) = delete;
		void operator=(const Image2d&) = delete;
		~Image2d();

		void writeImage2d(CommandQueue *commandQueue, int **mass);
		void readImage2d(CommandQueue *commandQueue, int **mass);

		cl_mem getMemImage();
	private:
		int m_image_width;
		int m_image_height;
		cl_mem m_image;
	};
}

#endif // IMAGE2d_H