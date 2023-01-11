#pragma once

#ifndef _MAIN_H_
#define _MAIN_H_

#define UP 65 //37 //32 //上方向鍵 旋轉
#define LEFT  68 //39 //75 //左方向鍵 向左移動
#define RIGHT 67 //40 //77 //右方向鍵 向右移動
#define DOWN 66 //38 //80 //下方向鍵 加速向下移 
#define ESC 27 //Esc鍵 退出 
#define SPACE 32
#define Wall 2 //圍墻 
#define Box 1 //方塊 
#define Kong 0 
#define FACE_X 30 //方塊長度 
#define FACE_Y 20 //方塊高度 
#define x_redis 29
#define y_redis 18

#define LIGHT_RED "\033[1;31m"
#define LIGHT_GREEN "\033[1;32m"
#define LIGHT_BLUE "\033[1;34m"
#define LIGHT_PURPLE "\033[1;35m"
#define LIGHT_GRAY "\033[1;37m"
#define YELLOW "\033[1;33m"
#define LIGHT_CYAN "\033[1;36m"
#define DARK_GRAY "\033[1;30m"
extern int redis_array[x_redis][y_redis];

struct Face
{
    int data[FACE_X][FACE_Y + 10]; //為1是方塊,為0是空格
    int color[FACE_X][FACE_Y + 10];
};

typedef struct Diamonds
{
    int space[4][4]; //4*4矩陣,為1是方塊,為0是空格
}Dia;

struct player {
    int b;
    int hold;
    int n;
    int space_t;
    int x, y;
    int k;
    int line;
};

extern struct Face face[2];
extern Dia dia[7][4]; //一維表示7種形狀,二維表示旋轉次數
extern struct player P[2];
extern int t;
extern int max, grade;
void gotoxy(int x, int y); //設定游標位置 
void hidden_cursor(); //隱藏游標 
void draw_dia(int base, int space_t, int x, int y); //畫方塊 
void draw_space(int base, int space_t, int x, int y); //畫空格 
void print_screen(int arr[][y_redis]);
void draw_dia_redis(int base, int space_t, int x, int y); //畫方塊 
void draw_space_redis(int base, int space_t, int x, int y); //畫空格 
void init_dia(); //初始化方塊形狀 
void init_face1(); //初始化界面 
void init_face2(); //初始化界面 
int kbhit(void);
int getch(void);
void title(int a,char b[15]);
int printcolor(int base);
#endif
