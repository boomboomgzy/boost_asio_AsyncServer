#include <iostream>
#include <boost/asio.hpp>

#include"session.h"
#include"server.h"
#include"msg.pb.h"
#include"iocontextpool.h"

//original version
//int main()
//{
//	try {
//		boost::asio::io_context ioc;
//		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
//        signals.async_wait([&ioc](auto, auto) {
//            ioc.stop();
//            });
//		Server s(ioc, 10086);
//		ioc.run();
//	}
//	catch (boost::system::system_error& e) {
//		std::cerr << "Exception: " << e.what() << std::endl;
//	}
//	return 0;
//
//
//}

//iocontextpool version
int main()
{
	try {
		boost::asio::io_context ioc;
		auto iocpool=Iocontextpool::get_instance_ptr();
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc,iocpool](auto, auto) {
            ioc.stop();
			iocpool->stop();
            });
		Server s(ioc, 10086);
		ioc.run();
	}
	catch (boost::system::system_error& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return 0;


}





