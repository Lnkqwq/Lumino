cmake_minimum_required(VERSION 3.10)
project(LuminoEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果你是 MinGW，可设置静态链接运行时库（可选）
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# 开启调试符号（便于调试）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# 查找 OpenGL 库（MinGW 下通常自动找到）
find_package(OpenGL REQUIRED)

# 手动指定第三方库路径（可根据你的实际位置修改）
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

# GLAD (需要编译 glad.c)
set(GLAD_INCLUDE_DIR ${THIRD_PARTY_DIR}/glad/include)
set(GLAD_SRC ${THIRD_PARTY_DIR}/glad/src/glad.c)

# GLFW (MinGW 预编译库路径)
set(GLFW_INCLUDE_DIR ${THIRD_PARTY_DIR}/glfw/include)
set(GLFW_LIB_DIR ${THIRD_PARTY_DIR}/glfw/lib-mingw-w64)
# 如果是静态链接使用 libglfw3.a，如果是动态链接使用 libglfw3dll.a
set(GLFW_LIB ${GLFW_LIB_DIR}/libglfw3.a)

# GLM (仅头文件)
set(GLM_INCLUDE_DIR ${THIRD_PARTY_DIR}/glm)

# STB (单头文件)
set(STB_INCLUDE_DIR ${THIRD_PARTY_DIR}/stb)

# CGLTF (单头文件)
set(CGLTF_INCLUDE_DIR ${THIRD_PARTY_DIR}/cgltf)

# 收集所有源文件 (递归添加 src 下所有 .cpp)
file(GLOB_RECURSE SRC_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${GLAD_SRC}"
)

# 包含头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
    ${CGLTF_INCLUDE_DIR}
)

# 创建可执行文件
add_executable(Lumino ${SRC_FILES})

# 链接库
target_link_libraries(Lumino 
    ${GLFW_LIB}
    ${OPENGL_LIBRARIES}
)

# 如果需要使用动态链接的 glfw3dll，可能需要添加以下定义
# target_compile_definitions(Lumino PRIVATE GLFW_DLL)

# 设置工作目录为项目根目录，以便正确加载 shaders 和 assets
set_property(TARGET Lumino PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# 打印信息帮助确认
message(STATUS "GLAD include: ${GLAD_INCLUDE_DIR}")
message(STATUS "GLFW include: ${GLFW_INCLUDE_DIR}")
message(STATUS "GLFW lib: ${GLFW_LIB}")
message(STATUS "GLM include: ${GLM_INCLUDE_DIR}")
message(STATUS "STB include: ${STB_INCLUDE_DIR}")
message(STATUS "CGLTF include: ${CGLTF_INCLUDE_DIR}")