cmake_minimum_required(VERSION 3.10)
project(ParkingSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(ParkingSystem
    main.cpp
    Car.cpp
    Stack.cpp
    Queue.cpp
    ParkingSystem.cpp
    ConsoleGUI.cpp
)

# 设置输出目录
set_target_properties(ParkingSystem PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 在Windows上使用控制台子系统
if(WIN32)
    target_link_libraries(ParkingSystem)
endif()
