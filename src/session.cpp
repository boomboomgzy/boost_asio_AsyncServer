#include"session.h"
#include"server.h"
#include"msg_node.h"
#include"msg.pb.h"
#include"logic_system.h"
#include"logic_node.h"
#include<utility>
#include<chrono>    
#include<thread>    
#include<iostream>
#include<cstring>
#include<string>
#include<iomanip>
#include<memory>

void Session::refresh_read_data(size_t len){
    delete read_data;
    read_data=new std::string(len,'\0');
}

Session::Session(Server& server,boost::asio::io_context& ioc):_server(server),_socket(ioc){
    read_data=new std::string(HEADER_LENGTH,'\0');
    boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
};
Session::~Session(){
    delete read_data;
}

void Session::send(std::shared_ptr<std::string> data){
    if(_send_que.size()<MAX_SENDQUE_LENGTH){
        _send_que.push(std::make_shared<msg_node>(data));
    }
    else{
        std::cout << "send queue fulled " <<  std::endl;
        return;
    }
    if(_send_que.size()==1){
        boost::asio::async_write(this->_socket, boost::asio::buffer(*(_send_que.front()->get_write_data())), std::bind(&Session::handle_write, this, std::placeholders::_1));
    }
}

std::string& Session::Getuuid() {
	return _uuid;
}
void Session::start(){
    boost::asio::async_read(_socket,boost::asio::buffer(*read_data),std::bind(&Session::handle_read_header,this,std::placeholders::_1,std::placeholders::_2));
}

void Session::handle_read_header(const boost::system::error_code& ec, std::size_t bytes_transfered){
    if(!ec){
        short data_len=static_cast<short>(std::stoi(*read_data));
        std::cout<<"server receive header len is "<<data_len<<std::endl;
        if(data_len>MAX_LENGTH){
            std::cout << "invalid data length is " << data_len << std::endl;
		    _server.delete_session(_uuid);
            return;
        }
        refresh_read_data(data_len);
        boost::asio::async_read(_socket, boost::asio::buffer(*read_data), std::bind(&Session::handle_read_data, this,std::placeholders::_1, std::placeholders::_2));
    }
    
    else{
        std::cout << "handle read failed, error is " << ec.what() << std::endl;
		_server.delete_session(_uuid);
    }
}
void Session::handle_read_data(const boost::system::error_code& ec, std::size_t bytes_transfered){
    if(!ec){
        
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        LogicSystem::get_instance_ptr()->postmsgtoque(std::make_shared<LogicNode>(shared_from_this(),std::make_shared<std::string>(*read_data)));
        refresh_read_data(HEADER_LENGTH);
        boost::asio::async_read(_socket,boost::asio::buffer(*read_data),std::bind(&Session::handle_read_header,this,std::placeholders::_1,std::placeholders::_2));
    }
    else{
        std::cout << "handle read failed, error is " << ec.what() << std::endl;
		_server.delete_session(_uuid);
    }
}

void Session::handle_write(const boost::system::error_code& ec){
    if(!ec){
        _send_que.pop();
        if(!_send_que.empty()){
            boost::asio::async_write(_socket, boost::asio::buffer(*(_send_que.front()->get_write_data())), std::bind(&Session::handle_write, this, std::placeholders::_1));
        }
    }
    else{
        std::cout << "handle write failed, error is " << ec.what() << std::endl;
		_server.delete_session(_uuid);
    }
}

boost_socket& Session::get_socket(){
    return _socket;
}

