
#include "oneplayer.h"
#include "main.h"
#include "hiredis.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

struct player P[2];
redisContext *redisConnect(const char *ip, int port);
void *redisCommand(redisContext *c, const char *format, ...);
void freeReplyObject(void *reply);

void init_player1()
{
    P[0].space_t = 0; //turn times
    P[0].x = 0;
    P[0].y = FACE_Y / 2 - 2; //
    P[0].k = 0; //player count
    P[0].n = rand() % 7; //block(seven)
    P[0].b = rand() % 7; //next block(seven)
    P[0].hold = -1;
}
void start_game1(char name[15])
{
    int ch, i, j;
    redisReply *reply1;
    redisContext *c = redisConnect("127.0.0.1", 6379);
    P[0].space_t = 0;
    P[0].x = 0, P[0].y = FACE_Y / 2 - 2;
    P[0].k = 0;
    draw_space(P[0].b, 0, 4, FACE_Y + 3);
    P[0].n = P[0].b;
    P[0].b = rand() % 7;
    draw_dia(P[0].b, 0, 4, FACE_Y + 3);

    gotoxy(2, 2 * FACE_Y+5);
    printf(LIGHT_GRAY"N E X T");
    
    while (1)
    {
    	
        draw_dia(P[0].n, P[0].space_t, P[0].x, P[0].y);
        if (t == 0)
            t = 20000 * 2;
        while (--t) {
            if (kbhit() != 0)
                break;
        }
        if (t == 0)
        {
            if (border1(P[0].n, P[0].space_t, P[0].x + 1, P[0].y) == 1)
            {
            	draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
            	draw_dia_redis(P[0].n, P[0].space_t, P[0].x + 1, P[0].y);
            	title(1,name);
            	/*for(int i=0;i<28;i++){
     			for(int j=0;j<18;j++){
     				reply1 = redisCommand(c, "lpush %s %d",listname, redis_array[i][j]);
        		}
     	    	}*/	
                draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                P[0].x++; 
     	    }	
            else
            {
            	draw_dia_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
            	title(1,name);
                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        if (dia[P[0].n][P[0].space_t].space[i][j] == 1)
                        {
                            face[0].data[P[0].x + i][P[0].y + j] = 1;
                            face[0].color[P[0].x + i][P[0].y + j] = P[0].n;
                            while (del1(name));
                        }
                    }
                }
                return;
            }
        }
        else
        {
            ch = getch();
            switch (ch) {

            case 'R':
            case 'r': {
                system("clear");
                init_player1();
                init_face1();
                t = 0;
                while (1)
                {
                    start_game1(name);
                }
                exit(0);
            }
            case 'S':
            case 's': {
                while (1)
                    if (kbhit() != 0)
                        break;
                break;
            }
            case 'x': {
                system("clear");
                gotoxy(FACE_X / 2 - 2, FACE_Y);
                printf(LIGHT_GRAY"遊戲結束!\n");
                usleep(4000);
                system("clear");
                exit(0);
                break;
            }

            case'H':
            case'h': {
            	draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
            	draw_space_redis(P[0].hold,P[0].space_t, P[0].x, P[0].y);
                draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                draw_space(P[0].hold, 0, 4, FACE_Y + 13);

                if (P[0].hold == -1) {
                    P[0].hold = P[0].n;
                    P[0].n = P[0].b;
                }
                else {
                    int tmp_h = P[0].hold;
                    P[0].hold = P[0].n;
                    P[0].n = tmp_h;
                }
                draw_dia(P[0].hold, P[0].space_t, 4, FACE_Y + 13);
            	draw_dia_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                break;
            }
            case SPACE: {
            	draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                while (border1(P[0].n, P[0].space_t, P[0].x + 1, P[0].y) == 1)
                    P[0].x++;
            	draw_dia_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                break;
            }
            case 91:
                ch = getch();
                switch (ch) {
                case LEFT: {
                    if (border1(P[0].n, P[0].space_t, P[0].x, P[0].y - 1) == 1) {
            		draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_dia_redis(P[0].n, P[0].space_t, P[0].x, P[0].y-1);
                        P[0].y--;
                    }
                    break;
                }
                case RIGHT: {
                    if (border1(P[0].n, P[0].space_t, P[0].x, P[0].y + 1) == 1) {
            		draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_dia_redis(P[0].n, P[0].space_t, P[0].x, P[0].y+1);
                        P[0].y++;
                    }
                    break;
                }
                case DOWN: {
                    if (border1(P[0].n, P[0].space_t, P[0].x + 1, P[0].y) == 1) {
            		draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_dia_redis(P[0].n, P[0].space_t, P[0].x+1, P[0].y);
                        P[0].x++;
                    }
                    break;
                }
                case UP: {
                    if (border1(P[0].n, (P[0].space_t + 1) % 4, P[0].x + 1, P[0].y) == 1) {
            		draw_space_redis(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_space(P[0].n, P[0].space_t, P[0].x, P[0].y);
                        draw_dia_redis(P[0].n, (P[0].space_t+1)%4, P[0].x+1, P[0].y);
                        P[0].space_t = (P[0].space_t + 1) % 4;
                    }
                    break;
                }

                default:break;
                }
            }
        }
    }
    return;
}

int border1(int n, int space_t, int x, int y)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (dia[n][space_t].space[i][j] == 0)
                continue;
            else if (face[0].data[x + i][y + j] == Wall || face[0].data[x + i][y + j] == Box)
                return 0;
        }
    }
    return 1;
}

int del1(char name[15])
{
    int i, j, k, sum;
    for (i = FACE_X - 2; i >= FACE_X - 6; i--)
    {
        sum = 0;
        for (j = 1; j < FACE_Y - 1; j++)
            sum += face[0].data[i][j];
        if (sum == 0)
            break;
        if (sum == FACE_Y - 2)
        {
            grade += 100;
            gotoxy(FACE_X - 4, 2 * FACE_Y + 2);
            printf(LIGHT_GRAY"當前分數：%d", grade);
            for (j = 1; j < FACE_Y - 1; j++)
            {
                face[0].data[i][j] = Kong;
                gotoxy(i, 2 * j);
                printf("  ");
            }
            for (j = i; j > 1; j--)
            {
                sum = 0;
                for (k = 1; k < FACE_Y - 1; k++)
                {
                    sum += face[0].data[j - 1][k] + face[0].data[j][k];
                    face[0].data[j][k] = face[0].data[j - 1][k];
                    if (face[0].data[j][k] == Kong)
                    {
                        gotoxy(j, 2 * k);
                        printf("  ");
                    }
                    else
                    {
                        gotoxy(j, 2 * k);
                        printcolor(face[0].color[j][k]);
                    }
                    redis_array[j][k-1]=face[0].data[j][k];
                }
                if (sum == 0)
                    return 1;
            }
        }
    }
    for (i = 1; i < FACE_Y - 1; i++)
    {
        if (face[0].data[1][i] == Box)
        {
            char n;
            usleep(2500);
            system("clear");
            gotoxy(FACE_X / 2 - 2, 2 * (FACE_Y / 3));
            if (grade > max)
            {
                printf(LIGHT_GRAY"恭喜您打破記錄，目前最高紀錄為：%d", grade);
                write_file();
            }
            else if (grade == max)
                printf(LIGHT_GRAY"與紀錄持平，請突破你的極限！");
            else
                printf(LIGHT_GRAY"請繼續努力，你與最高記錄只差：%d", max - grade);
            gotoxy(FACE_X / 2, 2 * (FACE_Y / 3));
            printf(LIGHT_GRAY"GAME OVER!\n");
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
                init_player1();
                init_face1();
                t = 0;
                max = 0;
                grade = 0;
                while (1)
                {
                    start_game1(name);
                }
                system("clear");
            }
        }
    }
    return 0;
}

void read_file()
{
    FILE* fp;
    fp = fopen("俄羅斯方塊記錄.txt", "r+");
    if (fp == NULL)
    {
        fp = fopen("俄羅斯方塊記錄.txt", "w+");
        fwrite(&max, sizeof(int), 1, fp);
    }
    fseek(fp, 0, 0);
    fread(&max, sizeof(int), 1, fp);
    fclose(fp);
}

void write_file()
{
    FILE* fp;
    fp = fopen("俄羅斯方塊記錄.txt", "r+");
    fwrite(&grade, sizeof(int), 1, fp);
    fclose(fp);
}
void init_face1()
{
    int i, j;

    for (i = 0; i < FACE_X; i++)
    {
        for (j = 0; j < FACE_Y + 10; j++)
        {
            if (j == 0 || j == FACE_Y - 1 || j == FACE_Y + 9)
            {
                face[0].data[i][j] = Wall;
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
            else if (i == FACE_X - 1)
            {
                face[0].data[i][j] = Box;
                gotoxy(i, 2 * j);
                printf(LIGHT_GRAY"■");
            }
            else
                face[0].data[i][j] = Kong;
        }
    }

    gotoxy(FACE_X - 14, 2 * FACE_Y + 2);
    printf( LIGHT_GRAY"左移：←");

    gotoxy(FACE_X - 13, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"右移：→");

    gotoxy(FACE_X - 12, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"下移：↓");

    gotoxy(FACE_X - 11, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"旋轉：↑");

    gotoxy(FACE_X - 10, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"降到最底：space");

    gotoxy(FACE_X - 9, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"保留：h");

    gotoxy(FACE_X - 8, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"暫停: S");

    gotoxy(FACE_X - 7, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"退出: x");

    gotoxy(FACE_X - 6, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"重新開始:R");

    gotoxy(FACE_X - 5, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"最高紀錄:%d", max);

    gotoxy(FACE_X - 4, 2 * FACE_Y + 2);
    printf(LIGHT_GRAY"當前分數:%d", grade);
}



