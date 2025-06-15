#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <deque>
#include <ctime>

using namespace sf;

const int width = 800, height = 600;
const int boxSize = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
};

int main() {
    RenderWindow window(VideoMode(width, height), "Snake Game");
    window.setFramerateLimit(10);

    Font font;
    font.loadFromFile("assets/font.ttf");

    SoundBuffer eatBuffer, dieBuffer;
    eatBuffer.loadFromFile("assets/eat.wav");
    dieBuffer.loadFromFile("assets/die.wav");
    Sound eatSound(eatBuffer), dieSound(dieBuffer);

    std::deque<SnakeSegment> snake = { {10, 10}, {10, 11}, {10, 12} };
    Direction dir = UP;

    SnakeSegment food = { rand() % (width / boxSize), rand() % (height / boxSize) };

    int score = 0;
    int speed = 10;
    bool gameOver = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up && dir != DOWN) dir = UP;
                if (event.key.code == Keyboard::Down && dir != UP) dir = DOWN;
                if (event.key.code == Keyboard::Left && dir != RIGHT) dir = LEFT;
                if (event.key.code == Keyboard::Right && dir != LEFT) dir = RIGHT;
            }
        }

        if (!gameOver) {
            SnakeSegment head = snake.front();
            switch (dir) {
                case UP: head.y--; break;
                case DOWN: head.y++; break;
                case LEFT: head.x--; break;
                case RIGHT: head.x++; break;
            }

            if (head.x < 0 || head.y < 0 || head.x >= width / boxSize || head.y >= height / boxSize) {
                dieSound.play();
                gameOver = true;
            }

            for (auto& s : snake)
                if (s.x == head.x && s.y == head.y) {
                    dieSound.play();
                    gameOver = true;
                }

            snake.push_front(head);

            if (head.x == food.x && head.y == food.y) {
                eatSound.play();
                score++;
                speed += 1;
                food = { rand() % (width / boxSize), rand() % (height / boxSize) };
                window.setFramerateLimit(speed);
            } else {
                snake.pop_back();
            }

            window.clear();

            for (auto& s : snake) {
                RectangleShape body(Vector2f(boxSize - 1, boxSize - 1));
                body.setPosition(s.x * boxSize, s.y * boxSize);
                body.setFillColor(Color::Green);
                window.draw(body);
            }

            RectangleShape foodRect(Vector2f(boxSize - 1, boxSize - 1));
            foodRect.setPosition(food.x * boxSize, food.y * boxSize);
            foodRect.setFillColor(Color::Red);
            window.draw(foodRect);

            Text scoreText("Score: " + std::to_string(score), font, 20);
            scoreText.setFillColor(Color::White);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
        } else {
            Text overText("GAME OVER\nScore: " + std::to_string(score), font, 30);
            overText.setFillColor(Color::Yellow);
            overText.setPosition(width / 3, height / 2);
            window.draw(overText);
        }

        window.display();
    }

    return 0;
}
