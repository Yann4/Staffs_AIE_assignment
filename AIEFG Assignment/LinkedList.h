#pragma once
#include <exception>

template <typename T>
class LinkedList
{
private:
	struct Node
	{
		T data;
		Node* next;

		Node() : data(T()), next(nullptr) {};
		Node(T data, Node* next) : data(data), next(next) {};
	};

	Node* head;
	int count;

public:
	LinkedList();
	~LinkedList();

	inline int size() { return count; };

	T at(int index);
	int contains(T data);

	void insertAt(T data, int index);
	void append(T data);
	void sortedInsert(T data);

	void remove(int index);
};

template <typename T>
LinkedList<T>::LinkedList()
{
	head = nullptr;
	count = 0;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	Node* temp;

	while (head != nullptr)
	{
		temp = head->next;
		delete head;
		head = temp;
	}
}

template <typename T>
void LinkedList<T>::insertAt(T data, int index)
{
	if (index < 0 || index > count)
	{
		throw std::out_of_range("Index is out of range");
		return;
	}

	int i = 0;
	Node* temp = head;

	if (temp == nullptr)
	{
		head = new Node(data, nullptr);
		count++;
		return;
	}

	if (index == 0)
	{
		head = new Node(data, temp);
		count++;
		return;
	}

	while (i != index - 1)
	{
		temp = temp->next;
		i++;
	}

	Node* newNode = new Node(data, temp->next);
	count++;
	temp->next = newNode;
}

template <typename T>
void LinkedList<T>::sortedInsert(T data)
{
	if (count == 0)
	{
		append(data);
		return;
	}

	for (int i = 0; i < count; i++)
	{
		if (data <= at(i))
		{
			if (i == 0)
			{
				insertAt(data, 0);
				return;
			}
			else
			{
				insertAt(data, i - 1);
				return;
			}
		}
	}
	append(data);
}

template <typename T>
void LinkedList<T>::append(T data)
{
	Node* temp = head;

	if (temp == nullptr)
	{
		head = new Node(data, nullptr);
		count++;
		return;
	}

	while (temp->next != nullptr)
	{
		temp = temp->next;
	}

	temp->next = new Node(data, nullptr);
	count++;
}

template <typename T>
T LinkedList<T>::at(int index)
{
	if (index < 0 || index > count - 1)
	{
		throw std::out_of_range("Index out of range");
		return T();
	}

	int i = 0;
	Node* temp = head;
	while (i != index)
	{
		temp = temp->next;
		i++;
	}

	return temp->data;
}

template <typename T>
int LinkedList<T>::contains(T data)
{
	if (head == nullptr)
	{
		return -1;
	}

	int index = 0;

	for (Node* temp = head; temp->next != nullptr; temp = temp->next)
	{
		if (data == temp->data)
		{
			return index;
		}
		index++;
	}
	return -1;
}

template <typename T>
void LinkedList<T>::remove(int index)
{
	if (index < 0 || index > count - 1)
	{
		throw std::out_of_range("Index out of range");
		return;
	}

	if (index == 0)
	{
		Node* next = head->next;
		delete head;
		count--;
		head = next;
		return;
	}

	int i = 0;
	Node* temp = head;
	while (i != index - 1)
	{
		temp = temp->next;
		i++;
	}

	Node* toRemove = temp->next;
	temp->next = toRemove->next;
	delete toRemove;
	count--;
}
