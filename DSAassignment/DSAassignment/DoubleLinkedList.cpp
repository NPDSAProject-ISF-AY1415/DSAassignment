#include "DoubleLinkedList.h"

using namespace std;

DoubleLinkedList::DoubleLinkedList(){ size = 0; }
/*DoubleLinkedList::~DoubleLinkedList(){
	Node *currentNode = firstNode;
	while (currentNode->next != NULL){
		Node *nextNode = currentNode->next;
		delete currentNode;
		currentNode = nextNode;
	}
}*/
bool DoubleLinkedList::addFront(ItemType item){
	//Create new node
	Node *newNode = new Node;
	newNode->item = item;	//Store item to node
	newNode->next = NULL;	//Set next and previous to NULL
	newNode->previous = NULL;

	if (size == 0){
		//First Node
		firstNode = newNode;
		lastNode = newNode;
	}
	else {
		firstNode->previous = newNode;	//Set the old first node as previous of new node
		newNode->next = firstNode;	//Point next at first node
		firstNode = newNode;		//Set first node as new node
	}
	size++;
	return true;
}

bool DoubleLinkedList::addBack(ItemType item){
	Node *newNode = new Node;
	newNode->item = item;
	newNode->next = NULL;
	newNode->previous = NULL;
	if (size == 0){
		//First Node
		firstNode = newNode;
		lastNode = newNode;
	}
	else {
		lastNode->next = newNode;		//Set new node as the next of last node;
		newNode->previous = lastNode;	//Set last node as the previous of new node
		lastNode = newNode;			//Sets the new node as the newest last node
	}
	size++;
	return true;
}

bool DoubleLinkedList::addAt(int index, ItemType item){
	if (!(index >= 1 && index <= size)) return false;
	Node *newNode = new Node;
	newNode->item = item;
	newNode->next = NULL;
	newNode->previous = NULL;

	if (index == 1){
		//Adding in front
		firstNode->previous = newNode;
		newNode->next = firstNode;
		newNode = firstNode;
	}
	else {
		//Traverse to index
		Node *tmpNode = firstNode;
		for (int i = 0; i < index - 2; i++){	//If insert at 8, stop at node 6 (i 5)
			tmpNode = tmpNode->next;
		}
		//Set new node point at indexed node
		newNode->next = tmpNode->next;
		tmpNode->next->previous = newNode;
		newNode->previous = tmpNode;
		tmpNode->next = newNode;
	}
	size++;
	return true;
}

void DoubleLinkedList::removeFront(){
	//Check if its the only node left
	if (size == 1){
		firstNode = NULL;
		lastNode = NULL;
	}
	else {
		//Set first node to 2nd node
		firstNode->next->previous = NULL;		//Set prev of 2nd node to NULL as its the first node now
		firstNode = firstNode->next;
	}
	size--;
}

void DoubleLinkedList::removeBack(){
	//Check if its the only node left
	if (size == 1){
		firstNode = NULL;
		lastNode = NULL;
	}
	else {
		//Set last node to 2nd last node
		lastNode->previous->next = NULL;	//Set next of last 2nd node to NULL as its the last node now
		lastNode = lastNode->previous;
	}
	size--;
}

void DoubleLinkedList::remove(int index){
	if (!(index >= 1 || index <= size)) return;	//Check if index valid
	if (index == 1){
		//Remove first index
		if (size == 1){
			//Only 1 node left
			firstNode = NULL;
			lastNode = NULL;
		}
		else {
			//Set 2nd node as first node
			firstNode->next->previous = NULL;
			firstNode = firstNode->next;
		}
	}
	else if (index == size){
		//It is the back node
		lastNode->previous->next = NULL;	//Set next of last 2nd node to NULL as its the last node now
		lastNode = lastNode->previous;
	}
	else {
		//Traverse to node to remove
		Node *tmpNode = firstNode;
		for (int i = 0; i < index - 2; i++){
			tmpNode = tmpNode->next;
		}
		Node *removedNode = tmpNode->next;
		tmpNode->next = removedNode->next;
		removedNode->next->previous = tmpNode;
		delete removedNode;
	}
	size--;
}

ItemType DoubleLinkedList::getFront(){
	return firstNode->item;		//Return firstnode item
}

ItemType DoubleLinkedList::getBack(){
	return lastNode->item;		//Return lastnode item
}

ItemType DoubleLinkedList::get(int index){
	if (!(index >= 1 || index <= size)) return NULL;	//Check for valid index
	Node *tmpNode = firstNode;
	for (int i = 0; i <= index - 2; i++){
		tmpNode = tmpNode->next;
	}
	return tmpNode->item;
}

bool DoubleLinkedList::isEmpty(){
	return size == 0;
}

int DoubleLinkedList::getLength(){
	return size;
}

void DoubleLinkedList::print(){
	//Set tmp pointer to first Node
	Node *tmpPointer = firstNode;
	cout << "=====" << endl;
	while (tmpPointer != NULL){
		//Retrive and display item
		cout << tmpPointer->item << endl;
		//Point tmpPointer to next node
		tmpPointer = tmpPointer->next;
	}
	cout << "=====" << endl;
}