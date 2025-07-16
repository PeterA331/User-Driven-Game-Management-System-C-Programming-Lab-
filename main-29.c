/*Test Code: försöker läsa från textfil och lagra data i structars-medlemmar..
..1- Nu kan koden läsa från fil och lagra inlästa datan i structar-medlemmar..
..2- Koden har en funktion som läser från filen(Read_From_File), det finns bland annat ett litet..
.. test i main-funktion..
..3-koden nu checkar om användaren finns i filen eller ej..
..4- Försöker att lägga till ett spel till användarens-spellista..(lyckats, men det behövs att kontrollera..
.. att betyget är mellan (1-10))..
..5- Lyckats skriva till Ratin2.txt efter att användare har lagt till ett spel
..6- Försöker implementera Start och startmenyn sidan:2,3 i lab-instruktioner(Lyckats)..
..7- Programmet kan tilläga en ny användare(user) via aminstratör..
..8- Försöker tillämpa adminstratör möjligheter(print,add user, remove user..), (pågår)
..8-a-Lyckats kunna lägga till en nyanvändare(user)-lyckats
..8-b-försöker ta bort(Remove) användar(User)-lyckats 

..9-a- implementera användaren lista,(add game ), (lyckats)..
..10- fixar Read fom file funktion (lyckats).. + implementera Användar-meny, (Remove game), (lyckats)
..11- sortera usernames i bokstavordning(lyckats)..
..12- print ut user-Lista med spel i bokstavordning efter usernames,(pågår)..*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define NAMESIZE 40+1
#define ANTAL 5
#define NUMBER_OF_USERS 8
#define LIBSIZE 100

#define RED    "\e[31m"            // Error messages
#define GREEN  "\e[32m"            // User mode
#define YELLOW "\e[33m"            // Information messages
#define BLUE   "\e[34m"            // Adm mode
#define PURPLE "\e[35m"            // Debug messages
#define CYAN   "\e[36m"            // Reserv
#define GREY   "\e[37m"            // Reserv
#define WHITE  "\e[0m"             // Reset to white

typedef struct {
    char Spel_namn[NAMESIZE];
    int  Betyg;//mellan 1-10
} Brad_Spel;

typedef struct {
   char USERname[NAMESIZE]; //40+1
   Brad_Spel USERSpel[ANTAL];//20 spel maximalt, den häe är array av struct knuten med Brad_Spel struct
   int     nbOfspel;
} USERS;

//
void fskip_line(FILE *f) {
  int c;
  while ( (c = fgetc(f)) != '\n' && c != EOF )
    ;  
}
 
void skip_line(void) {
  fskip_line(stdin); 
}

void skip_line_YorN(void) {
  int i;
  while ( (i = getchar()) != '\n' && i != EOF )
    ;  
}

_Bool remove_nl(char *a) {
  int i = strlen(a)-1;
  if (a[i] == '\n') {
    a[i] = '\0';
	return 1;
  }
  return 0;
}

 _Bool fread_line(char *a, int n, FILE *f) {
  if (fgets(a, n, f) == NULL)
    return 0;
  if (!remove_nl(a)) 
	fskip_line(f);
  return 1;  
}
//


int isOnlyletters(char s[]);

void Read_From_File(USERS userLista[], int *index, FILE *f);

FILE *open_file(char NameOfFile[],const char *mode);

FILE *Write_Over(char NameOfFile[],USERS userLista[],int antal_anvandare );

int Enter_controll(USERS Alla_anvandare[],char Vem[], int antal_anvandare);

void print_UserLista(USERS *pp);

void trim(char *to, const char *from);

int index_of_user(USERS userLista[], char Vem[], int antal_anvandare);

int add_game(USERS *p_add, USERS userLista[],int antal_anvandare );

void Writing_To_File( USERS *pp, FILE *f_write );

void AdminstrationLista(void);

//void add_NewUser(USERS *p_addUser, int *nummberOfusers );

//void Print_UsersNames(USERS *pp);

void print_usernames(USERS *pointer, int *nOfuser, int antal_anvandare,USERS userLista[]);

void print_usernames_without_games(USERS *pointer, int *nOfuser,int antal_anvandare,
                                                                     USERS userLista[]);
int check_inmatning(int *nummberOfusers );

USERS add_NewUser_struct(int *nummberOfusers,int *stop,USERS userLista[] );

int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[]);

int Remove_User(USERS Alla_anvandare[], int *antal_anvandare);

int Have_rated_game(USERS *pointer);

void moveIndex(USERS userLista[], int *antal_anvandare, int indexOfuser);

void user_Meny(char vem[]);//Användare-meny

int Search_Game(USERS *find, char game[],int *index_of_game);

int Remove_Game(USERS *remove, int indexOf_user,USERS userLista[]);

void delete_index_Of_game(USERS *delete, int game_index_to_remove);

int find_Game(USERS *get_game);

void print_games(USERS *p1);
/**********************************************************************************/

int main(void){
   int antal_anvandare=0;
   int antal_spel[ANTAL];
   USERS userLista[30];//array av struct av type USERS(min egen variabel)
   char NameOfFile[100];//Filen vi ska skriva till när vi avslutar programmet
   char FileName[100];
   char fileToopen[]="R.txt";
   int stop=1;
   int count_users;//för att skriva ut använadrensnamn i bokstavordning
   int all_users;
   //Variabels som gäller Användar-meny..........
   int stop_add_game;
   int indexOfanvandare;
   int anvandare_Val;
   char out_of_addGame[2];
   char game_to_search[NAMESIZE];
   int gameIndex=-1;
   //............................................
   
   USERS *pp;
   char Vem[NAMESIZE]; //För att undersöka ditt val(användare, admin eller q).
   //Enter you choice
   printf("Welcome to boardgame ratings.\n");
   printf("Which file do you want to use:");
   scanf("%s", NameOfFile);
   //Test....................
   FILE *infil=open_file(fileToopen , "r");//Rating2.txt
   //........................
   Read_From_File(userLista, &antal_anvandare, infil);/*Reaf from file an svae in array-struct*/
   
   int check=0;
   int UserVal=0;
   printf("\n");
   do{
    printf("Please enter user name, admin or quit:\n");
    scanf("%s", Vem);
    check=Enter_controll(userLista,Vem, antal_anvandare);
    if(strcmp(Vem,"admin")==0){
        //AdminstrationLista();
        do{
           AdminstrationLista(); 
           printf("Choose:");
           scanf("%d",&UserVal); 
           switch(UserVal){
              case 1:
                do{
                   all_users=antal_anvandare;
                   userLista[all_users++]=add_NewUser_struct(&antal_anvandare,&stop
                                                                 ,userLista);
                }while(stop!=0);
                    //add_NewUser(&userLista[antal_anvandare],&antal_anvandare);
                    break;
              case 2:printf("Antal_anvandare: %d\n", antal_anvandare);
                     //int Remove_User(USERS Alla_anvandare[], int *antal_anvandare);
                     do{
                     }while(Remove_User(userLista ,&antal_anvandare)!=-2);
                     break; 
              case 3:

                       if(antal_anvandare==0){
                        printf(RED"No users registrered" WHITE"\n");
                        break;
                     }else{//När antal användare är lika med 0, då körs for-loopen Inte. 
                        for(int c=0; c<antal_anvandare;c++){
                            print_usernames_without_games(&userLista[c],&c,
                                                   antal_anvandare,userLista);
                        }
                     }                                                                   
                    break;             
              case 4: if(antal_anvandare==0){
                        printf(RED"No users registrered" WHITE"\n");
                        break;
                     }else{//När antal användare är lika med 0, då körs for-loopen Inte. 
                        for(count_users=0; count_users<antal_anvandare; count_users++){
                        print_usernames(&userLista[count_users],&count_users,
                                                 antal_anvandare,userLista); 
                        }
                     }            
                    break;   
              case 5:if(UserVal==5) break;
              default:if(UserVal<1 || UserVal>5){
                         UserVal=5;
                      } 
           } 
          
        }while(UserVal!=5);
       
    }else if(strcmp(Vem,"quit")==0){
        char string1[]="Users and boardgames:";
        char string2[]="________________________________________";
        int j=0;
        strcpy(FileName,NameOfFile);
        FILE *utfil=open_file(FileName,"w");
        fprintf(utfil,"%s\n",string1);
        fprintf(utfil,"%s\n",string2);
        for(int i=0; i<antal_anvandare; i++){
      
            Writing_To_File( &userLista[i], utfil );
      
        }
        fclose(utfil);
        printf("Saving all data to %s\n",FileName); 
        printf("Goodbye! \n");
        break;
        /*______________________________________________________________________*/
        /*''''''''''''''''''''-Anvandar-meny-**************************************/
    }else{
       if(check!=0){
          //indexOfanvandare = index_of_user(userLista, Vem, antal_anvandare);//plocka upp användarens index  
        do{
            indexOfanvandare = index_of_user(userLista, Vem, antal_anvandare);//plocka upp användarens index  
            printf("**indexOfanvandare:%d\n",indexOfanvandare);
            user_Meny(Vem);
            printf("Choose:");
            scanf("%d",&anvandare_Val); 
            switch(anvandare_Val){
                case 1:
                      //print_games(USERS *p1);
                      print_games(&userLista[indexOfanvandare]);
                      break;
                case 2:
                        while(1){
                           if(!add_game(&userLista[indexOfanvandare],userLista,antal_anvandare))
                           break;  
                        }  
                      break;
                case 3:
                      while(1){
                        if(!find_Game(&userLista[indexOfanvandare]))//Användaren har ingen spel
                           break;  
                        printf("Search (q to quit):");
                        scanf("%s",game_to_search);
                        if(strcmp(game_to_search,"q")==0)
                           break;
                        if(!Search_Game(&userLista[indexOfanvandare],game_to_search,&gameIndex))
                           break;
                      }
                      break;
                case 4:
                      Remove_Game(&userLista[indexOfanvandare], indexOfanvandare, userLista);
                      break;
                case 5:
                      break; 
                default:if(anvandare_Val <1 || anvandare_Val >5){
                            anvandare_Val=5;
                        }                        
            }
        printf("indexOfanvandare is :" YELLOW "%d" WHITE "\n", indexOfanvandare);
           
         
        
        printf("antal_anvandare:"CYAN "%d"WHITE "\n", antal_anvandare);
      }while(anvandare_Val!=5);
     }
    }
   }while(strcmp(Vem,"quit")!=0);
   return 0;
}

/*************************************************/
//Funktion som kontrollerar om den som vill logga in är user(användare)..
//med andra ord (Please enter user name, admin or quit:)
//USERS Alla_anvandare[]= USERS userLista
int Enter_controll(USERS Alla_anvandare[],char Vem[], int antal_anvandare){
    if(antal_anvandare==0){
        printf("There are no Users\n");
        return 0;
    }else{
        //Leta efter användaren, om användaren finns, returnera 1
        for(int i=0; i<antal_anvandare; i++){
            if(strcmp(Alla_anvandare[i].USERname,Vem)==0){
                return 1;
            }
        }
       
    }
    //Efter att for-loop har löpt utan att hitta någon användare, return 0;
    printf("User does not exist\n");  
    return 0; 
}
/************************************/
//Funktion som kontrollerar om inlästa data från filen innehåller siffror eller inte.
int isOnlyletters(char s[]){
  for(int i=0; i<strlen(s); i++){
      if(isdigit(s[i]))
       return 0;
  }
  return 1;
}
/************************/
//Funktion som läser data från filen och lagrar dem i array av structar
// *index: räknar upp antal användare.
void Read_From_File(USERS userLista[], int *index, FILE *f){
  
  USERS *pekare;
  char s[150];
  char copy[150];
  int uppdela=0;
  int StructIndex=0;
  int j=0;//gäller första två rader i filen
  while (fread_line(s, 150,f)){
    trim(copy,s);
    if(j<2){ //gäller första två rader i filen
      printf("%s\n",copy);
      j++;
    }else{
     
        if(strstr(copy,"No games registrered")==NULL){
             //Användarens namn
             if(isOnlyletters(copy)){
               pekare=&userLista[*index];
               strcpy(pekare->USERname, copy);
               pekare->nbOfspel=0;
               uppdela=*index;
               *index=*index+1;
               StructIndex=0;
             //Användarens spel
            }else if(!isOnlyletters(copy)){
                pekare=&userLista[uppdela];
                sscanf(copy, "%s %1d", pekare->USERSpel[StructIndex].Spel_namn, 
                                          &pekare->USERSpel[StructIndex].Betyg); 
                StructIndex=StructIndex+1;
                pekare->nbOfspel=pekare->nbOfspel+1;
                
           }
          }else if(strstr(copy,"No games registrered")!=NULL){
            pekare=&userLista[uppdela];
            pekare->nbOfspel=0;
          }
        }
        
     }
     *index=*index-1;
    //fclose(f);/-----------<------------<-------------<-------------------<--------------?/
    for(int i=0; i<*index; i++){
      print_UserLista(&userLista[i]);
    }
   printf("Antal_anvandare is: %d\n", *index);
}
/***********************************/
//Funktion som skriver ut data som är lagrade i array av struct.
//pp=&userLista[i];
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
   printf("number of spel: %d\n",pp->nbOfspel);                                
}
/*********************************************************/
//Oppen File , and determine mode("r", "w", "a"...)
FILE *open_file(char NameOfFile[],const char *mode){
  FILE *f=fopen(NameOfFile, mode);
  if(f==NULL){
    printf("Cann't open the file! ");
    exit(1);
  }
  return f;
}
/****************************/
//Ta bort onödiga vita tecken i början och slutet av strängen(C från början bok)
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
/*********************************************/
//Bara kolla sen ta bort
/*USERS KOLLA_add_NewUser_struct(int *nummberOfusers,int *stop,USERS userLista[]){
       char NewUser[NAMESIZE];
       USERS nyAnvandare;
       while(strcmp(NewUser,"q")!=0){
            //int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[])
            printf("Register new user (q to quit):");
            scanf("%s", NewUser);
            
            if(strcmp(NewUser,"q")==0){
               *stop=0;
               break;
            }
              
            if(!Check_User(userLista, nummberOfusers,NewUser)){
                printf("User name exist! Please choose another.");
                *stop=1;
                break;
            }
            
            if(*nummberOfusers >= NUMBER_OF_USERS ){ 
              printf("The register is full! \n");
              //*stop=0;
              //break;
            }
            
            if(*nummberOfusers < NUMBER_OF_USERS){
               strcpy(nyAnvandare.USERname, NewUser);
               *nummberOfusers=*nummberOfusers+1;
               nyAnvandare.nbOfspel=0;
               *stop=1;
            return nyAnvandare;
            }
       }
    
}*/
/****************************-Användar-meny-************************************************/
//*p_add här pekar redan på rätt array struct:s index, d.v.s p_add=&userLista[UserIndex].
int add_game(USERS *p_add, USERS userLista[],int antal_anvandare ){
  int spel_index;
  while(1){  
    
         if(p_add->nbOfspel>=ANTAL){
        //RED "Error!" WHITE "\n"  
            printf(RED"Your game register is full."WHITE "\n");
            return 0;
         }
         spel_index=p_add->nbOfspel;
         printf("Register new boardgame (q to quit):");
         
         scanf("%s",p_add->USERSpel[spel_index].Spel_namn);
         
         for(int i=0; i< p_add->nbOfspel; i++){
           if(strcmp(p_add->USERSpel[spel_index].Spel_namn,p_add->USERSpel[i].Spel_namn)==0){
             printf("Boardgame already added. \n");
             return 1;
           }
         }
         
         if(strcmp(p_add->USERSpel[spel_index].Spel_namn,"q")==0){
            return 0;
         }
         printf("\n");
         //kör om så länge Betyg inte mellan 1 och 10
         while(p_add->USERSpel[spel_index].Betyg <1 ||p_add->USERSpel[spel_index].Betyg >10 ){
             printf("Ange Rating:");
             scanf("%d", &p_add->USERSpel[spel_index].Betyg);
             //Om betyget är mellan 1 och 10
             if(p_add->USERSpel[spel_index].Betyg>=1 && p_add->USERSpel[spel_index].Betyg<=10 ){
                p_add->nbOfspel= p_add->nbOfspel+1; //Öka antal spel på det struct-array *p_add pekar på.
                return 1;
              }
              printf(RED"Illegal value! "WHITE"\n");
         //*p_add här pekar redan på rätt array struct:s index, d.v.s p_add=&userLista[UserIndex].
         
         }
  }
}
/*****************************************************/
//Funktion som returnerar Användarens_index
int index_of_user(USERS userLista[], char Vem[], int antal_anvandare){
   int UserIndex;
   for(int i=0; i<antal_anvandare; i++){
            if(strcmp(userLista[i].USERname,Vem)==0){
                UserIndex=i;
                return UserIndex;
                break;
            }
        } 
    return -1;  
}
/************************************************/
//Funktion som skriver till fil
void Writing_To_File( USERS *pp, FILE *f_write ){
  
  char string3[]="No games registrered";
  int i=0;
  //fprintf(file, YELLOW "%s" WHITE "\n", s);
  fprintf( f_write,"%s\n", pp->USERname);
  if(pp->nbOfspel==0){
    fprintf(f_write,"%s\n",string3);
  }else{
    for(; i<pp->nbOfspel; i++)
        fprintf( f_write," %s  %d\n",pp->USERSpel[i].Spel_namn,
                                    pp->USERSpel[i].Betyg);
  }
}
//
void AdminstrationLista(void){
    printf("Adminstration\n");
    printf("      1)Add user\n");
    printf("      2) Remove user \n");
    printf("      3) Print all users\n"); 
    printf("      4) Print all users and all their ratings \n");
    printf("      5) Exit\n");
}
/**************************************/
//*stop används för att kunna anropa add_NewUser_struct funktion så länge 'q' inte inmatad.
USERS add_NewUser_struct(int *nummberOfusers,int *stop,USERS userLista[]){
       char NewUser[NAMESIZE];
       USERS nyAnvandare;
       while(strcmp(NewUser,"q")!=0){
            //int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[])
            printf("Register new user (q to quit):");
            scanf("%s", NewUser);
            
            if(strcmp(NewUser,"q")==0){
               *stop=0;
               break;
            }
              
            if(!Check_User(userLista, nummberOfusers,NewUser)){
                printf("User name exist! Please choose another.");
                *stop=1;
                break;
            }
            
            
            if(*nummberOfusers >= NUMBER_OF_USERS ){ 
              printf("The register is full! \n");
              //*stop=0;
              //break;
            }
            if(*nummberOfusers < NUMBER_OF_USERS){
               strcpy(nyAnvandare.USERname, NewUser);
               *nummberOfusers=*nummberOfusers+1;
               nyAnvandare.nbOfspel=0;
               *stop=1;
            return nyAnvandare;
            }
       }
}
/************************************************/
//Funktion för att kolla om användare redan finns eller inte
int Check_User(USERS userLista[], int *nummberOfusers,char NewUser[]){
    for(int i=0; i<*nummberOfusers; i++){
        if(strcmp(userLista[i].USERname, NewUser)==0){
            
            return 0;
        }
    }
    //Efter att for-loop har löpt utan att see användare-->return 1..
    //..det betyder att användaren inte redan finns.
    return 1;
}
/************************************************/
int check_inmatning(int *nummberOfusers ){
    
    char NewUser[NAMESIZE];
    char out[]="q";
    do{

      printf("Register new user (q to quit):");
      scanf("%s", NewUser);  
      if(*nummberOfusers >= NUMBER_OF_USERS){
        printf("The register is full! \n");
        return 0;
      }else if(strcmp(NewUser,out)==0){
        return 0;

      }else{
        
          return 1; 
        
      }
    }while(strcmp(NewUser,out)!=0);
    
}
/*****************************************/
void Print_UsersNames(USERS *pp){
  printf("%s\n", pp->USERname);
}
/**************************************--Sotera--*******************************************/
/*void sortera_fornamn(struct personer *pp, int *count){
    char str1[10][41];
    char strang_namn[40];
    char strang_FORnamn[10][41];
    char strang_EFTERnamn[10][41];
    int i;
    strcpy(strang_FORnamn[*count], pp->fornamn);
    //strcat(str1[*count], " ");
    strcpy(strang_EFTERnamn[*count], pp->efternamn);
    if(*count>=1){
      for(i=0; i<*count+1 ; i++){
        for(int j=i+1; j<*count+1; j++){
          if(strcmp(strang_EFTERnamn[i], strang_EFTERnamn[j])>0){
            strcpy(strang_namn, strang_EFTERnamn[i]);//--
            strcat(strang_namn, " ");
            strcat(strang_namn, strang_FORnamn[i]);
            
            strcpy(strang_EFTERnamn[i], strang_EFTERnamn[j]);//--
            strcat(strang_EFTERnamn[i], " ");
            strcat(strang_EFTERnamn[i], strang_FORnamn[j]);

            strcpy(strang_EFTERnamn[j], strang_namn);//--

         }
        }
      }
    }
    if(i==*count+1){
        for(int i=0; i<*count+1; i++)
          printf(">> %s\n", strang_EFTERnamn[i]);
        printf("%d\n",*count);

    }
    
}*/
/**********************************-----Sotera------*******************************************/
//skriv-ut med spel
void print_usernames(USERS *pointer, int *nOfuser,int antal_anvandare,USERS userLista[]){

    char str1[NUMBER_OF_USERS][NAMESIZE];
    char strang_namn[NAMESIZE];
    char strang_USERnamn[NUMBER_OF_USERS][NAMESIZE];
    strcpy(strang_USERnamn[*nOfuser], pointer->USERname);
    int i;
    if(*nOfuser>=1){
      for(i=0; i<*nOfuser+1 ; i++){
        for(int j=i+1; j<*nOfuser+1; j++){
          if(strcmp(strang_USERnamn[j], strang_USERnamn[i])<0){
            strcpy(strang_namn, strang_USERnamn[i]);//--
            strcpy(strang_USERnamn[i], strang_USERnamn[j]);//--
            strcpy(strang_USERnamn[j], strang_namn);//--

         }
        }
      }
    }
    int platc;
    int c;
    char name_for_user[NAMESIZE];
    USERS *pp;
    //int index_of_user(USERS userLista[], char Vem[], int antal_anvandare)
    if(i==antal_anvandare){
       for(c=0; c<*nOfuser+1; c++){
        //printf(BLUE "%s" WHITE "\n", pp->USERname);
          printf(BLUE"%s" WHITE"\n", strang_USERnamn[c]);
          strcpy(name_for_user,strang_USERnamn[c]);
          platc=index_of_user(userLista, name_for_user, antal_anvandare);
          pp=&userLista[platc];
          if(pp->nbOfspel==0){
              printf("No games registrered\n");
          }else if(pp->nbOfspel>0){
          for(int i=0; i<pp->nbOfspel; i++)
             printf("    %s  %d\n",pp->USERSpel[i].Spel_namn,
                                        pp->USERSpel[i].Betyg);
          }
       }
    }
}
/****************************---SORTERA-UTAN-SPEL---***************/
void print_usernames_without_games(USERS *pointer, int *nOfuser,int antal_anvandare,
                                                                     USERS userLista[]){

    char str1[NUMBER_OF_USERS][NAMESIZE];
    char strang_namn[NAMESIZE];
    char strang_USERnamn[NUMBER_OF_USERS][NAMESIZE];
    strcpy(strang_USERnamn[*nOfuser], pointer->USERname);
    int i;
    if(*nOfuser>=1){
      for(i=0; i<*nOfuser+1 ; i++){
        for(int j=i+1; j<*nOfuser+1; j++){
          if(strcmp(strang_USERnamn[j], strang_USERnamn[i])<0){
            strcpy(strang_namn, strang_USERnamn[i]);//--
            strcpy(strang_USERnamn[i], strang_USERnamn[j]);//--
            strcpy(strang_USERnamn[j], strang_namn);//--

         }
        }
      }
    }
    
    int c;
    
    
    //int index_of_user(USERS userLista[], char Vem[], int antal_anvandare)
    if(i==antal_anvandare){
       for(c=0; c<*nOfuser+1; c++){
          printf(BLUE" %s" WHITE"\n", strang_USERnamn[c]);
          
       }
    }
}
//Funktion att sortera och skriva ut användarens-namn med spel i bokstavordning
//    *pointer=&userLista[i];
/*void print_UserLista(USERS *pp){
   int i=0;
   printf(BLUE "%s" WHITE "\n", pp->USERname);
   if(pp->nbOfspel==0){
      printf("No games registrered\n");
   }else if(pp->nbOfspel>0){
      for(; i<pp->nbOfspel; i++)
          printf(" %s  %d\n",pp->USERSpel[i].Spel_namn,
                                    pp->USERSpel[i].Betyg);
   }
   printf("number of spel: %d\n",pp->nbOfspel);                                
}*/
void TEST_print_usernames(USERS *pointer, int *nOfuser,int antal_anvandare){
    
    char str1[NUMBER_OF_USERS][NAMESIZE];
    char strang_namn[NAMESIZE];
    char strang_USERnamn[NUMBER_OF_USERS][NAMESIZE];
    strcpy(strang_USERnamn[*nOfuser], pointer->USERname);
    int i;
    if(*nOfuser>=1){
      for(i=0; i<*nOfuser+1 ; i++){
        for(int j=i+1; j<*nOfuser+1; j++){
          if(strcmp(strang_USERnamn[j], strang_USERnamn[i])<0){
            strcpy(strang_namn, strang_USERnamn[i]);//--
            strcpy(strang_USERnamn[i], strang_USERnamn[j]);//--
            strcpy(strang_USERnamn[j], strang_namn);//--
            
         }
        }
      }
    }
    if(i==antal_anvandare){
       for(int c=0; c<*nOfuser+1; c++)
          printf(">> %s\n", strang_USERnamn[c]);
    }
}
/***********************************************************************************/
//Glöm inte att deklarera funktioner
//parametern: (Alla_anvandare) är copy by value till (USERS userLista[30];)
//int index_of_user(USERS userLista[], char Vem[], int antal_anvandare):return UserIdex or -1
int Remove_User(USERS Alla_anvandare[], int *antal_anvandare){
   
    char nameTOremove[NAMESIZE];
    char answer[2];
    int indexOfuser;
    int nummberOfusers=*antal_anvandare;   
    while(strcmp(nameTOremove,"q")!=0){
        printf("Remove user (q to quit):\n");
        scanf("%s", nameTOremove);
        if(strcmp(nameTOremove,"q")==0){
            return -2;
        }
        //Om användare att tas bort int finns
        if(index_of_user(Alla_anvandare, nameTOremove, nummberOfusers)==-1){
            printf("User do not exist! Please choose another. \n");
            return -1;
        }else{ //om användaren finns
           indexOfuser=index_of_user(Alla_anvandare, nameTOremove, nummberOfusers);//take indexet
           printf("indexOfuser to remove:%d, his name is: %s\n", indexOfuser,nameTOremove);
           
        }
        //Funktionen går int in här om indexOfuser < 0
        if(indexOfuser >= 0){
           if(!Have_rated_game(&Alla_anvandare[indexOfuser])){
              //Remove utan att varna
              //printf("Removed!\n");
              //test....................
              printf(GREEN "User has NO rated game" WHITE "\n");
              moveIndex(Alla_anvandare, antal_anvandare, indexOfuser);
              printf(GREEN "Removed" WHITE "\n");
              //.......................
           }else{
              //Varning att user har spel...y or n..,y=remove, n=don't remove
               printf(RED"Warning: User has rated games. " WHITE "\n");
               printf("Do you still want to remove Ella (y/n)?");
               scanf("%s",answer);
               if(strcmp(answer,"y")==0){
                 moveIndex(Alla_anvandare, antal_anvandare, indexOfuser);
                 printf(GREEN "Removed" WHITE "\n");
                 return -1;
               }else if(strcmp(answer,"n")==0)
                 return -1;
           }
        }
    }
          
}
/********************************************************/
//Funktion för att se om användaren har spel
int Have_rated_game(USERS *pointer){
    if(pointer->nbOfspel==0){
      return 0;
    }else if(pointer->nbOfspel >0 ){
      return 1;
    }
}
/**********************************************************/
//Funktion för att flytta upp userLista-array så att inte skapas luckor
void moveIndex(USERS userLista[], int *antal_anvandare, int indexOfuser){
    for(int i=indexOfuser;i<(*antal_anvandare)-1;i++){
        userLista[i]=userLista[i+1];
    }
    (*antal_anvandare)--;
}

void user_Meny(char vem[]){
  printf("%s's boardgames\n", vem);
                printf("      1) Print games\n");
                printf("      2) Add game\n");
                printf("      3) Search games\n");
                printf("      4) Remove game\n");
                printf("      5) Exit\n");
}
/********************-Användare-meny-**********************************/
//Funktion för att ta bort game via användare
int Remove_Game(USERS *remove, int indexOf_user,USERS userLista[]){
  int spel_index=-1;
  char gameToremove[NAMESIZE];
  int signal=0;
  char y_or_n[2];
  while(1){  
    
         if(remove->nbOfspel==0){
        //RED "Error!" WHITE "\n"  
            printf(RED"Your game register is empty."WHITE "\n");
            return 0;
         }
         printf("Search boardgame to remove (q to quit):");
         
         scanf("%s",gameToremove);
         if(strcmp(gameToremove,"q")==0){
            return 0;
         }else{
            //--söka spel och kolla om du får ett unikt träff eller inte,signal tar det returnerande värdet.
            signal=Search_Game(&userLista[indexOf_user],gameToremove,&spel_index);
            if(signal!=1||signal==0){
               printf("You did not find one unique boardgame\n");
            //Ett unikt träff hittades signal är endast signal=1.
            }else if(signal==1){
               
               printf("%s has index %d\n",remove->USERSpel[spel_index].Spel_namn, spel_index);
               printf("Do you want to remove this game (y/n):");
               //delete_index_Of_game(USERS *delete, int game_index_to_remove)
               
               scanf("%s",y_or_n);
               if(strcmp(y_or_n,"y")==0)
                  delete_index_Of_game(&userLista[indexOf_user], spel_index);
            }
         }
         printf("\n");
  }
}

void delete_index_Of_game(USERS *delete, int game_index_to_remove){
    for(int i=game_index_to_remove;i<(delete->nbOfspel)-1;i++){
        delete->USERSpel[i]=delete->USERSpel[i+1];
    }
    delete->nbOfspel=delete->nbOfspel-1;
}
/********************-Användare-meny-**********************************/
//Funktion för att söka spel, den returnerar antal hittade spel via en integer(j)
int Search_Game(USERS *find, char game[],int *index_of_game){
    int j=0;
    int i;
    if(find->nbOfspel==0){
      //printf(RED"Warning: User has rated games. " WHITE "\n");
      printf(RED"No games registrered"WHITE "\n");
      return j;
    }else if(find->nbOfspel > 0){
       for(i=0; i<find->nbOfspel; i++){
         if(strstr(find->USERSpel[i].Spel_namn,game )!=NULL){
            printf("%s   %d\n",find->USERSpel[i].Spel_namn,find->USERSpel[i].Betyg);
            j=j+1;
            *index_of_game=i;//*index_of_game kommer att har indexet av det unika träffet..
                             //..om och endast om (j==1), vilket reflekteras ovan på signal.
         }
       }
       if(j==0)
           printf(YELLOW"No games registrered"WHITE"\n");
      return j; //antal spel som fick träff efter att for-loop har sökt spel
   }  
}
/*********************************************************/
//Funktion för att checka om det finns några spel, eller är det spel-list är tom
int find_Game(USERS *get_game){
    
    if(get_game->nbOfspel==0){
      //printf(RED"Warning: User has rated games. " WHITE "\n");
      printf(RED"No games registrered"WHITE "\n");
      return 0;
    }else if(get_game->nbOfspel > 0){
      return 1;
    }
}
/**********************************************************/
//Funktion för att print ut game
void print_games(USERS *p1){
  if(p1->nbOfspel==0){
    printf("No games regitrered!\n");
    return;
  }
  for(int i=0; i<p1->nbOfspel; i++){
    printf(" %s   %d\n",p1->USERSpel[i].Spel_namn,p1->USERSpel[i].Betyg);
  }
}