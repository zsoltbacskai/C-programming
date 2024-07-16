#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_code(int *ar, int *br, int *cr) {
    while(*ar < 1) {
        *ar = rand() % 10;
    }
    while(*br < 1 || *br == *ar) {
        *br = rand() % 10;
    }
    while(*cr < 1 || *cr == *ar || *cr == *br) {
        *cr = rand() % 10;
    }
}

int is_valid_guess(int guess) {
    int a = guess / 100;
    int b = (guess / 10) % 10;
    int c = guess % 10;

    return (guess >= 123 && guess <= 987) && (a != b && a != c && b != c) && a != 0 && b != 0 && c != 0;
}

void get_feedback(int ar, int br, int cr, int guess, int *correct_digits, int *correct_positions) {
    int a = guess / 100;
    int b = (guess / 10) % 10;
    int c = guess % 10;

    *correct_digits = 0;
    *correct_positions = 0;

    if (a == ar) (*correct_positions)++;
    if (b == br) (*correct_positions)++;
    if (c == cr) (*correct_positions)++;

    if (a == ar || a == br || a == cr) (*correct_digits)++;
    if (b == ar || b == br || b == cr) (*correct_digits)++;
    if (c == ar || c == br || c == cr) (*correct_digits)++;
}

int give_hint(int attempts[], int feedback[][2], int attempt_count) {
    int possible_guesses[504];
    int possible_count = 0;

    for (int i = 123; i <= 987; i++) {
        if (is_valid_guess(i)) {
            int is_possible = 1;
            for (int j = 0; j < attempt_count; j++) {
                int correct_digits, correct_positions;
                get_feedback(i / 100, (i / 10) % 10, i % 10, attempts[j], &correct_digits, &correct_positions);
                if (correct_digits != feedback[j][0] || correct_positions != feedback[j][1]) {
                    is_possible = 0;
                    break;
                }
            }
            if (is_possible) {
                possible_guesses[possible_count++] = i;
            }
        }
    }

    printf("Lehetseges tippek szama: %d\n", possible_count);
    printf("Lehetseges tippek: ");
    for (int i = 0; i < possible_count; i++) {
        printf("%d ", possible_guesses[i]);
    }
    printf("\n");

    if (possible_count > 0) {
        return possible_guesses[rand() % possible_count];
    } else {
        return -1;
    }
}

int main() {
    srand((unsigned)time(NULL));
    int ar = 0, br = 0, cr = 0;
    int guess;
    int correct_digits, correct_positions;
    int attempts = 10;
    int count = 0;
    int previous_attempts[10];
    int feedback[10][2];

    generate_code(&ar, &br, &cr);
    int code = ar * 100 + br * 10 + cr;
    //printf("code: %d\n", code);

    printf("Mi a 3 jegyu kod (pl. 512)?\n");
    printf("- egy szamjegy (1-9) csak egyszer szerepelhet benne\n");
    printf("- tehat minden szamjegy kulonbozo es nincs benne 0\n");
    printf("- 10 probalkozasi lehetoseged van\n\n");

    while (count < attempts) {
        printf("%d. probalkozas!\n", count + 1);
        printf("Tipp (0 ha tippre van szukseged): ");
        scanf("%d", &guess);

        if (guess == 0) {
            guess = give_hint(previous_attempts, feedback, count);
            if (guess == -1) {
                printf("Nem sikerult tippet adni a rendelkezesre allo informaciok alapjan.\n");
                continue;
            } else {
                printf("A javasolt tipp: %d\n", guess);
            }
        }

        if (!is_valid_guess(guess)) {
            printf("A kod (pozitiv) 3 szamjegyu es nincs benne 0! Egy szamjegy csak egyszer szerepelhet!\n\n");
            continue;
        }

        get_feedback(ar, br, cr, guess, &correct_digits, &correct_positions);
        previous_attempts[count] = guess;
        feedback[count][0] = correct_digits;
        feedback[count][1] = correct_positions;

        if (correct_positions == 3) {
            printf("3 szamjegy helyes es mind a 3 jo helyen van.\nNYERTEL!\n");
            return 0;
        } else {
            printf("%d szamjegy helyes, ebbol %d jo helyen van.\n\n", correct_digits, correct_positions);
        }

        count++;
    }

    printf("\nFigyelem! Nem sikerult 10 probalkozasbol feltornod a kodot!\nVESZTETTEL!\n");
    printf("%d volt a kod.\n", code);

    return 0;
}
