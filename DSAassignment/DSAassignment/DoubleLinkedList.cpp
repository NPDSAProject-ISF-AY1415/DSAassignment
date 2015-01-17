#include "DoubleLinkedList.h"

using namespace std;

DoubleLinkedList::DoubleLinkedList(){ size = 0; }
bool DoubleLinkedList::addFront(ItemType item){
	//Create new node
	Node *newNode = new Node;
	newNode->item = item;	//Store item to node
	newNode->next = NULL;	//Set next and previous to NULL
	newNode->previous = NULL;
	return true;
}