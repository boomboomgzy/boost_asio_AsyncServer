#include<iostream>
#include<cstring>
#include<boost/asio.hpp>

const int MAX_LENGTH = 1024*2;

int main(){

try{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::endpoint server_ep(boost::asio::ip::address::from_string("127.0.0.1"),10086);
    boost::asio::ip::tcp::socket  sock(ioc);
    boost::system::error_code  ec ;
    sock.connect(server_ep,ec);
    if(ec){
        std::cout<<"connect failed,error code : "<<ec.value()<<" error message : "<<ec.message();
        return ec.value();
    }
    std::cout << "Enter message: ";
    char req_msg[MAX_LENGTH]={'\0'};
    while(1){
        if(std::cin.getline(req_msg,MAX_LENGTH)){
            char send_data[MAX_LENGTH]={'\0'};
            auto size=strlen(req_msg);
            std::memcpy(send_data,&size,2);
            std::memcpy(send_data+2,req_msg,size);
            boost::asio::write(sock, boost::asio::buffer(send_data, size+2));
            break;
        }
        else{
            if(std::cin.bad())
                throw std::runtime_error("IO stream is corrupted!");
            if(std::cin.fail())
            {
                std::cerr << "bad data, try again." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }
    }
    char reply_msg[MAX_LENGTH]={'\0'};
    boost::asio::read(sock,boost::asio::buffer(reply_msg, strlen(req_msg)));
    std::cout << "Reply is: "<<reply_msg<<std::endl;
}catch(const std::exception& e){
    std::cerr << "Caught an exception: " << e.what() << std::endl;
}

    
}