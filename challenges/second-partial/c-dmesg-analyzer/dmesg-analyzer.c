#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE "report.txt"

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


    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    // Implement your solution here.

    printf("Report is generated at: [%s]\n", report);
}
