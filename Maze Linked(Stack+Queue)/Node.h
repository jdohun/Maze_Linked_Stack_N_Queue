#pragma once
#include "Location2D.h"
	
// Student ��ü�� ������ ��� Ŭ���� : ��ü ���� link�� ���� ����
class Node : public Location2D{
public:
	Node(int row, int col)
		: Loc(new Location2D(row, col)), link(NULL)
	{ }
	~Node() { if (Loc != NULL) delete Loc; }
private:
	Location2D* Loc;
	Node* link;	// ���� ��带 ����ų ������ ����
public:
	Node* getLink() { return link; }
	void setLink(Node* p) { link = p; }
	Location2D* getLocation() { return Loc; }
};