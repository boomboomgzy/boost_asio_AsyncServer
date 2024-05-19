#include"logic_system.h"


bool LogicSystem::stop_deal=false;

LogicSystem::~LogicSystem(){
    std::cout<<"logicsystem destructed !"<<std::endl;
    stop_deal=true;
    logic_thread.join();
};

LogicSystem::LogicSystem():logic_que_empty(MAX_LOGICQUE_NUM),logic_que_full(0){
    std::cout<<"logicsystem constructed !"<<std::endl;
    reg_events();
    logic_thread=std::thread(&LogicSystem::deal_logic, this);
};

void LogicSystem::reg_events(){
    events_map[MSG_HELLO_WORD]=std::bind(&LogicSystem::event_hello,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
}
void LogicSystem::postmsgtoque(std::shared_ptr<LogicNode> node){
    logic_que_empty.wait();
    std::lock_guard<std::mutex> lock(logicque_mutex);
    logicque.push(node);
    logic_que_full.post();
}

void LogicSystem::deal_logic(){
    while(1){
    if(!stop_deal){
        logic_que_full.wait();
        std::lock_guard<std::mutex> lock(logicque_mutex);
        auto& node=logicque.front();
        msg_data rec_msg_data;
        rec_msg_data.ParseFromString(*(node->get_read_data()));
        auto it=events_map.find(rec_msg_data.id());
        if(it!=events_map.end()){
            it->second(node->get_session(),rec_msg_data.id(),rec_msg_data.data());
        }
        logicque.pop();
        logic_que_empty.post();
        }
    else{
        while(!logicque.empty()){
            logic_que_full.wait();
            std::lock_guard<std::mutex> lock(logicque_mutex);
            auto& node=logicque.front();
            msg_data rec_msg_data;
            rec_msg_data.ParseFromString(*(node->get_read_data()));
            auto it=events_map.find(rec_msg_data.id());
            if(it!=events_map.end()){
                it->second(node->get_session(),rec_msg_data.id(),rec_msg_data.data());
            }
            logicque.pop();
            logic_que_empty.post();
            }
        break;
        }
    }
    
}


void LogicSystem::event_hello(std::shared_ptr<Session> session,const int id,const std::string& data){
        std::cout<<"server receive data id : "<<id<<" data : "<<data<<std::endl;
        std::string send_data;
	    msg_data send_msg_data;
        send_msg_data.set_id(id);
        send_msg_data.set_data(data);
		send_msg_data.SerializeToString(&send_data);
        size_t send_len = send_data.size();
				std::string send_len_str=std::to_string(send_len);
				if(send_len_str.size()>HEADER_LENGTH){
					std::cout<<"send len too long "<<std::endl;
                    return;
                }
        session->send(std::make_shared<std::string>(send_len_str+send_data));
   
}

