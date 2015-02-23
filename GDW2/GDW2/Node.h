#pragma once

namespace flopse
{
	template <class T>
	class Node
	{
	public:
		Node(const T &item, Node<T> *prev = nullptr, Node<T> *next = nullptr);
		virtual ~Node();

		T data;
		Node<T> *prev;
		Node<T> *next;
	};

	#include "Node.tpp"
}