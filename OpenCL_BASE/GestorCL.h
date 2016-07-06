#pragma once
#include <iostream>
#include <CL/cl.hpp>

class GestorCL
{
public:
	GestorCL();
	~GestorCL();
	void Info();
	cl::Program build(std::string codigoCL);

};

