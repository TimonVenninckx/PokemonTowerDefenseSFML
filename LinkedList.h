#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <typename T>
class LinkedList {
public:
	T* value{nullptr};
	LinkedList<T>* next{ nullptr };

	// add a new item
	void add(T* item) {
		LinkedList<T>* temp = new LinkedList<T>;
		temp->value = item;
		temp->next = this->next;
		this->next = temp;
	}



};

#endif