// List.h - - Specification of List ADT
#ifndef LIST_H
#define LIST_H

#include<string>
#include<iostream>
using namespace std;
const int MAX_SIZE = 10000; // initialize the size of array, not sure what is the exact max
typedef string ItemType;

class ListArray
{
private:
	int      size; 
	ItemType items[MAX_SIZE]; //item array
public:
	ListArray::ListArray();//constructor
	bool ListArray::add(ItemType newItem);//add a new item to the back of the list (append)
	bool ListArray::add(int index, ItemType newItem);// add a new item at a specified position in the list (insert)
	//bool ListArray::addNoShift(int index, ItemType newItem);//add new item without shifting any position
	void ListArray::remove(int index);// remove an item at a specified position in the list
	ItemType ListArray::get(int index);// get an item at a specified position of the list (retrieve)
	bool ListArray::isEmpty();// check if the list is empty
	int ListArray::getLength();// check the size of the list
	void print();// display all the items in the list
};
#endif