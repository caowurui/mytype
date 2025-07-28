#include <raylib/raylib.h>
#include <vector>

const int WinWidth = 400;               //窗口宽度
const int WinHeight = 600;              //窗口高度
const int RedLine = 400;                //红线所在位置（纵坐标）
const int RedLineWidth = 4;             //红线高度

const int letter_rate = 100;            //字母生成速率
const int letter_size = 20;             //字母大小
const int letter_v = 2;                 //字母下落速率

bool GameOver = true;                   //游戏是否中止

struct aLetter{
    int x,y;
    aLetter(int _x) : x{_x},y{0} {};
};
std::vector<aLetter> letters[26];       //存储各种字母

void Process();                         //主体游戏逻辑
void GenerateLetter();                  //随机生成新的字母
void Draw();                            //绘制字母
void ToBeContinued();                   //游戏中止

int main()
{
    SetTargetFPS(60);
    InitWindow(WinWidth,WinHeight,"My Type");

    while(!WindowShouldClose()){
        if(!GameOver){
            Process();
            Draw();
        }
        else
            ToBeContinued();
    }

    CloseWindow();
    return 0;
}

void Process(){
//定时生成字母
    static int time=0;
    time++;
    if(time%letter_rate==0)
        GenerateLetter();

//按到对应字母后消除
    for(int i=65;i<=90;i++)
        if(IsKeyPressed(i)){
            letters[i-65].clear();
        }

//字母整体位移
    for(int i=0;i<26;i++)
        for(std::vector<aLetter>::iterator it = letters[i].begin();it!=letters[i].end();it++)
            it->y += letter_v;
//判断是否到达红线
    for(int i=0;i<26;i++)
        for(std::vector<aLetter>::iterator it = letters[i].begin();it!=letters[i].end();it++)
            if(it->y+letter_size>=RedLine)
                GameOver=true;
}

void GenerateLetter(){
    int ranchar=GetRandomValue(0,25);
    int ranx=GetRandomValue(letter_size,WinWidth-2*letter_size);
    aLetter t{ranx};
    letters[ranchar].push_back(t);
}

void Draw(){
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(0,RedLine,WinWidth,RedLineWidth,RED);

        for(int i=0;i<26;i++)
            for(aLetter it:letters[i]){
                char tmp[2]={'\0','\0'};
                tmp[0]=i+'a';
                DrawText(tmp,it.x,it.y,letter_size,BLACK);
            }

    EndDrawing();
}

void ToBeContinued(){
    if(IsKeyPressed(KEY_ENTER)){
        GameOver=false;
        for(int i=0;i<26;i++)
            letters[i].clear();
    }

    BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0,RedLine,WinWidth,RedLineWidth,RED);

        DrawText("Press [ENTER] to continue.",0,WinHeight/2,letter_size,BLACK);

    EndDrawing();
}
