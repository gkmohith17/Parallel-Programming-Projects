/*#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <omp.h>

#define N 50  // Size of the maze grid
#define WALL 1
#define PASSAGE 0
#define START_ROW 1
#define START_COL 1
#define GOAL_ROW (N - 2)
#define GOAL_COL (N - 2)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int maze[N][N];
int solutionFound = 0;

// Function prototypes
void generateMaze();
void divide(int rowStart, int colStart, int rowEnd, int colEnd);
void dfs(int row, int col);
void drawMaze(SDL_Renderer *renderer);

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Maze Solver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Generate a random maze
    generateMaze();

    // Event loop
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the maze
        drawMaze(renderer);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void generateMaze() {
    // Set all cells as walls
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            maze[i][j] = WALL;
        }
    }

    // Create passages using recursive division algorithm
    divide(0, 0, N, N);
}

void divide(int rowStart, int colStart, int rowEnd, int colEnd) {
    if (rowEnd - rowStart < 3 || colEnd - colStart < 3) {
        return;
    }

    // Choose a random point to divide
    int divRow = rowStart + 1 + 2 * (rand() % ((rowEnd - rowStart - 1) / 2));
    int divCol = colStart + 1 + 2 * (rand() % ((colEnd - colStart - 1) / 2));

    // Create a passage at the dividing point
    for (int i = rowStart; i < rowEnd; i++) {
        maze[i][divCol] = PASSAGE;
    }
    for (int j = colStart; j < colEnd; j++) {
        maze[divRow][j] = PASSAGE;
    }

    // Recursively divide sub-mazes
    divide(rowStart, colStart, divRow, divCol);
    divide(divRow + 1, colStart, rowEnd, divCol);
    divide(rowStart, divCol + 1, divRow, colEnd);
    divide(divRow + 1, divCol + 1, rowEnd, colEnd);
}

void dfs(int row, int col) {
    if (row == GOAL_ROW && col == GOAL_COL) {
        solutionFound = 1;
        return;
    }

    // Mark the current cell as visited
    maze[row][col] = WALL;

    // Explore neighboring cells (up, down, left, right) in parallel
    #pragma omp parallel for collapse(2) shared(solutionFound)
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int newRow = row + dr;
            int newCol = col + dc;
            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N &&
                maze[newRow][newCol] == PASSAGE && !solutionFound) {
                dfs(newRow, newCol);
            }
        }
    }
}

void drawMaze(SDL_Renderer *renderer) {
    int cellWidth = SCREEN_WIDTH / N;
    int cellHeight = SCREEN_HEIGHT / N;

    // Draw walls and passages
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == WALL) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_Rect rect = {j * cellWidth, i * cellHeight, cellWidth, cellHeight};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Draw solution path
    if (solutionFound) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Cyan color for solution path
        int row = GOAL_ROW;
        int col = GOAL_COL;
        while (row != START_ROW || col != START_COL) {
            SDL_Rect rect = {col * cellWidth, row * cellHeight, cellWidth, cellHeight};
            SDL_RenderFillRect(renderer, &rect);

            // Move to the previous cell in the solution path
            if (row > 0 && maze[row - 1][col] == PASSAGE) {
                row--;
            } else if (row < N - 1 && maze[row + 1][col] == PASSAGE) {
                row++;
            } else if (col > 0 && maze[row][col - 1] == PASSAGE) {
                col--;
            } else if (col < N - 1 && maze[row][col + 1] == PASSAGE) {
                col++;
            }
        }
    }

    // Draw start and goal cells
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for start cell
    SDL_Rect startRect = {START_COL * cellWidth, START_ROW * cellHeight, cellWidth, cellHeight};
    SDL_RenderFillRect(renderer, &startRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for goal cell
    SDL_Rect goalRect = {GOAL_COL * cellWidth, GOAL_ROW * cellHeight, cellWidth, cellHeight};
    SDL_RenderFillRect(renderer, &goalRect);
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <omp.h>

#define N 50  // Size of the maze grid
#define WALL 1
#define PASSAGE 0
#define START_ROW 1
#define START_COL 1
#define GOAL_ROW (N - 2)
#define GOAL_COL (N - 2)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int maze[N][N];
int solutionFound = 0;
int solutionPath[N][N] = {0}; // To record the solution path

// Function prototypes
void generateMaze();
void divide(int rowStart, int colStart, int rowEnd, int colEnd);
void dfs(int row, int col);
void drawMaze(SDL_Renderer *renderer);
void printSolutionPath();

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window and renderer
    SDL_Window *window = SDL_CreateWindow("Maze Solver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize random seed
    srand(time(NULL));

    // Generate a random maze
    generateMaze();

    // Event loop
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the maze
        drawMaze(renderer);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Print the solution path
    printSolutionPath();

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void generateMaze() {
    // Set all cells as walls
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            maze[i][j] = WALL;
        }
    }

    // Create passages using recursive division algorithm
    divide(0, 0, N, N);
}

void divide(int rowStart, int colStart, int rowEnd, int colEnd) {
    if (rowEnd - rowStart < 3 || colEnd - colStart < 3) {
        return;
    }

    // Choose a random point to divide
    int divRow = rowStart + 1 + 2 * (rand() % ((rowEnd - rowStart - 1) / 2));
    int divCol = colStart + 1 + 2 * (rand() % ((colEnd - colStart - 1) / 2));

    // Create a passage at the dividing point
    for (int i = rowStart; i < rowEnd; i++) {
        maze[i][divCol] = PASSAGE;
    }
    for (int j = colStart; j < colEnd; j++) {
        maze[divRow][j] = PASSAGE;
    }

    // Recursively divide sub-mazes
    divide(rowStart, colStart, divRow, divCol);
    divide(divRow + 1, colStart, rowEnd, divCol);
    divide(rowStart, divCol + 1, divRow, colEnd);
    divide(divRow + 1, divCol + 1, rowEnd, colEnd);
}

void dfs(int row, int col) {
    if (row == GOAL_ROW && col == GOAL_COL) {
        solutionFound = 1;
        return;
    }

    // Mark the current cell as visited
    maze[row][col] = WALL;
    solutionPath[row][col] = 1; // Mark cell in solution path

    // Explore neighboring cells (up, down, left, right) in parallel
    #pragma omp parallel for collapse(2) shared(solutionFound)
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int newRow = row + dr;
            int newCol = col + dc;
            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N &&
                maze[newRow][newCol] == PASSAGE && !solutionFound) {
                dfs(newRow, newCol);
            }
        }
    }
}

void drawMaze(SDL_Renderer *renderer) {
    int cellWidth = SCREEN_WIDTH / N;
    int cellHeight = SCREEN_HEIGHT / N;

    // Draw walls and passages
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == WALL) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_Rect rect = {j * cellWidth, i * cellHeight, cellWidth, cellHeight};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Draw start and goal cells
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for start cell
    SDL_Rect startRect = {START_COL * cellWidth, START_ROW * cellHeight, cellWidth, cellHeight};
    SDL_RenderFillRect(renderer, &startRect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color for goal cell
    SDL_Rect goalRect = {GOAL_COL * cellWidth, GOAL_ROW * cellHeight, cellWidth, cellHeight};
    SDL_RenderFillRect(renderer, &goalRect);
}

void printSolutionPath() {
    printf("Solution Path:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", solutionPath[i][j]);
        }
        printf("\n");
    }
}

