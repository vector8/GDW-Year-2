template <class T>
LinkedList<T>::LinkedList()
{
	head = NULL;
	tail = NULL;
	count = 0;
}

template <class T>
LinkedList<T>::~LinkedList()
{
	clear();
}

template <class T>
int LinkedList<T>::size()
{
	return count;
}

template <class T>
void LinkedList<T>::add(const T &item)
{
	Node<T> *n = new Node<T>(item, tail);

	if (head == NULL)	// list is empty
	{
		head = n;
		tail = n;
	}
	else
	{
		tail->next = n;
		tail = n;
	}

	count++;
}

template <class T>
void LinkedList<T>::add(int index, const T &item)
{
	Node<T> *n = new Node<T>(item);

	if (head == NULL)	// list is empty
	{
		head = n;
		tail = n;
	}
	else
	{
		Node<T> *current = head;

		for (int i = 0; i < index; i++)
		{
			if (current->next != NULL)
			{
				current = current->next;
			}
			else
			{
				// reached the end of the list before reaching index - add new node at the end
				current->next = n;
				n->prev = current;
				break;
			}
		}

		if (n->prev == NULL)
		{
			// reached index before reaching end - insert node here
			n->next = current;
			n->prev = current->prev;
			current->prev = n;
		}
	}

	count++;
}

template <class T>
bool LinkedList<T>::remove(int index)
{
	if (head != NULL)
	{
		Node<T> *current = head;

		for (int i = 0; i < index; i++)
		{
			if (current->next != NULL)
			{
				current = current->next;
			}
			else
			{
				return false;
			}
		}

		Node<T>* prev = current->prev;
		Node<T>* next = current->next;

		if (next != NULL)
		{
			next->prev = prev;
		}

		if (prev != NULL)
		{
			prev->next = next;
		}

		if (current == head)
		{
			head = head->next;
		}
		if (current == tail)
		{
			tail = tail->prev;
		}

		delete current;

		count--;
			
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
bool LinkedList<T>::remove(const T &item)
{
	Node<T> *current = head;

	for (int i = 0; i < count; i++)
	{
		if (current == NULL)
		{
			return false;
		}
		else if (&(current->data) == &item)
		{
			Node<T>* prev = current->prev;
			Node<T>* next = current->next;

			if (next != NULL)
			{
				next->prev = prev;
			}

			if (prev != NULL)
			{
				prev->next = next;
			}

			if (current == head)
			{
				head = head->next;
			}
			if (current == tail)
			{
				tail = tail->prev;
			}

			delete current;

			return true;
		}
		else
		{
			current = current->next;
		}
	}

	return false;
}

template <class T>
void LinkedList<T>::clear()
{
	Node<T>* current = head;

	while (current != NULL)
	{
		current->prev = NULL;
		Node<T>* next = current->next;
		delete current;
		current = next;
	}
}