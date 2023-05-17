#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

#define PADDLE_WIDTH 1
#define PADDLE_HEIGHT 5
#define BALL_DELAY 80000

int main() {
    int paddle1_x, paddle1_y, paddle2_x, paddle2_y;
    int ball_x, ball_y;
    int max_x, max_y;
    int next_x, next_y;
    int direction_x, direction_y;
    int paddle1_direction, paddle2_direction;
    bool game_over = false;

    // Initialize ncurses
    initscr();
    noecho();
    curs_set(0);

    // Get screen size
    getmaxyx(stdscr, max_y, max_x);

    // Initialize paddles and ball positions
    paddle1_x = 1;
    paddle1_y = max_y / 2 - PADDLE_HEIGHT / 2;
    paddle2_x = max_x - 2;
    paddle2_y = max_y / 2 - PADDLE_HEIGHT / 2;
    ball_x = max_x / 2;
    ball_y = max_y / 2;
    direction_x = -1;
    direction_y = -1;
    paddle1_direction = 0;
    paddle2_direction = 0;

    // Set up initial screen
    clear();
    refresh();

    // Game loop
    while (!game_over) {
        // Check for user input
        int ch = getch();
        switch (ch) {
            case 'w':
                paddle1_direction = -1;
                break;
            case 's':
                paddle1_direction = 1;
                break;
            case 'i':
                paddle2_direction = -1;
                break;
            case 'k':
                paddle2_direction = 1;
                break;
            case 'q':
                game_over = true;
                break;
        }

        // Move paddles
        if (paddle1_y + paddle1_direction >= 1 && paddle1_y + paddle1_direction <= max_y - PADDLE_HEIGHT)
            paddle1_y += paddle1_direction;
        if (paddle2_y + paddle2_direction >= 1 && paddle2_y + paddle2_direction <= max_y - PADDLE_HEIGHT)
            paddle2_y += paddle2_direction;

        // Calculate next ball position
        next_x = ball_x + direction_x;
        next_y = ball_y + direction_y;

        // Check for collision with paddles
        if (next_x == paddle1_x + PADDLE_WIDTH && next_y >= paddle1_y && next_y <= paddle1_y + PADDLE_HEIGHT)
            direction_x = 1;
        else if (next_x == paddle2_x - 1 && next_y >= paddle2_y && next_y <= paddle2_y + PADDLE_HEIGHT)
            direction_x = -1;

        // Check for collision with walls
        if (next_y >= max_y - 1 || next_y <= 0)
            direction_y *= -1;

        // Move ball
        ball_x += direction_x;
        ball_y += direction_y;

        // Clear screen
        clear();

        // Draw paddles
        for (int i = 0; i < PADDLE_HEIGHT; i++) {
            mvprintw(paddle1_y + i, paddle1_x, "|");
            mvprintw(paddle2_y + i, paddle2_x, "|");
        }

        // Draw ball
        mvprintw(ball_y, ball_x, "O");

        // Refresh screen
        refresh();

        // Delay
        usleep(BALL_DELAY);
    }

    // Clean up ncurses
    endwin();

    return 0;
}
