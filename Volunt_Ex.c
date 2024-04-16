#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //for tolower function
#include <regex.h> //for regex

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
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager, int numEntries);
void loadEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager);
void searchEntries(MovieEntry *entries, int numEntries);
void printEntry(MovieEntry *entry);
char* toLowerCase(char *str);
void editEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager);

int main() {
  printf("\n█▀▄▀█ █▀█ █░█ █ █▀▀   █▀▄ ▄▀█ ▀█▀ ▄▀█ █▄▄ ▄▀█ █▀ █▀▀   █▀▄▀█ ▄▀█ █▄░█ ▄▀█ █▀▀ █▀▀ █▀█\n");
    printf("█░▀░█ █▄█ ▀▄▀ █ ██▄   █▄▀ █▀█ ░█░ █▀█ █▄█ █▀█ ▄█ ██▄   █░▀░█ █▀█ █░▀█ █▀█ █▄█ ██▄ █▀▄\n\n");


    printf("\nWelcome to the Database Manager!\n");
    bool invalidOption = true;
    while (invalidOption) {
        MovieEntry entries[MAX_ENTRIES];
        int numEntries = 0;

        char mode[MAX_CHAR]; // variable to store the mode chosen by the user

        printf("\nChoose a mode:\n");
        printf("Options:\n");
        printf("  Save: Create or edit entries\n");
        printf("  Read: View existing entries\n");
        printf("  Exit: Exit the program\n");
        printf("Your choice: ");
        scanf("%s", mode);

        char* lowerMode = toLowerCase(mode);
    
        if (strcmp(lowerMode, "save") == 0 || strcmp(lowerMode,"s")==0) { 
            // Save mode: allows user to create new entries or edit existing ones
            DatabaseManager dbManager;
            strcpy(dbManager.filename, "Movie_data.csv");

            bool invalidOption2 = true;
            while (invalidOption2) {
                char edit[MAX_CHAR];
                printf("\nWould you like to create a new entry or edit an existing one?\n");
                printf("Options:\n");
                printf("  New: Create a new entry\n");
                printf("  Edit: Edit an existing entry\n");
                printf("Your choice: ");
                scanf("%s", edit);
                if (strcmp(toLowerCase(edit), "new") == 0 || strcmp(toLowerCase(edit), "n") == 0) {
                    invalidOption2 = false;
                    saveNewEntry(&entries[numEntries], &dbManager, numEntries);
                } else if (strcmp(toLowerCase(edit), "edit") == 0 || strcmp(toLowerCase(edit), "e") == 0) {
                    invalidOption2 = false;
                    loadEntries(entries, &numEntries, &dbManager);
                    editEntries(entries, &numEntries, &dbManager);
                } else {
                    printf("Invalid choice! Please select 'New' or 'Edit'.\n");
                }
            }
        } else if (strcmp(lowerMode, "read") == 0 || strcmp(lowerMode,"r")==0) {
            // Read mode: view existing entries and search
            DatabaseManager dbManager;
            strcpy(dbManager.filename, "Movie_data.csv");
            loadEntries(entries, &numEntries, &dbManager);
            searchEntries(entries, numEntries);
        } else if (strcmp(lowerMode, "exit") == 0 || strcmp(lowerMode, "e") ==0) {
            invalidOption = false;
        } else {
            printf("Invalid mode! Please select 'Save', 'Read', or 'Exit'.\n");
        } 
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

void editEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager) {
    // Looping through the edit menu so the user can edit more than once
    bool cont = true;
    while (cont) {
        // listing all of the titles of the entries so the user can select one to edit; also gives them the exit menu option
        printf("These are the titles of the entries you have already entered, please input the corresponding entry number to edit.\n");
        for (int i = 0; i < *numEntries; i++) {
            printf("   %d: %s\n", i, entries[i].title);
        }
        printf("   %d: Exit edit menu\n", *numEntries);

        int entryToEdit;
        scanf("%d", &entryToEdit);
        if (entryToEdit == *numEntries) {
            cont = false; //if they selected the last element, exit the editing menu
        } else if (entryToEdit >= 0 && entryToEdit < *numEntries) {
            char dummyVar[MAX_CHAR]; //variable I use to see if the scanf input is equal to {}, which would mean that they don't want to change anything for that field
            printf("Change the desired fields of this selected entry. If you don't wish to change that specified field, input {}. DO NOT INCLUDE COMMAS INTO AN ENTRY\n");
            MovieEntry *entry = &entries[entryToEdit];
            printf("\nPrevious title: %s\n", entries[entryToEdit].title);
            printf("New title: ");
            scanf(" %[^\n]s", dummyVar);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->title, dummyVar); //copies dummyVar into that field for that entry in the structure; I repeat this process for all fields
            }
            printf("\nPrevious author: %s\n", entries[entryToEdit].author);
            printf("New author: ");
            scanf(" %[^\n]s", dummyVar);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->author, dummyVar);
            }
            printf("\nPrevious duration: %s\n", entries[entryToEdit].duration);
            printf("New duration: ");
            scanf(" %[^\n]s", dummyVar);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->duration, dummyVar);
            }
            printf("\nPrevious genre: %s\n", entries[entryToEdit].genre);
            printf("New genre: ");
            scanf(" %[^\n]s", dummyVar);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->genre, dummyVar);
            }
            printf("\nPrevious comments: %s\n", entries[entryToEdit].comments);
            printf("New comments: ");
            scanf(" %[^\n]s", dummyVar);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->comments, dummyVar);
            }
            printf("\nPrevious link: %s\n", entries[entryToEdit].link);
            printf("New link: ");
            scanf(" %[^\n]s", entry -> link);
            if (strcmp(dummyVar, "{}")) {
                strcpy(entry->link, dummyVar);
            }
            printf("Submission Saved!\n\n");
        } else {
            printf("\nInvalid input, please try again.\n\n"); //if the user inputs anything other than the numbers between 0 and numEntries (inclusive) then re-prompt the menu
        }
    } // I close the while loop here so that it only edits the file once the user is completley done editing

    FILE *file = fopen(dbManager->filename, "w"); // Open the file in write mode, which delets all of the contents
    if (file == NULL) { // Check if file opening was successful
        printf("Error opening file for writing!\n");
        exit(1);
    } else {
        // append the entire structure for all entries
        for (int i = 0; i < *numEntries; i++) {
            MovieEntry *entry = &entries[i];
            fprintf(file, "%s,%s,%s,%s,%s,%s\n", entry->title, entry->author, entry->duration, entry->genre, entry->comments, entry->link);
        }
    }
    fclose(file);
}

// This is the function definition for saving a new entry to the data base
// it takes a pointer to a 'MovieEntry' structure representing the new entry and a pointer to a 'DatabaseManager' structure
void saveNewEntry(MovieEntry *entry, DatabaseManager *dbManager, int numEntries) {
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
    } else {
        fprintf(file, "%s,%s,%s,%s,%s,%s\n", entry->title, entry->author, entry->duration, entry->genre, entry->comments, entry->link);
        printf("Submission Saved!\n\n");
    }
    fclose(file);
    numEntries++;
}




// function definition for loading entries 
// takes an array of 'MovieEntry' structures, a pointer to an integer representing number of entries
// and a pointer to a 'database manager' structure
void loadEntries(MovieEntry *entries, int *numEntries, DatabaseManager *dbManager){ 
    FILE *file = fopen(dbManager->filename,"r"); //open the file in reading mode
    if (file == NULL){ // check if it worked
        printf("Error opening file for reading!\n");
        exit(1);
    }
    
// while loop that continues as long as the number of entries is less than the maximum allowed
// scanf function reads all six fields from the file
    while ((*numEntries < MAX_ENTRIES) && 
    (fscanf(file, "%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%99[^\n]\n", entries[*numEntries].title, entries[*numEntries].author, entries[*numEntries].duration, entries[*numEntries].genre, entries[*numEntries].comments, entries[*numEntries].link) == 6)) {
        (*numEntries)++;   // increment the number of entries 
    }
    fclose(file); 
}


//this is the function definitoin for searching entries in the data base
// it takes an array of 'MovieEntry' structyres and an integer representing the number of entries
void searchEntries(MovieEntry *entries, int numEntries) {
    char criteria[MAX_CHAR]; 
    char mode;
    bool loop = true;
    clearInputBuffer();
    printf("Enter search criteria: ");
    scanf("%[^\n]", criteria); 

    char* lowerCriteria = toLowerCase(criteria);
    
    regex_t regex;
    int reti;
    reti = regcomp(&regex, lowerCriteria, 0);
    if (reti) {
        printf("Could not compile regex\n");
        return;
    }

    bool found = false; 

    for (int i = 0; i < numEntries; i++) { 

        if ((regexec(&regex, toLowerCase(entries[i].title), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].author), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].genre), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].comments), 0, NULL, 0) == 0) ||
            (regexec(&regex, toLowerCase(entries[i].link), 0, NULL, 0) == 0)) {


            clearInputBuffer();

            while (loop) {
                printf("Which criteria would you like to view?\na: title\nb: director\nc: genre\nd: comments\nf: all entries\ng: exit\n");
                scanf(" %c", &mode); 

                switch (mode) {
                    case 'a':
                        printf("Title: %s\n", entries[i].title);
                        break;
                    case 'b':
                        printf("Author: %s\n", entries[i].author);
                        break;
                    case 'c':
                        printf("Genre: %s\n", entries[i].genre);
                        break;
                    case 'd':
                        printf("Comments: %s\n", entries[i].comments);
                        break;
                    case 'e':
                        printf("Link: %s\n", entries[i].link);
                        break;
                    case 'f':
                        printEntry(&entries[i]);
                        break;
                    case 'g':
                        loop = false; // Exit the loop when 'g' is entered
                        break;
                    default:
                        printf("Not an option!\n");
                        break;
                }
            }
            found = true; 
        }
    }
    if (!found) {
        printf("No matching entry found.\n");
    }
    regfree(&regex);
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