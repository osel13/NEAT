# NEAT
Neural network containong NEAT algorithm, created as a part of NEAT JAM

## Windows instructions
For Windows, it is VERY recommended to:

1. Install Visual Studio 2022
   1. https://visualstudio.microsoft.com/downloads/?cid=learn-navbar-download-cta
   2. Choose C++ development in the installer
2. Install VCPKG
   1. git clone https://github.com/microsoft/vcpkg.git
   2. cd vcpkg
   3. .\bootstrap-vcpkg.bat
   4. Edit PATH environmental variable to include this directory (IMPORTANT)
3. Install OpenCL (powershell)
   1. vcpkg install opencl:x64-windows (may be different for 32-bit platforms)
   2. Move the directory to `win-lib` in this project (IMPORTANT)
