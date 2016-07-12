#include "FactoryOpenCL.h"


FactoryOpenCL::FactoryOpenCL()
{
	try{
		std::vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		if (platform.empty()) {
			std::cerr << "No se encuentran plataformas OpenCL Disponibles." << std::endl;
		}


		// Get first available GPU device which supports double precision.
		for (auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
			std::vector<cl::Device> pldev;

			try {
				p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

				for (auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
					if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

					std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

					if (
						ext.find("cl_khr_fp64") == std::string::npos &&
						ext.find("cl_amd_fp64") == std::string::npos
						) continue;

					device.push_back(*d);
					context = cl::Context(device);
				}
			}
			catch (...) {
				device.clear();
			}
		}

		if (device.empty()) {
			std::cerr << "No se han detectado GPUs disponibles" << std::endl;
		}

		std::cout << "Se utilizará el siguiente GPU : " << device[0].getInfo<CL_DEVICE_NAME>() << std::endl;

		cl::CommandQueue qq(context, device[0]);
		queue = qq;

	}
	catch (...){

	}
}


FactoryOpenCL::~FactoryOpenCL()
{
}

void FactoryOpenCL::compilarFuente(std::string irl){

	using namespace std;

	std::ifstream cl_file(irl);
	std::string cl_string(istreambuf_iterator<char>(cl_file), (istreambuf_iterator<char>()));
	cl::Program::Sources source(1, make_pair(cl_string.c_str(), cl_string.length() + 1));

	cl::Program pro(context, source);

	try {
		pro.build(device);
	}
	catch (...) {
		std::cerr << "OpenCL compilation error" << std::endl << pro.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0]) << std::endl;
	}
	programa = pro;
}

cl::Kernel FactoryOpenCL::crearKernel(const char* nombreFuncion){

	cl::Kernel fun(programa, nombreFuncion);
	return fun;
}