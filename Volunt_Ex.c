#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHAR 100
#define MAX_ENTRIES 100

// define structure for an entry 
typedef struct {
    char title[MAX_CHAR];
    char author[MAX_CHAR];
    char duration[MAX_CHAR];
    char genre[MAX_CHAR];
    char comments[MAX_CHAR];
    char link[MAX_CHAR];
} MovieEntry;

// define structure for managing database
typedef struct {
    char filename[MAX_CHAR];
} DatabaseManager;


//function prototypes
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager);
void loadEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager);
void searchEntries(MovieEntry *entries, int numEntries);
void printEntry(MovieEntry *entry);

int main(){
    MovieEntry entries[MAX_ENTRIES];
    int numEntries = 0;
    char mode[MAX_CHAR]; // variale to store the mode cchosen by the user

    printf("Welcome to the Database Manager!\n");
    printf("Choose mode (save or read)");
    scanf("%s", mode);

    if (strcmp(mode, "save") == 0){
        DatabaseManager dbManager; // instance
        strcpy(dbManager.filename, "Movie_data.csv");
        saveNewEntry(&entries[numEntries], &dbManager); // call function to save new entry
    } else if (strcmp(mode, "read") == 0){
        DatabaseManager dbManager;
        strcpy(dbManager.filename, "Movie_data.csv");
        loadEntries(entries, &numEntries, &dbManager); // load existing entries
        searchEntries(entries, numEntries); // call function to search and display entries
    } else {
        printf("invalid mode!\n");
        return 1;
    }
    return 0; 
    }


// This is the function definition for saving a new entry to the data base
// it takes a pointer to a 'MovieEntry' structure representing the new entry and a pointer to a 'DatabaseManager' structure
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager) {
    printf("Enter title: ");
    scanf(" %[^\n]s", entry->title); 
    getchar(); 

    printf("Enter author: ");
    scanf(" %[^\n]s", entry->author);
    getchar(); 

    printf("Enter duration/number of pages: ");
    scanf(" %[^\n]s", entry->duration);
    getchar(); 

    printf("Enter genre: ");
    scanf(" %[^\n]s", entry->genre);
    getchar(); 

    printf("Enter comments or notes: ");
    scanf(" %[^\n]s", entry->comments);
    getchar();

    printf("Enter link: ");
    scanf(" %[^\n]s", entry->link);
    getchar(); 

    FILE *file = fopen(dbManager->filename, "a"); // Open the file in append mode
    if (file == NULL) { // Check if file opening was successful
        printf("Error opening file for writing!\n");
        exit(1);
    }
    // Write entry details to the file
    fprintf(file, "%s,%s,%s,%s,%s,%s\n", entry->title, entry->author, entry->duration,
            entry->genre, entry->comments, entry->link);

    fclose(file);
}



// function definition for loading entries 
// takes an array of 'MovieEntry' structures, a pointer to an integer representing number of entries
// and a pointer to a 'database manager' structure
void loadEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager){ 
    FILE *file = fopen(dbManager->filename,"r"); //open the file in reading mode
    if (file == NULL){ // check if it worked
        printf("error opening file for reading!\n");
        exit(1);
    }

// while loop that continues as long as the number of entries is less than the maximum allowed
// scanf function reads all six fields from the file
    while ((*numEntries < MAX_ENTRIES) && 
    (fscanf(file, "%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%99[^\n]\n", entries[*numEntries].title,
            entries[*numEntries].author, entries[*numEntries].duration, entries[*numEntries].genre,
            entries[*numEntries].comments, entries[*numEntries].link) == 6)){
             (*numEntries)++;   // increment the number of entries 
            }

            fclose(file);
}


//this is the function definitoin for searching entries in the data base
// it takes an array of 'MovieEntry' structyres and an integer representing the number of entries
void searchEntries(MovieEntry *entries, int numEntries) {
    char criteria[MAX_CHAR]; // variable to store the search criteria
    printf("Enter search criteria: ");
    scanf("%s", criteria);

    printf("search results: \n");
    for (int i = 0; i < numEntries; i++) { // loop through each entry in the 'entries' array
        // check if the search criteria matches any part using the strstr function
        // if any match is found in the title, author, genre, comments or links,
        // it proceeds to print the entry
        if (strstr(entries[i].title, criteria) != NULL || strstr(entries[i].author, criteria) != NULL ||
            strstr(entries[i].genre, criteria) != NULL || strstr(entries[i].comments, criteria) != NULL ||
            strstr(entries[i].link, criteria) != NULL) {
            printEntry(&entries[i]); // function to print the details of the matching entry. it passes the address of the current entry '&entries[i]' as an argument to the function
        }
    }
}

void printEntry(MovieEntry *entry){
    printf("Title: %s\n", entry->title);
    printf("Author: %s\n", entry->author);
    printf("Duration: %s\n", entry->duration);
    printf("Comments/Notes: %s\n", entry->comments);
    printf("link: %s\n", entry->link);
    printf("\n");

}
