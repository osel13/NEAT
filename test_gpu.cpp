#define __CL_ENABLE_EXCEPTIONS
#include <iostream>

#include "include_cl.h"
#include "test_gpu.h"

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
