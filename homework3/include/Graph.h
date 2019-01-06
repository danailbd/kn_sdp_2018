#pragma once

/* #include "Node.h" */
#include "LList.h"

template <typename T>
class Graph
{
private:
	LList<LList<T> > g;
public:
	Node<T>* getVertexPointer(const T&);
	void addVertex(const T&);
	void deleteVertex(const T&);
	bool hasVertex(const T&);
	void addEdge(const T&, const T&);
	void deleteEdge(const T&, const T&);
	bool hasEdge(const T&, const T&);

	void enterGraph();
	LList<T> getVertices();
	bool isEmpty();
	void print();
};

template <typename T>
void Graph<T>::addVertex(const T& a)
{
	LList<T> l;
	l.insertToEnd(a);

	g.insertToEnd(l);
}

template <typename T>
void Graph<T>::deleteVertex(const T& a)
{
	g.iterStart();
	Node<LList<T> > *listPtr = g.iterNext();
	
	while (listPtr != nullptr) {
		listPtr->data.iterStart();
		Node<T> *ptr = listPtr->data.iterNext();

		if (ptr->data == a) {
			LList<T> temp;
			g.deleteElem(listPtr, temp);
		}
		else {
			while (ptr != nullptr)
			{
				if (ptr->data == a) {
					T temp;
					listPtr->data.deleteElem(ptr, temp);
				}

				ptr = listPtr->data.iterNext();
			}
		}

		listPtr = g.iterNext();
	}
}

template <typename T>
void Graph<T>::addEdge(const T& a, const T& b)
{
	g.iterStart();
	Node<LList<T> > *listPtr = g.iterNext();

	while (listPtr != nullptr) {
		listPtr->data.iterStart();
		Node<T> *ptr = listPtr->data.iterNext();

		if (ptr->data == a)
			break;

		listPtr = g.iterNext();
	}

	if (listPtr != nullptr) {
		listPtr->data.insertToEnd(b);
	}
	else
		std::cerr << "Starting vertex " << a << " not found" << std::endl;
}

template <typename T>
void Graph<T>::deleteEdge(const T& a, const T& b)
{
	g.iterStart();
	Node<LList<T> > *p;
	Node<T> *q;
	T x;

	do
	{
		p = g.iterNext();
		p->data.iterStart();
		q = p->data.iterNext();
	} while (q->data != a);

	while (q->data != b)
		q = q->next;

	p->data.deleteElem(q, x);
}

template <typename T>
bool Graph<T>::hasVertex(const T& a)
{
	if (isEmpty())
		return false;
	g.iterStart();
	Node<LList<T> > *p = g.iterNext();
	Node<T> *q;
	do
	{
		p->data.iterStart();
		q = p->data.iterNext();
		p = p->next;
	} while (q->data != a && p);

	return q->data == a;
}

template <typename T>
bool Graph<T>::hasEdge(const T& a, const T& b)
{
	Node<T> *ptr = getVertexPointer(a);

	if (ptr == nullptr)
		return false;

	ptr = ptr->next;

	while (ptr != nullptr) {
		if (ptr->data == b)
			return true;
		ptr = ptr->next;
	}

	return false;
}

template <typename T>
bool Graph<T>::isEmpty()
{
	return g.length() == 0;
}

template <typename T>
Node<T>* Graph<T>::getVertexPointer(const T& a)
{
	g.iterStart();
	Node<LList<T> > *p;
	Node<T> *q;

	do
	{
		p = g.iterNext();
		p->data.iterStart();
		q = p->data.iterNext();
	} while (q->data != a);

	return q;
}

template <typename T>
LList<T> Graph<T>::getVertices()
{
	LList<T> l;
	g.iterStart();
	Node<LList<T> > *p = g.iterNext();
	while (p)
	{
		p->data.iterStart();
		Node<T> *q = p->data.iterNext();
		l.insertToEnd(q->data);
		p = p->next;
	}
	return l;
}

template <typename T>
void Graph<T>::enterGraph()
{
	char c;
	do
	{
		std::cout << "Enter graph vertex: ";
		int x; std::cin >> x;
		addVertex(x);
		std::cout << "Add another vertex (y/n)? "; std::cin >> c;
	} while (c == 'y');
	std::cout << "Edges:\n";
	do
	{
		std::cout << "Enter start vertex: ";
		int x; std::cin >> x;
		std::cout << "Enter end vertex: ";
		int y; std::cin >> y;
		addEdge(x, y);
		std::cout << "Add another edge (y/n)? "; std::cin >> c;
	} while (c == 'y');
}

template <typename T>
void Graph<T>::print()
{
	g.iterStart();
	Node<LList<T> > *p = g.iterNext();
	while (p)
	{
		p->data.print();
		p = p->next;
	}
	std::cout << std::endl;
}
