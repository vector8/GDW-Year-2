#pragma once

namespace flopse
{
	template <class T>
	class Node
	{
	public:
		Node(const T &item, Node<T> *prev = NULL, Node<T> *next = NULL);
		~Node();

		T data;
		Node<T> *prev;
		Node<T> *next;
	};

	#include "Node.tpp"
}