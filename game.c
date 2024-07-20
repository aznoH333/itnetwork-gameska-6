#include "raylib.h"
#include "gframework.c"


struct Food{
    int x;
    int y;
};
typedef struct Food Food;

void resetFood(Food* this){
    this->x = GetRandomValue(0, 24) * 8;
    this->y = GetRandomValue(0,14) * 8;
}

void drawFood(Food* this){
    draw(1, this->x, this->y);
}




struct SnakeTile{
    int x;
    int y;
    int age;
};
typedef struct SnakeTile SnakeTile;

#define MAX_SNAKE_LENGTH 256
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


struct Snake{
    int x;
    int y;
    int direction;
    SnakeTile body[MAX_SNAKE_LENGTH];
    int lastDirection;
    int length;
    int speed;
    int score;
    bool isDead;
};
typedef struct Snake Snake;


SnakeTile initSnakeTile(int x, int y){
    SnakeTile out;
    out.x = x;
    out.y = y;
    out.age = 0;
    return out;
}

Snake initSnake(int x, int y){
    Snake out;
    out.score = 0;
    out.x = x;
    out.y = y;
    out.direction = UP;
    out.length = 5;
    out.speed = 16;
    out.isDead = false;
    out.lastDirection = UP;
    
    for (int i = 0; i < MAX_SNAKE_LENGTH; i++){
        out.body[i].age = -1; 
    }
    
    return out;
}
const Color TEXT_COLOR = {0, 62, 11, 255};
void updateSnake(Snake* this, int timer, Food* food){
    
    // display score 
    char scoreText[10];
    sprintf(&scoreText, "%d", this->score);
    DrawText(scoreText, 95, 10, 20, TEXT_COLOR);
    
    if (this->isDead){
        DrawText("GAME OVER", 40, 50, 20, TEXT_COLOR);
        
        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_R)){
            resetFood(food);
            *this = initSnake(80, 80);
        }
    }
    // update body
    if (timer % this->speed == 0 && this->isDead == false){
        // move head
        {
            switch (this->direction){
                case UP: this->y -= 8; break;
                case DOWN: this->y += 8; break;
                case LEFT: this->x -= 8; break;
                case RIGHT: this->x += 8; break;
            }
            this->lastDirection = this->direction;
        }
        // warping
        {
            if (this->y < 0) this->y = 112;
            if (this->y > 112) this->y = 0;
            if (this->x < 0) this->x = 208;
            if (this->x > 208) this->x = 0;
        }
        
        // apple collisions
        {
            if (this->x == food->x && this->y == food->y){
                resetFood(food);
                if (this->length + 2 < MAX_SNAKE_LENGTH){
                    this->length += 2;
                }
                this->score++;
                this->speed = 16 - (this->score >> 3);
                if (this->speed < 6){
                    this->speed = 6;
                }
            }
        }
        
        // wall collisions
        {
            for (int i = 0; i < MAX_SNAKE_LENGTH; i++){
                SnakeTile* b = &this->body[i];
                
                if (b->age > 0 && this->x == b->x && this->y == b->y){
                    this->isDead = true;
                }
            }
        }
        
        // spawn new cell
        {
            int i = 0;
            while (this->body[i].age != -1){
                i++;
                
                if (i > MAX_SNAKE_LENGTH){
                   break;
                }
            }
            this->body[i] = initSnakeTile(this->x, this->y);
        }
        
        
        
    }
    
    // update movement
    {
        if (IsKeyDown(KEY_W) && this->lastDirection != DOWN) this->direction = UP;
        if (IsKeyDown(KEY_S) && this->lastDirection != UP) this->direction = DOWN;
        if (IsKeyDown(KEY_A) && this->lastDirection != RIGHT) this->direction = LEFT;
        if (IsKeyDown(KEY_D) && this->lastDirection != LEFT) this->direction = RIGHT;
    }
    
    // update cells
    {
        for (int i = 0; i < MAX_SNAKE_LENGTH; i++){
            SnakeTile* b = &this->body[i];
            
            if (b->age < 0){
                continue; // skip dead cells
            }
            
            // draw
            draw(0, b->x, b->y);
            
            // update
            if (timer % this->speed == 0){
                b->age++;
                
                if (b->age > this->length){
                    b->age = -1;
                }
            }
        }
    }
    
}
const Color col = {0, 148,26};
int main(){
    
    initFramework();
    Snake s = initSnake(32, 32);
    Food f;
    int timer = 0;
    resetFood(&f);
    
    while (!WindowShouldClose())
    {
        
        fDrawBegin();
        ClearBackground(c);
        updateSnake(&s, timer, &f);
        drawFood(&f);
        fDrawEnd();
        
        timer++;
        
    }
    
    disposeFramework();
    
    
    return 0;
}