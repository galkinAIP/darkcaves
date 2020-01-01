#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void runGame();
void initializeMerlin();
void initializeKnights();
void begin();
void endBadly();
void lookAround();


// general struct for the player character
struct Merlin {
    int hp;
    // aka attack, 
    int healing;
    // when a knight attacks, this value is take to avoid it
    int defense;
    // caves: [m]ain, [r]ight, [l]left 
    char location[1];
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
    // when the knight is healed, he will move to the main cave
    // caves: [m]ain, [r]ight, [l]left
    char location[1];
};

// two knights
static struct Knight lancelot;
static struct Knight persival;

// here come the texts
static char intro[400] = 
    " You wake up on cold stones.\n Through the constant buzzing in your head you can hear disturbing noises.\n Are those cries? Or screeches of a monster?\n And apparently something had been smashed your head.\n Twice at least.\n And you were left there... to die?\n And you are not ready for that yet, are you?\n";

// very first round in the main cave gives two choices
// \n means new line in between
static char roundOne[] =
    "[s] - try to stand up\n[d] - close your eyes and prepare to die";

// second and further rounds in the main cave
// look around, go left, go right
static char roundMain[] =
    "[a] - look around\n[l] - explore the left tunnel\n[r] - explore the right tunnel\n";

// look around, go left, go right
static char roundCave[] =
    "[a] - look around\n[b] - go back to the main cave\n";

// look around, go left, go right
static char roundHeal[] =
    "[h] - heal the knight\n";

// Merlin is not ready and wants to die.
static char death[] =
    " With your eyes shut, you wait until the darkeness engulfes you.\n...\n...\n...\n You are bored by now. You open your eyes.\n";

// the very beginning, Merlin stands up 
static char standintro[] =
    " Despite the awful headache you gather your last strength and stand up. You drop something in the process and it falls on the floor. You recognize a healer's bag, your mentor's bag!\n Now it starts to come back.\n You were on the on your very first mission with the knights of Camelot. All alone, young and unexperienced, but bubling of healing magic.\n Your head is killing you.\n";

// descriptions of the main cave
static char descMain[3][300] = {
        {" You take a look around.\n Behind you, the opening to the outside is closed up by huge boulders. You can't move them alone.\n The wall in front of you is dark, but you can distiguish darker patches. It could be entrances to other caves.\n "},
        // already rescued Lancelot
        {" In the middle of the cave Lancelot is curled on the floor.\n You checked him earlier, he'll be all right once you are out of here.\n"},
        // Persival is there as well 
        {" Persival is leaning next to the left tunnel.\n He is upright but you do not think he can fight now.\n"}
    };
// descriptions left cave
static char descLeft[3][200] = {
        {" The main cave is right behind you.\n"},
        {" A torch glimmers from below. in its light you . A figure croaches on the floor, a huge person. A knight?\n Could it be... Persival?"},
        {" There is some dim light coming from the ceiling.\n There is no one here. It is eery quite.\n"}
    };

static char descRight[3][200] = {
        {" You feel the dark tunnel behind you.\n That's where you came from.\n"},
        {" There is some dim light coming from the crack in the ceiling. A figure lies on the floor. A knight?\n Could it be... Lancelot?"},
        {" It is almost too dark to see anything.\n The torch went out. It is noone here.\n"}
    };

static char badEnd[] = 
    " That's it. You fail. Your friends die. You die too.\n"; 

static char goodEnd[] = 
    " That's it. You rescued your friends. They are alive.\n You are alive as well. Together you manage to find a way outside.\n"; 


// Merlin looks around
// depending on his location, the descriptions are chosen
void lookAround() {
    char location = *merlin.location;
    printf("You look around in the %s cave.\n", &location);
    switch (location)
    {   
        // main cave
        case 'm':
            // write description of the main cave
            printf("%s\n", descMain[0]);
            // check if knights are in cave
            if (*lancelot.location == 'm'){
                printf("%s\n", descMain[1]);    
            }
            if (*persival.location == 'm'){
                printf("%s\n", descMain[2]);    
            }
            break;
        
        case 'l':
            // write description of the left cave
            printf("%s\n", descLeft[0]);
            // check if knights are in cave
            if (*persival.location == 'l'){
                printf("%s\n", descLeft[1]);    
            } 
            else {
                printf("%s\n", descLeft[2]);
            }
            break;
        // Merlin is in the right cave
        case 'r':
            // write description of the right cave
            printf("%s\n", descRight[0]);
            // check if knights are in cave
            if (*lancelot.location == 'r'){
                printf("%s\n", descRight[1]);    
            } 
            else {
                printf("%s\n", descRight[2]);
            }
            break;
    }
}

// Merlin attempts to heal the knight
void heal(){

}
// Merlin enters a side cave, left or right
void enterCave(char cave) {
    // move Merlin to [m]ain, [l]eft or [r]ight cave and look around
    printf("You moved to cave [%s]\n", &cave);
    *merlin.location = cave;
    lookAround();
    switch (cave){
        case 'l':
            // move merlin to the left cave and look around
            *merlin.location = 'l';
            lookAround();
            break;
        case 'r':
            // move merlin to the right cave and look around
            *merlin.location = 'r';
            lookAround();
            break;
        case 'm':
            // move merlin to the main cave and look around
            *merlin.location = 'm';
            lookAround();
            break;
    }
}

// reads the choistandintroinput of the player
char getChoice() {
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
        
        case 's':
            printf("%s\n", standintro);
            break;
        
        case 'a':
            lookAround();
            break;
        
        // go right
        case 'r':
            // if Merlin is in the main cave go to right tunnel
            if (*merlin.location == 'm'){
                printf("You chose to go to the right tunnel.\n");
                enterCave('r');
            } 
            else {
                printf("There is no right tunnel here.\n");
            }
            break;
        // go left     
        case 'l':
            // if Merlin is in the main cave go to left tunnel
            if (*merlin.location == 'm'){
                printf("You chose to go to the left tunnel.\n");
                enterCave('r');
            }
            else {
                printf("%s\n", "There is no left tunnel here.");
            } 
            break;
        // go back
        case 'b':
            // if Merlin is not in the main cave but left of right
            if (*merlin.location != 'm'){
                printf("You turn around to go back to the main cave.\n");
                enterCave('m');
            }
            else {
                if (*persival.location != 'm' || *lancelot.location != 'm'){
                    printf("%s\n", " The opening is closed by huge boulders. Now way out.\n");
                } 
            } 
            break;
        // heal
        case 'h':
            if (*merlin.location != 'm'){
                printf(" You turn around to go back to the main cave.");
                enterCave('m');
            }
            break;
    }
    return 0;
}

// very beginning of the game
void begin(){
    printf("%s\n", intro);
    printf("%s\n\n", roundOne);
    handleChoice();
};

// Merlin dies
void endBadly(){
    printf("%s\n\n", badEnd);
    }
// all ends well 
void endWell(){
    printf("%s\n\n", badEnd);
}
// the game runs
void runGame() {
    int isAlive = 1;
    printf("The game is on.");
    while (isAlive){
        if (*merlin.location == 'm'){
            printf("%s\n\n", roundMain);
            if (*lancelot.location == 'm' && *persival.location == 'm') {
                endWell();
            }
        } else {
            printf("%s\n\n", roundCave);
        }
        handleChoice();
    }
    endBadly();
};

// fill knights with stats
void initializeKnights (){
    // lancelot's hp
    lancelot.hp = 10;
    // lancelot is in the right cave in the beginning
    *lancelot.location = 'r';
    // lancelot was injured and his strikes are slow and easy to dodge
    lancelot.attack = 3;
    lancelot.resistance = 5; 
    
    // persival.hp
    persival.hp = 15;
    // persival is in the left cave in the beginning
    *persival.location = 'l';
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
    *merlin.location = 'm';
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
    begin();
    runGame();
    return 0;
}

