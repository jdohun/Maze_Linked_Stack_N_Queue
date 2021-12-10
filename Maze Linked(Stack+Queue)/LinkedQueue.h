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
			rear->setLink(p);	// p 이전의 노드가 p를 link 하고
			rear = p;	// rear가 p를 가리키게
		}
	}
	Node* dequeue() {
		if (isEmpty()) return NULL;
		else {
			Node* p = front;	// 맨 앞의 노드 반환
			front = front->getLink();
			if (front == NULL) rear = NULL; // front가 NULL이면 큐가 비었음 -> 처음 생성과 같은 상태로 rear도 NULL로 초기화
			return p;
		}
	}
	Node* peek() { return front; };	// front가 NULL이면 NULL 반환되고 front에 노드가 있으면 node가 반환됨
	void display() {
		printf("[큐 내용] : ");
		for (Node* p = front; p != NULL; p = p->getLink()) {
			p->display();
		}
		printf("\n");
	}
};