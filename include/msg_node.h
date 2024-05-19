#pragma once 
#include<string>
#include<memory>

class msg_node{
    public:
        msg_node(std::shared_ptr<std::string>);
        std::shared_ptr<std::string> get_write_data();
    private:
    std::shared_ptr<std::string> write_data;

};