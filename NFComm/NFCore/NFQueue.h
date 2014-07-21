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

template<typename T>  
class NFQueue  
{  
public:  
	NFQueue();  
	virtual ~NFQueue();  
public:  

	bool Push(const T& object);  
	bool Pop(T& object);

protected:

	bool Empty(); 
	//bool Full();  

private:  
// 	int front;  
// 	int rear;  
// 	int size; 
	std::list<T> mList;
	//Object list[QUEUE_SIZE];  
	std::mutex queueMutex;  
};  

//------------------------------------------------------  
template<typename T>  
NFQueue<T>::NFQueue()  
{  
// 	front = rear = 0; 
// 	size = QUEUE_SIZE;
	//queueMutex.lock();

}  
//------------------------------------------------------  
template<typename T>  
bool NFQueue<T>::Push(const T& object)  
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
template<typename T>  
bool NFQueue<T>::Pop(T& object)  
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
template<typename T>  
bool NFQueue<T>::Empty()  
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
template<typename T>  
NFQueue<T>::~NFQueue()  
{  
	mList.clear(); 
}  
//------------------------------------------------------  


#endif
