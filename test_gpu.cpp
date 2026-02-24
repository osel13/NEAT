#define __CL_ENABLE_EXCEPTIONS
#include <cstdlib>
#include <iostream>
#include <vector>

#include "include_cl.h"
#include "test_gpu.h"

#if !NEAT_HAS_OPENCL
int test_gpu()
{
	std::cout << "OpenCL headers not available" << std::endl;
	return 0;
}
#else
int test_gpu()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if (platforms.size() == 0)
	{
		std::cout << "No OpenCL platforms found" << std::endl;
		exit(1);
	}

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
	cl::Device device = devices[0];

    std::cout << "Devices detected:" << std::endl;
    for (unsigned int i = 0; i < devices.size(); i++)
    {
        cl::Device device_to_show = devices[i];
        std::cout << "Device number "<< i <<" : " << device_to_show.getInfo<CL_DEVICE_NAME>() << std::endl;
    }

	return 0;
}
#endif
