#include <iostream>
#include <raylib.h>

using namespace std;

int PSCORE = 0;
int AISCORE = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) // AI MENANG !
        {
            AISCORE++;
            Reset();
        }
        if (x - radius <= 0) { // PLAYER MENANG !
            PSCORE++;
            Reset();
        }


    }

    void Reset()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speeed[2] = { -1, 1 };
        speed_x *= speeed[GetRandomValue(0, 1)];
        speed_y *= speeed[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMoving()
    {
        if (y <= 0)
        {
            y = 0;
        }

        if (y >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }


public:
    float x, y;
    float width, height;
    int speed;

    void draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }

        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }

        LimitMoving();
    }
};

class AIPaddle : public Paddle
{
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }

        LimitMoving();
    }
};

Ball ball;
Paddle paddle;
AIPaddle AI;

int main() {
    const int screen_width = 1200;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong By Flammablish !");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    // Ball Properties
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 10;
    ball.speed_y = 10;
    ball.radius = 10;

    // Paddle Properties
    paddle.width = 25;
    paddle.height = 120;
    paddle.x = screen_width - paddle.width - 10;
    paddle.y = screen_height / 2 - paddle.height / 2;
    paddle.speed = 10;

    // AI Properties
    AI.width = 25;
    AI.height = 120;
    AI.x = 10;
    AI.y = screen_height / 2 - AI.height / 2;
    AI.speed = 10;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        DrawFPS(10, 10);
        ClearBackground(BLACK);

        // Object Movement
        ball.update();
        paddle.Update();
        AI.Update(ball.y);

        // Check4Collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ paddle.x, paddle.y, paddle.width, paddle.height }))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ AI.x, AI.y, AI.width, AI.height }))
        {
            ball.speed_x *= -1;
        }

        // Object In Game
        ball.draw();
        paddle.draw();
        AI.draw();

        // Score
        DrawText(TextFormat("%i", AISCORE), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", PSCORE), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}