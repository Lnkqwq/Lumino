#!/usr/bin/env python3
"""
Lumino Engine 自动构建脚本（增强版）
用法: python build.py
"""

import os
import subprocess
import shutil
from pathlib import Path

# ========================================
# 配置区
# ========================================
PROJECT_DIR = Path("C:/Users/34035/Lumino")
CMAKE_BIN = PROJECT_DIR / "cmake" / "bin"
MINGW_BIN = PROJECT_DIR / "mingw64" / "bin"

# ========== 新增：资源复制配置 ==========
# 需要复制到 build/ 目录的文件/文件夹列表
# 支持：单个文件、整个文件夹、通配符（如 *.dll）
RESOURCES_TO_COPY = [
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
    "shaders",
    "assets",
]

# build 目录下的目标子目录（为空则直接复制到 build/ 根目录）
# 例如设为 "bin"，文件会被复制到 build/bin/
COPY_DEST_SUBDIR = ""   # 不改就留空
# ========================================

def copy_resources_to_build():
    """编译成功后，自动复制资源文件到 build 目录"""
    build_dir = PROJECT_DIR / "build"
    if COPY_DEST_SUBDIR:
        build_dir = build_dir / COPY_DEST_SUBDIR
    
    # 确保目标目录存在
    build_dir.mkdir(parents=True, exist_ok=True)
    
    print("\n--- Copying resources to build directory ---")
    
    for item in RESOURCES_TO_COPY:
        src = PROJECT_DIR / item
        
        # 处理通配符（比如 *.dll）
        if "*" in item:
            pattern = item
            parent_dir = PROJECT_DIR
            for file in parent_dir.glob(pattern):
                if file.is_file():
                    dest = build_dir / file.name
                    shutil.copy2(file, dest)
                    print(f"  Copied: {file.name}")
            continue
        
        # 普通文件或文件夹
        if src.exists():
            dest = build_dir / src.name
            if src.is_dir():
                # 复制整个文件夹（如果目标已存在则覆盖）
                if dest.exists():
                    shutil.rmtree(dest)
                shutil.copytree(src, dest)
                print(f"  Copied folder: {src.name}/")
            else:
                shutil.copy2(src, dest)
                print(f"  Copied file: {src.name}")
        else:
            print(f"  [WARN] Not found: {item}")
    
    print("--- Resource copy complete ---\n")

def main():
    print("========================================")
    print("Building Lumino Engine with MinGW")
    print("========================================")

    # 设置临时PATH
    original_path = os.environ.get("PATH", "")
    os.environ["PATH"] = f"{CMAKE_BIN};{MINGW_BIN};{original_path}"

    # 清理并创建build目录
    print("\nCleaning and creating build directory...")
    build_dir = PROJECT_DIR / "build"
    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir()
    os.chdir(build_dir)

    # CMake配置
    print("\nRunning CMake configuration...")
    cmake_cmd = [
        str(CMAKE_BIN / "cmake.exe"),
        "-G", "MinGW Makefiles",
        "-DCMAKE_C_COMPILER=gcc",
        "-DCMAKE_CXX_COMPILER=g++",
        str(PROJECT_DIR)
    ]
    result = subprocess.run(cmake_cmd, shell=True)
    if result.returncode != 0:
        print("CMake configuration failed!")
        input("Press Enter to exit...")
        exit(result.returncode)

    # 编译
    print("\nBuilding project...")
    make_cmd = [str(MINGW_BIN / "mingw32-make.exe")]
    result = subprocess.run(make_cmd, shell=True)
    if result.returncode != 0:
        print("Build failed!")
        input("Press Enter to exit...")
        exit(result.returncode)

    # ========== 新增：自动复制资源 ==========
    copy_resources_to_build()

    # 成功
    print("\n========================================")
    print("Build successful!")
    print("========================================")
    input("Press Enter to exit...")

if __name__ == "__main__":
    main()