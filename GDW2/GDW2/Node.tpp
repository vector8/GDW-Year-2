template <class T>
Node<T>::Node(const T &item, std::shared_ptr<Node<T>> prev, std::shared_ptr<Node<T>> next) : data(item), prev(prev), next(next)
{

}

template <class T>
Node<T>::~Node()
{
}