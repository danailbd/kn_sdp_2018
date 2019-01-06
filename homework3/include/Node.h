#pragma once

template <typename T>
struct Node {
	T data;
	Node<T> *next;

	Node(T const& _data, Node<T>* _next = nullptr) : data(_data), next(_next) {};
};