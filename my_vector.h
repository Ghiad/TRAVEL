#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include"pch.h"
#include<stdio.h>
#include<iostream>
using namespace std;
enum { MAX = 1 };

template<class T> class my_vector {
	public:
		T *preitems;
		T *items;
		my_vector() = default;
		//explicit my_vector(int ss = MAX);
		~my_vector();
		bool checkIndex(int index);//检查要操作的下标是否在数组容量范围内。
		int size()const { return counts; }//获取元素数目
		int getCapacity()const { return capacity; }//容量
		bool isEmpty()const { return counts == 0; }//判断数组空
		bool isFull()const { return counts >= capacity-1; }//判断数组满
		T& operator[](int index);
		void set(int index, T elem);//按索引修改值
		void renewCapacity();//扩容
		void add(int index,const T &elem);//添加数据
		void addFirst(T elem);//
		void push_back(const T &elem);//
		void remove(int index);//删除数据
		void pop_front();//
		void pop_back();//
		T *begin();
		T *end();
		T& front();
		T& back();
		void clear();
	private:
		int capacity;
		int counts;
		int itemsFlag;
};


/*template<class T>
//设定一个内存标志位 ItemsFlag 。当 ItemsFlag = 0，using preitems；当 ItemsFlag = 1，using items。
my_vector<T>::my_vector(int ss) : capacity(ss), counts(0)
{
	itemsFlag = 0;
	preitems = new T[capacity];
	items = nullptr;
}*/

template<class T>
my_vector<T>::~my_vector()
{
	if (preitems != nullptr)
		delete[]preitems;
	if (items != nullptr)
		delete[]items;
}

template<class T>
bool my_vector<T>::checkIndex(int index)
{
	if (index < 0 || index >= capacity)
	{
		int cap = capacity - 1;
		cout << "Out of the range!Please ensure the index be in 0 ~ " << cap << endl;
		return false;
	}
	return true;
}
template<class T>
T& my_vector<T>::operator[](int index) {
	if (!itemsFlag)
		return preitems[index];
	else
		return items[index];
}
template<class T>
void my_vector<T>::set(int index, T elem)
{
	if (checkIndex(index))
	{
		if (!itemsFlag)
			preitems[index] = elem;
		else
			items[index] = elem;
		return;
	}
}

template<class T>
void my_vector<T>::renewCapacity()
{
	capacity++;
	if (!itemsFlag)
	{
		itemsFlag = 1;
		items = new T[capacity];
		for (int i = 0; i < counts; i++)
			*(items + i) = *(preitems + i);
		delete[]preitems;
		preitems = nullptr;
	}
	else
	{
		itemsFlag = 0;
		preitems = new T[capacity];
		for (int i = 0; i < counts; i++)
			*(preitems + i) = *(items + i);
		delete[]items;
		items = nullptr;
	}
}

template<class T>
void my_vector<T>::add(int index,const T &elem)
{
	if (isFull())
	{
		renewCapacity();
	}
	if (checkIndex(index))
		if (!itemsFlag)
		{
			for (int i = counts; i > index; i--)
				preitems[i] = preitems[i - 1];
			preitems[index] = elem;
		}
		else
		{
			for (int i = counts; i > index; i--)
				items[i] = items[i - 1];
			items[index] = elem;
		}
	counts++;
	return;
}

template<class T>
void my_vector<T>::addFirst(T elem)
{
	add(0, elem);
}

template<class T>
void my_vector<T>::push_back(const T &elem)
{
	add(counts, elem);
}

template<class T>
void my_vector<T>::remove(int index)
{
	if (!isEmpty())
	{
		if (checkIndex(index))
		{
			if (!itemsFlag)
			{
				T temp = preitems[index];
				for (int i = index + 1; i < counts; i++)
					preitems[i - 1] = preitems[i];
				counts--;
				
			}
			else
			{
				T temp = items[index];
				for (int i = index + 1; i < counts; i++)
					items[i - 1] = items[i];
				counts--;
				
			}
		}
	}
	else
	{
		cout << "Array is empty!" << '\n';
	}
}
template<class T>
void my_vector<T>::pop_front()
{
	remove(0);
}
template<class T>
void my_vector<T>::pop_back()
{
	remove(counts - 1);
}
template<class T>
T *my_vector<T>::begin() {
	if (!itemsFlag)
		return preitems;
	else
		return items;
}
template<class T>
T *my_vector<T>::end() {
	if (!itemsFlag)
		return &preitems[counts];
	else
		return &items[counts];
}
template<class T>
T &my_vector<T>::front() {
	if (!itemsFlag)
		return preitems[0];
	else
		return items[0];
}
template<class T>
T &my_vector<T>::back() {
	if (!itemsFlag)
		return preitems[counts-1];
	else
		return items[counts-1];
}
template<class T>
void my_vector<T>::clear() {
	if (!itemsFlag)
	{
		itemsFlag = 1;
		items = new T[0];
		delete[]preitems;
		preitems = nullptr;
	}
	else
	{
		itemsFlag = 0;
		preitems = new T[0];
		delete[]items;
		items = nullptr;
	}
}
#endif 
