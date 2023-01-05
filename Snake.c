#include <stdio.h>
#include <string.h>

struct Snake{
    int length;
    int orientation;
    int tail[900][2];
    int points;
};

updateHead(struct Snake player, char field [30][30]){
    
}

void printField(char field[30][30], struct Snake player){
    printf("Points: %d\n", player.points);
    for(int i = 29; i >= 0; i--){
        for(int j = 0; j < 30; j++){
            printf("%c", field[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void setSnake(struct Snake player, char field [30][30]){
    player.length = 1;
    player.orientation = 180;
    player.points = 0;
    player.tail[0][0] = 10;
    player.tail[0][1] = 10; 
    field[player.tail[0][0]][player.tail[0][1]] = 'o';
}

void initialiseField(char field[30][30])
{
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            field[i][j] = '.';
        }
    }
}

int main(){
    int playing = 1;
    struct Snake player;
    char field [30][30];
    initialiseField(field);
    setSnake(player, field);
    printField(field, player);

    while(playing == 1){
        printField(field, player);
        updateHead(player, field);
    }
}