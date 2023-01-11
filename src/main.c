#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "hiredis.h"

#include "main.h"
#include "oneplayer.h"
#include "twoplayer.h"

#define x_redis 29
#define y_redis 18

int t = 0;
int max = 0;
int grade = 0;
struct player p[2];
struct Face face[2];
Dia dia[7][4];
int redis_array[x_redis][y_redis]={0}; //

redisContext *redisConnect(const char *ip, int port);
void *redisCommand(redisContext *c, const char *format, ...);
void freeReplyObject(void *reply);

int main(int argc, char *argv[])
{
    system("clear");
    hidden_cursor();
    srand(time(NULL));
    title(2,argv[1]);
    return 0;
}
void title(int a, char name[15]) {
    char ch;
    int redis_length;
    int name_length;
    char redis_value;
    int redis_queue[x_redis][y_redis];
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
    if (c) {
        printf("Error: %s\n", c->errstr);
        // handle error
    	} else {
        printf("Can't allocate redis context\n");
    	}
	}
     redisReply *reply1;
     if(strcmp(name,"list") == 0){
     reply1 = redisCommand(c, "llen namelist"); 
     name_length=reply1->integer;
     for (int i=0;i<name_length;i++){
     reply1 =redisCommand(c,"lindex namelist %d", i);
     printf("%s\n",reply1->str);
     }    
     return;
}
     int x,y;
     if(a==1){
     for(int i=0;i<x_redis;i++){
     			for(int j=0;j<y_redis;j++){
     				reply1 = redisCommand(c, "lpush %s %d",name, redis_array[i][j]);
        		}
     	    	}	
     }
     else{
    for (y = 0; y < FACE_Y + 20; y += 2) {
        for (x = 0; x < FACE_X; x++) {
            if (x == 0 || x == FACE_X - 1) {
                gotoxy(x, y);
                printf(LIGHT_GRAY"■");
            }
            if (y == 0) {
                gotoxy(x, y);
                printf(LIGHT_GRAY"■");
            }
            gotoxy(x, FACE_Y + 18);
            printf(LIGHT_GRAY"■");
        }
    }
    gotoxy(FACE_X / 2 - 2, FACE_Y / 2 + 5);
    printf(LIGHT_GRAY"俄羅斯方塊");
    gotoxy(FACE_X / 2, FACE_Y / 2 + 3);
    printf(LIGHT_GRAY"單人(1)/雙人(2)");
    gotoxy(FACE_X / 2 + 2, FACE_Y / 2 + 5);
    printf(LIGHT_GRAY"歷史紀錄(3)");
    gotoxy(FACE_X / 2 + 4, FACE_Y / 2 + 10);
    printf(LIGHT_GRAY"( )");
    gotoxy(FACE_X / 2 + 4, FACE_Y / 2 + 11);
    do {
        ch = getc(stdin);
    } while (ch == ' ' | ch == '\n');
    system("clear");
    init_dia();
    if(a==2){
     if(ch=='1')
     reply1 = redisCommand(c, "lpush namelist %s",name);
     }
    if (ch == '1') 
    {
        read_file();
        init_player1();
        init_face1();
        t = 0;
        max = 0;
        grade = 0;
        while (1) {
            start_game1(name);
            for(int i=0;i<29;i++){
     		for(int j=0;j<18;j++){
     		reply1 = redisCommand(c, "lpush %s %d",name, redis_array[i][j]);
        	}
     	    }		        
    	}	
    }
    else if(ch == '2')
    {
        init_player2();
        init_face2();
        t = 0;
        while (1) 
        {
            start_game2();
        }
    }
    else if(ch == '3'){
    reply1 = redisCommand(c, "llen %s",name); 
    redis_length=reply1->integer;
    while(redis_length){
    	for(int i=0;i<x_redis;i++){
    		for(int j=0;j<y_redis;j++){
    		redis_length--;    
    		reply1 =redisCommand(c,"lindex %s %d",name, redis_length);
    		//reply1 = redisCommand(c, "rpop %s",listname); 
    		//printf("%s"),reply1->str);
    		if(strcmp(reply1->str,"0"))redis_queue[i][j]=1;
    		else redis_queue[i][j]=0;
    		}
    		//printf("\n");
    		
	}
	print_screen(redis_queue);
	usleep(800);
}
}
}
}
void print_screen(int arr[][y_redis]){
	for(int i=0;i<x_redis;i++){
    		for(int j=0;j<y_redis;j++){
    		//printf("%d",arr[i][j]);
    			gotoxy(2+i,2*j+2);
    			if (arr[i][j]==1) printf(LIGHT_GRAY"■");
    			else printf("  ");
    		}
	}
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", x, y); 
}
void hidden_cursor()
{
    printf("\033[?25l");
}
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
int getch(void)
{
    int ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~(ECHO | ICANON | ECHOE | ECHOK |
        ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

void draw_space(int base, int space_t, int x, int y)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            gotoxy(x + i, 2 * (y + j));
            if (dia[base][space_t].space[i][j] == 1)
                printf("  ");
        }
    }
}
void draw_space_redis(int base, int space_t, int x, int y)
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (dia[base][space_t].space[i][j] == 1)
                redis_array[x+i][y+j-1]=0;
        }
    }
}
void draw_dia(int base, int space_t, int x, int y)
{

    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            gotoxy(x + i, 2 * (y + j));

            if (dia[base][space_t].space[i][j] == 1) {
                printcolor(base);
            }
        }
    }
}
void draw_dia_redis(int base, int space_t, int x, int y)
{

    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (dia[base][space_t].space[i][j] == 1) {
                redis_array[x+i][y+j-1]=1;
            }
        }
    }
}
void init_dia()
{
    int i, j, k, z;
    int b[4][4];
    for (i = 0; i < 3; i++)
        dia[0][0].space[1][i] = 1;
    dia[0][0].space[2][1] = 1; //凸形

    for (i = 1; i < 4; i++)
        dia[1][0].space[i][2] = 1;
    dia[1][0].space[1][1] = 1; //L形

    for (i = 1; i < 4; i++)
        dia[2][0].space[i][1] = 1;
    dia[2][0].space[1][2] = 1; //倒L形

    for (i = 0; i < 2; i++)
    {
        dia[3][0].space[1][i] = 1;
        dia[3][0].space[2][i + 1] = 1; //Z形

        dia[4][0].space[1][i + 1] = 1;
        dia[4][0].space[2][i] = 1; //倒Z形

        dia[5][0].space[1][i + 1] = 1;
        dia[5][0].space[2][i + 1] = 1; //方形
    }

    for (i = 0; i < 4; i++)
        dia[6][0].space[i][2] = 1; //直形 

    for (i = 0; i < 7; i++)
    {
        for (z = 0; z < 3; z++)
        {
            for (j = 0; j < 4; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    b[j][k] = dia[i][z].space[j][k];
                }
            }
            for (j = 0; j < 4; j++)
            {
                for (k = 0; k < 4; k++)
                {
                    dia[i][z + 1].space[j][k] = b[3 - k][j];
                }
            }
        }
    }
}
int printcolor(int base) {
    if (base == 0) printf(LIGHT_BLUE"■");
    else if (base == 1)printf(LIGHT_CYAN"■");
    else if (base == 2)printf(LIGHT_RED"■");
    else if (base == 4)printf(YELLOW"■");
    else if (base == 5)printf(LIGHT_GREEN"■");
    else if (base == 6)printf(LIGHT_PURPLE"■");
    else printf(DARK_GRAY"■");
    return 1;
}



