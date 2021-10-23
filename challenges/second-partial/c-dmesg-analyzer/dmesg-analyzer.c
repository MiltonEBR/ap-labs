#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>

//Milton Eduardo Barroso Ramírez. A01634505.

#define REPORT_FILE "_report.txt"

#define HASHSIZE 401

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
        np->next=hashtab[hashval];
        hashtab[hashval]=np;
    }else{
        // printf("here\n");
    }

    if(!np->val){
        np->val = (char*) malloc(strlen(val)*sizeof(char)+6);
        strcpy(np->val, "    ");
        strcat(np->val, val);
        strcat(np->val, "\n");
    }else{
        if(strstr(np->val, val)!=NULL) return np;

        char* tmp = (char*) malloc(strlen(np->val)*sizeof(char)+1);
        strcpy(tmp, np->val);
        np->val = (char*) malloc( (strlen(tmp) + strlen(val)) * sizeof(char) + 6);
        strcpy(np->val, tmp);
        strcat(np->val, "    ");
        strcat(np->val, val);
        strcat(np->val,"\n");
    }
    
    return np;
}

//Helpers
struct descAndKey{
    char* key;
    char* desc;
};

struct descAndKey descriptionAndKey(char* str){

    struct descAndKey res;

    char* timeStamp;
    int length = 0;

    char* curr = str;
    while(*curr){
        length++;
        curr++;
        if(*(curr-1)==']') break;
    }
    if(length<1){
        res.key="General:";
        res.desc= (char*) malloc(strlen(str)*sizeof(char)+1);
        strcpy(res.desc, str);
        return res;
    };
    timeStamp = (char*) malloc(length*sizeof(char));
    strncpy(timeStamp, str, length);

    int keyLength = 0;
    int trailingS = 0;
    int colonFound = 0;
    while(*curr){
        if(keyLength==0 && *curr==' ') {
            trailingS++;
        }else if(colonFound && *curr==' '){
            break;
        }else if(*curr==':' && !colonFound){
            colonFound=1;
            keyLength++;
        }else{
            keyLength++;
        }
        curr++;

    }

    char* tmpKey;
    char* fullDesc;
    if(colonFound){
        tmpKey=(char*) malloc(keyLength*sizeof(char));
        strncpy(tmpKey,str+trailingS+length,keyLength);

        fullDesc = (char*) malloc(strlen(str)*sizeof(char) - trailingS - keyLength + 1);
        strcpy(fullDesc, timeStamp);
        strcat(fullDesc, curr);
    }else{
        tmpKey = (char*) malloc(strlen("General:")*sizeof(char)+1);
        strcpy(tmpKey, "General:");
        fullDesc = (char*) malloc(strlen(str)*sizeof(char)+1);
        strcpy(fullDesc, str);
    }
    res.key=tmpKey;
    res.desc=fullDesc;

    return res;

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

    // printf("Analyzing: %d lines\n",file_len);
    lseek(file,0,SEEK_SET);
    int line_length = 0;
    do{
        bytes_read = read(file, &buff, 1);
        if(bytes_read>0){
            if(buff =='\n'){
                if(line_length<1) continue;
                line_length++;
                lseek(file,-line_length,SEEK_CUR);
                char* lineBuff = (char*) malloc(line_length*sizeof(char));
                read(file,lineBuff,line_length);
                lineBuff[strcspn(lineBuff, "\n")] = 0;
                line_length=0;
                struct descAndKey res = descriptionAndKey(lineBuff);
                add(res.key,res.desc);
            }else{
                line_length++;
            }
        }
    } while(bytes_read>0);

    close(file);
    //Iterating through hash to write
    FILE* output = fopen(report, "w+");
    struct nlist* head;

    for (int i=0; i<=HASHSIZE; i++) {
        head=hashtab[i];
        if(head==NULL) continue;
        struct nlist* curr;
        for (curr=head; curr!=NULL; curr=curr->next) {
            fputs(curr->key, output);
            fputs("\n", output);
            fputs(curr->val, output);
        }   
    }
    fclose(output);

    printf("Report is generated at: [%s]\n", report);
}
