@echo off  
echo 检测到 Windows 环境  
  
mkdir build 2>nul  
cd build  
  
cmake ..  
cmake --build .  
  
cd ..  
call generate_run_scripts.bat