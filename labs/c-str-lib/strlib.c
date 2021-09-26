//Milton Eduardo Barroso Ramírez.
//A01634505
#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){
    char *c = str;
    int i = 0;

    while(*c) {
        i++;
        c++;
    };

    return i;
}

char *mystradd(char *origin, char *addition){
    int oL = mystrlen(origin);
    char* tmpO = origin;

    int aL = mystrlen(addition);
    char* tmpA = addition;

    char* str = (char *) malloc(oL+aL);
    char* res = str;

    while(*tmpO){
        *str = *tmpO;
        str++;
        tmpO++;
    }
    
    while(*tmpA){
        *str = *tmpA;
        str++;
        tmpA++;
    }

    return res;
}

int mystrfind(char *origin, char *substr){

    char* o = origin;
    char* s = substr;

    int checking = -1;

    int len = mystrlen(o);
    int i = 0;

    while(i<len){
        if(checking>-1 && !*s) return checking;
        else if (checking>-1){
            if(*s == *o) {
                s++;
                o++;
                i++;
                continue;
            }else{
                checking = -1;
                s = substr;
            }
        }

        if(*s == *o){
            checking = i;
            s++;
        }

        o++;
        i++;

    }

    return checking;
}
