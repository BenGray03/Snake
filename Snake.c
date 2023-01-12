#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/select.h>
#include <unistd.h>


struct tail{ // structure for each 'piece' of the snake
    int x;
    int y;
    struct tail *next;
};

struct Snake{ //structure for the snake as a whole
    int length;
    int orientation;
    struct tail *head;
    int points;
    int alive;
};

void setApple(char field[30][30]){ // sets the apple pos on the map
    srand(time(0));
    int Aplx = (rand() % 29);
    int Aply = (rand() % 29);
    field[Aplx][Aply] = 'A';
}



void printField(char field[30][30], struct Snake *player){ //prints the field one row and column at a time
    printf("Points: %d\n", player->points);
    for(int i = 29; i >= 0; i--){
        for(int j = 0; j < 30; j++){
            printf("%c", field[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

struct tail* setTail(){ //sets the snake intial position
    struct tail *one = (struct tail*)malloc(sizeof(struct tail));
    one->x = 10;
    one->y = 10;
    one->next = NULL;
    return one;
}

void setSnake(struct Snake *player, char field [30][30]){ //sets the players intitial stats
    player->length = 1;
    player->orientation = 0;
    player->points = 0;
    player->alive = 1;
    player->head = setTail();
    field[player->head->x][player->head->y] = 'o';
}

void initialiseField(char field[30][30]) // first set field to all dots
{
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            field[i][j] = '.';
        }
    }
}
void reloadField(char field[30][30]) // sets field to all dots but leaves the apple
{
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            if(field[i][j] != 'A'){
                field[i][j] = '.';
            }
        }
    }
}
void updateField(struct Snake *player, char field[30][30]){//prints the snake on to the field
    struct tail *copyHead = player->head;
    for(int i = 0; i < player->length; i++){
        field[player->head->x][player->head->y] = 'o';
        player->head = player->head->next;
    }
    player->head = copyHead;
}

void checkEaten(char field[30][30], struct tail *front, struct Snake *player){//checks if the snake eats the apple
    if(field[front->x][front->y] == 'A'){ 
        player->points++;
        player->length++;
        setApple(field);
    }
}


struct tail * updateHead(struct Snake *player, char field [30][30]){//will be broken down into multiple parts as most compleax part
    int eaten = 0;
    struct tail *new_head = (struct tail *)malloc(sizeof(struct tail));//this will be returned head of the tail
    struct tail *upd = (struct tail *)malloc(sizeof(struct tail)); //used to help update the tail
    if(player->orientation == 0){ // checks rotation and sets upd to that
        upd->y = (player->head->y) +1;
        upd->x = player->head->x;
    }else if(player->orientation == 90){
        upd->x = (player->head->x) +1;
        upd->y = player->head->y;
    }else if(player->orientation == 180){
        upd->y = (player->head->y) -1;
        upd->x = player->head->x;
    }else if(player->orientation == 270){
        upd->x = (player->head->x) -1;
        upd->y = player->head->y;
    }
    checkEaten(field, upd, player);
    upd->next = player->head; //upd becomes new head
    new_head = upd;
    
    struct tail *current = player->head; // this part loops through the linked list of the tail and gets rid of the parts that are not in the length range anymore
    struct tail *previous = NULL;
    int i = 0;
    while (current != NULL) {
        if (i == player->length) {
            if (previous != NULL) {
                previous->next = NULL;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
        i++;
    }
    return new_head;
}

void changeOrientation(struct Snake *player, int rotate){ //changes the orientation of the snake based on the user input
    if(rotate==65 || rotate == 97){
        player->orientation = player->orientation-90;
        if(player->orientation < 0){
            player->orientation = player->orientation+360;
        }
    }else if(rotate == 68 || rotate == 100){
        player->orientation = player->orientation+90;
        if(player->orientation >= 360){
            player->orientation = player->orientation-360;
        }
    }
}
void checkOutOfBounds(struct Snake *player){ // checks if the user goes aout of bounds and kills them if so
    if(player->head->x > 29 || player->head->x < 0 || player->head->y > 29 || player->head->y < 0){
        player->alive = 0;
    }
}

void checkKillsSelf(struct Snake *player){// checks if user crosses their own path
    struct tail *check = (struct tail*)malloc(sizeof(struct tail));
    if(player->head->next != NULL){
            check =  player->head->next;
        for(int i = 4; i < player->length; i++){
            if(player->head->x == check->x && player->head->y == check->y){
                player->alive = 0;
            }
            check = check->next;
        }

    }
}

int main(){
    struct Snake *player = (struct Snake*)malloc(sizeof(struct Snake));

    fd_set readfds;
    struct timeval timeout;

    char ready;
    printf("Hello welcome to snake!\nUse a and d keys to turn left and right!\nPress any key to continue!");
    scanf("%c", &ready);


    char field [30][30];
    int rotate;
    initialiseField(field); 
    setSnake(player, field);
    setApple(field);
    while(player->alive == 1){
        rotate = 0;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 300000;
        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            rotate = getchar();
            if(rotate == 97 || rotate == 65 || rotate == 68 || rotate == 100){
                changeOrientation(player, rotate);
            }
        }
        player->head = updateHead(player, field);
        checkOutOfBounds(player);
        checkKillsSelf(player);
        reloadField(field);
        updateField(player, field);
        printField(field, player);

    }

    printf("The end you got a score of %d\nThank you for playing!\n", player->points);
    

}