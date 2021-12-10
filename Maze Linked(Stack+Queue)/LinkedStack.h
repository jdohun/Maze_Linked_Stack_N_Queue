#pragma once
#include "Node.h"

class LinkedStack {
public:
	LinkedStack() : top(NULL) {}
	~LinkedStack(){	// new�� �ݴ� delete�Ͽ� �޸� �Ҵ� ����
		while (!isEmpty()) {
			delete pop();
		}
	}
private:
	Node* top;	// head ������
public:
	bool isEmpty() { return top == NULL; }
	void push(Node* p) { // present node
		if (isEmpty()) { top = p; }
		else {
			p->setLink(top);	// ���ο� ��尡 �� �� ��带 ����Ŵ���ν� �� �� ��尡 ��
			top = p;			// p�� �� ��
		}
	}
	Node* pop() {
		if (isEmpty()) return NULL;
		Node* p = top;
		top = top->getLink();
		return p;
	}
	Node* peek() { return top; };	// top�� NULL�̸� NULL ��ȯ, node�� ������ node ��ȯ��
	void dispaly() {
		printf("[LinkedStack]\n");
		for (Node* p = top; p != NULL; p = p->getLink()) {
			p->display();
		}
		printf("\n");
	}
};