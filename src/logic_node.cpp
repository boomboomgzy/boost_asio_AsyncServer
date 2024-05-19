#include<memory>
#include"session.h"
#include"msg_node.h"
#include"logic_node.h"

LogicNode::LogicNode(std::shared_ptr<Session> session, std::shared_ptr<std::string> read_data):_session(session),_read_data(read_data){};

std::shared_ptr<std::string> LogicNode::get_read_data(){
    return _read_data;
}

std::shared_ptr<Session> LogicNode::get_session(){
    return _session;
}