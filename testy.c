#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXLENGTH 100 
/*
* Function: string_length
* -----------------------
* Counts the length of an array, which is pointed as parameter.
* Return the length of the array.
*/

int string_length(char string[]){
    int i = 0;
    
    while (string[i] != '\0'){
        i++;
    }
    return i;
}

/*
* Function: string_comp
* ---------------------
* Compares two given strings.
* If the string are exactly the same same, the return value is true, otherwise the function returns false.
*/

bool string_comp(char string1[], char string2[]){
    int length1;
    int length2;
    int i = 0;

    length1 = string_length(string1);
    length2 = string_length(string2);

    if (length1 == length2){
        while (string1[i] == string2[i] && i < length1){
            i++;
        }

        if (i == length1){
            return true;
        }
    }
    return false;
}

void fill_substring(char substring[], char password[], int parameter, int start_value){
    int k = 0;

    while (k < parameter){
        substring[k] = password [start_value + k];
        k++;
    }
    substring[k] = '\0';
}

/* 
* Function: is_in_2_same_strings
* ------------------------------
* This function goes threw every substring after the one starting with the given character in password.
* Length of each substring is equal to PARAMETER. 
* If it finds the same substring, the function returns true, contrarily it returns false.
*/

bool is_in_2_same_strings(int c_position, int parameter, char password[], int pass_length){
    int j = c_position + 1;
    char substring1[MAXLENGTH + 1];
    char substring2[MAXLENGTH + 1];

    fill_substring(substring1, password, parameter, c_position);
    while (j < (pass_length - parameter + 1)){
        fill_substring(substring2, password, parameter, j);
        j++;
        if (string_comp(substring1, substring2)) return true;
    }

    return false;
}

int main(int argc, char **argv){
    int parameter = atoi(argv[1]);
    char password[MAXLENGTH];
    fgets(password, MAXLENGTH + 2, stdin);
    int k = 0;
    if (argc < k){
        return -1;
    }
    while (password[k] != '\0'){
        k++;
    }
    int i = 0;
    while (password[i] != '\0'){
        if (is_in_2_same_strings(i, parameter, password, k)) printf("nice");
        i++;
    }
}