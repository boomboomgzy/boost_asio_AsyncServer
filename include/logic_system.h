#pragma once
#include"singleton.h"
#include"logic_node.h"
#include"session.h"
#include"const.h"
#include"msg.pb.h"
#include"session.h"
#include<queue>
#include<mutex>
#include<functional>
#include<boost/interprocess/sync/interprocess_semaphore.hpp>

using event_function=std::function<void(std::shared_ptr<Session>, const int ,const std::string&)>;

class LogicSystem:public Singleton<LogicSystem>{

    friend class Singleton<LogicSystem>;
    public:
        ~LogicSystem();
        void postmsgtoque(std::shared_ptr<LogicNode>);
    private:
        LogicSystem();
        void reg_events();
        void event_hello(std::shared_ptr<Session>,const int,const std::string&);
        void deal_logic();
        std::queue<std::shared_ptr<LogicNode>> logicque;
        std::mutex logicque_mutex;
        std::thread logic_thread;
        std::map<int,event_function> events_map;
        boost::interprocess::interprocess_semaphore logic_que_empty, logic_que_full;
        static bool stop_deal;
};
