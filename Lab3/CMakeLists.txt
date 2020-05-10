cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab3 main.cpp)


# 协调者##########################333
add_executable(coordinator
        common/loguru.cpp
        coordinator/main.cpp
        coordinator/CoServer.h
        coordinator/CoServer.cpp
        )

target_link_libraries(
        coordinator
        pthread
        dl
)


# 客户端####################################
set(CLIENT "client")
add_executable(${CLIENT}
        client/main.cpp)

# 添加 thread
target_link_libraries(
        ${PROJECT_NAME}
        pthread
)

# 参与者
set(PARTICIPANT "participant")
add_executable(${PARTICIPANT}
        participant/main.cpp)

# 添加 thread
target_link_libraries(
        ${PROJECT_NAME}
        pthread
)

##############测试
enable_testing()

add_executable(testBlockingqueue
        test/testBlockingqueue.cpp
        coordinator/BoundedBlockingQueue.h)
add_test(NAME testBlockingqueue COMMAND testBlockingqueue)


# 添加 thread
target_link_libraries(
        ${PROJECT_NAME}
        pthread
)