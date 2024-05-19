#pragma once

#include<iostream>
#include<boost/asio.hpp>
#include<boost/uuid/uuid_io.hpp>
#include<boost/uuid/uuid_generators.hpp>
#include<queue>
#include<memory>
#include<string>
#include<const.h>


using boost_socket=boost::asio::ip::tcp::socket;

class Server;
class msg_node;

class Session:public std::enable_shared_from_this<Session> {
    public:
        Session(Server&,boost::asio::io_context&);
        ~Session();
        boost_socket& get_socket();
        void start();
        std::string& Getuuid();
        void send(std::shared_ptr<std::string>);
        void refresh_read_data(size_t);

    private:

        void handle_read_header(const boost::system::error_code& ec, std::size_t bytes_transfered);
        void handle_read_data(const boost::system::error_code& ec, std::size_t bytes_transfered);
	    void handle_write(const boost::system::error_code& ec);
        Server& _server;
        boost_socket _socket;
        std::string* read_data;
        std::string _uuid;
        std::queue<std::shared_ptr<msg_node>> _send_que;

};


