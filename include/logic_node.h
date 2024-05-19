#pragma once
#include<memory>
#include"session.h"
#include"msg_node.h"

class LogicNode{
    public:
    LogicNode(std::shared_ptr<Session>, std::shared_ptr<std::string>);
    std::shared_ptr<std::string> get_read_data();
    std::shared_ptr<Session> get_session();
    private:
    std::shared_ptr<Session> _session;
    std::shared_ptr<std::string> _read_data;
    
};