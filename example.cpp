#include<iostream>
#include <raylib.h>

using namespace std ;

int player_score = 0 ;
int cpu_score = 0 ;

class Ball{
    public:
    float x , y ;
    int speed_x , speed_y ;
    int radius ;

    void Draw(){
        DrawCircle( x , y , radius , WHITE);
    }

    void Update(){
        x += speed_x ;
        y += speed_y ;

        if( y + radius >= GetScreenHeight() || y - radius <= 0 ){
            speed_y = -speed_y ;
        }
        if( x + radius >= GetScreenWidth()){
            cpu_score++;
            ResetBall();
        } 
        if ( x - radius <= 0 ){
            player_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth()/2 ;
        y = GetScreenHeight()/2 ;

        int speed_choices[2] = {1 , -1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle {
    public:
    float x , y ;
    int width , height ;
    int speed ;
    
    void Draw(){
        DrawRectangle( x , y , width , height , WHITE);
    }

    void Update(){
        if( IsKeyDown(KEY_UP) && y > 0 ){
            y -= speed ;
        }
        if( IsKeyDown(KEY_DOWN) && y < GetScreenHeight() - height ){
            y += speed ;
        }
    }
};

class CpuPaddle: public Paddle{
    public:

    void Update(int ball_y){
        if( y + height/2 < ball_y && y < GetScreenHeight() - height ){
            y += speed ;
        }
        if( y + height/2 > ball_y && y > 0 ){
            y -= speed ;
        }
    }
};

Ball ball;
Paddle player ;
CpuPaddle cpu ;

int main() {
    
    cout << "Starting the Game" << endl;
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Ping Pong Game");
    SetTargetFPS(60);

    ball.radius = 20 ;
    ball.x = screenWidth/2 ;
    ball.y = screenHeight/2 ;
    ball.speed_x = 7 ;
    ball.speed_y = 7 ;

    player.width = 25 ;
    player.height = 120 ;
    player.x = screenWidth - player.width - 10 ;
    player.y = screenHeight/2 - player.height/2 ;
    player.speed = 6 ;

    cpu.width = 25 ;
    cpu.height = 120 ;
    cpu.x = 10 ;
    cpu.y = screenHeight/2 - cpu.height/2 ;
    cpu.speed = 6 ;

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);
        
        //Cehcking Collision
        if( CheckCollisionCircleRec( Vector2{ball.x , ball.y} , ball.radius , Rectangle{player.x , player.y , player.width , player.height} ) ){
            ball.speed_x = -ball.speed_x ;
        }
        if( CheckCollisionCircleRec( Vector2{ball.x , ball.y} , ball.radius , Rectangle{cpu.x , cpu.y , cpu.width , cpu.height} ) ){
            ball.speed_x = -ball.speed_x ;
        }

        //Drawing
        ClearBackground(BLACK);
        DrawLine(screenWidth/2 , 0 , screenWidth/2 , screenHeight , WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i",cpu_score), screenWidth/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3*screenWidth/4 - 20, 20, 80, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
