#pragma once
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <CL/cl.hpp>

class FactoryOpenCL
{
private:
	cl::Context context;
	std::vector<cl::Device> device;
	cl::CommandQueue queue;
	cl::Program programa;

public:
	FactoryOpenCL();
	~FactoryOpenCL();
	void Info();

	void compilarFuente(std::string fuente);
	cl::CommandQueue cola(){ return queue; };
	cl::Context contexto(){ return context; };
	cl::Kernel crearKernel(const char* nombreFuncion);

};

