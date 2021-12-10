#include "LinkedStack.h"
#include "LinkedQueue.h"
#include <cstdlib>
#include <Windows.h>

class Maze {
public:
    Maze() { init(0, 0); }
    ~Maze() { reset(); }
private:
    int column;             // �̷��� �ʺ�
    int row;                // �̷��� ����
    int** map;              // �̷� ��
    LinkedStack locStack;   // ����
    LinkedQueue locQueue;   // ť
    Location2D exitLoc;     // �̷��� �ⱸ
public:
    void init(int row, int col) { //map ������ �迭�� �������� �Ҵ�
        map = new int* [row];
        for (int i = 0; i < row; i++)
            map[i] = new int[col];
    }
    void reset() { //�̷� �� maze�� �������� ����
        for (int i = 0; i < row; i++)
            delete[]map[i];
        delete[]map;
    }

    bool CheckData(const char* fname) {     // �࿭ ���� �־������� Ȯ��
        char load;
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze != NULL) {
            fscanf_s(Maze, "%d %d", &column, &row);
            if ((load = fgetc(Maze)) == '\n' || load == '\r') {
                init(row, column);
                fclose(Maze);
                return true;    // �࿭ �� ����
            }
            else {
                fclose(Maze);
                RowNCol(fname);
                return false;   /// �࿭ �� ����
            }
        }
    }

    void RowNCol(const char* fname) {    // �࿭ ����
        char load;
        int c = 0, r = 1;
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze != NULL) {
            while ((load = fgetc(Maze)) != '\n' && load != '\r') { if (load != ' ') ++c; }
            while ((load = fgetc(Maze)) != EOF) { if (load == '\n') ++r; }
            fclose(Maze);
        }
        // printf("�� * ��  = %d * %d\n", r, c);
        row = r;
        column = c;
        init(row, column);
    }

    void Load(const char* fname, int check) { //���Ͽ��� �̷� ������ �о��
        char load;
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze == NULL) {
            printf("FILE OPEN ERROR\n");
        }
        else {
            if (CheckData(fname)) { // �࿭ ���� ������ �� �� �ǳʶ�
                while (1) {
                    if (fgetc(Maze) == '\n') break;
                };
            }

            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < column; ++j) {
                    while (1) {
                        load = getc(Maze);
                        if ((load >= '0' && load <= '9')) {
                            ungetc(load, Maze);
                            fscanf_s(Maze, "%d", &map[i][j]);
                            break;
                        }
                        else if (load == 'e') { // �Ա� ��ġ ����
                            map[i][j] = load;
                            Node* entry = new Node(i, j);
                            if (check == 1)
                                locQueue.enqueue(entry);
                            else
                                locStack.push(entry);
                            break;
                        }
                        else if (load == 'x') { // �ⱸ ��ġ ����
                            map[i][j] = load;
                            exitLoc.setRow(i);
                            exitLoc.setCol(j);
                            break;
                        }
                    }
                    // �̷� üũ��
                    //if (map[i][j] == 'e' || map[i][j] == 'x') { printf("%2c ", map[i][j]); }
                    //else { printf("%2d ", map[i][j]); }
                }
                //printf("\n");
            }
            fclose(Maze);
        }
    }

    bool isValidLoc(int r, int c) {
        if (r < 0 || c < 0 || r >= row || c >= column) //������ ����� �� �� ����
            return false;
        else //����ִ� ��γ� ���������� ���� true
            return (map[r][c] == 0 || map[r][c] == 'x');
    }

    void print() {  //���� Maze�� ȭ�鿡 ���
        printf("��ü �̷��� ũ�� = %d * %d\n", row, column);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < column; ++j) {
                if (map[i][j] == 0) {
                    printf("  ");   // ��
                }
                else if (map[i][j] == 1) {
                    printf("��");    // ��
                }
                else if (map[i][j] == 7) {
                    printf("��");    // ������ ���
                }
                else if (map[i][j] == 'e') {
                    printf("��");    // �Ա�
                }
                else if (map[i][j] == 'x') {
                    printf("��");    // �ⱸ
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    void searchExitToStack() { //���� �̷θ� Ž���ϴ� �Լ�
        print();
        Sleep(400);
        while (locStack.isEmpty() == false) {  //������ ������� �ʴ� ����
            Location2D* here = locStack.pop()->getLocation(); //���ÿ� ��� ��ü ����
            int r = here->getRow();
            int c = here->getCol();

            map[r][c] = 7; //�������� ���� ���
            if (exitLoc.getCol() == c && exitLoc.getRow() == r) {
                return;
            }
            else {
                map[r][c] = 7; //������ ������ ǥ��
                system("cls");
                print();
                //�� �� �ִ� �� �� ������
                if (isValidLoc(r - 1, c)) { locStack.push(new Node(r - 1, c)); }
                if (isValidLoc(r + 1, c)) { locStack.push(new Node(r + 1, c)); }
                if (isValidLoc(r, c - 1)) { locStack.push(new Node(r, c - 1)); }
                if (isValidLoc(r, c + 1)) { locStack.push(new Node(r, c + 1)); }
                Sleep(400);
            }
        }
    }

    void searchExitToQueue() { //���� �̷θ� Ž���ϴ� �Լ�
        print();
        while (locQueue.isEmpty() == false) {  //������ ������� �ʴ� ����
            Location2D* here = locQueue.dequeue()->getLocation(); //���ÿ� ��� ��ü ����
            int r = here->getRow();
            int c = here->getCol();

            map[r][c] = 7; //�������� ���� ���
            if (exitLoc.getCol() == c && exitLoc.getRow() == r) {
                return;
            }
            else {
                map[r][c] = 7; //������ ������ ǥ��
                system("cls");
                print();
                Sleep(400);

                //�� �� �ִ� �� �� ������
                if (isValidLoc(r - 1, c)) { locQueue.enqueue(new Node(r - 1, c)); }
                if (isValidLoc(r + 1, c)) { locQueue.enqueue(new Node(r + 1, c)); }
                if (isValidLoc(r, c - 1)) { locQueue.enqueue(new Node(r, c - 1)); }
                if (isValidLoc(r, c + 1)) { locQueue.enqueue(new Node(r, c + 1)); }
            }
        }
    }
};

int main() {
    Maze maze;
    int check = 0;
    while (true) {
        printf("�ʺ�켱���� Ž���ϰ� ������ 1��, ���� �켱���� Ž���ϰ� ������ 2�� �Է��ϼ���.");
        scanf_s("%d", &check);
        maze.Load("�̷� test.txt", check);
        if (check == 1)
            maze.searchExitToQueue();
        else
            maze.searchExitToStack();
        printf(" �ʱ�ȭ������ ���ð� ������ 1�� ���α׷� ���Ḧ ���Ͻø� 2���� �Է��ϼ���");
        scanf_s("%d", &check);
        if (check == 2)
            break;
    }
}