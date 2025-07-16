/*---Test Code: attempting to read from a text file and store data in struct members..
..1- The code can now read from a file and store the read data in struct members..
..2- The code has a function that reads from the file (Read_From_File), there is also a small..
.. test in the main function..
..3- The code now checks if the user exists in the file or not..
..4- Attempting to add a game to the user's game list..(successful, but it needs to check..
.. that the rating is between (1-10))..
..5- Successfully wrote to Rating2.txt after a user added a game.
..6- Attempting to implement the Start and start menu page:2,3 in lab instructions(Successful)..
..7- The program can add a new user via the administrator..
..8- Attempting to apply administrator capabilities(print, add user, remove user..), (in progress)
..8-a-Successfully able to add a new user-successful
..8-b-attempting to remove(Remove) user(User)-successful

..9-a- implement the user list,(add game), (successful)..
..10- fixes Read from file function (successful).. + implement User-menu, (Remove game), (successful)
..11- sort usernames in alphabetical order(successful)..
..12- print out user-List with games in alphabetical order by usernames,(in progress)..*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// MACRO DEFINITIONS
#define NAMESIZE 40+1       // Defines the maximum size for names (e.g., user names, game names).
#define ANTAL 5             // Defines the maximum number of games a user can have.
#define NUMBER_OF_USERS 20  // Defines the maximum number of users the system can handle.
#define LIBSIZE 100         // Defines a general library size, used for file name buffers.

// ANSI Color Codes for console output
#define RED    "\e[31m"            // Used for error messages
#define GREEN  "\e[32m"            // Used for user mode messages
#define YELLOW "\e[33m"            // Used for information messages
#define BLUE   "\e[34m"            // Used for admin mode messages
#define PURPLE "\e[35m"            // Used for debug messages
#define CYAN   "\e[36m"            // Reserved
#define GREY   "\e[37m"            // Reserved
#define WHITE  "\e[0m"             // Resets text color to white (default)

// STRUCT DEFINITIONS

// Represents a single board game with its name and rating.
typedef struct {
    char Spel_namn[NAMESIZE]; // Game's name
    int  Betyg;               // Rating, intended to be between 1-10
} Brad_Spel;

// Represents a user, containing their username, a list of their games, and the count of games.
typedef struct {
   char USERname[NAMESIZE];         // User's name (40 chars + null terminator)
   Brad_Spel USERSpel[ANTAL];       // Array of games for the user (max defined by ANTAL)
   int     nbOfspel;                // Number of games the user has rated
} USERS;

// FUNCTION PROTOTYPES

// Input/String handling utility functions
void fskip_line(FILE *f);
void skip_line(void);
void skip_line_YorN(void);
_Bool remove_nl(char *a);
_Bool fread_line(char *a, int n, FILE *f);
void trim(char *to, const char *from);
int isOnlyletters(char s[]);

// File handling functions
FILE *open_file(char NameOfFile[],const char *mode);
void Read_From_File(USERS userLista[], int *index, FILE *f);
void Writing_To_File( USERS *pp, FILE *f_write );

// Core logic functions
int Enter_controll(USERS Alla_anvandare[],char Vem[], int antal_anvandare);
int index_of_user(USERS userLista[], char Vem[], int antal_anvandare);
int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[]);

// Admin menu functions
void AdminstrationLista(void);
USERS add_NewUser_struct(int *nummberOfusers,int *stop,USERS userLista[] );
int Remove_User(USERS Alla_anvandare[], int *antal_anvandare);
void moveIndex(USERS userLista[], int *antal_anvandare, int indexOfuser);

// User menu functions
void user_Meny(char vem[]);
int add_game(USERS *p_add, USERS userLista[],int antal_anvandare );
int Remove_Game(USERS *remove, int indexOf_user,USERS userLista[]);
void delete_index_Of_game(USERS *delete, int game_index_to_remove);
int Search_Game(USERS *find, char game[],int *index_of_game);
int find_Game(USERS *get_game);
int Have_rated_game(USERS *pointer);


// Printing functions
void print_UserLista(USERS *pp);
void print_games(USERS *p1);
void print_usernames(USERS *pointer, int *nOfuser, int antal_anvandare,USERS userLista[]);
void print_usernames_without_games(USERS *pointer, int *nOfuser,int antal_anvandare, USERS userLista[]);

// Unused or test functions
int check_inmatning(int *nummberOfusers );
//void Print_UsersNames(USERS *pp); // Appears to be an unused function
/**********************************************************************************/

int main(void){
   int antal_anvandare=0; // Total number of users, initialized to 0.
   USERS userLista[30];   // Array of USERS structs to hold all user data.
   char NameOfFile[100];  // Buffer for the filename to write to on exit.
   char fileToopen[]="Ra.txt"; // The default file to read from.
   int stop=1; // Loop control variable.
   int count_users; // Loop counter for printing users.
   int all_users; // Temporary variable to hold user count.
   
   // Variables for the User-menu
   int indexOfanvandare; // Index of the currently logged-in user.
   int anvandare_Val; // User's menu choice.
   char game_to_search[NAMESIZE]; // Buffer for game search input.
   int gameIndex=-1; // Index of a found game.

   char Vem[NAMESIZE]; // Buffer to store input for username, "admin", or "quit".
   
   printf("Welcome to boardgame ratings.\n");
   printf("Which file do you want to use:");
   scanf("%s", NameOfFile); // Asks user for the filename to save to later.

   FILE *infil=open_file(fileToopen, "r"); // Opens the source data file.
   
   // Read data from the file and populate the userLista array of structs.
   Read_From_File(userLista, &antal_anvandare, infil);
   
   int check=0; // Flag to check if a user exists.
   int UserVal=0; // Admin's menu choice.
   printf("\n");

   // Main program loop. Continues until the user types "quit".
   do{
    printf("Please enter user name, admin or quit:\n");
    scanf("%s", Vem);
    
    // Check if the input is a valid user before proceeding.
    check=Enter_controll(userLista,Vem, antal_anvandare);
    
    // --- ADMIN LOGIC ---
    if(strcmp(Vem,"admin")==0){
        do{
           AdminstrationLista(); // Display the admin menu.
           printf("Choose:");
           scanf("%d",&UserVal); 
           switch(UserVal){
              case 1: // Add a new user
                do{
                   all_users=antal_anvandare;
                   // Add a new user and update the total count.
                   userLista[all_users++]=add_NewUser_struct(&antal_anvandare,&stop,userLista);
                }while(stop!=0); // Continue adding until 'q' is entered in the function.
                    break;
              case 2: // Remove a user
                     do{
                     }while(Remove_User(userLista ,&antal_anvandare)!=-2); // Continue removing until 'q' is entered.
                     break; 
              case 3: // Print all usernames (sorted)
                       if(antal_anvandare==0){
                        printf(RED"No users registrered" WHITE"\n");
                        break;
                     }else{ 
                        for(int c=0; c<antal_anvandare;c++){
                            print_usernames_without_games(&userLista[c],&c, antal_anvandare,userLista);
                        }
                     }                                                                   
                    break;             
              case 4: // Print all usernames and their games (sorted)
                     if(antal_anvandare==0){
                        printf(RED"No users registrered" WHITE"\n");
                        break;
                     }else{
                        for(count_users=0; count_users<antal_anvandare; count_users++){
                           print_usernames(&userLista[count_users],&count_users, antal_anvandare,userLista); 
                        }
                     }            
                    break;   
              case 5: // Exit admin menu
                    if(UserVal==5) break;
              default: // Handle invalid input.
                      if(UserVal<1 || UserVal>5){
                         UserVal=5; // Force exit from admin menu.
                      } 
           } 
        }while(UserVal!=5);
       
    // --- QUIT LOGIC ---
    }else if(strcmp(Vem,"quit")==0){
        char string1[]="Users and boardgames:";
        char string2[]="________________________________________";
        FILE *utfil=open_file(NameOfFile,"w"); // Open the specified file for writing.
        fprintf(utfil,"%s\n",string1);
        fprintf(utfil,"%s\n",string2);
        // Loop through all users and write their data to the file.
        for(int i=0; i<antal_anvandare; i++){
            Writing_To_File( &userLista[i], utfil );
        }
        fclose(utfil);
        printf("Saving all data to %s\n",NameOfFile); 
        printf("Goodbye! \n");
        break; // Exit the main loop.

    // --- USER LOGIC ---
    }else{
       if(check!=0){ // 'check' is non-zero if the user exists.
        do{
            // Get the index of the current user to work with their data.
            indexOfanvandare = index_of_user(userLista, Vem, antal_anvandare);  
            user_Meny(Vem); // Display the user-specific menu.
            printf("Choose:");
            scanf("%d",&anvandare_Val); 
            switch(anvandare_Val){
                case 1: // Print user's games.
                      print_games(&userLista[indexOfanvandare]);
                      break;
                case 2: // Add a game.
                        while(1){
                           if(!add_game(&userLista[indexOfanvandare],userLista,antal_anvandare))
                           break;  
                        }  
                      break;
                case 3: // Search for a game.
                      while(1){
                        if(!find_Game(&userLista[indexOfanvandare])) // Break if user has no games.
                           break;  
                        printf("Search (q to quit):");
                        scanf("%s",game_to_search);
                        if(strcmp(game_to_search,"q")==0)
                           break;
                        // Search and break if a unique game is not found.
                        if(!Search_Game(&userLista[indexOfanvandare],game_to_search,&gameIndex))
                           break;
                      }
                      break;
                case 4: // Remove a game.
                      Remove_Game(&userLista[indexOfanvandare], indexOfanvandare, userLista);
                      break;
                case 5: // Exit user menu.
                      break; 
                default: // Handle invalid input.
                        if(anvandare_Val <1 || anvandare_Val >5){
                            anvandare_Val=5; // Force exit from user menu.
                        }                        
            }
      }while(anvandare_Val!=5);
     }
    }
   }while(strcmp(Vem,"quit")!=0);
   return 0;
}

/**********************************************************************************/
// UTILITY FUNCTIONS
/**********************************************************************************/

/**
 * @brief Skips the remainder of a line in a file stream.
 * @param f The file pointer.
 */
void fskip_line(FILE *f) {
  int c;
  while ( (c = fgetc(f)) != '\n' && c != EOF )
    ;  
}
 
/**
 * @brief Skips the remainder of a line from standard input.
 */
void skip_line(void) {
  fskip_line(stdin); 
}

/**
 * @brief Consumes remaining characters in the input buffer until a newline or EOF.
 * Identical to skip_line, perhaps intended for specific prompts.
 */
void skip_line_YorN(void) {
  int i;
  while ( (i = getchar()) != '\n' && i != EOF )
    ;  
}

/**
 * @brief Removes a trailing newline character from a string.
 * @param a The string to modify.
 * @return 1 if a newline was removed, 0 otherwise.
 */
_Bool remove_nl(char *a) {
  int i = strlen(a)-1;
  if (a[i] == '\n') {
    a[i] = '\0';
	return 1;
  }
  return 0;
}

/**
 * @brief Reads a line from a file safely, handling the newline character.
 * @param a The buffer to store the line.
 * @param n The size of the buffer.
 * @param f The file pointer.
 * @return 0 on failure or EOF, 1 on success.
 */
_Bool fread_line(char *a, int n, FILE *f) {
  if (fgets(a, n, f) == NULL)
    return 0;
  if (!remove_nl(a)) 
	fskip_line(f);
  return 1;  
}

/**
 * @brief Removes leading and trailing whitespace from a string.
 * @param to The destination buffer for the trimmed string.
 * @param from The source string to trim.
 */
void trim(char *to, const char *from) {
  int i = 0, j = strlen(from)-1;
  while(isspace(from[i]))
      i++;
  while(j >= 0 && isspace(from[j]))
      j--;
  if (j >= i)
    strncpy(to, from+i, j-i+1);
  to[j+1] = '\0';
}

/**
 * @brief Checks if a string contains only alphabetic characters.
 * @param s The string to check.
 * @return 1 if the string contains only letters, 0 otherwise.
 */
int isOnlyletters(char s[]){
  for(int i=0; i<strlen(s); i++){
      if(isdigit(s[i]))
       return 0;
  }
  return 1;
}

/**********************************************************************************/
// FILE AND DATA HANDLING FUNCTIONS
/**********************************************************************************/

/**
 * @brief Opens a file with the specified mode and handles errors.
 * @param NameOfFile The name of the file to open.
 * @param mode The mode to open the file in ("r", "w", etc.).
 * @return A file pointer to the opened file. Exits on failure.
 */
FILE *open_file(char NameOfFile[],const char *mode){
  FILE *f=fopen(NameOfFile, mode);
  if(f==NULL){
    printf("Cann't open the file! ");
    exit(1);
  }
  return f;
}

/**
 * @brief Reads user and game data from a file and populates the structs.
 * @param userLista The array of USERS structs to fill.
 * @param index A pointer to the total number of users, which gets updated.
 * @param f The file pointer to read from.
 */
void Read_From_File(USERS userLista[], int *index, FILE *f){
  USERS *pekare; // Pointer to the current user being processed.
  char s[150]; // Buffer to read each line.
  char copy[150]; // Buffer for the trimmed line.
  int uppdela=0; // Stores the index of the current user for adding games.
  int StructIndex=0; // Index for the game array within a user struct.
  int j=0; // Counter to skip the first two header lines in the file.

  while (fread_line(s, 150,f)){
    trim(copy,s);
    if(j<2){ // Skips the first two lines (header).
      printf("%s\n",copy);
      j++;
    }else{
        // If the line is not "No games registrered".
        if(strstr(copy,"No games registrered")==NULL){
             // If the line contains only letters, it's a username.
             if(isOnlyletters(copy)){
               pekare=&userLista[*index];
               strcpy(pekare->USERname, copy);
               pekare->nbOfspel=0;
               uppdela=*index; // Save the current user's index.
               *index=*index+1; // Increment total user count.
               StructIndex=0; // Reset game index for the new user.
             // If the line contains numbers, it's a game and rating.
            }else if(!isOnlyletters(copy)){
                pekare=&userLista[uppdela]; // Point to the last added user.
                sscanf(copy, "%s %d", pekare->USERSpel[StructIndex].Spel_namn, 
                                          &pekare->USERSpel[StructIndex].Betyg); 
                StructIndex=StructIndex+1;
                pekare->nbOfspel=pekare->nbOfspel+1; // Increment game count for this user.
           }
        // If the line is "No games registrered".
        }else if(strstr(copy,"No games registrered")!=NULL){
            pekare=&userLista[uppdela];
            pekare->nbOfspel=0;
        }
    }
  }
}

/**
 * @brief Writes the data of a single user (name and games) to an open file.
 * @param pp Pointer to the user's struct.
 * @param f_write The file pointer to write to.
 */
void Writing_To_File( USERS *pp, FILE *f_write ){
  char string3[]="No games registrered";
  int i=0;
  fprintf( f_write,"%s\n", pp->USERname);
  if(pp->nbOfspel==0){
    fprintf(f_write,"%s\n",string3);
  }else{
    for(; i<pp->nbOfspel; i++)
        fprintf( f_write," %s  %d\n",pp->USERSpel[i].Spel_namn,
                                    pp->USERSpel[i].Betyg);
  }
}

/**********************************************************************************/
// LOGIN AND USER CHECKING FUNCTIONS
/**********************************************************************************/

/**
 * @brief Checks if a given username exists in the list of registered users.
 * @param Alla_anvandare The array of all users.
 * @param Vem The username to search for.
 * @param antal_anvandare The total number of users.
 * @return 1 if the user exists, 0 otherwise.
 */
int Enter_controll(USERS Alla_anvandare[],char Vem[], int antal_anvandare){
    if(antal_anvandare==0){
        // No need to print this if input is "admin" or "quit"
        if (strcmp(Vem, "admin") != 0 && strcmp(Vem, "quit") != 0) {
            printf("There are no Users\n");
        }
        return 0;
    }else{
        // Search for the user. If found, return 1.
        for(int i=0; i<antal_anvandare; i++){
            if(strcmp(Alla_anvandare[i].USERname,Vem)==0){
                return 1;
            }
        }
    }
    // After the loop finishes without finding the user, return 0.
    if (strcmp(Vem, "admin") != 0 && strcmp(Vem, "quit") != 0) {
        printf("User does not exist\n");  
    }
    return 0; 
}

/**
 * @brief Returns the array index of a given user.
 * @param userLista The array of all users.
 * @param Vem The username to find.
 * @param antal_anvandare The total number of users.
 * @return The index of the user if found, -1 otherwise.
 */
int index_of_user(USERS userLista[], char Vem[], int antal_anvandare){
   int UserIndex;
   for(int i=0; i<antal_anvandare; i++){
            if(strcmp(userLista[i].USERname,Vem)==0){
                UserIndex=i;
                return UserIndex;
            }
        } 
    return -1;  
}

/**
 * @brief Checks if a new username already exists in the user list.
 * @param userLista The array of all users.
 * @param nummberOfusers Pointer to the total number of users.
 * @param NewUser The new username to check.
 * @return 1 if the user does NOT exist (it's available), 0 if it already exists.
 */
int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[]){
    for(int i=0; i<*nummberOfusers; i++){
        if(strcmp(userLista[i].USERname, NewUser)==0){
            return 0; // User found, name is not available.
        }
    }
    // After the loop runs without finding the user, return 1.
    // This means the username is available.
    return 1;
}

/**********************************************************************************/
// ADMIN MENU FUNCTIONS
/**********************************************************************************/

/**
 * @brief Prints the administrator menu options to the console.
 */
void AdminstrationLista(void){
    printf("Adminstration\n");
    printf("      1)Add user\n");
    printf("      2) Remove user \n");
    printf("      3) Print all users\n"); 
    printf("      4) Print all users and all their ratings \n");
    printf("      5) Exit\n");
}

/**
 * @brief Handles the logic for adding a new user.
 * @param nummberOfusers Pointer to the total number of users, which gets incremented.
 * @param stop Pointer to a loop control variable in main. Set to 0 to stop the calling loop.
 * @param userLista The array of all users, used to check for existing names.
 * @return A new USERS struct for the newly created user.
 */
USERS add_NewUser_struct(int *nummberOfusers,int *stop,USERS userLista[]){
       char NewUser[NAMESIZE];
       USERS nyAnvandare;
       while(1){ // Loop until a valid action (add, quit) is taken.
            printf("Register new user (q to quit):");
            scanf("%s", NewUser);
            
            if(strcmp(NewUser,"q")==0){
               *stop=0; // Signal the main loop to stop asking for new users.
               break;
            }
              
            if(!Check_User(userLista, nummberOfusers,NewUser)){
                printf("User name exist! Please choose another.\n");
                *stop=1; // Continue the loop in main.
                // Loop again in this function for another name.
                continue; 
            }
            
            if(*nummberOfusers >= NUMBER_OF_USERS ){ 
              printf("The register is full! \n");
              *stop = 0; // Stop the loop in main.
              break;
            }
            
            if(*nummberOfusers < NUMBER_OF_USERS){
               strcpy(nyAnvandare.USERname, NewUser);
               *nummberOfusers=*nummberOfusers+1; // Increment total user count.
               nyAnvandare.nbOfspel=0;
               *stop=1; // Signal main loop to continue.
               return nyAnvandare; // Return the new user struct.
            }
       }
       // Return an empty struct if user quits or register is full.
       strcpy(nyAnvandare.USERname, "");
       nyAnvandare.nbOfspel=0;
       return nyAnvandare;
}

/**
 * @brief Handles the user removal process.
 * @param Alla_anvandare The array of all users.
 * @param antal_anvandare Pointer to the total number of users.
 * @return -2 to signal quitting the remove-loop, -1 for other cases.
 */
int Remove_User(USERS Alla_anvandare[], int *antal_anvandare){
    char nameTOremove[NAMESIZE];
    char answer[2];
    int indexOfuser;
    int nummberOfusers=*antal_anvandare;   

    printf("Remove user (q to quit):\n");
    scanf("%s", nameTOremove);

    if(strcmp(nameTOremove,"q")==0){
        return -2; // Signal to exit the remove loop in main.
    }

    // If the user to be removed does not exist.
    indexOfuser=index_of_user(Alla_anvandare, nameTOremove, nummberOfusers);
    if(indexOfuser == -1){
        printf("User do not exist! Please choose another. \n");
        return -1;
    }
    
    // If user exists, check if they have rated games.
    if(!Have_rated_game(&Alla_anvandare[indexOfuser])){
        // If they have no games, remove them directly.
        moveIndex(Alla_anvandare, antal_anvandare, indexOfuser);
        printf(GREEN "Removed" WHITE "\n");
        return -1; // Done with this removal attempt.
    } else {
        // Warn that the user has rated games and ask for confirmation.
        printf(RED"Warning: User has rated games. " WHITE "\n");
        printf("Do you still want to remove %s (y/n)?", nameTOremove);
        scanf("%s",answer);
        if(strcmp(answer,"y")==0){
            moveIndex(Alla_anvandare, antal_anvandare, indexOfuser);
            printf(GREEN "Removed" WHITE "\n");
            return -1; // Done.
        }else if(strcmp(answer,"n")==0) {
            return -1; // Abort removal.
        }
    }
    return -1; // Default return.       
}

/**
 * @brief Shifts all elements in the user array up to fill a gap from a removed user.
 * @param userLista The array of users.
 * @param antal_anvandare Pointer to the total user count, which is decremented.
 * @param indexOfuser The index of the user to remove.
 */
void moveIndex(USERS userLista[], int *antal_anvandare, int indexOfuser){
    for(int i=indexOfuser;i<(*antal_anvandare)-1;i++){
        userLista[i]=userLista[i+1];
    }
    (*antal_anvandare)--;
}


/**********************************************************************************/
// USER MENU FUNCTIONS
/**********************************************************************************/

/**
 * @brief Prints the menu for a logged-in user.
 * @param vem The name of the user.
 */
void user_Meny(char vem[]){
  printf("%s's boardgames\n", vem);
                printf("      1) Print games\n");
                printf("      2) Add game\n");
                printf("      3) Search games\n");
                printf("      4) Remove game\n");
                printf("      5) Exit\n");
}

/**
 * @brief Adds a new game and rating to the specified user's list.
 * @param p_add Pointer to the user's struct who is adding the game.
 * @param userLista The main array of users.
 * @param antal_anvandare The total number of users.
 * @return 0 to exit the add-game loop, 1 to continue.
 */
int add_game(USERS *p_add, USERS userLista[],int antal_anvandare ){
  int spel_index;
  while(1){  
         if(p_add->nbOfspel>=ANTAL){
            printf(RED"Your game register is full."WHITE "\n");
            return 0;
         }
         spel_index=p_add->nbOfspel; // Index for the new game.
         printf("Register new boardgame (q to quit):");
         scanf("%s",p_add->USERSpel[spel_index].Spel_namn);
         
         if(strcmp(p_add->USERSpel[spel_index].Spel_namn,"q")==0){
            return 0; // Exit loop.
         }

         // Check if the game has already been added.
         for(int i=0; i< p_add->nbOfspel; i++){
           if(strcmp(p_add->USERSpel[spel_index].Spel_namn,p_add->USERSpel[i].Spel_namn)==0){
             printf("Boardgame already added. \n");
             return 1; // Continue loop.
           }
         }
         
         printf("\n");
         // Loop until a valid rating (1-10) is entered.
         int rating = 0;
         while(rating < 1 || rating > 10 ){
             printf("Ange Rating:");
             scanf("%d", &rating);
             // If rating is valid, store it and increment game count.
             if(rating >= 1 && rating <= 10 ){
                p_add->USERSpel[spel_index].Betyg = rating;
                p_add->nbOfspel = p_add->nbOfspel+1; 
                return 1; // Continue loop for another game.
              }
              printf(RED"Illegal value! "WHITE"\n");
         }
  }
}

/**
 * @brief Handles removing a game from a user's list.
 * @param remove Pointer to the user's struct.
 * @param indexOf_user The index of the user in the main array.
 * @param userLista The main array of users.
 * @return 0 to exit the remove-game loop.
 */
int Remove_Game(USERS *remove, int indexOf_user,USERS userLista[]){
  int spel_index=-1; // Will hold the index of the game to remove.
  char gameToremove[NAMESIZE];
  int signal=0; // Will hold the number of matches from the search.
  char y_or_n[2];

  while(1){  
         if(remove->nbOfspel==0){
            printf(RED"Your game register is empty."WHITE "\n");
            return 0;
         }
         printf("Search boardgame to remove (q to quit):");
         scanf("%s",gameToremove);

         if(strcmp(gameToremove,"q")==0){
            return 0;
         } else {
            // Search for the game. 'signal' gets the number of matches.
            signal=Search_Game(&userLista[indexOf_user],gameToremove,&spel_index);
            if(signal!=1){ // If not exactly one match was found.
               printf("You did not find one unique boardgame\n");
            } else if(signal==1){ // If exactly one match was found.
               printf("%s has index %d\n",remove->USERSpel[spel_index].Spel_namn, spel_index);
               printf("Do you want to remove this game (y/n):");
               scanf("%s",y_or_n);
               if(strcmp(y_or_n,"y")==0) {
                  delete_index_Of_game(&userLista[indexOf_user], spel_index);
               }
            }
         }
         printf("\n");
  }
}

/**
 * @brief Deletes a game from a user's game array by shifting elements.
 * @param delete Pointer to the user's struct.
 * @param game_index_to_remove The index of the game to be removed.
 */
void delete_index_Of_game(USERS *delete, int game_index_to_remove){
    for(int i=game_index_to_remove;i<(delete->nbOfspel)-1;i++){
        delete->USERSpel[i]=delete->USERSpel[i+1];
    }
    delete->nbOfspel=delete->nbOfspel-1; // Decrement the game count.
}


/**
 * @brief Searches a user's game list for a partial string match.
 * @param find Pointer to the user's struct to search within.
 * @param game The search string (substring).
 * @param index_of_game Pointer to an int that will store the index of the game if ONE match is found.
 * @return The number of games found that match the search string.
 */
int Search_Game(USERS *find, char game[],int *index_of_game){
    int j=0; // Counter for number of matches.
    int i;
    if(find->nbOfspel==0){
      printf(RED"No games registrered"WHITE "\n");
      return j;
    }else if(find->nbOfspel > 0){
       for(i=0; i<find->nbOfspel; i++){
         // Use strstr to find a substring match.
         if(strstr(find->USERSpel[i].Spel_namn,game )!=NULL){
            printf("%s   %d\n",find->USERSpel[i].Spel_namn,find->USERSpel[i].Betyg);
            j=j+1;
            // *index_of_game will hold the index of the unique match
            // if and only if (j==1), which is checked by the calling function.
            *index_of_game=i;
         }
       }
       if(j==0)
           printf(YELLOW"No matching games found"WHITE"\n");
      return j; // Return the number of matches.
   }  
   return 0;
}

/**
 * @brief Checks if a user has any rated games.
 * @param pointer Pointer to the user's struct.
 * @return 1 if the user has games, 0 otherwise.
 */
int Have_rated_game(USERS *pointer){
    if(pointer->nbOfspel==0){
      return 0;
    }else if(pointer->nbOfspel > 0 ){
      return 1;
    }
    return 0;
}

/**
 * @brief Checks if a user has any games, prints a message if not.
 * @param get_game Pointer to the user's struct.
 * @return 1 if games exist, 0 otherwise.
 */
int find_Game(USERS *get_game){
    if(get_game->nbOfspel==0){
      printf(RED"No games registrered"WHITE "\n");
      return 0;
    }else if(get_game->nbOfspel > 0){
      return 1;
    }
    return 0;
}


/**********************************************************************************/
// PRINTING FUNCTIONS
/**********************************************************************************/

/**
 * @brief Prints a list of all games for a specific user.
 * @param p1 Pointer to the user's struct.
 */
void print_games(USERS *p1){
  if(p1->nbOfspel==0){
    printf("No games regitrered!\n");
    return;
  }
  for(int i=0; i<p1->nbOfspel; i++){
    printf(" %s   %d\n",p1->USERSpel[i].Spel_namn,p1->USERSpel[i].Betyg);
  }
}

/**
 * @brief Prints all usernames alphabetically, along with their rated games.
 * Note: This function has a side effect of sorting an array on each call, which is inefficient.
 * It sorts the names up to the current user index `*nOfuser` on each iteration of the parent loop.
 * @param pointer Pointer to the current user struct in the loop.
 * @param nOfuser Pointer to the current loop index.
 * @param antal_anvandare The total number of users.
 * @param userLista The complete list of users.
 */
void print_usernames(USERS *pointer, int *nOfuser,int antal_anvandare,USERS userLista[]){
    // This function re-sorts a growing portion of the list with every call.
    // It's inefficient but works for the final printout.
    static char strang_USERnamn[NUMBER_OF_USERS][NAMESIZE];
    char strang_namn[NAMESIZE];
    
    // Copy the current username into the static array for sorting.
    strcpy(strang_USERnamn[*nOfuser], pointer->USERname);
    
    // Bubble sort the usernames collected so far.
    for(int i=0; i<*nOfuser+1 ; i++){
        for(int j=i+1; j<*nOfuser+1; j++){
            if(strcmp(strang_USERnamn[j], strang_USERnamn[i])<0){
                strcpy(strang_namn, strang_USERnamn[i]);
                strcpy(strang_USERnamn[i], strang_USERnamn[j]);
                strcpy(strang_USERnamn[j], strang_namn);
            }
        }
    }

    // This block only executes when the function has been called for the last user.
    if(*nOfuser == antal_anvandare - 1){
       for(int c=0; c < antal_anvandare; c++){
          printf(BLUE"%s" WHITE"\n", strang_USERnamn[c]);
          // Find the user's full data to print their games.
          int platc = index_of_user(userLista, strang_USERnamn[c], antal_anvandare);
          USERS *pp = &userLista[platc];
          
          if(pp->nbOfspel==0){
              printf("  No games registrered\n");
          }else if(pp->nbOfspel>0){
              // TODO: Games are not sorted alphabetically here.
              for(int i=0; i<pp->nbOfspel; i++)
                 printf("    %s  %d\n",pp->USERSpel[i].Spel_namn,
                                            pp->USERSpel[i].Betyg);
          }
       }
    }
}

/**
 * @brief Prints all usernames in alphabetical order, without their games.
 * Note: This function uses the same inefficient sorting method as print_usernames.
 * @param pointer Pointer to the current user struct in the loop.
 * @param nOfuser Pointer to the current loop index.
 * @param antal_anvandare The total number of users.
 * @param userLista The complete list of users.
 */
void print_usernames_without_games(USERS *pointer, int *nOfuser,int antal_anvandare,
                                                                     USERS userLista[]){
    static char strang_USERnamn[NUMBER_OF_USERS][NAMESIZE];
    char strang_namn[NAMESIZE];

    // Copy the current username into the static array.
    strcpy(strang_USERnamn[*nOfuser], pointer->USERname);
    
    // Bubble sort the collected usernames.
    for(int i=0; i<*nOfuser+1 ; i++){
        for(int j=i+1; j<*nOfuser+1; j++){
            if(strcmp(strang_USERnamn[j], strang_USERnamn[i])<0){
                strcpy(strang_namn, strang_USERnamn[i]);
                strcpy(strang_USERnamn[i], strang_USERnamn[j]);
                strcpy(strang_USERnamn[j], strang_namn);
            }
        }
    }
    
    // Only print the full sorted list on the last call.
    if(*nOfuser == antal_anvandare - 1){
       for(int c=0; c < antal_anvandare; c++){
          printf(BLUE" %s" WHITE"\n", strang_USERnamn[c]);
       }
    }
}


/**
 * @brief Prints the details of a single user (name, games, game count).
 * Used for debugging/initial data load confirmation.
 * @param pp Pointer to the USERS struct to print.
 */
void print_UserLista(USERS *pp){
   int i=0;
   printf(BLUE "%s" WHITE "\n", pp->USERname);
   if(pp->nbOfspel==0){
      printf("No games registrered\n");
   }else if(pp->nbOfspel>0){
      for(; i<pp->nbOfspel; i++)
          printf(" %s  %d\n",pp->USERSpel[i].Spel_namn,
                                    pp->USERSpel[i].Betyg);
   }
   //printf("number of spel: %d\n",pp->nbOfspel); // Debug print                               
}