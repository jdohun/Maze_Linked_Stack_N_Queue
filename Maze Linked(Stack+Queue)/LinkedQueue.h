#pragma once
#include "Node.h"

class LinkedQueue{
public:
	LinkedQueue()
		: front(NULL), rear(NULL)
	{}
	~LinkedQueue() {
		while (!isEmpty()) {
			delete dequeue();
		}
	}
private:
	Node* front;
	Node* rear;
public:
	bool isEmpty() { return front == NULL; }
	void enqueue(Node* p) {	// present node
		if (isEmpty()) front = rear = p;
		else {
			rear->setLink(p);	// p ������ ��尡 p�� link �ϰ�
			rear = p;	// rear�� p�� ����Ű��
		}
	}
	Node* dequeue() {
		if (isEmpty()) return NULL;
		else {
			Node* p = front;	// �� ���� ��� ��ȯ
			front = front->getLink();
			if (front == NULL) rear = NULL; // front�� NULL�̸� ť�� ����� -> ó�� ������ ���� ���·� rear�� NULL�� �ʱ�ȭ
			return p;
		}
	}
	Node* peek() { return front; };	// front�� NULL�̸� NULL ��ȯ�ǰ� front�� ��尡 ������ node�� ��ȯ��
	void display() {
		printf("[ť ����] : ");
		for (Node* p = front; p != NULL; p = p->getLink()) {
			p->display();
		}
		printf("\n");
	}
};