// NAME: Simeon Dimitrov
// CLASS: XIb
// NUMBER: 23
// PROBLEM: #1
// FILE NAME: ls.c
// FILE PURPOSE: Realisation of the terminal command "ls"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
// Globalni promenlivi za flagovete
int R = 0;
int A = 0;
int l = 0;
//-----------------------------------------
// FUNCTION: typeF
// Proverqva tipa na faila i go printira
// PARAMETERS: st - informaciq za faila
//-----------------------------------------
int typeF(struct stat *st){
    switch (st->st_mode & S_IFMT) {
        case S_IFDIR:  printf("d "); return 1; // proverka za direktoriq
        case S_IFREG:  printf("- "); return 2; // proverka za fail
        case S_IFLNK:  printf("l "); return 3; // proverka za link
        case S_IFBLK:  printf("b "); return 4; // proverka za blok
        case S_IFIFO:  printf("p "); return 5; // proverka za pipe
        case S_IFSOCK: printf("s "); return 6; // proverka za socket
        case S_IFCHR:  printf("c "); return 7; // proverka za simvol
        default:       printf("? "); return 8; // defaultna proverka ako neshto se precaka
    }
}
//-------------------------------------------------------------
// FUNCTION: details
// Priema informaciq za fail i negovoto ime i printira 
// suotvetnite danni za nego pri flag -l
// PARAMETERS: name - ime na faila, stat - informaciq za faila
//-------------------------------------------------------------
void details(char* name,struct stat *stat){
    struct passwd *user = getpwuid(stat->st_uid); // vzimame user-a
    struct group *group = getgrgid(stat->st_gid); // vzimame grupata
    char* permisions = malloc(10 * sizeof(char)); // zadelq pamet za permision-i
	// Setvane na masiva ot permision-i za suotvetniq fail
    if((S_ISDIR(stat->st_mode))){
        permisions[0] = 'd';
    }else{
        permisions[0] = '-';
    } 
    if((stat->st_mode & S_IRUSR)){
        permisions[1] = 'r';
    }else{
        permisions[1] = '-';
    } 
    if((stat->st_mode & S_IWUSR)){
        permisions[2] = 'w';
    }else{
        permisions[2] = '-';
    } 
    if((stat->st_mode & S_IXUSR)){
        permisions[3] = 'x';
    }else{
        permisions[3] = '-';
    } 
    if((stat->st_mode & S_IRGRP)){
        permisions[4] = 'r';
    }else{
        permisions[4] = '-';
    } 
    if((stat->st_mode & S_IWGRP)){
        permisions[5] = 'w';
    }else{
        permisions[5] = '-';
    } 
    if((stat->st_mode & S_IXGRP)){
        permisions[6] = 'x';
    }else{
        permisions[6] = '-';
    } 
    if((stat->st_mode & S_IROTH)){
        permisions[7] = 'r';
    }else{
        permisions[7] = '-';
    } 
    if((stat->st_mode & S_IWOTH)){
        permisions[8] = 'w';
    }else{
        permisions[8] = '-';
    } 
    if((stat->st_mode & S_IXOTH)){
        permisions[9] = 'x';
    }else{
        permisions[9] = '-';
    } 
    permisions[10] = '\0'; // slaga krai na masiva
    //printirane na dannite za faila:
	printf("%s ",permisions);
    free(permisions);
    printf("%ld ",stat->st_nlink);
    printf("%s ",user->pw_name);
    printf("%s ",group->gr_name);
    printf("%ld ",stat->st_size);
    char buff[100];
	// vzimane i printirane na vremeto
    struct tm * i = localtime (&(stat->st_mtime)); 
    strftime(buff, 100, "%b %d %H:%M", i);
    printf("%s ",buff);
    printf("%s\n",name);
}
//------------------------------
// FUNCTION: connect
// suedinqva dva stringa
// PARAMETERS: p - put, n - ime
//------------------------------
char * connect(char* p, char* n){
    int fullsize = strlen(p) + strlen(n); // suedinqvame goleminata dvata stringa obshto
    char* filepath = malloc(fullsize*sizeof(char)); // zadelqme pamet za dvata stringa
    int i;
    for(i = 0;i<strlen(p);i++){
        filepath[i] = p[i];
    } // kopirame purviq string
    int g = 0;
    int j;
    for(j = i;g < strlen(n);j++,g++){
        filepath[j] = n[g];
    } // kopirame vtoriq string
    filepath[j] = '\0';
    return filepath;
}
//---------------------------------------------------
// FUNCTION: filepath
// Suedinqvame dva stringa kato dobavq mejdu tqh "/"
// PARAMETERS: p - put, n - ime
//---------------------------------------------------
char* filepath(char* p, char* n){
    int fullsize = strlen(p) + strlen(n) + 1; // suedinqvame goleminata dvata stringa obshto i dobavqme mqsto za chertata
    char* filepath = malloc(fullsize*sizeof(char)); // zadelqme pamet za dvata stringa s chertata
    int i;
    for(i = 0;i < strlen(p);i++){
        filepath[i] = p[i];
    } // kopirame purviq string
    filepath[i] = '/'; // dobavqme chertata
    int g = 0;
    int j;
    for(j = i + 1;g < strlen(n);j++,g++){
        filepath[j] = n[g];
    } // kopirame vtoriq string
    filepath[j] = '\0';
    return filepath;
}
//------------------------------------------------
// FUNCTION: sizeDirectory
// Otvarq edna direktoriq i namira kolko obshto e 
// razmera na vsichki failove
// PARAMETERS: d - putq na direktoriqta
//------------------------------------------------
int sizeDirectory(char * d){
    struct stat stat1; //structura s informaciq za failovete
    DIR* dir = opendir(d); //otvarqne na direktoriqta
    if(dir == NULL){ //proverka za greshka
        char *errmsg = connect("ls: cannot open directory ",d);
        perror(errmsg);
        free(errmsg);
        return 1;
    }
    int s = 0; 
    while(1) //minavame prez vsichki failove i vzimame razmera im
	{
        struct dirent *ent = readdir( dir ); //poreden fail
        if(ent != NULL){ 
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0){
                continue;
            }
            char *path = filepath(d,ent->d_name); //suedinqvame imeto i putq
            stat(path,&stat1); 
            free(path);
            if(A){ //proverqvame za flag -A
                s = s + stat1.st_blocks;
            }else if(ent->d_name[0] != '.'){ 
                s = s + stat1.st_blocks;
            }
        }else if( ent == NULL && errno != 0){
            perror("readdir");
        }if(ent == NULL){ 
            break;
        }
    }
    int rc = closedir(dir);
    return s/2;
}
//----------------------------------------------------
// FUNCTION: Dir
// Priema direktoriq i printira vsichki failove v neq
// PARAMETERS: direc - putq na directoriqta
//----------------------------------------------------
int Dir(char* direc){
    DIR *dir;  //direktoriq
    struct stat stat1; //informaciq za poredniq fail
    struct dirent *ent;
    int size = sizeDirectory(direc); 
    int arrlenght = 0; //
    char **arr = malloc((arrlenght)*sizeof(char*)); //zadelqme pamet za dvumeren masiv za rekursiven flag

    dir = opendir(direc); //otvarqme directoriq
    if(dir == NULL){ //proverka za greshka
        char *errmsg = connect("ls: cannot open directory ",direc);
        perror(errmsg);
        free(errmsg);
        return 1;
    }
    
    int chek = 1;
    while(1) //bezkraen cikul, koito printira vsichki failove v direktoriqta sprqmo flagovete -r, -l, -a
	{
        ent = readdir( dir ); //vzimame faila
        if(ent != NULL){
        if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0){
            continue;
        }
        char *path = filepath(direc,ent->d_name); //vzimame putq na faila
        stat(path,&stat1); // vzimame informaciq za faila
            if(ent->d_name[0] != '.'){ //proverqvame za normalni fialove
                if(chek && l){
                    printf("total %d\n",size);
                    chek = 0;
                }
                if(l){
                    details(ent->d_name,&stat1);
                }else{
                    int type = typeF(&stat1);
                    printf("%s\n",ent->d_name);
                }                  
            }else if(A){ //proverqvame za skriti failove
                if(chek && l){
                    printf("total %d\n",size);
                    chek = 0;
                }
                if(l){
                    details(ent->d_name,&stat1);
                }else{
                    int type = typeF(&stat1);
                    printf("%s\n",ent->d_name);
                }  
            }
            if(R && (stat1.st_mode & S_IFDIR)){ //proverqvame za rekursiq
                if(A){ //dobavqme putq na direktoriqta v dinamichniq masiv pri skriti failove
                    arrlenght++;
                    arr = realloc(arr,(arrlenght)*sizeof(char*)); //prezadelqme pamet za po-golqm dinamichen masiv
                    arr[arrlenght-1] = path; //zapazvame direktoriqta
                }else if(ent->d_name[0] != '.'){ //dobavqme putq na direktoriqta v dinamichniq masiv pri normalni failove
                    arrlenght++;
                    arr = realloc(arr,(arrlenght)*sizeof(char*)); //prezadelqme pamet za po-golqm dinamichen masiv
                    arr[arrlenght-1] = path; //zapazvame direktoriqta
                }       
            }
        }else if( ent == NULL && errno != 0){
            perror("readdir");
        }if(ent == NULL){
            break;
        }
    }

    int rc = closedir(dir); //zatvarqme direktoriqta

    if(R){ // deistviq pri flag -R
        for(int i = 0 ;i<arrlenght;i++){
            printf("\n%s:\n",arr[i]);
            Dir(arr[i]);
        } //izpulnqvame funkciqta za printirane na direktoriq za vskq direktoriq v dinamichniq masiv
        for(int i = arrlenght-1;i>=0;i--){ // osvobojdavame zadelenata pamet
            free(arr[i]); 
        }
    } 
    free(arr); //osvobojdavame napulno zadelenata pamet
    return 0;
}

int main(int argc,  char **argv)
{
for(int j = 1;j < argc;j++){ //obhojdame vshicki argumenti za da zapishem vsichki flagove
    if(argv[j][0] == '-'){
        for(int i = 0;i < strlen(argv[j]);i++){
            if(argv[j][i] == 'A'){ //zapisvame flaga -A
                A = 1;
                break;
            }
        }
        for(int i = 0;i < strlen(argv[j]);i++){ //
            if(argv[j][i] == 'l'){ //zapisvame flaga -l
                l = 1;
                break;
            }
        }
        for(int i = 0;i < strlen(argv[j]);i++){ //
            if(argv[j][i] == 'R'){ //zapisvame flaga -R
                R = 1;
                break;
            }
        }
    }
}

int an = 0;
int j=0;
int true_ = 1;
for(int i = 1;i < argc;i++){ //proverka za istinski argumenti bez flagove
    if(argv[1][0] == '-'){
       continue;
    }else{
        true_ = 0;
        an++;
    }
}

if(true_){ //ako ne ni e podaden argument, a samo flagove 
    if(R){
        printf(".:\n");
    }
    Dir(".");
    return 1;
}

DIR *dir; 
struct stat stat1;
int help = 0; 
int newLine = 0; 

    for(int i = 1;i < argc;i++)
	{ //obhojdame vsichki argumenti i ako e direktoriq q otvarqme 
	  //i printirame, a ako e fail printirame suotvetnata informaciq za nego spored flaga
        if(argv[i][0] == '-'){ //preskachame flagovete
            continue;
        }
        int err = stat(argv[i],&stat1); //vzimame info na faila
        if(err!=0){ //proverka za greshka
            char *errmsg = connect("ls: cannot access ",argv[i]);
            perror(errmsg);
            free(errmsg);
            continue;
        }
        if((stat1.st_mode & S_IFDIR)){ //proverka za direktoriq
            if(newLine == 1){
                printf("\n");
                newLine = 0; //ako segashniq argument e direktoriq nulirame promenlivata za nov red
            }
            if(an >= 2){ //ako imame poveche ot 2 faila da im printirame imenata
                printf("%s:\n",argv[i]);
            }
            Dir(argv[i]);
            j++;
            if(an >= 2 && j+1 != an){ //ako posledniq argument e direktoruq da ne printira nov red
                printf("\n");
            }
        }else{ //proverka za normalen fail
            j++;
            if(l){ //proverka za dopulnitleni detaili na faila
                details(argv[i],&stat1);
            }else{
                int type = typeF(&stat1);
                printf("%s\n",argv[i]);
            }  
            newLine = 1; //ako segashniq argument e fail a sledvashtiq direktoriq dobavq nov red
        }
    }
}