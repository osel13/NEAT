#define CL_HPP_MINIMUM_OPENCL_VERSION 100
#define CL_HPP_TARGET_OPENCL_VERSION 300

#include <iostream>
#include <fstream>
#include <vector>

#include "include_cl.h"
#include "win_test.h"

void win_test()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    auto platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

    auto device = devices.front();

    std::ifstream kernel_file("kernel.cl");
    std::string kernel_source(std::istreambuf_iterator<char>(kernel_file), (std::istreambuf_iterator<char>()));

    cl::Context context(device);
    cl::Program program(context, { kernel_source });

    auto err = program.build("-cl-std=CL1.2");

    char buf[16];
    cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));
    cl::Kernel kernel(program, "HelloWorld", &err);
    kernel.setArg(0, memBuf);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), buf);
}
