#include "server.h"
#include "session.h"
#include<iostream>
#include"iocontextpool.h"


Server::Server(boost::asio::io_context& ioc, short port) :_ioc(ioc),
_acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	start_accept();
}

void Server::delete_session(std::string& uuid){
	std::lock_guard<std::mutex> lock(_sessions_mutex);
	_sessions.erase(uuid);
}

void Server::start_accept() {
	auto& deal_ioc=Iocontextpool::get_instance_ptr()->Getioc();
	auto new_session=std::make_shared<Session>(*this,deal_ioc);
	_acceptor.async_accept(new_session->get_socket(),
		std::bind(&Server::handle_accept, this, new_session, std::placeholders::_1));
}


void Server::handle_accept(std::shared_ptr<Session> new_session, const boost::system::error_code& error) {
	if (!error) {
		new_session->start();
		std::lock_guard<std::mutex> lock(_sessions_mutex);
		_sessions.insert(std::make_pair(new_session->Getuuid(), new_session));
	}
	else {
		std::cout << "session accept failed, error is " << error.what() << std::endl;
	}

	start_accept();
}

