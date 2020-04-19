//
// Created by Firefly on 2020/4/18.
//

#include "request.h"

bool clients_pool::add_one(httpt_request *request) {
    return false;
}

httpt_request *clients_pool::get_by_fd(int client_sockt) {
    return nullptr;
}

bool httpt_request::is_static() {
    return false;
}

void httpt_request::response_static() {

}
