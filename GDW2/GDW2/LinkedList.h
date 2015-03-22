#pragma once
#include <memory>

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

		std::shared_ptr<Node<T>> head;
		std::shared_ptr<Node<T>> tail;

		int size();

		void add(const T &item);
		void add(int index, const T &item);

		bool remove(int index);
		bool remove(const T &item);

		void clear();
	};

	#include "LinkedList.tpp"
}

