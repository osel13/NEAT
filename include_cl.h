#pragma once

#if defined(__WINDOWS__) || defined(_WIN32) || defined(_WIN64)
	#if defined(__has_include)
		#if __has_include(<CL/opencl.hpp>)
			#include <CL/opencl.hpp>
			#define NEAT_HAS_OPENCL 1
		#else
			#define NEAT_HAS_OPENCL 0
		#endif
	#else
		#include <CL/opencl.hpp>
		#define NEAT_HAS_OPENCL 1
	#endif
#elif defined(__LINUX__) || defined(__linux__) || defined(__linux)
	#if defined(__has_include)
		#if __has_include(<CL/cl.hpp>)
			#include <CL/cl.hpp>
			#define NEAT_HAS_OPENCL 1
		#else
			#define NEAT_HAS_OPENCL 0
		#endif
	#else
		#include <CL/cl.hpp>
		#define NEAT_HAS_OPENCL 1
	#endif
#else
	#define NEAT_HAS_OPENCL 0
#endif
