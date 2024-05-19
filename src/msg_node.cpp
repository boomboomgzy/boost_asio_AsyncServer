#include "msg_node.h"

std::shared_ptr<std::string> msg_node::get_write_data(){
    return write_data;
};


msg_node::msg_node(std::shared_ptr<std::string> data):write_data(data){
};

