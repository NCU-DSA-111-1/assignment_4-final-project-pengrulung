#include"main.h"


#pragma once

#ifndef _ONEPLAYER_H_
#define _ONEPLAYER_H_


void start_game1(); //遊戲回圈 
void read_file(); //讀入最高紀錄 
void write_file(); //寫入最高紀錄 
int kbhit(void);
int getch(void);
void init_player1();
int del1(); //洗掉增加分數 
int border1(int n, int space_t, int x, int y); //邊界檢測

#endif

