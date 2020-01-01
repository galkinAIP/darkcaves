#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void runGame();
void initializeMerlin();
void initializeKnights();

// general struct for the player character
struct Merlin {
    int hp;
    // aka attack, 
    int healing;
    // when a knight attacks, this value is take to avoid it
    int defense;
    // caves: [m]ain, [r]ight, [l]left 
    char location[5];
};

// this is our character in flesh
// this struct will be filled out in initialization
static struct Merlin merlin;


// there will be two knights, each in one cave
struct Knight {
    int hp;
    // aka defense, resistanse against healing
    int resistance;
    // a knght attacks with 
    int attack;
    // where is the knigth
    // when the knight is healed, he will wait in the main cave
    // caves: [m]ain, [r]ight, [l]left
    char location[5];
};

// two knights
static struct Knight lancelot;
static struct Knight persival;

// here come the texts
static char intro[400] = 
    " You wake up on cold stones.\n Through the constant buzzing in your head you can hear disturbing noises.\n Are those cries? Or screeches of a monster?\n And apparently something had been smashed your head.\n Twice at least.\n And you were left there... to die?\n And you are not ready for that yet, are you?\n";

static char roundOne[] =
    "s - try to stand up\nd - close your eyes and prepare to die";

static char death[] =
    "With your eyes shut, you wait until the darkeness engulfes you.\n...\n...\n...\nNothing happens.\n";

// reads the choice input of the player
char getChoice(){
    char selected;
    scanf("%s", &selected);
    return (char)selected;
}

// handles the choices
int handleChoice (){
    char choice = getChoice();
    printf("You chose: %s\n", &choice);
    switch (choice)
    { 
        case 'd':
            printf("%s\n", death);
            break; 

    }
    return 0;
}

void runGame() {
    printf("%s\n", intro);
    printf("%s\n\n", roundOne);
    handleChoice(getChoice());
};

// fill knights with stats
void initializeKnights (){
    // lancelot's hp
    lancelot.hp = 10;
    // lancelot is in the right cave in the beginning
    // пользуемся функцией из import <string.h>
    // копирует слово right в char array, в данном случае в lancelot.location
    strcpy(lancelot.location, "right");
    // lancelot was injured and his strikes are slow and easy to dodge
    lancelot.attack = 3;
    lancelot.resistance = 5; 
    
    // persival.hp
    persival.hp = 15;
    // persival is in the left cave in the beginning
    strcpy(persival.location, "left");
    // persival is extremly strong
    persival.attack = 7;
    // persival is easy to heal
    persival.resistance = 2;
    
};

// fill Merlin with stats
void initializeMerlin (){
    // Merlin's hp
    merlin.hp = 10;
    // Merlin is in the main cave in the beginning
    strcpy(merlin.location, "main");
    // Merlin's "attack", he heals
    merlin.healing = 5;
    // Merlin tries to dodge the knight's attack
    merlin.defense = 5;
};



//main
int main()
{
    //Initialize stats for Merlin and knights
    initializeMerlin();
    initializeKnights();
    //enter Game loop
    runGame();
    return 0;
}

