template <class T>
Node<T>::Node(const T &item, Node<T> *prev, Node<T> *next) : data(item), prev(prev), next(next)
{

}

template <class T>
Node<T>::~Node()
{
	//delete data;
	//data = nullptr;
}