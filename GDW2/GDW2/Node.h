#pragma once

#include <memory>

namespace flopse
{
	template <class T>
	class Node
	{
	public:
		Node(const T &item, std::shared_ptr<Node<T>> prev = nullptr, std::shared_ptr<Node<T>> next = nullptr);
		virtual ~Node();

		T data;
		std::shared_ptr<Node<T>> prev;
		std::shared_ptr<Node<T>> next;
	};

	#include "Node.tpp"
}