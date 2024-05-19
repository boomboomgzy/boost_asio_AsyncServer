#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <map>
#include <vector>
#include<string>
#include <mutex>

class Session;
class Iocontextpool;

class Server {
public:
	Server(boost::asio::io_context& ioc, short port);
	void delete_session(std::string&);
private:
	void start_accept();
	void handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error);
	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
	std::map<std::string,std::shared_ptr<Session>> _sessions;
	std::mutex _sessions_mutex;
};