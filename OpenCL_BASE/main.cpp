#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "FactoryOpenCL.h"

using namespace std;


int main() {
	const size_t N = 1 << 20;
	try{
		FactoryOpenCL *program = new FactoryOpenCL();

		program->compilarFuente("./kernel.cl");
		cl::Kernel add = program->crearKernel("add");
		cl::CommandQueue queue = program->cola();
		const clock_t begin_time = std::clock();
		// Prepare input data.
		std::vector<float> a(N, 1);
		std::vector<float> b(N, 2);
		std::vector<float> c(N, 0);

		// Allocate device buffers and transfer input data to device.
		cl::Buffer A(program->contexto(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, a.size() * sizeof(float), a.data());

		cl::Buffer B(program->contexto(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, b.size() * sizeof(float), b.data());

		cl::Buffer C(program->contexto(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, c.size() * sizeof(float), c.data());

		// Set kernel parameters.
		add.setArg(0, C);
		add.setArg(1, B);
		add.setArg(2, C);

		// Launch kernel on the compute device.
		queue.enqueueNDRangeKernel(add, cl::NullRange, N , cl::NullRange);
		queue.enqueueNDRangeKernel(add, cl::NullRange, N , cl::NullRange);


		// Get result back to host.
		queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(float), c.data());

		// Should get '3' here.
		std::cout << c[N-1] << std::endl;
	}
	catch (const cl::Error &err) {
		std::cerr
			<< "OpenCL error: "
			<< err.what() << "(" << err.err() << ")"
			<< std::endl;
		system("pause");
		return 1;
	}
	system("pause");
}