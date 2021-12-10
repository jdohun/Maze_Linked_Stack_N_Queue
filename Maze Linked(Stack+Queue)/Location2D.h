#pragma once
#include <cstdio>
class Location2D {
public:
	Location2D(int r = 0, int c = 0)
		: row(r), col(c) 
	{ }
private:
	int row;	 // ���� ��ġ�� �� ��ȣ
	int col;	 // ���� ��ġ�� �� ��ȣ
public:
	// ��ġ p�� �ڽ��� �̿����� �˻��ϴ� �Լ�
	bool isNeighbor(Location2D& p) {
		return ((row == p.row && (col == p.col - 1 || col == p.col + 1)) // ����ȣ�� ���� ���� �̿��ϰų�
			|| (col == p.col && (row == p.row - 1 || row == p.row + 1))); // ���ȣ�� ���� ���� �̿��ϸ� true
	}
	// ��ġ p�� �ڽŰ� ���� ��ġ������ �˻��ϴ� �Լ� (������ �����ε�)
	bool operator==(Location2D& p) { return row == p.row && col == p.col; }
	
	void display() { printf("row : %d, col : %d\n", row, col); }

	int getRow() { return row; }
	int getCol() { return col; }
	void setRow(int r) { row = r; }
	void setCol(int c) { col = c; }
};