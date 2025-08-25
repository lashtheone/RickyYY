#!/bin/bash

# 检测操作系统
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "检测到 Linux 环境"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "检测到 macOS 环境"
fi

# 创建构建目录
mkdir -p build
cd build

# 运行 CMake
cmake ..
make

# 返回根目录
cd ..

#!/bin/bash

# 生成服务器脚本
echo '#!/bin/bash
echo "启动服务器在端口 9000..."
./build/simple_server 9000' > run_server.sh

# 生成客户端脚本  
echo '#!/bin/bash
echo "连接到本地服务器..."
./build/simple_client localhost 9000' > run_client.sh