#include <iostream>
#include "./coordinator/CoServer.h"

// 如何放到全局 public ??? todo 重复包含




int main() {
    CoServer().init().run();
    return 0;
}
