/*
 * 끔찍했다.
 * https://www.acmicpc.net/problem/15778
 * 시작: 2019-07-24 02:00
 * 종료 : 2019-07-24 05:45
 * https://github.com/xlrnux/boj_15778_yutnori
 */

#include <cstdio>
#define DEBUG 0
using namespace std;

template<typename T>
class vector{
private:
    int _size;
    T DATA[4];
public:
    vector() : _size(0){}
    void clear(){
        _size = 0;
    }
    void push_back(const T& val){
        DATA[_size++] = val;
    }
    T& operator [] (const int pos){
        return DATA[pos];
    }

    T operator [] (const int pos) const{
        return DATA[pos];
    }
    void pop_back(){
        _size--;
    }
    bool empty() const{
        return _size == 0;
    }
    T back() const{
        return DATA[_size-1];
    }
};
vector<char> State[39];
int MOVE[] = {5,1,2,3,4};
const int PATTERN[39][5] = {
        {0,0,0,0,0},
        {2,3,4,5,6},
        {3,4,5,6,7},
        {4,5,6,7,8},
        {5,6,7,8,9},
        {20,21,22,23,24},
        {7,8,9,10,11},
        {8,9,10,11,12},
        {9,10,11,12,13},
        {10,11,12,13,14},
        {25,26,22,27,28},
        {12,13,14,15,16},
        {13,14,15,16,17},
        {14,15,16,17,18},
        {15,16,17,18,19},
        {16,17,18,19,29},
        {17,18,19,29,30},
        {18,19,29,30,30},
        {19,29,30,30,30},
        {29,30,30,30,30},
        {21,22,23,24,15},
        {22,23,24,15,16},
        {27,28,29,30,30}, // 22
        {24,15,16,17,18},
        {15,16,17,18,19},
        {26,22,27,28,29},
        {22,27,28,29,30}, //26
        {28,29,30,30,30},
        {29,30,30,30,30},
        {30,30,30,30,30},
        {30,30,30,30,30}, // 30->30 엄한곳 가지 말고 고정.
        {1,2,3,4,5}, // 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5},// 'A' ~ 'D', 'a'~'d' 스타트
        {1,2,3,4,5}// 'A' ~ 'D', 'a'~'d' 스타트
};
struct Horse{
    char team; // 팀식별
    char name;
    int start; // 각 말별로 시작지점을 차별화한다.
    int point; // 현재 지점.
    Horse() : point(-1){
        // -1 로 초기화 함으로서 반드시 재초기화를 시키는걸 도모한다.
        // 까먹고 안할수도 있으니까 안하면 에러나도록
    }
    void init(char _name){
        name = _name;
        if(_name <= 'D'){ // 말이 대문자
            team = 'A'; // 대문자팀
            point = start = _name - 'A' + 31;
        }else{ // 말이 소문자
            team = 'a'; // 소문자팀
            point = start = _name - 'a' + 35;
        }
        State[start].push_back(_name);
    }
    void set()
    {
        point = start;
        State[start].push_back(name);
    }
} horse[128];

void init(){
    horse['A'].init('A');
    horse['B'].init('B');
    horse['C'].init('C');
    horse['D'].init('D');
    horse['a'].init('a');
    horse['b'].init('b');
    horse['c'].init('c');
    horse['d'].init('d');
}

struct tagPos { // 지점별 좌표를 위해 정의
    int n;
    int m;
    tagPos(){}
    tagPos(int n, int m) : n(n), m(m){}
};

tagPos printPos[][4] = {
{tagPos(0,0),tagPos(0,0),tagPos(0,0),tagPos(0,0)},
{tagPos(24,30),tagPos(24,31),tagPos(25,30),tagPos(25,31)},
{tagPos(18,30),tagPos(18,31),tagPos(19,30),tagPos(19,31)},
{tagPos(12,30),tagPos(12,31),tagPos(13,30),tagPos(13,31)},
{tagPos(6,30),tagPos(6,31),tagPos(7,30),tagPos(7,31)},
{tagPos(0,30),tagPos(0,31),tagPos(1,30),tagPos(1,31)},
{tagPos(0,24),tagPos(0,25),tagPos(1,24),tagPos(1,25)},
{tagPos(0,18),tagPos(0,19),tagPos(1,18),tagPos(1,19)},
{tagPos(0,12),tagPos(0,13),tagPos(1,12),tagPos(1,13)},
{tagPos(0,6),tagPos(0,7),tagPos(1,6),tagPos(1,7)},
{tagPos(0,0),tagPos(0,1),tagPos(1,0),tagPos(1,1)},
{tagPos(6,0),tagPos(6,1),tagPos(7,0),tagPos(7,1)},
{tagPos(12,0),tagPos(12,1),tagPos(13,0),tagPos(13,1)},
{tagPos(18,0),tagPos(18,1),tagPos(19,0),tagPos(19,1)},
{tagPos(24,0),tagPos(24,1),tagPos(25,0),tagPos(25,1)},
{tagPos(30,0),tagPos(30,1),tagPos(31,0),tagPos(31,1)},
{tagPos(30,6),tagPos(30,7),tagPos(31,6),tagPos(31,7)},
{tagPos(30,12),tagPos(30,13),tagPos(31,12),tagPos(31,13)},
{tagPos(30,18),tagPos(30,19),tagPos(31,18),tagPos(31,19)},
{tagPos(30,24),tagPos(30,25),tagPos(31,24),tagPos(31,25)},
{tagPos(5,25),tagPos(5,26),tagPos(6,25),tagPos(6,26)},
{tagPos(10,20),tagPos(10,21),tagPos(11,20),tagPos(11,21)},
{tagPos(15,15),tagPos(15,16),tagPos(16,15),tagPos(16,16)},
{tagPos(20,10),tagPos(20,11),tagPos(21,10),tagPos(21,11)},
{tagPos(25,5),tagPos(25,6),tagPos(26,5),tagPos(26,6)},
{tagPos(5,5),tagPos(5,6),tagPos(6,5),tagPos(6,6)},
{tagPos(10,10),tagPos(10,11),tagPos(11,10),tagPos(11,11)},
{tagPos(20,20),tagPos(20,21),tagPos(21,20),tagPos(21,21)},
{tagPos(25,25),tagPos(25,26),tagPos(26,25),tagPos(26,26)},
{tagPos(30,30),tagPos(30,31),tagPos(31,30),tagPos(31,31)},
};
int main() {
    if(DEBUG) {
        for (int i = 0; i < 39; i++) {
            printf("%d: ", i);
            for (int j = 1; j <= 5; j++) {
                printf("%d ", PATTERN[i][j]);
            }
            printf("\n");
        }
    }
    int TC = 1;
    // 비주얼 스튜디오면 아래소스
    //freopen("input.txt","r",stdin); scanf(" %d", &TC);
    // CLion 이면 아래 소스
    //freopen("../input.txt","r",stdin); scanf(" %d", &TC);
    for (int tc = 1; tc <= TC; tc++) {
        char Map[32][33] = {
                "..----..----..----..----..----..",
                "..    ..    ..    ..    ..    ..",
                "| \\                          / |",
                "|  \\                        /  |",
                "|   \\                      /   |",
                "|    ..                  ..    |",
                "..   ..                  ..   ..",
                "..     \\                /     ..",
                "|       \\              /       |",
                "|        \\            /        |",
                "|         ..        ..         |",
                "|         ..        ..         |",
                "..          \\      /          ..",
                "..           \\    /           ..",
                "|             \\  /             |",
                "|              ..              |",
                "|              ..              |",
                "|             /  \\             |",
                "..           /    \\           ..",
                "..          /      \\          ..",
                "|         ..        ..         |",
                "|         ..        ..         |",
                "|        /            \\        |",
                "|       /              \\       |",
                "..     /                \\     ..",
                "..   ..                  ..   ..",
                "|    ..                  ..    |",
                "|   /                      \\   |",
                "|  /                        \\  |",
                "| /                          \\ |",
                "..    ..    ..    ..    ..    ..",
                "..----..----..----..----..----.."
        };
        init();
        int throwCnt;
        scanf(" %d", &throwCnt);
        while(throwCnt--){
            /* State[30] (=도착한 애들) 체크하고  수거하자 */
            State[30].clear();
            /* 수거 완료 */
            char name;
            scanf(" %c", &name);
            if(DEBUG) {
                printf(" %c\t", name);
            }
            int move = 0;
            for(int i = 0; i < 4; i++){
                char input;
                scanf(" %c", &input);
                if(input - 'B') move++;
            }
            int nowPoint = horse[name].point;
            int nextPoint = PATTERN[ nowPoint ][ MOVE[move] -1 ];
            if(DEBUG){
                printf("nowPoint: %2d\t", nowPoint);
                printf("MOVE[move]:%2d\t", MOVE[move]);
                printf("nextPoint: %2d\t", PATTERN[ nowPoint ][ MOVE[move] ]);
            }
            if(!State[nextPoint].empty()
            && horse[name].team != horse[State[nextPoint].back()].team // 적이다!!
            && nextPoint != 0 // 이건 의미 없는 인덱스라 제외
            && nextPoint < 30 // 도착지점과 출발지점은 잡기 안하는걸로
            ) {
                if(DEBUG){
                    printf("Kill!!: ");
                }
                while(!State[nextPoint].empty()){
                    char _name = horse[State[nextPoint].back()].name;
                    State[nextPoint].pop_back();
                    horse[_name].set();
                    if(DEBUG){
                        printf("%c ", _name);
                    }
                }
                if(DEBUG){
                    printf("\t");
                }
            }

            while(!State[nowPoint].empty()){
                char _name = horse[State[nowPoint].back()].name;
                horse[_name].point = nextPoint;
                if(DEBUG){
                    printf("%c %d->%d\t",_name, nowPoint, nextPoint);
                }
                State[nextPoint].push_back(_name);

                State[nowPoint].pop_back();
            }
            if(DEBUG){
                printf("\n");
            }
        }

        for(char name = 'A'; name <= 'D'; name++){
            int nowPoint = horse[name].point;
            if(nowPoint >= 30 ) {
                State[nowPoint].pop_back(); // 초기화 대비하여 뽑아만준다
                continue;
            }
            if(DEBUG){
                printf("%c is in point %2d! ", name, nowPoint);
            }

            tagPos target = printPos[nowPoint][name-'A'];
            Map[target.n][target.m] = name;
            if(DEBUG) printf("%c:[%d,%d]  ", name, target.n,target.m);
        }
        for(char name = 'a'; name <= 'd'; name++){
            int nowPoint = horse[name].point;
            if(nowPoint >= 30 ) {
                State[nowPoint].pop_back(); // 초기화 대비하여 뽑아만준다
                continue;
            }
            if(DEBUG){
                printf("%c is in point %2d! ", name, nowPoint);
            }

            tagPos target = printPos[nowPoint][name-'a'];
            Map[target.n][target.m] = name;
            if(DEBUG) printf("%c:[%d,%d]  ", name, target.n,target.m);
        }

        if(DEBUG) printf("\n#%d\n", tc);

        for(int i = 0; i < 32; i++){
            printf("%s\n", Map[i]);
        }
    }
    return 0;
}