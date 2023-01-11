
#include "twoplayer.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

/*
雙人模式: 沒有score
*/



void init_player2() 
{
    for (int k = 0; k < 2; k++) {
        P[k].n = rand() % 7;
        P[k].b = rand() % 7;
        P[k].x = 0;
        P[k].space_t = 0;
        P[k].k = k;
        P[k].y = FACE_Y / 2 - 2;
        P[k].line = 0;
    }
    draw_space(P[0].b, 0, 6, FACE_Y + 3);
    draw_space(P[1].b, 0, FACE_X - 6, FACE_Y + 3);
}
void start_game2()
{
    int ch, i, j;

    //下一個方塊預告
    draw_dia(P[0].b, 0, 6, FACE_Y + 3);
    draw_dia(P[1].b, 0, FACE_X - 8, FACE_Y + 3);

    while (1)
    {
        draw_dia(P[0].n, P[0].space_t, P[0].x, P[0].y);
        draw_dia(P[1].n, P[1].space_t, P[1].x, P[1].y + 30);

        if (t == 0) t = 20000 * 4;

        while (--t) {
            if (kbhit() != 0)
                break;
        }
        if (t == 0)
        {
            if (border2(P[0].n, P[0].space_t, P[0].x + 1, P[0].y, P[0]) == 1 &&
                border2(P[1].n, P[1].space_t, P[1].x + 1, P[1].y, P[1]) == 1)
            {
                draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                draw_space(P[1].n, P[1].space_t, P[1].x, P[1].y + 30);
                P[0].x++;
                P[1].x++;
            }
            else if (border2(P[0].n, P[0].space_t, P[0].x + 1, P[0].y, P[0]) == 1 &&
                     border2(P[1].n, P[1].space_t, P[1].x + 1, P[1].y, P[1]) == 0)
            {
                draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                draw_space(P[1].b, 0 , FACE_X - 8, FACE_Y + 3);
                P[0].x++;

                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        if (dia[P[1].n][P[1].space_t].space[i][j] == 1)
                        {
                            face[1].data[P[1].x + i][P[1].y + j] = 1;
                            face[1].color[P[1].x + i][P[1].y + j] = P[1].n;
                            while (del2(P[1]));
                        }
                    }
                }
                P[1].n = P[1].b;
                P[1].b = rand() % 7;
                P[1].x = 0;
                P[1].y = FACE_Y / 2 - 2;
                P[1].space_t = 0;
                return;
            }
            else if (border2(P[1].n, P[1].space_t, P[1].x + 1, P[1].y, P[1]) == 1 &&
                border2(P[0].n, P[0].space_t, P[0].x + 1, P[0].y, P[0]) == 0)
            {
                draw_space(P[1].n, P[1].space_t, P[1].x, P[1].y + 30);
                draw_space(P[0].b, 0, 6, FACE_Y + 3);
                P[1].x++;

                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        if (dia[P[0].n][P[0].space_t].space[i][j] == 1)
                        {
                            face[0].data[P[0].x + i][P[0].y + j] = 1;
                            face[0].color[P[0].x + i][P[0].y + j] = P[0].n;
                            while (del2(P[0]));
                        }
                    }
                }
                P[0].n = P[0].b;
                P[0].b = rand() % 7;
                P[0].x = 0;
                P[0].y = FACE_Y / 2 - 2;
                P[0].space_t = 0;
                return;
            }
            else
            {
                draw_space(P[0].b, P[0].space_t, 6, FACE_Y + 3);
                draw_space(P[1].b, 0, FACE_X-8, FACE_Y + 3);
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        if (dia[P[0].n][P[0].space_t].space[i][j] == 1)
                        {
                            face[0].data[P[0].x + i][P[0].y + j] = 1;
                            face[0].color[P[0].x + i][P[0].y + j] = P[0].n;
                            while (del2(P[0]));
                        }
                        if (dia[P[1].n][P[1].space_t].space[i][j] == 1)
                        {
                            face[1].data[P[1].x + i][P[1].y + j] = 1;
                            face[1].color[P[1].x + i][P[1].y + j] = P[1].n;
                            while (del2(P[1]));
                        }
                    }
                }
                P[1].n = P[1].b;
                P[1].b = rand() % 7;
                P[1].x = 0;
                P[1].y = FACE_Y / 2 - 2;
                P[1].space_t = 0;
                P[0].n = P[0].b;
                P[0].b = rand() % 7;
                P[0].x = 0;
                P[0].y = FACE_Y / 2 - 2;
                P[0].space_t = 0;
                return;
            }
        }
        else
        {
            ch = getch();
            switch (ch) {//wasd 上下左右 p暫停 n結束
            case 'P':
            case 'p': {
                while (1)
                    if (kbhit() != 0)
                        break;
                break;
            }
            case 'N':
            case 'n': {
                system("clear");
                gotoxy(FACE_X / 2 - 2, FACE_Y);
                printf(LIGHT_GRAY"遊戲結束!\n");

                system("clear");
                exit(0);
                break;
            }

            case'w': {
                PressUp(&P[0]);
                break;
            }
            case'a': {
                PressLeft(&P[0]);
                break;
            }
            case's': {
                PressDown(&P[0]);
                break;
            }
            case'd': {
                PressRight(&P[0]);
                break;
            }
            case'0': {
                PressSpace(&P[1]);
                break;
            }
            case SPACE: {
                PressSpace(&P[0]);
                break;
            }
            case 91:
                do {
                    ch = getch();
                } while (ch == 91);

                switch (ch) {
                case LEFT: {
                    PressLeft(&P[1]);
                    break;
                }
                case RIGHT: {
                    PressRight(&P[1]);
                    break;
                }
                case DOWN: {
                    PressDown(&P[1]);
                    break;
                }
                case UP: {
                    PressUp(&P[1]);
                    break;
                }

                }
            }
        }
    }
}
void PressUp(struct player* P) {
    if (border2(P->n, (P->space_t + 1) % 4, P->x + 1, P->y, *P) == 1) {
        draw_space(P->n, P->space_t, P->x, P->y + 30 * P->k);
        P->space_t = (P->space_t + 1) % 4;
    }
}
void PressDown(struct player* P) {
    if (border2(P->n, P->space_t, P->x + 1, P->y, *P) == 1) {
        draw_space(P->n, P->space_t, P->x, P->y + 30 * P->k);
        P->x++;
    }
}
void PressLeft(struct player* P) {
    if (border2(P->n, P->space_t, P->x, P->y - 1, *P) == 1) {
        draw_space(P->n, P->space_t, P->x, P->y + 30 * P->k);
        P->y--;
    }
}

void PressRight(struct player* P) {
    if (border2(P->n, P->space_t, P->x, P->y + 1, *P) == 1) {
        draw_space(P->n, P->space_t, P->x, P->y + 30 * P->k);
        P->y++;
    }
}

void PressSpace(struct player* P) {
    draw_space(P->n, P->space_t, P->x, P->y + 30 * P->k);
    while (border2(P->n, P->space_t, P->x + 1, P->y, *P) == 1)
        P->x++;
}

int border2(int n, int space_t, int x, int y, struct player P)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (dia[n][space_t].space[i][j] == 0)
                continue;
            else if (face[P.k].data[x + i][y + j] == Wall || face[P.k].data[x + i][y + j] == Box)
                return 0;
        }
    }
    return 1;
}

int del2(struct player pl)
{
    int i, j, k, sum[2];
    for (i = FACE_X - 2; i >= FACE_X - 6; i--)
    {
        sum[pl.k] = 0;
        for (j = 1; j < FACE_Y - 1; j++)
            sum[pl.k] += face[pl.k].data[i][j];
        if (sum[pl.k] == 0)
            break;
        if (sum[pl.k] == FACE_Y - 2)
        {
            P[pl.k].line += 1;
            gotoxy(FACE_X - 12, 2 * FACE_Y + 2);
            printf(LIGHT_GRAY"%d 消滅行數 %d",P[0].line,P[1].line);
            for (j = 1; j < FACE_Y - 1; j++)
            {
                face[pl.k].data[i][j] = Kong;
                gotoxy(i, 2 * j + 60 * pl.k);
                printf("  ");
            }
            for (j = i; j > 1; j--)
            {
                sum[pl.k] = 0;
                for (k = 1; k < FACE_Y - 1; k++)
                {
                    sum[pl.k] += face[pl.k].data[j - 1][k] + face[pl.k].data[j][k];
                    face[pl.k].data[j][k] = face[pl.k].data[j - 1][k];
                    if (face[pl.k].data[j][k] == Kong)
                    {
                        gotoxy(j, 2 * k + 60 * pl.k);
                        printf(LIGHT_GRAY"  ");
                    }
                    else
                    {
                        gotoxy(j, 2 * k + 60 * pl.k);
                        printcolor(face[pl.k].color[j][k]);
                    }
                }
                if (sum[pl.k] == 0)
                    return 1;
            }
        }
    }
    for (i = 1; i < FACE_Y - 1; i++)
    {
        if (face[pl.k].data[1][i] == Box)
        {
            char n;
            usleep(1500);
            system("clear");

            gotoxy(FACE_X / 2, 2 * (FACE_Y / 3));
            int winner = (pl.k == 1) ? 0 : 1;
            printf(LIGHT_GRAY"玩家%d勝利!\n", winner + 1);
            do
            {
                gotoxy(FACE_X / 2 + 2, 2 * (FACE_Y / 3));
                printf(LIGHT_GRAY"是否重新開始遊戲(y/n): ");
                scanf("%c", &n);
                gotoxy(FACE_X / 2 + 4, 2 * (FACE_Y / 3));
                if (n != 'n' && n != 'N' && n != 'y' && n != 'Y')
                    printf(LIGHT_GRAY"輸入錯誤，請重新輸入!");
                else
                    break;
            } while (1);
            if (n == 'n' || n == 'N')
            {
                gotoxy(0, 0);
                system("clear");
                exit(0);
            }
            else if (n == 'y' || n == 'Y') {
                system("clear");
                init_player2();
                init_face2();
                t = 0;
                while (1)
                {
                    start_game2();
                }
                exit(0);
            }
        }
    }
    return 0;
}




void init_face2()
{
    int i, j;

    for (i = 0; i < FACE_X; i++)
    {
        for (j = 0; j < FACE_Y + 10; j++)
        {
            if (j == 0 || j == FACE_Y - 1)
            {
                face[0].data[i][j] = Wall;
                face[1].data[i][j] = Wall;
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
            else if (j == FACE_Y + 9) {
                face[0].data[i][j] = Wall;
                face[1].data[i][j] = Wall;
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"  ■");
            }
            else if (i == FACE_X - 1)
            {
                face[0].data[i][j] = Box;
                face[1].data[i][j] = Box;
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
            else {
                if (j == FACE_Y + 9) {
                    gotoxy(i, 2 * j);
                    printf(LIGHT_GRAY"■");
                }
                face[0].data[i][j] = Kong;
                face[1].data[i][j] = Kong;
            }
        }
    }
    for (i = 0; i < FACE_X; i++)
    {
        for (j = FACE_Y + 10; j < FACE_Y + 30; j++)
        {
            if (j == FACE_Y + 29) {
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
            if (i == FACE_X - 1) {
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
        }
    }

    gotoxy(i-1, 2 * (FACE_Y + 9));
    printf(LIGHT_GRAY"■");

    gotoxy(FACE_X - 18, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"A   左移   ←");

    gotoxy(FACE_X - 17, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"D   右移   →");

    gotoxy(FACE_X - 16, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"S   下移   ↓");

    gotoxy(FACE_X - 15, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"W   旋轉   ↑");

    gotoxy(FACE_X - 14, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"P   暫停   P");

    gotoxy(FACE_X - 13, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"N   退出   N");

    gotoxy(FACE_X - 12, 2 * FACE_Y + 3);
    printf(LIGHT_GRAY"%d 消滅行數 %d", P[0].line, P[1].line);

    /*
    gotoxy(FACE_X - 13, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"重新開始:R");

    gotoxy(FACE_X - 12, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"最高紀錄:%d", max);

    gotoxy(FACE_X - 11, 2 * FACE_Y + 2);    
    printf(LIGHT_GRAY"當前分數:%d", grade);
    */
}

