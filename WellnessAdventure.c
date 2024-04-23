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

// This method shows the options that the user can choose, to be displayed when on the start screen
void show_start_screen() {
    puts("1. Start New Game");
    puts("2. Continue Game (From File)");
    puts("3. Quit Game");
}

// This method handles the command line input the user should enter on the start screen
void handle_start_option() {
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {

    } else if (opt == 2) {

    } else {
        exit(1);
    }
}

// This method loads all of the questions and facts to be used within the game
// This method will prompt the user to save/quit the game, if they choose not to then the game will continue
// Like the name states, this method clears the command line interface
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
// This method displays a summary of the game, alongside a detailed examplanation of how one would play the game
void display_summary() {
    puts("StayingWell is an educational game directed torwards a younger audience, with the sole intention of informing the said audience all about Good Health & Well Being.");
    puts("How to play: This game is a decision making role-playing game. Throughout this game, you play as an adventurer that seems to have plenty of run-ins with Health & Well-Being concerns. Additionally, throughout this game your goal is to get as much money as possible while keeping a healthy status. You might be prompted about a question regarding this topic, and answering said question will reward you. Additionally, you might be handed information helpful to keeping a healthy and well state.\n");
}
