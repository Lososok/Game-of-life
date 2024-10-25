#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

void settings();
int menu();
int isEnd(char current_condition[][WIDTH], char next_condition[][WIDTH]);
void startFillField(char matrix[][WIDTH]);
void fillField(char current_condition[][WIDTH], char next_condition[][WIDTH]);
void updateField(char current_condition[][WIDTH], char next_condition[][WIDTH]);
char updateCell(char matrix[][WIDTH], int x, int y);
int countAlive(char matrix[][WIDTH], int x, int y);
void onButtonPush(char *key, int *sleep, int *is_end);
void startMenuGeneration();
void fieldGeneration(char matrix[][WIDTH], int sleep);
void endOfGameMessage();

int main() {
    char currentField[HEIGHT][WIDTH] = {{' '}};
    int sleep = 1000;

    startFillField(currentField);
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        char nextField[HEIGHT][WIDTH] = {{' '}};
        printf("\033[8;%d;%d;t", HEIGHT + 2, WIDTH + 2);
        initscr();
        settings();

        int is_end = menu();
        fieldGeneration(currentField, 1000);

        while (!is_end) {
            char key = ' ';
            onButtonPush(&key, &sleep, &is_end);
            napms(sleep);
            if (!is_end) {
                updateField(currentField, nextField);
                is_end = isEnd(currentField, nextField);
                fillField(currentField, nextField);
                fieldGeneration(currentField, sleep);
            }
        }
        endOfGameMessage();
        endwin();
    }
    return 0;
}

void settings() {
    raw();
    noecho();
    nodelay(stdscr, 1);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
}

int menu() {
    int is_end = 0;
    startMenuGeneration();
    char key = getchar();
    if (key == 'q') {
        is_end = 1;
    }
    return is_end;
}

int isEnd(char current_condition[][WIDTH], char next_condition[][WIDTH]) {
    int count_coincident = 0, count_dead = 0;
    int is_end = 0, cells = WIDTH * HEIGHT;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (current_condition[i][j] == next_condition[i][j]) {
                count_coincident++;
            }
            if (next_condition[i][j] == ' ') {
                count_dead++;
            }
        }
    }
    is_end = (count_coincident == cells || count_dead == cells) ? 1 : 0;
    return is_end;
}

void startFillField(char matrix[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH + 1; j++) {
            char sym = getchar();
            if (sym == '.') {
                matrix[i][j] = ' ';
            } else if (sym == 'O') {
                matrix[i][j] = '+';
            }
        }
    }
}

void fillField(char current_condition[][WIDTH], char next_condition[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            current_condition[i][j] = next_condition[i][j];
        }
    }
}

void updateField(char current_condition[][WIDTH], char next_condition[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            next_condition[i][j] = updateCell(current_condition, j, i);
        }
    }
}

char updateCell(char matrix[][WIDTH], int x, int y) {
    int alive = countAlive(matrix, x, y);
    char cell;
    if (alive < 2) {
        cell = ' ';
    } else if (alive == 2) {
        if (matrix[y][x] == '+') {
            cell = '+';
        } else {
            cell = ' ';
        }
    } else if (alive == 3) {
        cell = '+';
    } else {
        cell = ' ';
    }
    return cell;
}

int countAlive(char matrix[][WIDTH], int x, int y) {
    int count = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int n = 0, m = 0;
            if (x == 0 && j == -1) {
                m = WIDTH - 1;
            } else if (x == WIDTH - 1 && j == 1) {
                m = 0;
            } else {
                m = x + j;
            }
            if (y == 0 && i == -1) {
                n = HEIGHT - 1;
            } else if (y == HEIGHT - 1 && i == 1) {
                n = 0;
            } else {
                n = y + i;
            }
            if (matrix[n][m] == '+' && !(i == 0 && j == 0)) {
                count += 1;
            }
        }
    }
    return count;
}

void onButtonPush(char *key, int *sleep, int *is_end) {
    *key = getch();
    if (*key == 'q') {
        *is_end = 1;
    } else if (*key == '1') {
        *sleep = 2000;
    } else if (*key == '2') {
        *sleep = 1000;
    } else if (*key == '3') {
        *sleep = 100;
    }
}

void startMenuGeneration() {
    clear();
    printw("-------------------------------------------------------------------------------\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                    Hello!                                   -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                        If you want to (after begining):                     -\n");
    printw("-                                                                             -\n");
    printw("-                                Exit - press q                               -\n");
    printw("-                                                                             -\n");
    printw("-                To change speed - press one number from 1 till 3             -\n");
    printw("-                         (when you will start the game)                      -\n");
    printw("-                                                                             -\n");
    printw("-                               Have a nice game                              -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-------------------------------------------------------------------------------\n");
    refresh();
}

void fieldGeneration(char matrix[][WIDTH], int sleep) {
    clear();
    if (sleep == 2000) {
        printw("Speed: 1\n");
    } else if (sleep == 1000) {
        printw("Speed: 2\n");
    } else if (sleep == 100) {
        printw("Speed: 3\n");
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] == '+') {
                attron(COLOR_PAIR(1));
                printw("%c", ' ');
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                printw("%c", ' ');
                attroff(COLOR_PAIR(2));
            }
        }
        printw("\n");
    }
    refresh();
}

void endOfGameMessage() {
    clear();
    printw("-------------------------------------------------------------------------------\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                          Thank you for the game!                            -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-                                                                             -\n");
    printw("-------------------------------------------------------------------------------\n");
    refresh();
    getchar();
}
