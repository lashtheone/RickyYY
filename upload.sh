#!/bin/bash  
  
# 检查是否在 git 仓库中  
if [ ! -d ".git" ]; then  
    echo "错误: 当前目录不是 Git 仓库"  
    echo "请先运行: git init"  
    exit 1  
fi  
  
# 确保使用 SSH URL（避免 HTTPS 认证问题）  
CURRENT_URL=$(git remote get-url origin 2>/dev/null)  
if [[ "$CURRENT_URL" == https* ]]; then  
    echo "检测到 HTTPS URL，切换到 SSH..."  
    git remote set-url origin git@github.com:lashtheone/RickyYY.git  
fi  
  
# 检查是否有远程仓库  
if ! git remote get-url origin >/dev/null 2>&1; then  
    echo "错误: 未设置远程仓库"  
    echo "添加 SSH 远程仓库..."  
    git remote add origin git@github.com:lashtheone/RickyYY.git  
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
  
# 检查工作目录状态  
if ! git diff-index --quiet HEAD --; then  
    echo "检测到未提交的更改"  
      
    # 添加所有文件（排除构建产物）  
    git add .  
      
    # 强制添加重要的脚本文件  
    git add -f RickyYY/build.sh RickyYY/run_*.sh RickyYY/CMakeLists.txt 2>/dev/null || true  
      
    # 确保 .gitignore 包含构建产物排除规则  
    if ! grep -q "RickyYY/build/" .gitignore 2>/dev/null; then  
        echo "RickyYY/build/" >> .gitignore  
        echo "*.exe" >> .gitignore  
        echo "*.o" >> .gitignore  
        echo "*.obj" >> .gitignore  
        echo ".vs/" >> .gitignore  
        echo "RickyYY/.vs/" >> .gitignore  
        git add .gitignore  
    fi  
      
    # 提交更改  
    echo "提交更改: $COMMIT_MSG"  
    git commit -m "$COMMIT_MSG"  
else  
    echo "没有需要提交的更改"  
fi  
  
# 推送到远程仓库  
echo "推送到 GitHub..."  
git push origin main  
  
if [ $? -eq 0 ]; then  
    echo "上传完成！"  
else  
    echo "推送失败，尝试强制推送..."  
    git push -f origin main  
    if [ $? -eq 0 ]; then  
        echo "强制推送成功！"  
    else  
        echo "推送失败，请检查网络连接和 SSH 密钥配置"  
        exit 1  
    fi  
fi  
