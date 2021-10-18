#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

#define REPORT_FILE "_report.txt"

#define HASHSIZE 101

//Hash table implementation
struct nlist{
    struct nlist* next;
    char* key;
    char* val;
};

unsigned hash(char *s){
    unsigned hashval;

    for (hashval=0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }

    return hashval % HASHSIZE;
}

static struct nlist *hashtab[HASHSIZE];

struct nlist* lookup(char *s){
    struct nlist* np;

    for (np=hashtab[hash(s)]; np!=NULL; np=np->next) {
        if(strcmp(s, np->key)==0){
            return np;
        }
    }
    return NULL;
}

struct nlist* add(char* key, char* val){
    struct nlist* np;
    unsigned hashval;

    if( (np=lookup(key) )==NULL){
        np = (struct nlist* ) malloc(sizeof(*np));
        if(np == NULL || (np->key = strdup(key))==NULL) return NULL;

        hashval= hash(key);
        np->next=hashtab[hash(key)];
        hashtab[hashval]=np;
    }else{
        free((void*) np->val);
    }
    if((np->val=strdup(val))==NULL) return NULL;

    return np;
}



//Analizer
void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer logfile.txt\n");
	return 1;
    }

    //Generate output file name
    char* reportName = (char*) malloc(strlen(argv[1])-4+strlen(REPORT_FILE)+1);
    strncpy(reportName, argv[1], strlen(argv[1])-4);
	strcat(reportName, REPORT_FILE);

    //Run analyzer
    analizeLog(argv[1], reportName);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int file = open(logFile, O_RDONLY);
	if (file == -1){
        printf("Error reading the log file\n");
        return;
    }

	int file_len = 0;
	char buff;
    int bytes_read;
    int newLine = 1;
	do {
        
        bytes_read = read(file, &buff, 1);
        if(newLine && bytes_read>0) {
            file_len++;
            newLine=0;
        }
        if(buff=='\n') newLine=1;
    } while (bytes_read>0);

    printf("Analyzing: %d lines\n",file_len);
    lseek(file,0,SEEK_SET);
    int line_length = 0;
    do{
        bytes_read = read(file, &buff, 1);
        if(bytes_read>0){
            if(buff =='\n'){
                line_length++;
                lseek(file,-line_length,SEEK_CUR);
                char* lineBuff = (char*) malloc(line_length*sizeof(char));
                read(file,lineBuff,line_length);
                lineBuff[strcspn(lineBuff, "\n")] = 0;
                printf("%s\n",lineBuff);
                line_length=0;
            }else{
                line_length++;
            }
        }
    } while(bytes_read>0);

    

    printf("Report is generated at: [%s]\n", report);
}
