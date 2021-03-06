cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(kvstore2pcsystem
        main.cpp
        coordinator/KeepAlive.cpp
        coordinator/KeepAlive.h
        common/loguru.cpp
        coordinator/CoServer.h
        coordinator/CoServer.cpp
        coordinator/clientHandler.cpp
        coordinator/clientHandler.h
        coordinator/participantHandler.h
        coordinator/participantHandler.cpp
        participant/PaServer.h
        participant/PaServer.cpp
        common/Util.h
        common/Config.h
        common/ThreadPool.h
        common/ThreadPool.cpp
        coordinator/syncDataHandler.cpp)

target_link_libraries(
        kvstore2pcsystem
        pthread
        dl
)

#############################for test
add_executable(kvstore2pcsystemTEST2
        main.cpp
        coordinator/KeepAlive.cpp
        coordinator/KeepAlive.h
        common/loguru.cpp
        coordinator/CoServer.h
        coordinator/CoServer.cpp
        coordinator/clientHandler.cpp
        coordinator/clientHandler.h
        coordinator/participantHandler.h
        coordinator/participantHandler.cpp
        participant/PaServer.h
        participant/PaServer.cpp
        common/Util.h
        common/Config.h
        common/ThreadPool.h
        common/ThreadPool.cpp
        coordinator/syncDataHandler.cpp
        )

target_link_libraries(
        kvstore2pcsystemTEST2
        pthread
        dl
)
#############################for test
add_executable(kvstore2pcsystemTEST4
        main.cpp
        coordinator/KeepAlive.cpp
        coordinator/KeepAlive.h
        common/loguru.cpp
        coordinator/CoServer.h
        coordinator/CoServer.cpp
        coordinator/clientHandler.cpp
        coordinator/clientHandler.h
        coordinator/participantHandler.h
        coordinator/participantHandler.cpp
        participant/PaServer.h
        participant/PaServer.cpp
        common/Util.h
        common/Config.h
        common/ThreadPool.h
        common/ThreadPool.cpp
        coordinator/syncDataHandler.cpp
        )

target_link_libraries(
        kvstore2pcsystemTEST4
        pthread
        dl
)
#############################for test
add_executable(kvstore2pcsystemTEST3
        main.cpp
        coordinator/KeepAlive.cpp
        coordinator/KeepAlive.h
        common/loguru.cpp
        coordinator/CoServer.h
        coordinator/CoServer.cpp
        coordinator/clientHandler.cpp
        coordinator/clientHandler.h
        coordinator/participantHandler.h
        coordinator/participantHandler.cpp
        participant/PaServer.h
        participant/PaServer.cpp
        common/Util.h
        common/Config.h
        common/ThreadPool.h
        common/ThreadPool.cpp
        coordinator/syncDataHandler.cpp
        )

target_link_libraries(
        kvstore2pcsystemTEST3
        pthread
        dl
)
#
#
## 协调者##########################333
#add_executable(coordinator
#        common/loguru.cpp
#        coordinator/main.cpp
#        coordinator/CoServer.h
#        coordinator/CoServer.cpp
#        coordinator/clientHandler.cpp
#        coordinator/clientHandler.h
#        coordinator/participantHandler.h
#        coordinator/participantHandler.cpp
#        common/Util.h coordinator/KeepAlive.cpp coordinator/KeepAlive.h
#        common/ThreadPool.cpp coordinator/syncDataHandler.h coordinator/syncDataHandler.cpp)
#
#target_link_libraries(
#        coordinator
#        pthread
#        dl
#)
#
#
## 客户端####################################
set(CLIENT "client")
add_executable(${CLIENT}
        common/loguru.cpp
        client/main.cpp client/Client.h client/Client.cpp common/WaitGroup.h)

# 添加 thread   dynamic link?
target_link_libraries(
        client
        pthread
        dl
)
#
## 参与者
#set(PARTICIPANT "participant")
#add_executable(${PARTICIPANT}
#        participant/main.cpp participant/PaServer.h participant/PaServer.cpp common/loguru.cpp)
#
## 添加 thread
#target_link_libraries(
#        ${PARTICIPANT}
#        pthread
#        dl
#)
#
#
###############测试 "生产者消费者"
#enable_testing()
#
#
#add_executable(testBlockingqueue
#        common/loguru.cpp
#        test/testBlockingqueue.cpp
#        coordinator/BoundedBlockingQueue.h test/testEncoder.cpp test/testWaitGroup.cpp )
#add_test(NAME testBlockingqueue COMMAND testBlockingqueue)
#
## 添加 thread
#target_link_libraries(
#        testBlockingqueue
#        pthread
#)
#
#############测试decoder
#add_executable(testDecoder
#        test/testDecoder.cpp
#        common/Util.h
#        common/public.h
#        )
#
#
########################################测试encoder
#add_executable(testEncoder
#        #        common/loguru.cpp
#        test/testEncoder.cpp
#        common/Util.h
#        )
#
##add_test(NAME testEncoder COMMAND testEncoder)
##
### 添加 thread
##target_link_libraries(
##        testEncoder
##        pthread
##        dl
##)
#
## 测试半包
#############测试decoder
#add_executable(testBanbao
#        test/testBanBao.cpp
#        common/Util.h
#        common/public.h
#        )
#
#
###################测试 TCP 连接的中断情况
################### 测试 client
#add_executable(testTCPClient
#        test/testTCPClient.cpp)
#target_link_libraries(
#        testTCPClient
#        pthread
#)
#
################ ####测试 server
#add_executable(testTCPServer
#        test/testTCPServer.cpp)
#target_link_libraries(
#        testTCPServer
#        pthread
#)
#
#
##################################测试 waitgroup
#add_executable(WaitGroup
#        test/testWaitGroup.cpp
#        common/WaitGroup.h)
#target_link_libraries(
#        WaitGroup
#        pthread
#)
#
##################################测试 waitgroup
#add_executable(ThreadPool
#        test/testThreadPool.cpp
#        common/loguru.cpp
#        common/ThreadPool.cpp)
#target_link_libraries(
#        ThreadPool
#        pthread
#        dl
#)
#
##################################测试 select
#add_executable(testSelect
#        test/testSelect.cpp)
#
#target_link_libraries(
#        ThreadPool
#        pthread
#)
#
#
#
#### tse
##add_executable(Test
##        test.cpp
##        common/loguru.cpp
##        coordinator/CoServer.h
##        coordinator/CoServer.cpp
##        coordinator/clientHandler.cpp
##        coordinator/clientHandler.h coordinator/participantHandler.h coordinator/participantHandler.cpp common/Util.h)
##
##target_link_libraries(
##        Test
##        pthread
##        dl
##)
#
## 测试 get 的结果编码
#add_executable(testEncodeResult
#        #        common/loguru.cpp
#        test/testEncodeResult.cpp
#        common/Util.h
#        )