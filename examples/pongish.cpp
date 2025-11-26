#include "byteframe.hpp"
#include <raylib.h>
#include <algorithm> 

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

struct Paddle {
    bf::Rectangle rect;
};

struct Ball {
    bf::Circle circle;
    bf::Vec2   vel;
};

void DrawPaddle(bf::Canvas& c, const Paddle& p) {
    p.rect.draw(c);
}

void DrawBall(bf::Canvas& c, const Ball& b) {
    b.circle.draw(c);
}

bool BallHitsPaddle(const Paddle& p, const Ball& b) {
    return p.rect.collidesWith(b.circle);
}

void ResetBall(Ball& ball) {
    ball.circle.setPos(WIDTH / 2.0f, HEIGHT / 2.0f);
    ball.vel.x = 250.0f;
    ball.vel.y = 120.0f;
}

int main() {

    bf::Canvas canva(WIDTH, HEIGHT);

    InitWindow(WIDTH, HEIGHT, "ByteFrame Pong..ish");
    SetTargetFPS(60);

    Image img{};
    img.data = canva.data();
    img.width = WIDTH;
    img.height = HEIGHT;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    Texture2D tex = LoadTextureFromImage(img);

    Paddle left{
        bf::Rectangle(
            50.0f,
            HEIGHT / 2.0f - 50.0f,
            20.0f,
            100.0f,
            bf::WHITE)
    };

    Paddle right{
        bf::Rectangle(
            WIDTH - 70.0f,
            HEIGHT / 2.0f - 50.0f,
            20.0f,
            100.0f,
            bf::WHITE)
    };

    Ball ball{
        bf::Circle({ WIDTH / 2.0f, HEIGHT / 2.0f }, 10.0f, bf::WHITE),
        { 250.0f, 120.0f }
    };

    int scoreLeft = 0;
    int scoreRight = 0;

    float paddleSpeed = 300.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

       
        if (IsKeyDown(KEY_W)) left.rect.move(0.0f, -paddleSpeed * dt);
        if (IsKeyDown(KEY_S)) left.rect.move(0.0f, paddleSpeed * dt);

        if (IsKeyDown(KEY_UP))   right.rect.move(0.0f, -paddleSpeed * dt);
        if (IsKeyDown(KEY_DOWN)) right.rect.move(0.0f, paddleSpeed * dt);

      
        if (left.rect.getY() < 0.0f)
            left.rect.setPos(left.rect.getX(), 0.0f);
        if (left.rect.getY() + left.rect.getH() > HEIGHT)
            left.rect.setPos(left.rect.getX(), HEIGHT - left.rect.getH());

        if (right.rect.getY() < 0.0f)
            right.rect.setPos(right.rect.getX(), 0.0f);
        if (right.rect.getY() + right.rect.getH() > HEIGHT)
            right.rect.setPos(right.rect.getX(), HEIGHT - right.rect.getH());

        
        bf::Vec2 pos = ball.circle.getPos();
        pos.x += ball.vel.x * dt;
        pos.y += ball.vel.y * dt;
        ball.circle.setPos(pos.x, pos.y);

      
        if (pos.y - ball.circle.getRadius() < 0.0f) {
            pos.y = ball.circle.getRadius();
            ball.vel.y *= -1.0f;
            ball.circle.setPos(pos.x, pos.y);
        }
        if (pos.y + ball.circle.getRadius() > HEIGHT) {
            pos.y = HEIGHT - ball.circle.getRadius();
            ball.vel.y *= -1.0f;
            ball.circle.setPos(pos.x, pos.y);
        }

        
        if (pos.x + ball.circle.getRadius() < 0.0f) {
            scoreRight++;
            ResetBall(ball);
            pos = ball.circle.getPos();
        }
        if (pos.x - ball.circle.getRadius() > WIDTH) {
            scoreLeft++;
            ResetBall(ball);
            pos = ball.circle.getPos();
        }

       
        if (ball.vel.x < 0.0f && BallHitsPaddle(left, ball)) {
            float newX = left.rect.getX() + left.rect.getW() + ball.circle.getRadius();
            ball.circle.setPos(newX, pos.y);
            ball.vel.x *= -1.0f;

            float rel = (pos.y - left.rect.getY()) / left.rect.getH() - 0.5f;
            ball.vel.y += rel * 200.0f;
        }

        if (ball.vel.x > 0.0f && BallHitsPaddle(right, ball)) {
            float newX = right.rect.getX() - ball.circle.getRadius();
            ball.circle.setPos(newX, pos.y);
            ball.vel.x *= -1.0f;

            float rel = (pos.y - right.rect.getY()) / right.rect.getH() - 0.5f;
            ball.vel.y += rel * 200.0f;
        }

        
        canva.fill(bf::GREY);

        DrawPaddle(canva, left);
        DrawPaddle(canva, right);
        DrawBall(canva, ball);

        int scale = 5;
        canva.drawNumber(scoreLeft, WIDTH / 2 - 80, 20, scale, bf::WHITE);
        canva.drawNumber(scoreRight, WIDTH / 2 + 20, 20, scale, bf::WHITE);

        UpdateTexture(tex, canva.data());

        BeginDrawing();
        DrawTexture(tex, 0, 0, WHITE);
        DrawFPS(700, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
