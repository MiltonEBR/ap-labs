//Milton Eduardo Barroso Ramírez.
//A01634505
#include <stdio.h>

/*
NOTE: I had to add prototype functions since mystradd() got assigned as a default return value int instead of char*,
which resulted in a segementation fault, although the function does works
*/
int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);


int main(int argc, char* argv[]) {

    if(argc<4){
        printf("Please use correct parameters <-find/-add> <String 1> <String 2>\n");
        return 0;
    }

    if(mystrfind(argv[1], "-add")>-1){
        char* res = mystradd(argv[2], argv[3]);
        printf("Initial Length: %d\n",mystrlen(argv[2]));
        printf("New String: %s\n",res);
        printf("New Length: %d\n",mystrlen(res));

    }else if(mystrfind(argv[1], "-find")>-1){
        int f = mystrfind(argv[2], argv[3]);
        printf("['%s'] string was found at [%d] position\n",argv[3],f);
    }else{
        printf("Please us the flag -add or -find\n");
    }

    return 0;
}
