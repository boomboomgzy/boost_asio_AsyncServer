#pragma once
#include<thread>
#include<vector>
#include<boost/asio.hpp>
#include"singleton.h"
#include<iostream>
#include<memory>


using iocontext=boost::asio::io_context;
using work_guard=boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
using work_ptr=std::unique_ptr<work_guard>;

class Iocontextpool:public Singleton<Iocontextpool>{
    
    friend class Singleton<Iocontextpool>;
    public:
        ~Iocontextpool();
        iocontext& Getioc();
        void stop();
    private:
        Iocontextpool(std::size_t size=std::thread::hardware_concurrency());
        std::vector<std::thread> _threads;
        std::vector<iocontext> _iocs;
        std::vector<work_ptr> _ioc_guards;
        std::size_t _nextioc_id;
};