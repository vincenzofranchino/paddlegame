#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define BALL_SIZE 20

typedef struct {
    float x, y;
    float width, height;
    float dx, dy;
} Paddle;

typedef struct {
    float x, y;
    float size;
    float dx, dy;
} Ball;

void move_paddle(Paddle *paddle, int up, int down) {
    if (up) {
        paddle->y -= 10;
    }
    if (down) {
        paddle->y += 10;
    }

    // Keep paddle within window bounds
    if (paddle->y < 0) paddle->y = 0;
    if (paddle->y + paddle->height > WINDOW_HEIGHT) paddle->y = WINDOW_HEIGHT - paddle->height;
}

void move_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle) {
    ball->x += ball->dx;
    ball->y += ball->dy;

    // Ball collision with top and bottom
    if (ball->y <= 0 || ball->y + ball->size >= WINDOW_HEIGHT) {
        ball->dy = -ball->dy; // Reverse direction
    }

    // Ball collision with paddles
    if (ball->x <= left_paddle->x + left_paddle->width && ball->y + ball->size >= left_paddle->y && ball->y <= left_paddle->y + left_paddle->height) {
        ball->dx = -ball->dx; // Reverse direction
    }

    if (ball->x + ball->size >= right_paddle->x && ball->y + ball->size >= right_paddle->y && ball->y <= right_paddle->y + right_paddle->height) {
        ball->dx = -ball->dx; // Reverse direction
    }

    // Ball out of bounds (reset to center)
    if (ball->x <= 0 || ball->x + ball->size >= WINDOW_WIDTH) {
        ball->x = WINDOW_WIDTH / 2 - ball->size / 2;
        ball->y = WINDOW_HEIGHT / 2 - ball->size / 2;
        ball->dx = -ball->dx; // Reset direction
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Paddle Game",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Define paddles and ball
    Paddle left_paddle = {50, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, 0, 0};
    Paddle right_paddle = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, 0, 0};
    Ball ball = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, 5, 5};

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Get key states
        const Uint8 *key_state = SDL_GetKeyboardState(NULL);

        // Control paddles
        move_paddle(&left_paddle, key_state[SDL_SCANCODE_W], key_state[SDL_SCANCODE_S]);
        move_paddle(&right_paddle, key_state[SDL_SCANCODE_UP], key_state[SDL_SCANCODE_DOWN]);

        // Move the ball
        move_ball(&ball, &left_paddle, &right_paddle);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw paddles and ball
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red paddles
        SDL_Rect left_rect = {left_paddle.x, left_paddle.y, left_paddle.width, left_paddle.height};
        SDL_Rect right_rect = {right_paddle.x, right_paddle.y, right_paddle.width, right_paddle.height};
        SDL_RenderFillRect(renderer, &left_rect);
        SDL_RenderFillRect(renderer, &right_rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow ball
        SDL_Rect ball_rect = {ball.x, ball.y, ball.size, ball.size};
        SDL_RenderFillRect(renderer, &ball_rect);

        // Present the render
        SDL_RenderPresent(renderer);

        // Delay for smooth gameplay
        SDL_Delay(16); // Approx 60 FPS
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    
    

    return 0;
}
