#ifndef OPEN_SOURCE_READ_H
#define OPEN_SOURCE_READ_H

#include <string>
#include <sstream>
#include <fstream>

namespace OpenclSource
{
	class Opencl_source_read
	{
	public:
		Opencl_source_read(std::string fileName);
		~Opencl_source_read();

		void setTemplates(std::string set, std::string get);
		const char* getConstChar();
	private:
		std::string m_opencl_source;
	};
}

#endif // OPEN_SOURCE_READ_H