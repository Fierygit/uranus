cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab3 main.cpp)

# 协调者
add_executable(coordinator coordinator/main.cpp)


# 客户端
set(CLIENT "client")
add_executable(${CLIENT}
        client/main.cpp)

# 参与者
set(PARTICIPANT "participant")
add_executable(${PARTICIPANT}
        participant/main.cpp)

##############测试
enable_testing()

add_executable(testBlockingqueue
        test/testBlockingqueue.cpp
        coordinator/BoundedBlockingQueue.h)
add_test(NAME testBlockingqueue COMMAND testBlockingqueue)


# 添加 socket
target_link_libraries(
        ${PROJECT_NAME}
        pthread
)