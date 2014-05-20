#pragma once
// -------------------------------------------------------------------------
//    @FileName      :    NFQueue.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2011-01-21 21:49
//    @Module    : 
//
// -------------------------------------------------------------------------

#ifndef _NF_QUEUE_H_
#define _NF_QUEUE_H_

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <thread>
#include <mutex>
#include "NFPlatform.h"

const int QUEUE_SIZE = 10000;  

template<typename Object>  
class NFQueue  
{  
public:  
	NFQueue();  
	virtual ~NFQueue();  
public:  

	bool Push(const Object& object);  
	bool Pop(Object& object);

protected:

	bool Empty(); 
	//bool Full();  

private:  
// 	int front;  
// 	int rear;  
// 	int size; 
	std::list<Object> mList;
	//Object list[QUEUE_SIZE];  
	std::mutex queueMutex;  
};  

//------------------------------------------------------  
template<typename Object>  
NFQueue<Object>::NFQueue()  
{  
// 	front = rear = 0; 
// 	size = QUEUE_SIZE;
	//queueMutex.lock();

}  
//------------------------------------------------------  
template<typename Object>  
bool NFQueue<Object>::Push(const Object& object)  
{  
	queueMutex.lock();
	if(Full())  
	{  
		queueMutex.unlock();

		return false;  
	}

	mList.push_back(object);
// 	list[rear] = object;  
// 	rear = (rear + 1) % size;  

	queueMutex.unlock();


	return true;  
}  
//------------------------------------------------------  
template<typename Object>  
bool NFQueue<Object>::Pop(Object& object)  
{
	queueMutex.lock();
	if(Empty())  
	{  
		queueMutex.unlock();

		return false;  
	}

	object = mList.pop_front();
// 	object = list[front];  
// 	front = (front + 1) % size;  

	queueMutex.unlock();

	return true;  
}  
//------------------------------------------------------  
template<typename Object>  
bool NFQueue<Object>::Empty()  
{  
	return mList.empty();
// 	if(rear == front)  
// 		return true;  
// 	else  
// 		return false;  
}  
//------------------------------------------------------  
// template<typename Object>  
// bool NFQueue<Object>::Full()  
// {  
// 	if((rear + 1) % size == front)  
// 		return true;  
// 	else  
// 		return false;  
// }  
//------------------------------------------------------  
template<typename Object>  
NFQueue<Object>::~NFQueue()  
{  
	mList.clear(); 
}  
//------------------------------------------------------  


#endif
