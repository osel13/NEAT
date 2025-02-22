#define SIZE 10

#include <iostream>
#define CL_USE_DEPRECATED_OPENCL_3_0_APIS

#include "include_cl.h"
#include <vector>
#include <string>

std::string kernel_code =
    "       void kernel func(global const float* A, global const float* B, global float* C){"
    "       int mw = 10;" // matrix width
    "       int cindex;"  // = row*mw + col;" //c[i][j]
    "       float cij;"   // matrix val
    "       for(int i = get_global_id(0);i<mw;i++){"
    "       for(int j = get_global_id(1);j<mw;j++){"
    "       cindex = i*mw + j;"
    "       cij = A[i]*B[j];"
    "       C[cindex] = cij;"
    "       }"
    "       }"
    "    }";
/*"   void kernel simple_add(global const int* A, global const int* B, global int* C){ "
"       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                   "
"        C[get_global_id(0)]=pow(A[get_global_id(0)],B[get_global_id(0)]);
"   }                                                                               ";*/

int scratch_test()
{

    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0)
    {
        std::cout << " No OpenCL platforms found" << std::endl;
        exit(1);
    }

    // identify platform
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

    // identify device
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0)
    {
        std::cout << "No devices found" << std::endl;
        exit(2);
    }

    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << std::endl;

    // context
    cl::Context context(default_device);

    // Allocating memory -> Buffer
    //*_data = host
    //*_buffer = device
    float A_data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    float B_data[] = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    float C_data[SIZE * SIZE];

    cl::Buffer A_buffer(context, CL_MEM_READ_WRITE, sizeof(A_data));
    cl::Buffer B_buffer(context, CL_MEM_READ_WRITE, sizeof(B_data));
    cl::Buffer C_buffer(context, CL_MEM_READ_WRITE, sizeof(C_data)); // sizeof(float) * SIZE);

    // Queue
    cl::CommandQueue queue(context, default_device);

    // Writing into device memory
    queue.enqueueWriteBuffer(A_buffer, CL_TRUE, 0, sizeof(A_data), A_data);
    queue.enqueueWriteBuffer(B_buffer, CL_TRUE, 0, sizeof(B_data), B_data);

    // Kernel
    // Organizing source code
    cl::Program::Sources sources;

    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);

    if (program.build({default_device}) != CL_SUCCESS)
    {
        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        exit(1);
    }

    // add function to Kernel on device
    cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> func(cl::Kernel(program, "func"));

    cl::NDRange global(SIZE);

    // Krenel! Metelesku blesku!
    func(cl::EnqueueArgs(queue, global), A_buffer, B_buffer, C_buffer).wait();
    queue.enqueueNDRangeKernel(func, cl::NullRange, global, cl::NullRange);
    // Kernel execution on the device
    // Retrieving data from device
    queue.enqueueReadBuffer(C_buffer, CL_TRUE, 0, sizeof(float) * SIZE * SIZE, C_data);

    // read result
    std::cout << " result:" << std::endl;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            int k = i * SIZE + j;
            std::cout << C_data[k] << " ";
        }
        std::cout << std::endl;
    }
}
