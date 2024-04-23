#include <stdio.h>
#include <stdlib.h>

void clear_console();
void display_logo();
void display_summary();
void show_start_screen();
void handle_start_option();
void create_save_file();
void load_save_file(char *fileName);
void load_data_files();
void do_flow();
void load_questions();

typedef struct {
    char question[100];
    char optionOne[100];
    char optionTwo[100];
    char optionThree[100];
    char optionFour[100];
    int correctOption;
} Question;

typedef struct {
    long balance;
    long score;
} Session;

Session session;
Question *questions;
char **facts;

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
void load_data_files() {
    FILE *fp;
    fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        puts("There was an error loading the list of questions (questions.txt)");
        exit(1);
    }
    int totalQuestions = 0;
    int totalFacts = 0;
    char question[100];
    char one[100];
    char two[100];
    char three[100];
    char four[100];
    int correct;
    char fact[100];
    questions = calloc(0, sizeof(Question));
    if (questions == NULL) {
        puts("There was an error allocating memory");
        exit(1);
    }
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", question, one, two, three, four, &correct) != EOF) {
        Question q;
        strcpy(q.question, question);
        strcpy(q.optionOne, one);
        strcpy(q.optionTwo, two);
        strcpy(q.optionThree, three);
        strcpy(q.optionFour, four);
        q.correctOption = correct;
        questions = realloc(questions, (totalQuestions+1)*sizeof(Question));
        if (questions == NULL) {
            puts("There was an error allocating memory");
            exit(1);
        }
        questions[totalQuestions++] = q;
    }
    fp = fopen("facts.txt", "r");
    if (fp == NULL) {
        puts("There was an error loading the list of facts (facts.txt)");
        exit(1);
    }
    while (fscanf(fp, "%[^\n]s\n", fact)) {
        facts = realloc(facts, (totalFacts + 1)*100);
        if (facts == NULL) {
            puts("There was an error allocating memory");
            exit(1);
        }
        facts[totalFacts++] = fact;
    }
}
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
