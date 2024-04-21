#include <stdio.h>
#include <stdlib.h>

void clear_console();
void display_logo();
void display_summary();
void show_start_screen();
void handle_start_option();

int main() {
    clear_console();
    display_logo();
    display_summary();
    show_start_screen();
    handle_start_option();
    return 0;
}

void show_start_screen() {
    puts("1. Start New Game");
    puts("2. Continue Game (From File)");
    puts("3. Quit Game");
}

void handle_start_option() {
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {

    } else if (opt == 2) {

    } else {
        exit(1);
    }
}

void clear_console() {
    puts("\x1B[1;1H\x1B[2J");
}

void display_logo() {
    puts("   _____ _              _          __          __  _ _ ");
    puts("  / ____| |            (_)         \\ \\        / / | | |");
    puts(" | (___ | |_ __ _ _   _ _ _ __   __ \\ \\  /\\  / /__| | |");
    puts("  \\___ \\| __/ _` | | | | | '_ \\ / _` \\ \\/  \\/ / _ \\ | |");
    puts("  ____) | || (_| | |_| | | | | | (_| |\\  /\\  /  __/ | |");
    puts(" |_____/ \\__\\__,_|\\__, |_|_| |_|\\__, | \\/  \\/ \\___|_|_|");
    puts("                   __/ |         __/ |                 ");
    puts("                  |___/         |___/                  ");
}

void display_summary() {
    puts("StayingWell is an educational game directed torwards a younger audience, with the sole intention of informing the said audience all about Good Health & Well Being.");
}