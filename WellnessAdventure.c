#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void clear_console();
void display_logo();
void display_summary();
void show_start_screen();
void handle_start_option();
void create_save();
void load_save_file(char *fileName);
void create_new_session();
void load_data_files();
void run_level();
void load_questions();
void display_game_data();
void intermission();

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
    long health;
} Session;

Session session;
Question *questions;
char **facts;
char **events;
time_t t;
int totalFacts = 0;
int totalQuestions = 0;
int totalEvents = 0;

int main() {
    srand((unsigned) time(&t));
    clear_console();
    display_logo();
    display_summary();
    show_start_screen();
    load_data_files();
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
    printf("%d | %d\n", totalQuestions, totalFacts);
    printf("%s\n", facts[0]);
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {
        create_new_session();
        run_level();
    } else if (opt == 2) {
        load_save_file("");
        run_level();
    } else if (opt == 3) {
        exit(1);
    }
}

// This method saves the current game to a file
void create_save() {
    char *fileName;
    printf("Enter the file name of the save you would like to load: ");
    scanf("%s", fileName);
    FILE *fp;
    fp = fopen(fileName, "w");
    if (fp == NULL) {
        puts("There was an error saving the current game");
        exit(1);
    }
    fprintf(fp, "%lu,%lu,%lu", session.score, session.balance, session.health);
}

// This method loads a game session from a file
void load_save_file(char *fileName) {
    FILE *fp;
    printf("%s", fileName);
}

// This method, is an in-game action that takes 10 of the users balance and replneishes their health back
void replenish_health() {
    if (session.health < 10 && session.balance >= 10) {
        session.balance -= 10;
        session.health = 10;
    }
}

// This method creates a new session from the hardcoded starting point
void create_new_session() {
    Session s;
    s.score = 0;
    s.balance = 0;
    s.health = 10;
    session = s;
}

// This method loads all of the questions and facts to be used within the game
void load_data_files() {
    FILE *fp;
    fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        puts("There was an error loading the list of questions (questions.txt)");
        exit(1);
    }
    char question[100];
    char one[100];
    char two[100];
    char three[100];
    char four[100];
    int correct;
    char fact[250];
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
    facts = calloc(0, 100*sizeof(char));
    if (facts == NULL) {
        puts("There was an error allocating memory");
        exit(1);
    }
    fclose(fp);
    fp = fopen("facts.txt", "r");
    if (fp == NULL) {
        puts("There was an error loading the list of facts (facts.txt)");
        exit(1);
    }
    while (fscanf(fp, "%[^\n]s\n", fact) != EOF) {
        facts = realloc(facts, (totalFacts + 1)*(250*sizeof(char)));
        if (facts == NULL) {
            puts("There was an error allocating memory");
            exit(1);
        }
        strcpy(facts[totalFacts++], fact);
    }
    fclose(fp);
    fp = fopen("events.txt", "r");
    events = malloc(0);
    if (events == NULL) {
        puts("There was an error allocating memory");
        exit(1);
    }
    if (fp == NULL) {
        puts("There was an error loading the list of events (events.txt)");
        exit(1);
    }
    while (fscanf(fp, "%[^\n]s\n", fact) != EOF) {
       events = realloc(events, (totalEvents + 1)*(250*sizeof(char)));
    }
}

void display_game_data() {
    printf("Health %.2f%% | Current Score: %lu | Balance: $%lu\n", ((double) session.health/10)*100, session.score, session.balance);
}

void run_level() {
    clear_console();
    display_game_data();
    int isAQuest = rand() % 3;
    if (!isAQuest) {
        puts("You happen to come across a somewhat interesting travelling market!!!");
        puts("The travelling salesperson has a few offers for you:");
        if (session.balance >= 10) printf("(1) %s: $%d\n", "A suprisingly but promising healing potion", 10);
        if (session.balance >= 2) printf("(2) %s: $%d\n", "A random (but good to know) fact", 2);
        int opt;
        printf("What would you like to buy: ");
        scanf("%d", &opt);
        if (opt == 1) {
            replenish_health();
        } else if (opt == 2 && session.balance >= 2) {
            printf("Here's a good tip: %s\n", facts[rand() % totalFacts]);
        }
        puts("The travelling salesperson leaves abruptly...");
        sleep(5);
        intermission();
    } else {
        session.balance++;
        session.score++;
        Question currentQuestion = questions[rand() % totalQuestions];
        int randomEvent = rand() % totalEvents;
        int showHint = rand() % 5;
        if (showHint == 3) printf("You remember: %s\n", facts[rand() % totalFacts]);
        printf("%s\n", events[randomEvent]);
        printf("%s\n", currentQuestion.question);
        printf("1. %s\n", currentQuestion.optionOne);
        printf("2. %s\n", currentQuestion.optionTwo);
        printf("3. %s\n", currentQuestion.optionThree);
        printf("4. %s\n", currentQuestion.optionFour);
        int answer;
        printf("Choose an answer (the number next to the option): ");
        scanf("%d", &answer);
        if (answer-1 == currentQuestion.correctOption) {
            puts("You answered this question correctly");
            int findingRoll = rand() % 3;
            if (findingRoll == 1) {
                int rewardRoll = (rand() % 10)+1;
                printf("...and you stumbled across some sort of treasure ($%d)\n", rewardRoll);
                session.balance += rewardRoll;
            }
            session.score++;
        }
        else {
            int lossHP = (rand() % 3)+1;
            session.health -= lossHP;
            printf("You answered this question incorrectly (the correct answer was %s), and suffered an injury (-%d HP)\n", correctAnswer(currentQuestion.optionOne, currentQuestion.optionTwo, currentQuestion.optionThree, currentQuestion.optionFour, currentQuestion.correctOption), lossHP);
            if (session.health <= 0) {
                do_death();
            }
        }
        sleep(5);
        intermission();
    }
}

// This method will prompt the user to save/quit the game, if they choose not to then the game will continue
void intermission() {
    clear_console();
    display_game_data();
    printf("You have reached a stopping point, would you like to save the game (1) or continue (2): ");
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {
        puts("not impl");
    } else {
        run_level();
    }
}

// Like the name states, this method clears the command line interface
void clear_console() {
    puts("\x1B[1;1H\x1B[2J");
}

// This method displays the title of the game, in an ascii art representation 
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

// This method displays an ascii art representation of a skull emoji, as part of the death part of the game
void draw_skull() {
    puts("                 uuuuuuu");
    puts("             uu$$$$$$$$$$$uu");
    puts("          uu$$$$$$$$$$$$$$$$$uu");
    puts("         u$$$$$$$$$$$$$$$$$$$$$u");
    puts("        u$$$$$$$$$$$$$$$$$$$$$$$u");
    puts("       u$$$$$$$$$$$$$$$$$$$$$$$$$u");
    puts("       u$$$$$$$$$$$$$$$$$$$$$$$$$u");
    puts("       u$$$$$$\"   \"$$$\"   \"$$$$$$u");
    puts("       \"$$$$\"      u$u       $$$$\"");
    puts("        $$$u       u$u       u$$$");
    puts("        $$$u      u$$$u      u$$$");
    puts("         \"$$$$uu$$$   $$$uu$$$$\"");
    puts("          \"$$$$$$$\"   \"$$$$$$$\"");
    puts("            u$$$$$$$u$$$$$$$u");
    puts("             u$\"$\"$\"$\"$\"$\"$u");
    puts("  uuu        $$u$ $ $ $ $u$$       uuu");
    puts(" u$$$$        $$$$$u$u$u$$$       u$$$$");
    puts("  $$$$$uu      \"$$$$$$$$$\"     uu$$$$$$");
    puts("u$$$$$$$$$$$uu    \"\"\"\"\"    uuuu$$$$$$$$$$");
    puts("$$$$\"\"\"$$$$$$$$$$uuu   uu$$$$$$$$$\"\"\"$$$\"");
    puts(" \"\"\"      \"\"$$$$$$$$$$$uu \"\"$\"\"\"");
    puts("           uuuu \"\"$$$$$$$$$$uuu");
    puts("  u$$$uuu$$$$$$$$$uu \"\"$$$$$$$$$$$uuu$$$");
    puts("  $$$$$$$$$$\"\"\"\"           \"\"$$$$$$$$$$$\"");
    puts("   \"$$$$$\"                      \"\"$$$$\"\"");
    puts("     $$$\"                         $$$$\"");
}

// This method displays the death screen to the user, informing of stats and prompting main screen
void do_death() {
    clear_console();
    puts("You failed to take care of your health & well-being, resulting in a loss of the game. Thank you for playing and I hope you learned something useful (and interesting) about health & well-being!!");
    printf("You ended the game with a total score of %f and a balance of $%ld\n", ((double) session.health/10)*100, session.balance);
    draw_skull();
    while(getchar() != '\n');
    printf("Press Enter to return to the main menu.");
    while(getchar() != '\n');
    clear_console();
    display_logo();
    display_summary();
    show_start_screen();
    handle_start_option();
}

// This method displays a summary of the game, alongside a detailed examplanation of how one would play the game
void display_summary() {
    puts("StayingWell is an educational game directed torwards a younger audience, with the sole intention of informing the said audience all about Good Health & Well Being.");
    puts("How to play: This game is a decision making role-playing game. Throughout this game, you play as an adventurer that seems to have plenty of run-ins with Health & Well-Being concerns. Additionally, throughout this game your goal is to get as much money as possible while keeping a healthy status. You might be prompted about a question regarding this topic, and answering said question will reward you. Additionally, you might be handed information helpful to keeping a healthy and well state.\n");
}
