#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include "deck.h"
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

enum flag  {All, None, Standard,Default};
enum Phase { STARTUP, PLAYING, Exit };
enum Deck {Shuffled, UnShuffled};
enum Command {LD, SW, SL, SR, SD, QQ, P, Q,Invalid};
void run();

//------------------loadGame----------------------- -----------------------------------
void loadGame(char* pInput);                                                      //---
void emptyColumnsFoundations();                                                   //---
void emptyList(Card** head);                                                      //---
enum Command convert(char* pInput);                                               //---

//-----------------show---------------------------                                //---
void show(enum flag status);
int getMaxLength();
Card* searchData(Card* head,int r,int c);

//----------------saveGame-------------------------
void saveGame(char* pInput);
bool saveFoundations(char* fName);
bool saveColumns(char* cName);

//--------------------play--------------------------
void play(char* pInput);
bool checkWin();
void moveCard(char* from, char* card, char* to);
bool validate(char* pName);
Card* findCard(Card* head, char suit, char value);
Card* getTopCard(Card* head);
int getIntValue(char value);
void addCard(Card** head, Card* card, int c);
void updateCard(Card* card, int r, int c);
void removeCard(Card** head, Card* card);

//------------------shuffles----------------------
void shuffleCards(char* pInput);
Card* pollFirst(Card** head);
void addToPile(Card** head, Card* card, int randomNr);
int getRandomNr(int* randomNr);
void load(enum Deck type);

//-----public variables-------------------------------------------------------------------------------------------------
enum Phase phase;
enum Command cmd;
Card* columns[7]= {NULL};
Card* foundations[4]= {NULL};
Card* unShuffledDeck=NULL;
Card* shuffledDeck=NULL;
char message[50];

int main() {
    run();
    return 0;
}

//-------------------------------------------------------implementation---------------------------------------------------
void run(){
    show(Default);
    phase= STARTUP;
    char pInput[20];
    strcpy(pInput," ");
    while(phase != Exit) {
        printf("Last Command: %s\n", pInput);
        printf("Message: %s\n", message);
        printf("Input:");
        gets(pInput);
        cmd=  convert(pInput);
        switch (cmd) {
            case LD:    loadGame(pInput);
                break;
            case SW:    show(All); strcpy(message,"Ok");
                break;
            case SL:
            case SR:    unShuffledDeck= loadDeck(unShuffledDeck); shuffleCards(pInput); load(Shuffled); show(Standard);
                break;
            case SD:    saveGame(pInput); show(Standard);
                break;
            case QQ:    phase= Exit;
                break;
            case P:     phase= PLAYING; strcpy(message, "Playing_Phase"); play(pInput);
                break;
            default:    strcpy(message, "Invalid STARTUP_Phase Command.");
                break;
        }
    }
}

enum Command convert(char* pInput){
    if(strncmp(pInput, "LD", 2)==0 || strncmp(pInput, "ld", 2)==0 ){ return LD;}
    else if(strncmp(pInput, "SW", 2)==0  || strncmp(pInput, "sw", 2)==0  ) { return SW; }
    else if(strncmp(pInput, "SL", 2)==0  || strncmp(pInput, "sl", 2)==0  ) { return SL; }
    else if(strncmp(pInput, "SR", 2)==0  || strncmp(pInput, "sr", 2)==0  ) { return SR; }
    else if(strncmp(pInput, "SD", 2)==0  || strncmp(pInput, "sd", 2)==0  ) { return SD; }
    else if(strncmp(pInput, "QQ", 2)==0  || strncmp(pInput, "qq", 2)==0  ) { return QQ; }
    else if(strncmp(pInput, "P", 1) ==0  || strncmp(pInput, "p", 1)==0  )  { return P; }
    else if(strncmp(pInput, "Q", 1) ==0  || strncmp(pInput, "q", 1)==0  )  { return Q; }
    else{ return Invalid;}
}

//------------------loadGame-----------------------
void loadGame(char* pInput){
    emptyColumnsFoundations();
    if(strncmp(pInput+2, "\0",1) != 0){
        char cName[20], fName[20];
        strncpy(cName, (pInput+3),18);
        strncpy(fName, (pInput+3),18);
        strcat(cName,"Columns.txt");
        strcat(fName,"Foundations.txt");
        loadColumns(cName, columns, message);
        loadFoundations(fName,foundations, message);
        show(Standard);
    }else{
        load(UnShuffled);
        show(None);
    }
}

void emptyColumnsFoundations(){
    for(int i=0; i<7; i++){
        emptyList(&columns[i]);
        if(i<4){
            emptyList(&foundations[i]);
        }
    }
}

void emptyList(Card** head)
{
    Card* prev = *head;

    while (*head)
    {
        *head = (*head)->next;
        free(prev);
        prev = *head;
    }
}

//-----------------shufflesCard SR-------------------
void load(enum Deck type){
    emptyColumnsFoundations();
    if(type== Shuffled) {
        int maxRow[7] = {1, 6, 7, 8, 9, 10, 11};
        int maxInvisible[7] = {0, 1, 2, 3, 4, 5, 6};
        int r = 0;
        int c = 0;
        while (shuffledDeck != NULL) {
            Card *temp = pollFirst(&shuffledDeck);

            if (c == 7) {
                r++;
                c = 0;
            }

            while (r >= maxRow[c]) {
                c++;
            }

            if (r <= maxRow[c]) {
                if (r < maxInvisible[c]) {
                    temp->face = "DOWN";
                }
                addCard(&columns[c], temp, c);
            }
            c++;
        }
    }
    else{
        unShuffledDeck= loadDeck(unShuffledDeck);
        int r=0;
        int c=0;
        while(r < 52){
            Card *temp = pollFirst(&unShuffledDeck);
            if (c == 7) {
                r++;
                c = 0;
            }
            addCard(&columns[c], temp, c);
            c++;
        }
        strcpy(message, "loaded successfully.");
    }
}

void shuffleCards(char* pInput){
    cmd = convert(pInput);
    if(cmd == SR) {
        int i = 0;
        int randomNr[52];
        getRandomNr(randomNr);
        while (unShuffledDeck != NULL) {
            Card *temp = pollFirst(&unShuffledDeck);
            addToPile(&shuffledDeck, temp, randomNr[i]);
            i++;
        }
        strcpy(message,"SR shuffled successfully.");
    }
    else if(cmd == SL){
        char nr[2];
        strncpy(nr, (pInput+3),2);
        int split= atoi(nr);
        if(split == 0){
            split= rand() % 52 + 1;
        }

        Card* pile1= NULL;
        Card* pile2= NULL;
        int i=0;
        while(unShuffledDeck != NULL){
            if(i < split){
                Card *temp = pollFirst(&unShuffledDeck);
                addCard(&pile1, temp, 0);
                i++;
            }
            else{
                Card *temp = pollFirst(&unShuffledDeck);
                addCard(&pile2, temp, 1);
                i++;
            }
        }

        i=0;
        emptyList(&shuffledDeck);
        while(i<52){
            Card *temp1 = pollFirst(&pile1);
            if(temp1 !=NULL){
                addCard(&shuffledDeck, temp1, 0);
                i++;
            }

            Card* temp2= pollFirst(&pile2);
            if(temp2 != NULL){
                addCard(&shuffledDeck, temp2, 0);
                i++;
            }
        }
        strcpy(message,"SL shuffled successfully.");
    }
    else{strcpy(message,"Invalid shuffled command.");}
}

Card* pollFirst(Card** head){

    /*Linked list does not exist or the list is empty*/
    if(head == NULL || *head == NULL) return NULL;

    Card* temp = *head;

    /*Moving head to the next node*/
    *head = (*head)->next;

    temp->next=NULL;
    return temp;
}

int getRandomNr(int* randomNr){
    int i, j;

    srand(time(NULL));

    for (i = 0; i < 52; i++) {
        int uniq = 0;

        while (!uniq) {
            randomNr[i] = rand() % 52 + 1;
            uniq = 1;

            for (j = 0; j < i; j++) {
                if (randomNr[i] == randomNr[j]) uniq = 0;
            }
        }
    }
    return randomNr[52];
}

void addToPile(Card** head, Card* card, int randomNr){
    card->row= randomNr;
    if(*head == NULL){
        *head = card;
    }else{
        Card* current= *head;
        if (card->row < current->row) {
            card->next = *head; /*Pointing the new node to where head is currently pointing to*/
            *head = card;/*Pointing head to new node.*/
            return;
        }
        while (current->next != NULL) {
            if (current->row < card->row && card->row < current->next->row) {
                card->next = current->next;
                current->next = card;
                return;
            }
            current= current->next;
        }

        if (card->row < current->row) {
            Card* temp= current;
            current = card;
            card->next = temp;
            return;
        }

        current->next=card;
        card= current;
    }
}

//-----------------show---------------------------
void show(enum flag status){
    system("cls");
    printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n", "C1", "C2", "C3", "C4", "C5", "C6", "C7");
    int f=0;
    int rows;
    if(status == Default){
        rows=4;
    }else{
        rows= getMaxLength(columns);
    }

    for(int r=0;r<=rows;r++)
    {
        for(int c=0; c<=7; c++)
        {
            if(c<7){
                Card * data= searchData(columns[c], r, c);
                if(data != NULL) {
                    if (status == All) {
                        printf("%c%c\t", data->value,data->suit);
                    } else if (status == None) {
                        printf("[]\t");
                    } else {
                        if (strcmp(data->face, "UP") == 0) {
                            printf("%c%c\t", data->value,data->suit);
                        } else {
                            printf("[]\t");
                        }
                    }
                }else{
                    printf("  \t");
                }
            }else if( c== 7 && f<4){
                Card* dataF=getTopCard(foundations[f]) ;
                if(dataF != NULL){
                    printf("%c%c\t F%d", dataF->value,dataF->suit, (f+1));
                }else{
                    printf("[]\t F%d", (f+1));
                }
                f++;
            }
        }
        printf("\n");
    }
}

int getMaxLength()
{
    int max=0;
    for (int i = 0; i < 7; i++) {
        Card* current= columns[i];
        int size=0;
        if(current != NULL) {
            while (current->next != NULL) {
                size++;
                current = current->next;
            }
        }

        if(size > max){
            max= size;
        }
    }

    return max;
}

Card* searchData(Card* head,int r,int c)
{
    Card *current= head;
    while(current!=NULL)
    {
        if(current->column==c && current->row==r)
            return current;
        current=current->next;
    }
    return NULL;
}


//----------------saveGame-------------------------
void saveGame(char* pInput){
    char cName[20];
    char fName[20];
    strncpy(cName, (pInput+3),18);
    bool cStatus= saveColumns(cName);
    strncpy(fName, (pInput+3),18);
    bool fStatus= saveFoundations(fName);
    if(cStatus == true && fStatus== true){
        strcpy(message,"Game saved successfully");
    }else{
        strcpy(message,"Error occurred.");
    }
}

bool saveColumns(char* cName){
    strcat(cName,"Columns.txt");
    FILE * file;
    if( access( cName, F_OK ) == 0 ) {
        // file exists
        file = fopen(cName, "w+");
    } else {
        // file doesn't exist
        file = fopen(cName, "w");
    }

    if (file == NULL) {
        //printf("file can't be created \n");
        //return false;
        strcpy(cName, "../cardsColumns.txt");
        file = fopen(cName, "w");
        if (file == NULL){return false;}
    }
    int skipC[7] = {0, 0, 0, 0, 0, 0, 0};
    int rows = getMaxLength(columns);
    for (int r = 0; r <= rows; r++) {
        for (int c = 0; c < 7; c++) {
            if (skipC[c] == 0) {
                Card *data = searchData(columns[c], r, c);
                if (data != NULL) {
                    fputc(data->value, file);
                    fputc(data->suit, file);
                    fputs(" ",file);
                    fputs(data->face, file);
                } else {
                    skipC[c] = 1;
                }
                fputc('\n', file);
            }
        }
    }
    fclose(file);
    return true;
}

bool saveFoundations(char* fName){
    strcat(fName,"Foundations.txt");
    FILE * file;
    if( access( fName, F_OK ) == 0 ) {
        // file exists
        file = fopen(fName, "w+");
    } else {
        // file doesn't exist
        file = fopen(fName, "w");
    }

    if (file == NULL) {
        //printf("file can't be created \n");
        //return false;
        strcpy(fName, "../cardsFoundations.txt");
        file = fopen(fName, "w");
        if (file == NULL){return false;}
    }
    int f = 0;
    while (f < 4) {
        Card *current = foundations[f];
        while (current != NULL) {
            fputc(current->value, file);
            fputc(current->suit, file);
            fputc('\n', file);
            current = current->next;
        }
        fputc('\n', file);
        f++;
    }

    fclose(file);
    return true;
}


//--------------------play--------------------------
void play(char* pInput){

    while(phase == PLAYING) {
        show(Standard);
        printf("Last Command: %s\n", pInput);
        printf("Message: %s\n", message);
        printf("Input:");
        scanf("%s",pInput);
        cmd= convert(pInput);
        if (cmd == Q) {
            getchar();
            phase= STARTUP;
            strcpy(message, "moved In STARTUP_Phase.");
        } else {
            char from[2];
            char to[2];
            char cardName[2];
            strncpy(from, (pInput+0),2);
            strncpy(to, (pInput+7),2);
            strncpy(cardName, (pInput+3),2);
            moveCard(from, cardName, to);
            if(checkWin()== true){
                phase= STARTUP;
                strcpy(message, "moved in STARTUP_Phase. you won the game.");
            }
        }
    }
    show(Standard);
}

bool checkWin(){
    if(getTopCard(foundations[0]) != NULL && getIntValue(getTopCard(foundations[0])->value) == 13 &&
       getTopCard(foundations[1]) != NULL && getIntValue(getTopCard(foundations[1])->value) == 13 &&
       getTopCard(foundations[2]) != NULL && getIntValue(getTopCard(foundations[2])->value) == 13 &&
       getTopCard(foundations[3]) != NULL && getIntValue(getTopCard(foundations[3])->value) == 13 ){

        return true;
    }
    return false;
}

void moveCard(char* from, char* card, char* to){

    if(validate(from) ==true && validate(to)== true){
        int f= from[1]- '1';
        Card* sCard= findCard(columns[f], card[1], card[0]);
        if(sCard != NULL && strcmp(sCard->face, "UP") == 0){
            int t= to[1]- '1';
            if(to[0]== 'C'){
                Card* dCard= getTopCard(columns[t]);
                if( dCard == NULL && getIntValue(sCard->value)== 13){
                    addCard(&columns[t], sCard, t);
                    removeCard(&columns[f], sCard);
                    strcpy(message, "moved successfully.");
                }else if(dCard != NULL) {
                    if(getIntValue(sCard->value) == (getIntValue(dCard->value) - 1) && sCard->suit != dCard->suit) {
                        addCard(&columns[t], sCard, t);
                        removeCard(&columns[f], sCard);
                        strcpy(message, "moved successfully.");
                    }else{
                        strcpy(message, "card-type match || card-order NOT match.");
                    }
                }else{
                    strcpy(message, "card-order NOT match.");
                }
            }else{
                Card* dCard= getTopCard(foundations[t]);
                if(dCard == NULL && sCard->next==NULL && getIntValue(sCard->value)== 1){
                    addCard(&foundations[t],sCard, t);
                    removeCard(&columns[f], sCard);
                    strcpy(message,"moved successfully.");
                }else if(dCard != NULL && sCard->next==NULL){
                    if(getIntValue(sCard->value) == (getIntValue(dCard->value)+1) && sCard->suit == dCard->suit) {
                        addCard(&foundations[t], sCard, t);
                        removeCard(&columns[f], sCard);
                        strcpy(message, "moved successfully.");
                    }
                    else{
                        strcpy(message, "card-type or card-order not match.");
                    }
                }
                else{
                    strcpy(message,"card-order not match.");
                }
            }
        }
        else{
            strcpy(message,"Card not exited.");
        }
    }
    else{
        strcpy(message,"Source or destination is invalid.");
    }
}

bool validate(char* pName){
    char *place[11] ={"C1","C2","C3","C4","C5","C6","C7","F1","F2","F3","F4"};
    for(int i=0; i< sizeof place; i++){
        int result= strncmp(pName,place[i],2);
        if(result == 0){
            return true;
        }
    }
    return false;
}

Card* findCard(Card* head, char suit, char value){
    Card *current= head;
    while(current!=NULL)
    {
        if(current->suit == suit && current->value == value)
            return current;
        current=current->next;
    }
    return NULL;
}

Card* getTopCard(Card* head){
    Card *current= head;
    if(current !=NULL) {
        while (current->next != NULL) {
            current = current->next;
        }
    }
    return current;
}

int getIntValue(char value){
    int v;
    if (value== 'A') { v = 1; }
    else if (value == 'T') { v = 10; }
    else if (value == 'J') { v = 11; }
    else if (value == 'Q') { v = 12; }
    else if (value == 'K') { v = 13; }
    else {
        v = value - '0';
    }
    return v;
}

void addCard(Card** head, Card* card, int c){

    if(*head == NULL){
        *head = card;
       updateCard(*head, 0, c);
    }
    else {
        Card *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = card;
        updateCard(card, (current->row + 1), c);
    }

}

void updateCard(Card* card,int row, int col){
    Card *c= card;
    while(c != NULL){
        c->row=  row;
        c->column= col;
        c= c->next;
        row++;
    }
}

void removeCard(Card** head, Card* card){
    if(*head == card) {
        *head=NULL;
    }else{
        Card *current= *head;
        while (current->next != card && current->next != NULL) {
            current = current->next;
        }
        current->next=NULL;
        current->face="UP";
    }
}










