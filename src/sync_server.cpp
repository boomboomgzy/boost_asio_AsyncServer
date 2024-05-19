#include <iostream>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <set>

const int MAX_LENGTH=1024;
using sock=boost::asio::ip::tcp::socket;
std::set<std::shared_ptr<std::thread>> server_thread_set;


void session(std::shared_ptr<sock> server_sock_ptr){
    try{
        while(1){
            char data[MAX_LENGTH]={'\0'};
            boost::system::error_code ec;
            size_t length = server_sock_ptr->read_some(boost::asio::buffer(data, MAX_LENGTH), ec);
            if(ec){
                if(ec==boost::asio::error::eof){
                    std::cout << "connection closed by peer" << std::endl;
                    break;
                }
                else{
                    throw boost::system::system_error(ec);
                }
            }
            std::cout << "receive from " << server_sock_ptr->remote_endpoint().address().to_string() << std::endl;
            std::cout << "receive message is " << data << std::endl;
            //回传信息值
            boost::asio::write(*server_sock_ptr, boost::asio::buffer(data, length));
            }
        }catch(std::exception& e){
        std::cerr << "Exception in thread: " << e.what() << "\n" << std::endl;
    }
}

void server(boost::asio::io_context& ioc,unsigned short port){
    boost::asio::ip::tcp::acceptor acpt(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    while(1){
        auto server_sock_ptr=std::make_shared<sock>(ioc);
        acpt.accept(*server_sock_ptr);
        auto server_thread_ptr = std::make_shared<std::thread>(session, server_sock_ptr);
        server_thread_set.insert(server_thread_ptr);
    }
}

int main()
{
    try {
        boost::asio::io_context  ioc;
        server(ioc, 10086);
        for (auto &t : server_thread_set) {
            t->join();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception " << e.what() << "\n";
    }
    return 0;
}