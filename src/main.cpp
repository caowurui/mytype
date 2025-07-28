#include <raylib/raylib.h>
#include <vector>

const int WinWidth = 480;               //窗口宽度
const int WinHeight = 600;              //窗口高度
const int RedLine = 400;                //红线所在位置（纵坐标）
const int RedLineWidth = 4;             //红线高度
const Color BackgroundColor = LIGHTGRAY;//背景颜色

const int letter_rate = 100;            //字母生成速率
const int letter_v = 2;                 //字母下落速率

const int font_size = 30;               //字母大小
const float spacing = 0.0f;             //字母间距
const char font_name[] =
        "C:/Windows/Fonts/simhei.ttf";  //字体文件路径
const char characters[]=
        "abcdefghijklmnopqrstuvwxyzENPRT0123456789.:[]最高分数";
                                        //字符集
Font font;                              //字体资源文件

bool GameOver = true;                   //游戏是否中止
int point = 0;                          //分数
int max_point = 0;                      //最高分数

struct aLetter{
    int x,y;
    aLetter(int _x) : x{_x},y{0} {};
};
std::vector<aLetter> letters[26];       //存储各种字母

void GetFont();                         //获得并初始化字体
void Process();                         //主体游戏逻辑
void GenerateLetter();                  //随机生成新的字母
void Draw();                            //绘制字母
void ToBeContinued();                   //游戏中止

int main(){

    SetTargetFPS(60);                           //设置FPS
    InitWindow(WinWidth,WinHeight,"My Type");   //初始化窗口

    GetFont();                                  //获取字体

    while(!WindowShouldClose()){                //游戏循环
        if(!GameOver){
            Process();                          //游戏主逻辑
            Draw();                             //绘图
        }
        else
            ToBeContinued();                    //游戏中止
    }

    CloseWindow();
    return 0;
}

void GetFont(){
    int filesize;
    unsigned char* fileData=LoadFileData(font_name,&filesize);
    int cnt;
    int* codepoints=LoadCodepoints(characters,&cnt);
    font=LoadFontFromMemory(".ttf",fileData,filesize,font_size,codepoints,cnt);
    UnloadCodepoints(codepoints);
    UnloadFileData(fileData);
}

void Process(){
//定时生成字母
    static int time=0;
    time++;
    if(time%letter_rate==0)
        GenerateLetter();

//按到对应字母后消除
    for(int i=65;i<=90;i++)
        if(IsKeyPressed(i))
            if(!letters[i-65].empty()){
                point++;
                if(point>=max_point)max_point=point;
                letters[i-65].clear();
            }

//字母整体位移
    for(int i=0;i<26;i++)
        for(std::vector<aLetter>::iterator it = letters[i].begin();it!=letters[i].end();it++)
            it->y += letter_v;

//判断是否到达红线
    for(int i=0;i<26;i++)
        for(std::vector<aLetter>::iterator it = letters[i].begin();it!=letters[i].end();it++)
            if(it->y+font_size>=RedLine)
                GameOver=true;
}

void GenerateLetter(){
    int ranchar=GetRandomValue(0,25);
    int ranx=GetRandomValue(font_size,WinWidth-2*font_size);
    aLetter t{ranx};
    letters[ranchar].push_back(t);
}

void Draw(){
    BeginDrawing();

        ClearBackground(BackgroundColor);

        DrawRectangle(0,RedLine,WinWidth,RedLineWidth,RED);

        for(int i=0;i<26;i++)
            for(aLetter it:letters[i]){
                DrawTextEx(font,TextFormat("%c",i+'a'),Vector2{(float)it.x,(float)it.y},font_size,spacing,BLACK);
            }

        Vector2 tmp=MeasureTextEx(font,TextFormat("分数:%d",point),font_size,spacing);
        DrawTextEx(font,TextFormat("分数:%d",point),Vector2{(WinWidth-tmp.x)/2.0f,0.0f},font_size,spacing,SKYBLUE);
        DrawTextEx(font,TextFormat("最高分数:%d",max_point),Vector2{(WinWidth-MeasureTextEx(font,TextFormat("最高分数:%d",max_point),font_size,spacing).x)/2.0f,tmp.y},font_size,spacing,MAROON);

    EndDrawing();
}

void ToBeContinued(){
    if(IsKeyPressed(KEY_ENTER)){
        point=0;
        GameOver=false;
        for(int i=0;i<26;i++)
            letters[i].clear();
    }

    BeginDrawing();

        ClearBackground(BackgroundColor);
        DrawRectangle(0,RedLine,WinWidth,RedLineWidth,RED);

        Vector2 tmp=MeasureTextEx(font,"Press [ENTER] to continue.",font_size,spacing);
        DrawTextEx(font,"Press [ENTER] to continue.",Vector2{(WinWidth-tmp.x)/2.0f,RedLine-font_size},font_size,spacing,DARKBROWN);

    EndDrawing();
}
