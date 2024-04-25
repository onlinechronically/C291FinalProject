#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void show_start_screen();
void handle_start_option();
void create_save();
void load_save_file(char *fileName);
void replenish_health();
void roll_health();
void create_new_session();
void load_data_files();
void display_game_data();
char *correctAnswer(char *optionOne, char *optionTwo, char *optionThree, char *optionFour, int correctOption);
void run_level();
char *get_card(int card);
void blackjack();
void intermission();
void clear_console();
void draw_blackjack_logo();
void display_logo();
void draw_skull();
void do_death();
void display_summary();

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
    for (int i = 0; i < 52; i++) get_card(i);
    blackjack();
    /*int opt;
    scanf("%d", &opt);
    if (opt == 1) {
        create_new_session();
        run_level();
    } else if (opt == 2) {
        char fileName[100];
        printf("Enter the file name of the you would like to load: ");
        fgets(fileName, 100, stdin);
        load_save_file(fileName);
        run_level();
    } else if (opt == 3) {
        exit(1);
    }*/
}

// This method saves the current game to a file
void create_save() {
    char fileName[100];
    printf("Enter the file name of the save you would like to save: ");
    fgets(fileName, 100, stdin);
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
    //FILE *fp;
    printf("%s", fileName);
}

// This method, is an in-game action that takes 10 of the users balance and replneishes their health back
void replenish_health() {
    if (session.health < 10 && session.balance >= 10) {
        session.balance -= 10;
        session.health = 10;
        puts("You consume the substance and feel so much better.");
    }
}

// This method, is an in-game action that based on rng either replenishes the users health or takes away 5
void roll_health() {
    if (session.balance >= 1) {
        session.balance -= 1;
        int roll = (rand() %5)+1;
        if (roll == 0) {
            session.health = 10;
            puts("You consume the substance and feel so much better.");
        } else {
            session.health -= 5;
            puts("You consume the substance and start to feel unwell (resulting in you losing -5 HP)");
            if (session.health <= 0) {
                do_death();
            }
        }
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
    while (fscanf(fp, "%[^\n]\n", fact) != EOF) {
        facts = realloc(facts, (totalFacts + 1)*(250*sizeof(char)));
        if (facts == NULL) {
            puts("There was an error allocating memory");
            exit(1);
        }
        int i = totalFacts++;
        facts[i] = malloc(250*sizeof(char));
        if (facts[i] == NULL) {
            puts("There was an error allocating memory");
            exit(1);
        }
        strcpy(facts[i], fact);
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
    while (fscanf(fp, "%[^\n]\n", fact) != EOF) {
       events = realloc(events, (totalEvents + 1)*(250*sizeof(char)));
       if (events == NULL) {
           puts("There was an error allocating memory");
           exit(1);
       }
       int i = totalEvents++;
       events[i] = malloc(250*sizeof(char));
       if (events[i] == NULL) {
           puts("There was ane rror allocating memory");
           exit(1);
       }
       strcpy(events[i], fact);
    }
}

// This method displays the stats for the current session, at the point in time (typically displayed at most times during the game)
void display_game_data() {
    printf("Health %.2f%% | Current Score: %lu | Balance: $%lu\n", ((double) session.health/10)*100, session.score, session.balance);
}

// Given all of the fields for a Question, this method returns a character pointer (a dynamically sized string) of the correct answer
char *correctAnswer(char *optionOne, char *optionTwo, char *optionThree, char *optionFour, int correctOption) {
    if (correctOption == 0) return optionOne;
    else if (correctOption == 1) return optionTwo;
    else if (correctOption == 2) return optionThree;
    else if (correctOption == 3) return optionFour;
    return "N/A";
}

// This method runs the overall flow for the questions being asked to the user, as well as handling special events such as the travelling salesperson
void run_level() {
    clear_console();
    display_game_data();
    int isAQuest = rand() % 3;
    if (!isAQuest) {
        puts("You happen to come across a somewhat interesting travelling market!!!");
        puts("The travelling salesperson has a few offers for you:");
        printf("(0) %s: Free\n", "A risky but rewarding gambling game");
        if (session.balance >= 10) printf("(1) %s: $%d\n", "A suprisingly but promising healing potion", 10);
        if (session.balance >= 2) printf("(2) %s: $%d\n", "A random (but good to know) fact", 2);
        if (session.balance >= 1) printf("(3) %s: $%d\n", "A mysterious magic mushroom", 1);
        int opt;
        printf("What would you like to buy (-1 to leave): ");
        scanf("%d", &opt);
        if (opt == 0) {

        } else if (opt == 1) {
            replenish_health();
        } else if (opt == 2 && session.balance >= 2) {
            printf("Here's a good tip: %s\n", facts[rand() % totalFacts]);
        } else if (opt == 3) {
            roll_health();
        }
        if (opt != -1) puts("The travelling salesperson leaves abruptly...");
        sleep(5);
        intermission();
    } else {
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

char *get_card(int card) {
    int newCard = card + 1;
    char *values[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    char *suits[] = {"Clubs", "Hearts", "Spades", "Diamonds"};
    printf("Card #: %s | Suit: %s | num: %d\n", values[newCard % 13], suits[card / 13], card);
    char *cardName = malloc(strlen(values[newCard % 13]) + strlen(suits[card / 13]) + 3 + 1);
    if (cardName == NULL) {
        puts("There was an error allocating memory");
        exit(1);
    }
    size_t i;
    for (i = 0; i < strlen(values[newCard % 13]); i++) {
        cardName[i] = values[newCard % 13][i];
    }
    cardName[i++] = ' ';
    cardName[i++] = '(';
    for (i = i; i < strlen(values[newCard % 13]) + 2 + strlen(suits[card / 13]); i++) {
        cardName[i] = suits[card / 13][i - strlen(values[newCard % 13]) - 2];
    }
    cardName[i++] = ')';
    cardName[i++] = '\0';
    return cardName;
}

int *get_total_points(int *values, int totalValues) {
    int *allValues = calloc(1, sizeof(int));
    int mainTotal = 0;
    int areAcesFine = 1;
    for (int i = 0; i < totalValues; i++) {
        if (values[i] == 11) {
            if (mainTotal + 11 > 21) {

            }
            // do ace logic
        } else {
            mainTotal += values[i];
        }
    }
    if (!areAcesFine) {
        allValues[0] = mainTotal;
    }
    return allValues;
}

// This method runs a single mini-game of blackjack - using random number generation and not an actual deck
void blackjack() {
    Session debugSession;
    debugSession.health = 10;
    debugSession.balance = 1000;
    debugSession.score = 50;
    session = debugSession;
    clear_console();
    display_game_data();
    puts("");
    draw_blackjack_logo();
    puts("Welcome to BlackJack - 21, in order to play you must either gamble your well-being or your monetary possesions. Both you and the dealer are dealt 2 cards, you may either get another card or stay with the cards you have. Whoever has the higher cumulative values of cards is the winner (but dont go over 21).");
    printf("Would you like to gamble your well-being (1) or a portion of your balance (2): ");
    int opt;
    scanf("%d", &opt);
    if (opt == 1 && ) {
        if (session.health >= 2) {

        } else {
            // doesnt have enough health, kick outta blackjack
        }
    } else if (opt == 2)
    int cardValues[52] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    char **dealer_cards = calloc(2, sizeof(char*));
    int *dealer_values = calloc(2, sizeof(int));
    int dealer_total_cards = 2;
    char **player_cards = calloc(2, sizeof(char*));
    int *player_values = calloc(2, sizeof(int));
    int player_total_cards = 2;
    if (dealer_cards == NULL || player_cards == NULL || dealer_values == NULL || player_values == NULL) {
        puts("There was an error allocating memory");
        exit(1);
    }
    int dealer_first = rand() % 52;
    int dealer_second = rand() % 52;
    int player_first = rand() % 52;
    int player_second = rand() % 52;
    get_card(player_first);
    get_card(player_second);
    get_card(dealer_first);
    get_card(dealer_second);
    dealer_values[0] = cardValues[dealer_first];
    dealer_values[1] = cardValues[dealer_second];
    player_values[0] = cardValues[player_first];
    player_values[1] = cardValues[player_second];
    player_cards[0] = get_card(player_first);
    player_cards[1] = get_card(player_second);
    dealer_cards[0] = get_card(dealer_first);
    dealer_cards[1] = get_card(dealer_second);
    int *total_dealer_value = get_total_points(dealer_values, dealer_total_cards);
    int *total_player_value = get_total_points(player_values, player_total_cards);
    printf("Your hand: ");
    for (int i = 0; i < player_total_cards; i++) printf("%s | ", player_cards[i]);
    //printf("Total Points: 
    free(dealer_values);
    free(player_values);
    free(total_dealer_value);
    free(total_player_value);
    for (int i = 0; i < dealer_total_cards; i++) free(dealer_cards[i]);
    for (int i = 0; i < player_total_cards; i++) free(player_cards[i]);
    free(dealer_cards);
    free(player_cards);
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

// This method displays an ascii art representation of a title for the BlackJack mini-game
void draw_blackjack_logo() {
    puts(" ▄▄▄▄    ██▓    ▄▄▄       ▄████▄   ██ ▄█▀▄▄▄██▀▀▀▄▄▄       ▄████▄   ██ ▄█▀");
    puts("▓█████▄ ▓██▒   ▒████▄    ▒██▀ ▀█   ██▄█▒   ▒██  ▒████▄    ▒██▀ ▀█   ██▄█▒ ");
    puts("▒██▒ ▄██▒██░   ▒██  ▀█▄  ▒▓█    ▄ ▓███▄░   ░██  ▒██  ▀█▄  ▒▓█    ▄ ▓███▄░ ");
    puts("▒██░█▀  ▒██░   ░██▄▄▄▄██ ▒▓▓▄ ▄██▒▓██ █▄▓██▄██▓ ░██▄▄▄▄██ ▒▓▓▄ ▄██▒▓██ █▄ ");
    puts("░▓█  ▀█▓░██████▒▓█   ▓██▒▒ ▓███▀ ░▒██▒ █▄▓███▒   ▓█   ▓██▒▒ ▓███▀ ░▒██▒ █▄");
    puts("░▒▓███▀▒░ ▒░▓  ░▒▒   ▓▒█░░ ░▒ ▒  ░▒ ▒▒ ▓▒▒▓▒▒░   ▒▒   ▓▒█░░ ░▒ ▒  ░▒ ▒▒ ▓▒");
    puts("▒░▒   ░ ░ ░ ▒  ░ ▒   ▒▒ ░  ░  ▒   ░ ░▒ ▒░▒ ░▒░    ▒   ▒▒ ░  ░  ▒   ░ ░▒ ▒░");
    puts(" ░    ░   ░ ░    ░   ▒   ░        ░ ░░ ░ ░ ░ ░    ░   ▒   ░        ░ ░░ ░ ");
    puts(" ░          ░  ░     ░  ░░ ░      ░  ░   ░   ░        ░  ░░ ░      ░  ░   ");
    puts("      ░                  ░                                ░               ");
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
