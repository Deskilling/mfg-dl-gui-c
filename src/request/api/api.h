#ifndef MFG_API_H
#define MFG_API_H

#include <curl/curl.h>

int health(CURL* curl);

typedef struct {
    double score;
    char* query;
    char* href;
} Score;

typedef struct {
    char* key;
    Score value;
} ScoreEntry;

typedef struct {
    void* prev;
    void* next;
    char* service;
    char* name;
    char* href;
    char* description;
    char* cover;
    char* productionYear;
    ScoreEntry* scores;
    int scoreCount;
} SearchResult;

SearchResult* search(CURL* curl, char* query);
void freeSearchResult(SearchResult* result);

#endif
