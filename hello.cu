#include <stdio.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>


__global__ void test()
{
#if __CUDA_ARCH__ >= 200
    printf("Hi Cuda World");
#endif
}

int main( int argc, char** argv )
{
    test<<<1,1>>>();
    cudaDeviceSynchronize();
    return 0;
}
