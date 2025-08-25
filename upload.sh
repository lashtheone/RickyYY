#!/bin/bash  
  
# 检查是否在 git 仓库中  
if [ ! -d ".git" ]; then  
    echo "错误: 当前目录不是 Git 仓库"  
    echo "请先运行: git init"  
    exit 1  
fi  
  
# 检查是否有远程仓库  
if ! git remote get-url origin >/dev/null 2>&1; then  
    echo "错误: 未设置远程仓库"  
    echo "请先添加远程仓库: git remote add origin <your-repo-url>"  
    exit 1  
fi  
  
# 获取提交信息  
COMMIT_MSG="$1"  
if [ -z "$COMMIT_MSG" ]; then  
    echo "请输入提交信息:"  
    read -r COMMIT_MSG  
    if [ -z "$COMMIT_MSG" ]; then  
        COMMIT_MSG="Auto commit $(date '+%Y-%m-%d %H:%M:%S')"  
    fi  
fi  
  
echo "开始上传到 GitHub..."  
  
# 添加所有文件（排除构建产物）  
git add .  
git add -f RickyYY/build.sh RickyYY/run_*.sh  RickyYY/CMakeLists.txt  
  
# 确保不上传构建产物  
echo "RickyYY/build/" >> .gitignore  
echo "*.exe" >> .gitignore  
echo "*.o" >> .gitignore  
git add .gitignore  
  
# 提交更改  
echo "提交更改: $COMMIT_MSG"  
git commit -m "$COMMIT_MSG"  
  
# 推送到远程仓库  
echo "推送到 GitHub..."  
git push origin main || git push origin master  
  
echo "上传完成！"  
