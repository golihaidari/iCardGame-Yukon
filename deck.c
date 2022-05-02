#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>


void insertCard(Card **head, char *word, int i, int j);

Card* loadColumns(char* fileName, Card** columns, char* message){
    int skipColumn[7]={0,0,0,0,0,0,0};
    FILE* file;
    char line[10];
    file = fopen(fileName, "r");

    if (file == NULL) {
        strcpy(message,"file does not exist.");
        return NULL;
    }

    int i=0;
    int j= 0;

    char word[8];
    while(fgets(line, sizeof line, file)){
        line[strlen(line)-1]='\0';

        while(skipColumn[j] == 1){
            j++;
        }

        if (j == 7) {
            i++;
            j = 0;
        }

        while(skipColumn[j] == 1){
            j++;
        }

        if(*line != '\0') {
            strcpy(word, line);
            insertCard(&columns[j], word, i, j);
        }else{
            skipColumn[j]= 1;
        }

        j++;
    }

    fclose(file);
    strcpy(message,"loaded successfully.");

    return columns[7];
}

Card* loadFoundations(char* fileName, Card** foundations, char* message){
    FILE* file;
    char line[4];
    file = fopen(fileName, "r");

    if (file == NULL) {
        strcpy(message,"file does not exist.");
        return NULL;
    }

    int f=0;
    int i=0;
    char word[2];
    while(fgets(line, sizeof line, file)) {
        line[strlen(line) - 1] = '\0';

        if(*line != '\0' && f< 4) {
            strcpy(word, line);
            insertCard(&foundations[f], word, i, f);
            i++;
        }else if(*line == '\0' && f<4){
            f++;
        }
    }
    fclose(file);
    strcpy(message,"file loaded successfully.");
    return foundations[4];
}

void insertCard(Card **head, char* word, int i, int j) {
    Card* newCard = (Card*) malloc(sizeof (Card));
    if(newCard == NULL){
        exit(1);
    }
    newCard->row= i;
    newCard->column= j;
    newCard->suit= word[1];
    newCard->value =word[0];
    if(strncmp("DOWN",(word+3),5)==0){
        newCard->face="DOWN";
    }else{
        newCard->face="UP";
    }
    newCard->color =(char*) malloc(strlen(word)+1);
    if(word[1] == 'C' || word[1] == 'S'){
        strcpy(newCard->color,"Black");
    }else
    {
        strcpy(newCard->color,"Red");
    }
    newCard->next = NULL;

    if(*head == NULL){
        *head= newCard;
        return;
    }
    Card* current = *head;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = newCard;
}

Card* loadDeck(Card* deck){
    char deckCards[52][3]={"AC","2C","3C","4C","5C","6C","7C","8C","9C","TC","JC","QC","KC",
                           "AD","2D","3D","4D","5D","6D","7D","8D","9D","TD","JD","QD","KD",
                           "AH","2H","3H","4H","5H","6H","7H","8H","9H","TH","JH","QH","KH",
                           "AS","2S","3S","4S","5S","6S","7S","8S","9S","TS","JS","QS","KS"};
    int i=0;
    while(i<52){
        insertCard(&deck, deckCards[i], i, 0);
        i++;
    }
    return deck;
}

Card* loadDeckFromFile(char* fileName, Card* deck){
    FILE* file;
    char line[4];
    file = fopen(fileName, "r");

    if (file == NULL) {
        printf("file can't be opened \n");
        return NULL;
    }

    int i=0;
    char word[2];
    while(fgets(line, sizeof line, file)) {
        line[strlen(line) - 1] = '\0';

        if(*line != '\0' && i< 52) {
            strcpy(word, line);
            insertCard(&deck, word, i, 0);
            i++;
        }
    }
    fclose(file);
    return deck;
}











