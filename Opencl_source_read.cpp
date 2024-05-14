#include "Opencl_source_read.h"

#include <regex>
#include <iostream>

namespace OpenclSource
{
	Opencl_source_read::Opencl_source_read(std::string fileName)
	{
		std::ifstream in(fileName);
		std::stringstream ss;
		ss << in.rdbuf();
		m_opencl_source = ss.str();
		in.close();
	}

	Opencl_source_read::~Opencl_source_read()
	{
	}

	void Opencl_source_read::setTemplates(std::string set, std::string get)
	{
	    std::regex rx(set);
		std::string temp(std::regex_replace(m_opencl_source, rx, get));
		m_opencl_source = temp;
	}
	const char* Opencl_source_read::getConstChar()
	{
		return m_opencl_source.c_str();
	}
}