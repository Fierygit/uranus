//
// Created by Firefly on 2020/4/17.
//

#include "http_server.h"

#include "debug_log.h"
#include <unistd.h>

http_server::http_server() : http_server(8888) {}

http_server::http_server(uint16_t server_port) : server_port(server_port) {
    init();
    //std::cout << this->epfd << " "  << std::endl;
}

http_server::~http_server() {
    std::cout << "destruct" << std::endl;
    close(serv_sock); //关闭socket
    close(epfd);     //关闭内核
}

/**
 * epoll_wait
 *
 * epfd 等待刚才创建的server 的 ep
 * 参数events用来从内核得到事件的集合，
 * maxevents 告之内核这个events有多大， * 这个 maxevents 的值不能大于创建 epoll_create() 时的size，
 * 参数 timeout 是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。
 *
 * 该函数返回需要处理的事件数目，如返回0表示已超时。
 *
 * !!! 当对方关闭连接(FIN), EPOLLERR，都可以认为是一种EPOLLIN事件，在read的时候分别有0，-1两个返回值。
 */

void http_server::start() {

    while (true) {
        //std::cout << this->epfd << std::endl;
        // epoll_events_count表示就绪事件的数目, 这里会循环等待， 直到有事件的到来, 本质也是阻塞 i/o
        int epoll_events_count = epoll_wait(epfd, events, epoll_size, -1);
        // 返回 -1 代表失败
        if (epoll_events_count < 0) {
            DEBUG_LOG("epoll failure");
            //std::cout << epfd << " "  << std::endl;
            break;
        }

        DEBUG_LOG("epoll events count : %d", epoll_events_count);

        //处理这epoll_events_count个就绪事件
        for (int i = 0; i < epoll_events_count; ++i) {
            while (!(handler->dispatch(events[i], serv_sock, epfd))) {
                DEBUG_LOG("处理失败");
                //sleep(1);
            }
            // dispatcher(events[i], listener, epfd);
        }
    }
}


void http_server::init_epoll() {
    /**
     * 创建一个epoll的句柄。自从linux2.6.8之后，size参数是被忽略的。需要注意的是，当创建好epoll句柄后，
     * 它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，
     * !!!必须调用close()关闭，否则可能导致fd被耗尽。!!!
     */
    this->epfd = epoll_create(epoll_size);
    if (epfd < 0) {
        DEBUG_LOG("epfd create error");
        exit(1);
    }
    DEBUG_LOG("epoll created, server epollfd = %d", this->epfd);


    // epoll 返回之前的数据到这个 epoll_size 大小的数组里面
    events = new epoll_event[epoll_size];  // 表示监听的事件类型

    epoll_event event{};  //!!! 初始化｛｝
    //往内核事件表里添加事件
    event.events = EPOLLIN;
    event.data.fd = serv_sock;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111

    //是否为 边界出发，
    if (enable_et) { // 默认不开启， 开启了只触发一次
        event.events = EPOLLIN | EPOLLET;
    }
    /**
    epoll的事件注册函数，它不同于select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。

    第一个参数是epoll_create()的返回值。
    第二个参数表示动作，用三个宏来表示：
            EPOLL_CTL_ADD：注册新的fd到epfd中；
            EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
            EPOLL_CTL_DEL：从epfd中删除一个fd；
    第三个参数是需要监听的fd。
    第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：

    //保存触发事件的某个文件描述符相关的数据（与具体使用方式有关）
        typedef union epoll_data {
            void *ptr;
            int fd;
            __uint32_t u32;
            __uint64_t u64;
        } epoll_data_t;
        //感兴趣的事件和被触发的事件
        struct epoll_event {
            __uint32_t events; // Epoll events
            epoll_data_t data; // User data variable
        };

        EPOLLIN     //表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
        EPOLLOUT    //表示对应的文件描述符可以写；
        EPOLLPRI    //表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
        EPOLLERR    //表示对应的文件描述符发生错误；
        EPOLLHUP    //表示对应的文件描述符被挂断；
        EPOLLET     //将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
        EPOLLONESHOT//只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
*/
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    //所有的连接都用这个 sock和 event， 每一个 client 都要注册一个event， 多个 client 复用一个 server sock



    /**
     *  设置为非阻塞 io
     */
    fcntl(epfd, F_SETFL, fcntl(epfd, F_GETFD, 0) | O_NONBLOCK);
    DEBUG_LOG("epoll init over!!!");
}

void http_server::init_others() {
    this->handler = new http_handler();
}


/*
 * 顺序不要乱
 */
void http_server::init() {
    init_socket();
    init_epoll();
    init_others();
}

void http_server::init_socket() {
    /**
       * 1:创建套接字socket, 只是绑定一些基础的类型
       * param1:指定地址族为IPv4;param2:指定传输协议为流式套接字;param3:指定传输协议为TCP,可设为0,由系统推导
        * PF 是 posix， AF 是 BSD 没有什么区别
       */
    this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    /**
     * 设置 server 端的 协议， 端口， 和 地址，
     */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /** 把socket 绑定在 一个端口上
     * 返回-1 代表失败
     */
    if (bind(serv_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) <
        0) {
        DEBUG_LOG("bind error");
        exit(1);
    }

    /**
     *  在linux 2.2以后  listen的第二个参数。指的是在完成TCP三次握手后的队列。即在系统accept之前的队列。
     *  已经完成的队列。如果系统没有调用accpet把这个队列的数据拿出来。一旦这个队列满了。未连接队列的请求过不来。
     *  导致未连接队列里的请求会超时或者拒绝。如果系统调用了accpet队列接受请求数据。那么就会把接受到请求移除已完成队列。
     *  这时候已完成队列又可以使用了。
     *   最后 说了如果开启了syncookies  忽略listen的第二个参数。
     * */
    if (listen(serv_sock, SOMAXCONN) < 0) {
        DEBUG_LOG("bind error");
        exit(1);
    }

//    //地址复用
//    int on = 1;
//    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
//        DEBUG_LOG("setsockopt error");
//        exit(EXIT_FAILURE);
//    }
}

