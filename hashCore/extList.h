#pragma  once

template <class T>
class node {
public:
	T key;
	node<T>* next;

	node(const T& _key, node<T>* _next = nullptr) {
		key = _key;
		next = _next;
	}
};



template <class T>
class list {
protected:
	node<T>* head;

public:
	list(void) { head = nullptr; }

	bool push(const T& key);
	bool isEmpty(void);
	T find(const T&);
};

template <class T>
bool list<T>::push(const T& key) {
	node<T>* p = head;
	node<T>* prev = nullptr;
	
	while (p) {
		prev = p;
		p = (p)->next;
	}

	p = new node<T>(key);
	if (!head) head = p;
	if (prev) prev->next = p;

	return true;
}

template <class T>
bool list<T>::isEmpty() {
	return (!head);
}

template <class T>
T list<T>::find(const T& key) {
	node<T>* p = head;

	while (p) {
		if (p->key != key)
			p = p->next;
		else
			return p->key;
	}

	return key;
}