# 俄羅斯方塊 in C 

## Guide For Playing Tetris

You need to install Redis and Libev before playing this game.

109503509林俐嫺/109503510龍芃如

### Install redis
$ sudo apt update

$ sudo apt install redis
### Install Hiredis
$ git clone https://github.com/redis/hiredis.git

$ cd hiredis

$ make

$ make install
### Install libev
$ sudo apt-get install libevent-dev

### Compile source codes and header files

$ gcc -o main main.c main.h oneplayer.c oneplayer.h twoplayer.c twoplayer.h hiredis.h -levent -lhiredis

### Run the executable file

Decide the key you want to save in Redis.
$ ./main 'key'
If you want to check the key list, enter "list" as key.

### Choose mode

  There are two game modes : 1 Player and 2 Players .
  
  If you want to watch history for 1 Player,choose mode 3.
(Mode 1) 1 Player:

        '↑' to rotate the diamond.
        '↓' to move the diamond down.
        '→' to move the diamond to go right.
        '←' to move the diamond to go left.
        Space to move the diamond to bottom.
        'H' to hold the diamond.
        'X' to exit the game.
        's' to pause the game.
        
(Mode 2) 2 Players:
        
        'n' to exit the game.
        'p' to pause the game.
        
        Player 1(left):
        
            '↑' to rotate the diamond.
            '↓' to move the diamond down.
            '→' to move the diamond to go right.
            '←' to move the diamond to go left.
            Space to move the diamond to bottom.
            
        Player 2(left):
        
            'W' to rotate the diamond.
            'S' to move the diamond down.
            'D' to move the diamond to go right.
            'A' to move the diamond to go left.
            '0' to move the diamond to bottom.
            
(Mode 3) Check history:
        
        Check the history of one player according to the key you enter.
    
        
HAVE FUN!
