#include<iostream>
#include<boost/asio.hpp>
#include<thread>
#include<vector>
#include<chrono>
#include<string>
#include<const.h>
#include<msg.pb.h>


using namespace boost::asio::ip;

std::vector<std::thread> vec_threads;

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0;i<100;i++){
		vec_threads.emplace_back([](){
			try {
		//创建上下文服务
		boost::asio::io_context   ioc;
		//构造endpoint
		tcp::endpoint  remote_ep(address::from_string("127.0.0.1"), 10086);
		tcp::socket  sock(ioc);
		boost::system::error_code   error = boost::asio::error::host_not_found; ;
		sock.connect(remote_ep, error);
		if (error) {
			std::cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
			return;
		}
		int j=0;
		while(j<500){
				j++;
				//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				msg_data send_msg_data;
				send_msg_data.set_id(1001);
				send_msg_data.set_data("hello world!");
				std::string req;
				send_msg_data.SerializeToString(&req);
				size_t req_len = req.size();
				std::string req_len_str=std::to_string(req_len);
				if(req_len_str.size()>HEADER_LENGTH){
					std::cout<<"req len too long "<<std::endl;
					break;
				}
				std::string send_data=req_len_str+req;
				boost::asio::write(sock, boost::asio::buffer(send_data, req_len + HEADER_LENGTH));

				//std::this_thread::sleep_for(std::chrono::milliseconds(4000));
				std::cout << "begin to receive..." << std::endl;
				std::string* reply_header=new std::string(HEADER_LENGTH,'\0');
				boost::asio::read(sock, boost::asio::buffer(*reply_header));
        		short data_len=static_cast<short>(std::stoi(*reply_header));
        		std::cout<<"client receive header len is "<<data_len<<std::endl;
				if(data_len>MAX_LENGTH){
					std::cout << "invalid data length is " << data_len << std::endl;
					break;
				}
				std::string* reply_data=new std::string(data_len,'\0');
				boost::asio::read(sock, boost::asio::buffer(*reply_data));
				msg_data rec_msg_data;
				rec_msg_data.ParseFromString(*reply_data);
				std::cout << "Reply is id: "<< rec_msg_data.id()<<" data: " <<rec_msg_data.data()<< std::endl;
				delete reply_header;
				delete reply_data;
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

		});
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //降低连接的频率 从而缓解服务器压力
	}

	for (auto& t : vec_threads) {
		t.join();
	}

	auto end = std::chrono::high_resolution_clock::now(); // 获取结束时间
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "Time spent: " << duration.count() << " seconds." << std::endl;
}


