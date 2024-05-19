#pragma once
#include <memory>
#include <iostream>


template <typename T>
class Singleton
{
public:
    static std::shared_ptr<T> get_instance_ptr(){
        //错误写法  static std::shared_ptr<T> instance_ptr=std::make_shared<T>();
        static std::shared_ptr<T> instance_ptr=std::shared_ptr<T>(new T);
        return instance_ptr;
    }
    virtual ~Singleton()=default;
protected:
    Singleton()=default;
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
};
