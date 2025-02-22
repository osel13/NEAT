#define SIZE 10

#include <iostream>
#define CL_USE_DEPRECATED_OPENCL_3_0_APIS
#include <CL/cl.hpp>
#include <vector>
#include <string>

std::string kernel_code =
    "       void kernel func(global const float* A, global const float* B, global float* C){"
    "       int mw = 10;" //matrix width
    //"       int row = get_global_id(0);" //matrix filled row
    //"       int col = get_global_id(1);" //matrix filled column
    //"       int aindex = row;" //a[i]
    //"       int bindex = col;" //b[i]
    "       int cindex;"// = row*mw + col;" //c[i][j]
    "       float cij;" //matrix val
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
int main(int arg, char* args[])
{
    
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cout << " No OpenCL platforms found" << std::endl;
        exit(1);
    }
    
    
    //identify platform
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " <<default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

    //identify device
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() ==0) {
        std::cout << "No devices found" << std::endl;
        exit(1);
    }
    
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " <<default_device.getInfo<CL_DEVICE_NAME>() << std::endl;
    
    //context
    cl::Context context({default_device});
    
    cl::Buffer A_d(context, CL_MEM_READ_WRITE, sizeof(float) * SIZE);
    cl::Buffer B_d(context, CL_MEM_READ_WRITE, sizeof(float) * SIZE);
    cl::Buffer C_d(context, CL_MEM_READ_WRITE, sizeof(float) * SIZE*SIZE);//sizeof(float) * SIZE);
    
    //Allocating memory -> Buffer
    //*_h = host
    //*_d = device
    
    float A_h[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    float B_h[] = { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0 };
    
    //Queue
    cl::CommandQueue queue(context, default_device);

    //Writing into device memory
    queue.enqueueWriteBuffer(A_d, CL_TRUE, 0, sizeof(float) * SIZE, A_h);
    queue.enqueueWriteBuffer(B_d, CL_TRUE, 0, sizeof(float) * SIZE, B_h);
    
    //Kernel
    //Hamilton
        
    //Organizing source code
        cl::Program::Sources sources;
        
        sources.push_back({ kernel_code.c_str(),kernel_code.length() });
        
        cl::Program program(context, sources);
        
        if (program.build({ default_device }) != CL_SUCCESS) {
                std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
                exit(1);
        }
        
    //add function to Kernel on device
        cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> func(cl::Kernel(program, "func"));
        
        cl::NDRange global(SIZE);
        
    //Krenel! Metelesku blesku!
        func(cl::EnqueueArgs(queue,global), A_d, B_d, C_d).wait();
        
    //Kernel execution on the device
        float C_h[SIZE*SIZE];
        
    //Retrieving data from device
        queue.enqueueReadBuffer(C_d, CL_TRUE, 0, sizeof(float) * SIZE*SIZE, C_h);
        
    //read result
        std::cout << " result:" <<std::endl;
    for (int i = 0; i<SIZE; i++) {
        for (int j = 0; j<SIZE; j++) {
            int k = i*SIZE + j;
        std::cout << C_h[k] << " ";
        }   
        std::cout << std::endl;
    }
    
    return 0;
}





