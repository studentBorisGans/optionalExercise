#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //for tolower function
#include <regex.h> // for regex functionality

// These are preprocessor directives defining constants
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

// define structure for managing database. Single field 'filename' which is an array of characters to store the filename
typedef struct {
    char filename[MAX_CHAR];
} DatabaseManager;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//function prototypes
void clearInputBuffer();
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager);
void loadEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager);
void searchEntries(MovieEntry *entries, int numEntries);
void printEntry(MovieEntry *entry);
char* toLowerCase(char *str);

int main(){
    MovieEntry entries[MAX_ENTRIES];
    int numEntries = 0;
    char mode[MAX_CHAR]; // variale to store the mode cchosen by the user

    printf("Welcome to the Database Manager!\n");
    printf("Choose mode (save or read): ");
    scanf("%s", mode);

    char* lowerMode = toLowerCase(mode);
    
    if (strcmp(lowerMode, "save") == 0 || strcmp(lowerMode,"s")==0){ // if user chooses save it creaters a databasemanger instance, sets its filename and calls the savenewentry function with the address of the next available entry in the 'entries' array and the address of the dbManager instance
        DatabaseManager dbManager; // instance
        strcpy(dbManager.filename, "Movie_data.csv");
        saveNewEntry(&entries[numEntries], &dbManager); // call function to save new entry
    } else if (strcmp(lowerMode, "read") == 0 || strcmp(lowerMode,"r")==0){ // if user chooses read also creates databasemanger instance and calls the load entries function to load existing entries into the entries array and increments numentries. 
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

char* toLowerCase(char *str) { // 'char*' returns a pointer to a character which represents the lowercase version of the input string 
    int length = strlen(str); //calculates length 
    char *lowerStr = malloc(length + 1); // dynamically allocate memory for the lowercase string ( the +1 ensures that theres enough space for the null terminator
    for (int i = 0; i < length; i++) { // this loop iterates over each character. for each character toLower is called. The lowercase character is then assigned to the corresponding position in the Lowerstr array
        lowerStr[i] = tolower(str[i]); // Convert each character to lowercase
    }
    lowerStr[length] = '\0'; // Null-terminate the string . signifies end of the string
    return lowerStr;
}


void printEntry(MovieEntry *entry) {  
    printf("Title: %s\n", entry->title);
    printf("Author: %s\n", entry->author);
    printf("Duration: %s\n", entry->duration);
    printf("Genre: %s\n", entry->genre);
    printf("Comments/Notes: %s\n", entry->comments);
    printf("Link: %s\n", entry->link);
    printf("\n");
}


// This is the function definition for saving a new entry to the data base
// it takes a pointer to a 'MovieEntry' structure representing the new entry and a pointer to a 'DatabaseManager' structure
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager) {
    printf("Enter title: ");
    scanf(" %[^\n]s", entry->title); 
    getchar(); 

    printf("Enter author: ");
    scanf(" %[^\n]s", entry->author);
    getchar(); // read and discard the newline character (clear input buffer)

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
//this is the function definition for searching entries in the data base
// it takes an array of 'MovieEntry' structures and an integer representing the number of entries
void searchEntries(MovieEntry *entries, int numEntries) {
    char criteria[MAX_CHAR]; // variable to store the search criteria
    clearInputBuffer();
    printf("Enter search criteria: ");
    scanf("%[^\n]s", criteria);
    printf("search results: \n");

    char* lowerCriteria = toLowerCase(criteria);
    
    regex_t regex;
    int reti;
    reti = regcomp(&regex, lowerCriteria, 0);
    if (reti) {
        printf("Could not compile regex\n");
        return;
    }

    bool found = false; // Variable to track if any matching entry is found

    for (int i = 0; i < numEntries; i++) { // loop through each entry in the 'entries' array
        // check if the search criteria matches any part using the regex function regexec
        // if any match is found in the title, author, genre, comments or links,
        // it proceeds to print the entry
        if ((regexec(&regex, toLowerCase(entries[i].title), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].author), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].genre), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].comments), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].link), 0, NULL, 0) == 0)) {
            printEntry(&entries[i]); // function to print the details of the matching entry. it passes the address of the current entry '&entries[i]' as an argument to the function
            found = true; // Set found to true if any match is found
        }
    }

    if (!found) {
        printf("No matching entry found.\n");
    }

    regfree(&regex);
}
