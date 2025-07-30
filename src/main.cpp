#include <vector>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

const int WinWidth = 480;               //窗口宽度
const int WinHeight = 600;              //窗口高度
const int RedLine = 400;                //红线所在位置（纵坐标）
const int RedLineWidth = 4;             //红线高度
const Color BackgroundColor = LIGHTGRAY;//背景颜色

const int letter_rate = 120;            //字母生成标准周期
float r_times=1.0f;                     //字母生成频率倍率
float letter_v = 1;                     //字母下落速率
const float times=2.0f;                 //字母下落速率倍率

const int font_size = 30;               //字母大小
const float spacing = 0.0f;             //字母间距
const char font_name[] =
        "C:/Windows/Fonts/simhei.ttf";  //字体文件路径
const char characters[]=
        "abcdefghijklmnopqrstuvwxyz0123456789.:最高分数频速率请按任意键继续暂停生成";
                                        //字符集
Font font;                              //字体资源文件

const Rectangle button_rec={WinWidth-3*font_size,WinHeight-font_size*1.5f,font_size*3,font_size*1.5f};

const Rectangle label_speed_character_rec={0,RedLine+RedLineWidth,3*font_size,font_size};
const Rectangle slider_speed_rec={3.5f*font_size,RedLine+RedLineWidth,WinWidth-4.5*font_size,font_size};
const Rectangle label_speed_num_rec={(WinWidth+1.0f*font_size)/2.0f,RedLine+RedLineWidth,2*font_size,font_size};

const Rectangle label_gen_character_rec={0,RedLine+RedLineWidth+font_size,6*font_size,font_size};
const Rectangle slider_gen_rec={5.5f*font_size,RedLine+RedLineWidth+font_size,WinWidth-7.0*font_size,font_size};
const Rectangle label_gen_num_rec={(WinWidth+1.5f*font_size)/2.0f,RedLine+RedLineWidth+font_size,2*font_size,font_size};

bool GameOver = true;                   //游戏是否中止
bool IsPaused = false;                  //游戏是否暂停
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
    GuiSetFont(font);
    GuiSetStyle(DEFAULT,TEXT_SIZE,font_size);
}

void Process(){
//速度条
    GuiLabel(label_speed_character_rec,"速率:");
    GuiSlider(slider_speed_rec,"0","9",&letter_v,0,9);
    GuiLabel(label_speed_num_rec,TextFormat("%.1f",letter_v));

//生成速率条
    GuiLabel(label_gen_character_rec,"生成频率:");
    GuiSlider(slider_gen_rec,"1","20",&r_times,1.0f,20.0f);
    GuiLabel(label_gen_num_rec,TextFormat("%d",(int)r_times));

//暂停机制
    if(!IsPaused&&GuiButton(button_rec,"暂停"))
        IsPaused=true;
    else if(IsPaused&&GuiButton(button_rec,"继续"))
        IsPaused=false;
    if(IsPaused)return;

//定时生成字母
    static int time=0;
    time++;
    if(time%(letter_rate/((int)r_times))==0)
        GenerateLetter();

//按到对应字母后消除
    for(int i=65;i<=90;i++)
        if(IsKeyPressed(i))
            if(!letters[i-65].empty()){
                point=point+letter_v*r_times;
                if(point>=max_point)max_point=point;
                letters[i-65].clear();
            }

//字母整体位移
    for(int i=0;i<26;i++)
        for(std::vector<aLetter>::iterator it = letters[i].begin();it!=letters[i].end();it++)
            it->y += letter_v*times;

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

        Vector2 tmp=MeasureTextEx(font,"请按任意键继续",font_size,spacing);
        DrawTextEx(font,"请按任意键继续",Vector2{(WinWidth-tmp.x)/2.0f,RedLine-font_size},font_size,spacing,DARKBROWN);

    EndDrawing();
}
