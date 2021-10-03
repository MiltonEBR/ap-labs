//Milton Eduardo Barroso Ramírez. A01634505.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quicksort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));


int compStrings(void* l, void* r) {
    	return strcmp(l, r);
}

int compNums(void* l, void* r) {
		int n1 = atoi(l);
		int n2 = atoi(r);
		if(n1>n2) return 1;
		else return -1;
}

int main(int argc, char **argv)
{
	if(argc<3){
		printf("Please use valid arguments <-n: optional> <fileToRead.txt> <-quicksort/-mergesort> <-o output_file.txt: optional>\n");
		return 0;
	}

	char* fileToRead=NULL;
	char* outputFile=NULL;
	int isNumberFile = 0;
	char sortType ='n';
	for (int i=1; i<argc; i++) {

		if(strlen(argv[i]) > 4 && !strcmp(argv[i] + strlen(argv[i]) - 4, ".txt")){
			fileToRead=argv[i];
		}else if(strcmp(argv[i], "-o")==0){
			if(i+1>=argc){
				printf("Please input a file to output after -o\n");
				return 0;
			}
			i++;
			outputFile=argv[i];
		}else if(strcmp(argv[i], "-n")==0){
			isNumberFile=1;
		}else if(strcmp(argv[i], "-quicksort")==0){
			sortType='q';
		}else if(strcmp(argv[i], "-mergesort")==0){
			sortType='m';
		}
	}

	if(fileToRead==NULL){
				printf("Please input a file to read\n");
				return 0;
	}
	if(sortType=='n'){
				printf("Please input a sort type\n");
				return 0;
	}

	if(outputFile==NULL) {
		char* tmp = "sorted_";
		outputFile = (char*) malloc(strlen(fileToRead)+strlen(tmp)+1);
		strcpy(outputFile, tmp);
		strcat(outputFile, fileToRead);
	}

	// printf("%s,%s,%d,%c\n",fileToRead,outputFile,isNumberFile,sortType);
	FILE *file;


    file = fopen(fileToRead, "r");
	if (file == NULL){
        printf("Error reading the file\n");
        return 0;
    }

	int file_len = 0;
	char ch=0;
	while(!feof(file)){
		ch = fgetc(file);
		if(ch == '\n') file_len++;
	}

	fseek(file, 0, SEEK_SET);

	char** lines = malloc(file_len * sizeof(char*));

	// printf("%d\n",file_len);
    
	int i=0;
	char buffer[256];
	while(fgets(buffer, 256, file)) {
		buffer[strcspn(buffer, "\n")] = 0;
		lines[i] = malloc(strlen(buffer)*sizeof(char));
		strcpy(lines[i], buffer);
		i++;
	}
    fclose(file);
	
	if(sortType=='q'){
		quicksort((void** )lines, 0, file_len-1, isNumberFile ? compNums : compStrings);
	}else{
		mergesort((void** )lines, 0, file_len-1, isNumberFile ? compNums : compStrings);
	}
	
	FILE *out = fopen(outputFile, "wb");

	for(i=0;i<file_len;i++){
		fputs(strcat(lines[i],"\n"), out);
	}
	fclose(out);



    return 0;
}
