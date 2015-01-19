//DoubleLinkedList.h -- Specification of Doubly Linked List ADT
#ifndef DOUBLE_LINKED_LIST
#define DOUBLE_LINKED_LIST

#include<string>
#include<iostream>
using namespace std;

typedef string ItemType;

class DoubleLinkedList{
private:
	struct Node{
		ItemType item;
		Node *next;
		Node *previous;
	};
	Node *firstNode;
	Node *lastNode;
	int size;
public:
	DoubleLinkedList();
	~DoubleLinkedList();
	bool addFront(ItemType item);
	bool addBack(ItemType item);
	bool addAt(int index, ItemType item);
	void removeFront();
	void removeBack();
	void remove(int index);
	ItemType getFront();
	ItemType getBack();
	ItemType get(int index);
	bool isEmpty();
	int getLength();
	void print();
};

#endif