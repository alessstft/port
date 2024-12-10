#include <iostream>
#include <windows.h>
#include <conio.h>
#include <list>
#include <cstdlib>
using namespace std;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const int w = 48;
const int h = 20;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

struct Snake
{
    list<COORD> snake;
    Direction direction;
    COORD fruit;
    int score;

    Snake()
    {
        snake = { {20, 8}, {20, 9}, {20, 10} };
        direction = UP;
        score = 0;
        spawnFruit();
    }

    void spawnFruit()
    {
        fruit.X = rand() % (w - 2) + 1;
        fruit.Y = rand() % (h - 2) + 1;
    }

    void move()
    {
        COORD h = snake.front(), b = h;
        if (direction == Direction::UP)
        {
            b.X = h.X;
            b.Y = b.Y - 1;
        }

        snake.push_back(b);
        snake.pop_back();
    };

    void draw()
    {
        COORD pos = { 0, 0 };
        SetConsoleCursorPosition(console, pos);

        for (int i = 0; i < w + 2; i++) cout << "~";
        cout << endl;

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (j == 0) cout << "!";
                bool print = false;

                if (i == fruit.Y && j == fruit.X) { 
                    print = true;
                    cout << "o";
                }
                else
                for (auto segment : snake)
                {
                    if (segment.X == j && segment.Y == i)
                    {
                        cout << ".";
                        print = true;
                    }
                }

                if (!print) cout << " ";

                if (j == w - 2) cout << "!"; // Правосторонняя граница
            }
            cout << endl;
        }

        for (int i = 0; i < w + 2; i++) cout << "~"; // Нижняя граница
        cout << "\nScore: " << score << endl;
    }

    void logic()
    {
        COORD newHead = snake.front();

        switch (direction)
        {
        case UP: newHead.Y--; break;
        case DOWN: newHead.Y++; break;
        case LEFT: newHead.X--; break;
        case RIGHT: newHead.X++; break;
        }

        if (newHead.X >= w) newHead.X = 0; else if (newHead.X < 0) newHead.X = w - 1;
        if (newHead.Y >= h) newHead.Y = 0; else if (newHead.Y < 0) newHead.Y = h - 1;

        for (auto segment : snake)
        {
            if (segment.X == newHead.X && segment.Y == newHead.Y)
            {
                exit(0);
            }
        }

        snake.push_front(newHead);

        if (newHead.X == fruit.X && newHead.Y == fruit.Y)
        {
            score += 10;
            spawnFruit();
        }
        else
        {
            snake.pop_back();
        }
    }
};



void getKey(Snake* snake) //управление стрелочками 
{
    Direction newDir = snake->direction;
    if (GetAsyncKeyState(VK_UP)) newDir = Direction::UP;
    else if (GetAsyncKeyState(VK_LEFT)) newDir = Direction::LEFT;
    else if (GetAsyncKeyState(VK_DOWN)) newDir = Direction::DOWN;
    else if (GetAsyncKeyState(VK_RIGHT)) newDir = Direction::RIGHT;

    if (newDir != -snake->direction) snake->direction = newDir;
}

int main()
{
    Snake snake;
    snake.spawnFruit();
    while (true)
    {
        getKey(&snake);
        snake.draw();
        snake.logic();
        Sleep(50);
    }
    return 0;
}