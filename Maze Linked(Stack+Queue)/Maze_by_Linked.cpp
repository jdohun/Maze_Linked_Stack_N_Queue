#include "LinkedStack.h"
#include "LinkedQueue.h"
#include <cstdlib>
#include <Windows.h>

int choice = 0;  // 

class Maze {
public:
    Maze() { init(0, 0); }
    ~Maze() { reset(); }
private:
    int column;             // 미로의 너비
    int row;                // 미로의 높이
    int** map;              // 미로 맵
    LinkedStack locStack;   // 스택
    LinkedQueue locQueue;   // 큐
    Location2D exitLoc;     // 미로의 출구
    int count = 0;          // 길을 찾는 횟수
public:
    void init(int row, int col) { //map 이차원 배열을 동적할당
        map = new int* [row];
        for (int i = 0; i < row; i++)
            map[i] = new int[col];
    }

    void reset() { //미로 맵 maze 동적할당 해제
        for (int i = 0; i < row; i++)
            delete[]map[i];
        delete[]map;
    }

    bool CheckData(const char* fname) {  // 행열 값이 주어지는지 확인
        char load;
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze != NULL) {
            fscanf_s(Maze, "%d %d", &row, &column);         // 첫 줄에서 두개의 숫자 이후에
            while (1) {
                if ((load = fgetc(Maze)) == ' ') continue;  // 공백이 아닐때까지 읽어서
                else if (load == '\n' || load == '\r') {    // 개행 문자 or 캐리지리턴일 경우에
                    init(row, column);     // 첫 줄에서 두개의 숫자는 행열 표기가 맞으므로 2차원 배열 초기화
                    fclose(Maze);
                    return true;    // 행열 값 있음
                }
                else {
                    fclose(Maze);
                    RowNCol(fname); // 행열을 세는 함수 호출
                    return false;   // 행열 값 없음
                }
            }
        }
    }

    void RowNCol(const char* fname) {    // 행열 세기
        char load;
        int c = 0, r = 1;      // 첫줄부터 읽으므로 row는 1부터 시작
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze != NULL) {
            // 첫번째 while문에서는 첫줄만 읽어서 공백이 아닌 부분의 개수를 세어 col의 값을 센다.
            while ((load = fgetc(Maze)) != '\n' && load != '\r') { if (load != ' ') ++c; }
            
            // 두번째 while문에서는 개행문자의 개수만 세어서 row의 값을 센다.
            while ((load = fgetc(Maze)) != EOF) { if (load == '\n') ++r; }
            
            fclose(Maze);
        }
        // printf("행 * 열  = %d * %d\n", r, c);
        row = r;
        column = c;
        init(row, column);
    }

    void Load(const char* fname) { // 파일에서 미로 파일을 읽어옴
        char load;
        FILE* Maze;
        fopen_s(&Maze, fname, "rb");
        if (Maze == NULL) {
            printf("FILE OPEN ERROR\n");
        }
        else {
            if (CheckData(fname)) { // 행열 값이 있으면 한 줄 건너뜀
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
                        else if (load == 'e') { // 입구 위치 저장
                            map[i][j] = load;
                            Node* entry = new Node(i, j);
                            if (choice == 1)
                                locQueue.enqueue(entry);
                            else
                                locStack.push(entry);
                            break;
                        }
                        else if (load == 'x') { // 출구 위치 저장
                            map[i][j] = load;
                            exitLoc.setRow(i);
                            exitLoc.setCol(j);
                            break;
                        }
                    }
                    // 미로 체크용
                    //if (map[i][j] == 'e' || map[i][j] == 'x') { printf("%2c ", map[i][j]); }
                    //else { printf("%2d ", map[i][j]); }
                }
                //printf("\n");
            }
            fclose(Maze);
        }
    }

    bool isValidLoc(int r, int c) {
        if (r < 0 || c < 0 || r >= row || c >= column) //범위를 벗어나면 갈 수 없다
            return false;
        else { //비어있는 통로나 도착지점일 때만 true
            return (map[r][c] == 0 || map[r][c] == 'x');
        }
    }

    void print() {  //현재 Maze를 화면에 출력
        printf("전체 미로의 크기 = %d * %d\n", row, column);

        if (choice == 1) printf("너비 우선 탐색(Queue) 방법으로 ");
        else if (choice == 2) printf("깊이 우선 탐색(Stack) 방법으로 ");
        printf("길을 탐색한 횟수 : %d\n", count);

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < column; ++j) {
                if (map[i][j] == 0) {
                    printf("  ");   // 길
                }
                else if (map[i][j] == 1) {
                    printf("■");    // 벽
                }
                else if (map[i][j] == 7) {
                    printf("□");    // 지나온 경로
                }
                else if (map[i][j] == 'e') {
                    printf("○");    // 입구
                }
                else if (map[i][j] == 'x') {
                    printf("◎");    // 출구
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    void searchExitToStack() { // 실제 미로를 탐색하는 함수
        print();
        while (locStack.isEmpty() == false) {  // 스택이 비어있지 않는 동안
            Location2D* here = locStack.pop()->getLocation(); // 스택에 상단 객체 복사
            ++count;
            int r = here->getRow();
            int c = here->getCol();

            map[r][c] = 7; // 시작점은 최적 경로
            if (exitLoc.getCol() == c && exitLoc.getRow() == r) {
                printf("미로탐색 성공!!\n");
                count = 0;
                choice = 0;
                return;
            }
            else {
                map[r][c] = 7; //지나온 곳으로 표기
                system("cls");
                print();
                // 갈 수 있는 곳 다 가본다
                if (isValidLoc(r - 1, c)) { locStack.push(new Node(r - 1, c)); }
                if (isValidLoc(r + 1, c)) { locStack.push(new Node(r + 1, c)); }
                if (isValidLoc(r, c - 1)) { locStack.push(new Node(r, c - 1)); }
                if (isValidLoc(r, c + 1)) { locStack.push(new Node(r, c + 1)); }
                Sleep(400);
            }
        }
        printf("미로탐색 실패ㅠㅠ\n");
        count = 0;
        choice = 0;
    }

    void searchExitToQueue() { // 실제 미로를 탐색하는 함수
        print();
        while (locQueue.isEmpty() == false) {  // 스택이 비어있지 않는 동안
            Location2D* here = locQueue.dequeue()->getLocation(); // 스택에 상단 객체 복사
            ++count;
            int r = here->getRow();
            int c = here->getCol();

            map[r][c] = 7; // 시작점은 최적 경로
            if (exitLoc.getCol() == c && exitLoc.getRow() == r) {
                printf("미로탐색 성공!!\n");
                count = 0;
                choice = 0;
                return;
            }
            else {
                map[r][c] = 7; // 지나온 곳으로 표기
                system("cls");
                print();
                Sleep(400);

                //갈 수 있는 곳 다 가본다
                if (isValidLoc(r - 1, c)) { locQueue.enqueue(new Node(r - 1, c)); }
                if (isValidLoc(r + 1, c)) { locQueue.enqueue(new Node(r + 1, c)); }
                if (isValidLoc(r, c - 1)) { locQueue.enqueue(new Node(r, c - 1)); }
                if (isValidLoc(r, c + 1)) { locQueue.enqueue(new Node(r, c + 1)); }
            }
        }
        printf("미로탐색 실패ㅠㅠ\n");
        count = 0;
        choice = 0;
    }
};

int main() {
    Maze maze;
    while (true) {
        printf("너비 우선 탐색(Queue) : 1 , 깊이 우선 탐색(Stack) : 2\n");
        scanf_s("%d", &choice);
        maze.Load("미로 행열개수세기2.txt");
        if (choice == 1)
            maze.searchExitToQueue();
        else
            maze.searchExitToStack();
        printf("초기화면 1, 프로그램 종료 2\n");
        scanf_s("%d", &choice);
        if (choice == 2)
            break;
    }
}