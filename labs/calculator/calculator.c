#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {

    long result = 0;

    char sign;
    switch (operator) {
    case 1:
        sign='+';
        break;
    case 2:
        sign='-';
        break;
    case 3:
        sign='*';
        break;
    default:
        break;
    }


    for(int i=0; i<nValues;i++){

        switch (operator) {
        case 1:
            result+=values[i];
            break;
        case 2:
            result = result - values[i];
            break;
        case 3:
            result = result * values[i];
            break;
        default:
            break;
        }

        if(i==0) result = values[i];

        if(i+1<nValues){
            printf("%d %c ",values[i],sign);
        }else{
            printf("%d = %ld\n",values[i],result);
        }
    }

    return result;
}

int* argsToNumArray(char *args[], int n){

    int* nums = malloc(n);

    for (int i=0; i<n; i++) {
        nums[i]=atoi(args[2+i]);
    }

    return nums;
}

int main(int argc, char *argv[]) {

    if(argc<=2) {
        printf("You must introduce the proper arguments ./calculator <operator> <numbers separated by space>\n");
        return -1;
    }

    char *sign = argv[1];
    int operator;

    if (strcmp(sign,"add")==0) operator=1;
    else if (strcmp(sign,"sub")==0) operator=2;
    else if (strcmp(sign,"mult")==0) operator=3;
    else {
        printf("The operator provided is not valid\n");
        return -1;
    }

    int *nums = argsToNumArray(argv, argc-2);
    
    calc(operator,argc-2,nums);


    return -1;
}
