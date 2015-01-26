/*
ListArray.cpp
Array-based implementation Object Class File
Team Name: Wat.jpg
Team Member 1: Soh Yan Quan, Kenneth (S10147249G)
Team Member 2: Soh Hak Wee (S10146869K)
*/

#include "ListArray.h"  // header file


// constructor
ListArray::ListArray()
{
	size = 0;
}

// add a new item to the back of the list (append)
bool ListArray::add(ItemType newItem)
{
	bool success = size < MAX_SIZE;
	if (success)
	{
		items[size] = newItem; // add to the end of the list
		size++;                // increase the size of the list by one
	}
	return success;
}

// add a new item at a specified position in the list (insert)
bool ListArray::add(int index, ItemType newItem)
{
	bool success = (index >= 1) && (index <= size + 1) && (size < MAX_SIZE);
	if (success)
	{  // make room for new item by shifting all items at
		// positions >= index toward the end of the
		// List (no shift if index == size + 1)
		for (int pos = size; pos >= index; pos--)
			items[pos] = items[pos - 1];
		// insert new item
		items[index - 1] = newItem;
		size++;  // increase the size of the list by one
	}
	return success;
}

/*bool ListArray::addNoShift(int index, ItemType newItem)
{
	bool success = (index >= 1) && (index <= size + 1) && (size < MAX_SIZE);
	if (success)
	{  
		// insert new item
		items[index - 1] = newItem;
		size++;  // increase the size of the list by one
	}
	return success;
}*/

// remove an item at a specified position in the list
void ListArray::remove(int index)
{
	bool success = (index >= 1) && (index <= size);
	if (success)
	{  // delete item by shifting all items at positions >
		// index toward the beginning of the list
		// (no shift if index == size)
		for (int fromPosition = index + 1; fromPosition <= size; fromPosition++)
			items[fromPosition - 2] = items[fromPosition - 1];
		size--;  // decrease the size of the list by one
	}

}

// get an item at a specified position of the list (retrieve)
ItemType ListArray::get(int index)
{
	ItemType dataItem;
	bool success = (index >= 1) && (index <= size);
	if (success)
		dataItem = items[index - 1];

	return dataItem;
}

// check if the list is empty
bool ListArray::isEmpty()
{
	return size == 0;
}

// check the size of the list
int ListArray::getLength()
{
	return size;
}

void ListArray::print()
{	
	if (size == 0)
	{
		cout << "Array is Empty" << endl;
	}
	cout << "=====" << endl;
	for (int i = 0; i < size - 1; i++)
	{
		cout << items[i] << endl; //for loop to print all elements in the array;
	}
	cout << "=====" << endl;
}


