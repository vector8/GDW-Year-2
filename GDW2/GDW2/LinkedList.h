#pragma once
#include "Node.h"

namespace flopse
{
	template <class T>
	class LinkedList
	{
	private:
		int count;

	public:
		LinkedList();
		virtual ~LinkedList();

		Node<T> *head;
		Node<T> *tail;

		int size();

		void add(const T &item);
		void add(int index, const T &item);

		bool remove(int index);
		bool remove(const T &item);

		void clear();
	};

	#include "LinkedList.tpp"
}

