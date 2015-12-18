#pragma once

#include <queue>

template <class T>
class BSTnode {
private:
	unsigned char height;
	BSTnode<T>* left;
	BSTnode<T>* right;
	BSTnode<T>* parent;

public:
	T key;

	BSTnode(const T& _key, BSTnode<T>* _left = nullptr, BSTnode<T>* _right = nullptr, unsigned char _height = 1) {
		key = _key;
		height = _height;
		left = _left;
		right = _right;
		parent = nullptr;
	}

	unsigned char getHeight(void);
	void updateHeight(void);
	char bfactor(void);
	void rotateLeft(void);
	void rotateRight(void);
	bool balance(void);

	template <class U>
	friend class AVLtree;
};

template <class T>
unsigned char BSTnode<T>::getHeight() {
	return height;
}

template <class T>
void BSTnode<T>::updateHeight() {
	unsigned char lheight = (this->left) ? this->left->height : 0;
	unsigned char rheight = (this->right) ? this->right->height : 0;

	this->height = ((lheight > rheight) ? lheight : rheight) + 1;
}

template <class T>
char BSTnode<T>::bfactor() {
	unsigned char lheight = (this->left) ? this->left->height : 0;
	unsigned char rheight = (this->right) ? this->right->height : 0;

	return (rheight - lheight);
}

template <class T>
void BSTnode<T>::rotateLeft() {
	BSTnode<T>* temp = this->right;
	this->right->parent = this->parent;
	this->parent = this->right;
	this->right = this->right->left;
	if (this->right) this->right->parent = this;

	temp->left = this;
	this->updateHeight();
	temp->updateHeight();

	if (temp->parent) {
		if (temp->key > temp->parent->key) temp->parent->right = temp;
		else temp->parent->left = temp;		
	}
}

template <class T>
void BSTnode<T>::rotateRight() {
	BSTnode<T>* temp = this->left;
	this->left->parent = this->parent;
	this->parent = this->left;
	this->left = this->left->right;
	if (this->left) this->left->parent = this;

	temp->right = this;
	this->updateHeight();
	temp->updateHeight();

	if (temp->parent) {
		if (temp->key > temp->parent->key) temp->parent->right = temp;
		else temp->parent->left = temp;
	}
}

template <class T>
bool BSTnode<T>::balance() {
	updateHeight();

	if (bfactor() == 2) {
		if (this->right->bfactor() < 0)
			this->right->rotateRight();
		this->rotateLeft();

		return true;
	}
	else if (bfactor() == -2) {
		if (this->left->bfactor() > 0)
			this->left->rotateLeft();
		this->rotateRight();

		return true;
	}

	return false; // no balance excuted
}



template <class T>
class AVLtree {
private:
	BSTnode<T>* root;

public:
	AVLtree() { root = nullptr; }

	void insert(const T&);
	T find(const T&);
	void cache(const char*);
	void uncache(const char*);
};

template <class T>
void AVLtree<T>::insert(const T& key) {
	BSTnode<T>** p = &root;
	BSTnode<T>* pre = nullptr;

	while (*p) {
		pre = *p;
		if (key > (*p)->key) p = &((*p)->right);
		else p = &((*p)->left);
	}

	*p = new BSTnode<T>(key);
	(*p)->parent = pre;

	// Restore AVL's property
	BSTnode<T>* temp = (*p)->parent;

	while (temp) {
		if (temp->balance()) break;
		temp = temp->parent;
	}

	if (root->parent != nullptr) root = root->parent;
}

template <class T>
T AVLtree<T>::find(const T& key) {
	BSTnode<T>* p = root;

	while (p != nullptr) {
		if (key > p->key) p = p->right;
		else if (key < p->key) p = p->left;
		else break;
	}

	if (p) return p->key;
	else return key;
}

template <class T>
void AVLtree<T>::cache(const char* filename) {
	fstream f;
	f.open(filename, ios::out);

	BSTnode<T>* p = root;
	queue<BSTnode<T>*> nodeQueue;

	if (p) {
		bool flag = false;

		nodeQueue.push(p);
		while (!nodeQueue.empty()) {
			p = nodeQueue.front();
			nodeQueue.pop();

			if (flag) {
				f << '\n';
			}
				
			if (p) f << p->key;
				else f << "null";

			flag = true;

			if (p) {
				if (p->left) {
					nodeQueue.push(p->left);
				}
				else nodeQueue.push(nullptr);

				if (p->right) {
					nodeQueue.push(p->right);
				}
				else nodeQueue.push(nullptr);
			}
		}
	}

	f.close();
}

template <class T>
void AVLtree<T>::uncache(const char* filename) {
	fstream f;
	f.open(filename, ios::in);

	queue<BSTnode<T>*> nodeQueue;

	string word;
	while (getline(f, word, ':')) {
		string denotation;
		if (word != "null") getline(f, denotation);

		if (!root) {
			if (word != "null") root = new BSTnode<T>(dict(word, denotation));
			nodeQueue.push(root);
		}
		else {
			BSTnode<T>* p = nodeQueue.front();
			nodeQueue.pop();

			// left node
			if (word != "null") {
				p->left = new BSTnode<T>(dict(word, denotation));
				nodeQueue.push(p->left);
			}

			// right node
			if (getline(f, word, ':')) {
				if (word != "null") {
					getline(f, denotation);
					p->right = new BSTnode<T>(dict(word, denotation));
					nodeQueue.push(p->right);
				}
			}
		}
	}

	f.close();
}