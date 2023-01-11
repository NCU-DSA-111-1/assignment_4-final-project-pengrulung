#include"main.h"


#pragma once

#ifndef _TWOPLAYER_H_
#define _TWOPLAYER_H_


void PressUp(struct player* P);
void PressDown(struct player* P);
void PressLeft(struct player* P);
void PressRight(struct player* P);
void PressSpace(struct player* P);

void read_file(); //讀入最高紀錄 
void write_file(); //寫入最高紀錄 
int del2(struct player pl);
int border2(int n, int space_t, int x, int y, struct player P);
void start_game2(); //遊戲回圈 
// TODO: 在此參考您的程式所需的其他標頭。
void init_player2(); 

#endif

