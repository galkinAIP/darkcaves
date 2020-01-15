#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// some function declarations
void runGame();
void initializeMerlin();
void initializeKnights();
void begin();
void endBadly();
void lookAround();
int writeToFile();
void handleChoice();
int writeStats();

// if DEBUG is 1, debug messages are printed. 
int DEBUG = 0;

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
    // a knight attacks with a sword
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
        {" A torch glimmers on the floor. A figure croaches next to it, a bear of a person. A knight?\n Could it be... Persival?"},
        {" The torch is almost dark now. There is no one here. It is eery quite.\n"}
    };

static char descRight[3][200] = {
        {" You feel the dark tunnel behind you. That's where you came from.\n"},
        {" There is some dim light coming from the crack in the ceiling. A figure lies on the floor. A knight?\n Could it be... Lancelot?\n"},
        {" It is noone here. You hear something from the main cave.\n"}
    };

static char fight[6][200] = {
    {" Cautiously you move closer to the knight, concentrating on healing.\n He has the sword pointed to you...\n"},
    {" You feel the healing magic around both of you. The knight looks at you and for a moment, his eyes are clear, but his sword is still raised.\n Also, you feel slightly better.\n"}, 
    {" You feel as if the live is seeping out of you. The knight attacks you!\n"},
    {" But you manage to dodge the knigth's attack just so!\n"},
    {" The sword comes down on you, you don't have time to dodge. You feel pain where the sword pierces the skin.\n"},
    {" The knight lowers the sword. He sways as if his strength has left him and pushes himself past you to the tunnel.\n "}
};

static char badEnd[] = 
    " That's it. You fail. Your friends die. You die too.\n GAME OVER."; 

static char goodEnd[] = 
    " That's it. You rescued your friends. They are alive.\n You are alive as well. Together you manage to find a way outside.\n GAME OVER."; 


// Merlin looks around
// happens each time Merlin enters a cave
// depending on his location, the descriptions are chosen
void lookAround() {
    char cave = *merlin.location;
    if (DEBUG) printf("DEBUG: You look around in the %s cave.\n", &cave);
            
    switch (cave)
    {   
        // main cave
        case 'm':
            // write description of the main cave
            printf("%s\n", descMain[0]);
            // check if knights are in cave
            if (DEBUG) printf("DEBUG: lancelot.location = %s\n", lancelot.location);
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

// random result for healing or attack attempt
int attempt(){
    int randomResult;
    time_t t;
    srand((unsigned) time(&t));
    randomResult = rand() % 6;
    if(randomResult > 1){
        if (DEBUG) printf("DEBUG: attempt worked!\n");
        return 1;
    }else{
        if (DEBUG) printf("DEBUG: attempt did not work.\n");
        return 0;
   }
}

// Merlin attempts to heal the knight
void heal(){
    // which knight is it? depends on Merlin's location
    char cave = *merlin.location;
    // Set a pointer *k to the knight
    struct Knight *k = NULL;
    if (DEBUG) printf("DEBUG: healing, Merlin has %d hp, checking location", merlin.hp);
    switch (cave){
        case 'r':
            if (DEBUG) printf("DEBUG: Here is Lancelot!\n");
            k = &lancelot;
            break;
        case 'l':
            if (DEBUG) printf("DEBUG: Here is Persival!\n");
            k = &persival;
            break;
        }

    if (DEBUG) printf("DEBUG: attack 3 - lancelot, attack 7 - persival: %d\n", k->attack );
    // resistance is up until the knight is healed
    if (k->resistance>0) {
        printf("%s\n", fight[0]);
        // how does healing go?
        if (attempt()) {
            // the healing went well!
            if (DEBUG) printf("DEBUG: healing worked\n");
            k->resistance -= 1;
            // Merling gets one more hp
            merlin.hp += 1;
            printf("%s\n", fight[1]);
        } 
        // Merlin fails to heal the knight and the knight attacks Merlin
        else 
        {
            if (DEBUG) printf("DEBUG: healing did not worked");
            printf("%s\n", fight[2]);
            // merlin looses 2 hp
            merlin.hp -= 2;
            if (DEBUG) printf("DEBUG merlin.hp = %d\n", merlin.hp);
                
            // the knight attacks
            // the attack has the same probability to be successfull as a healing attemt
            if (attempt()){ 
                // the knight manages to atack
                if (DEBUG) printf("DEBUG: attack worked\n");
                printf("%s\n", fight[4]);
                merlin.hp = merlin.hp - k->attack + merlin.defense;
                if (DEBUG) printf("DEBUG Merlin survives the fight with %d hp\n", merlin.hp);
            } else {
                // the attack does not got well, Merlin dodges
                // no hp points are added or 
                if (DEBUG) printf("DEBUG: attack did not work\n");
                printf("%s\n", fight[3]);
            }
        }
        if (DEBUG) printf("DEBUG: k->resistance = %d\n", k->resistance);
        printf("%s", roundHeal);        
    } else {
        // move knight to main cave
        if (DEBUG) printf("DEBUG: move knight to main cave.\n");
        *k->location = 'm';
        if (DEBUG) printf("DEBUG: k->location = %s\n", k->location);
        printf("%s\n", fight[5]);    
    }
    //if (k->resistance > 0){
    //    printf("%s", roundHeal);
    //}
    if (DEBUG) printf("DEBUG: healing round done\n");
}

// Merlin enters a side cave, left or right
void enterCave(char cave) {
    // move Merlin to [m]ain, [l]eft or [r]ight cave and look around
    if (DEBUG) printf("DEBUG: You moved to cave [%s]\n", &cave);
    int e = writeToFile();
    *merlin.location = cave;
    lookAround();
    if (cave != 'm' && (*lancelot.location == cave || *persival.location == cave)){
            printf("%s", roundHeal);
        }
}

// reads the choistandintroinput of the player
char getChoice() {
    char selected;
    scanf("%s", &selected);
    return (char)selected;
}

// handles the general choices except of during the healing/ fight choices
void handleChoice (){
    char choice = getChoice();
    if (DEBUG) printf("DEBUG: You chose: %s\n", &choice);
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
                enterCave('l');
            }
            else {
                printf("%s\n", "There is no left tunnel here.");
            } 
            break;
        // go back
        case 'm':
            // if Merlin is not in the main cave but left of right
            if (*merlin.location != 'm'){
                printf("You turn around and go back to the main cave.\n");
                enterCave('m');
            }
            else {
                if (*persival.location != 'm' || *lancelot.location != 'm'){
                    printf("%s\n", " The opening is closed by huge boulders. No way out.\n");
                } 
            } 
            break;
        // while healing, Merlin backs off and runs to the main cave
        case 'b':
            // if Merlin is not in the main cave but left of right
            if (*merlin.location != 'm'){
                printf("The fear takes over you, you run back to the main cave.\n");
                enterCave('m');
            }
            else {
                if (*persival.location != 'm' || *lancelot.location != 'm'){
                    printf("%s\n", " The opening is closed by huge boulders. No way out.\n");
                } 
            } 
            break;
        // heal
        case 'h':
            if (*merlin.location != 'm'){
                heal();
            }
            else {
                printf("There is noone to heal here.\n");
            }
            break;
        // print statistics
        case 'p':
            printf("Print statistics\n");
            writeStats();
            break;
        // some other command
        default:
            printf("You check your options. (Wise advise - look above.)\n");
            break;
    }
}

// very beginning of the game
void begin(){
    printf("%s\n", intro);
    printf("%s\n", roundOne);
    handleChoice();
};

// Merlin dies
void endBadly(){
    printf("%s\n", badEnd);
    }
// all ends well 
void endWell(){
    printf("%s\n", goodEnd);
}
// the game runs
void runGame() {
    // check if merlin is still alive
    while (merlin.hp>0){
        if (*merlin.location == 'm'){
            printf("%s\n", roundMain);
            if (*lancelot.location == 'm' && *persival.location == 'm') {
                // both knights rescued, the game ends.
                endWell();
                break;
            }
        } else {
            printf("%s\n", roundCave);
        }
        handleChoice();           
    }
    // if merlin is dead, end badly
    if (merlin.hp<=0) endBadly();
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


int writeToFile(){
    FILE *gamesave = fopen("gamesave.txt", "w");
    if (gamesave == NULL)
    {
        printf("Error opening file!\n");
        return 1;
        exit(1);

        }
    /* print some text */
    const char *text = "Write this to the file";
    fprintf(gamesave, "Merlin stats:");
    fprintf(gamesave, "hp: %d\n", merlin.hp);
    fprintf(gamesave, "location: %s\n", merlin.location);

    fprintf(gamesave, "Lancelot stats:\n");
    fprintf(gamesave, "hp: %d\n", lancelot.hp);
    fprintf(gamesave, "location: %s\n", lancelot.location);

    fprintf(gamesave, "Persival stats:\n");
    fprintf(gamesave, "hp: %d\n", persival.hp);
    fprintf(gamesave, "location: %s\n", persival.location);

    fclose(gamesave);
    return 0;

}


int writeStats(){
    /* print some text */
    printf("Merlin stats:\n");
    printf(" hp: %d\n", merlin.hp);
    printf(" location: %s\n", merlin.location);
    printf(" defense: %d\n", merlin.defense);   
    printf(" healing: %d\n", merlin.healing);

    printf("Lancelot stats:\n");
    printf(" hp: %d\n", lancelot.hp);
    printf(" location: %s\n", lancelot.location);
    printf(" resistance: %d\n", lancelot.resistance);
    printf(" attack: %d\n", lancelot.attack);

    printf("Persival stats:\n");
    printf(" hp: %d\n", persival.hp);
    printf(" location: %s\n", persival.location);
    printf(" resistance: %d\n", persival.resistance);
    printf(" attack: %d\n", persival.attack);

    return 0;

}


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

