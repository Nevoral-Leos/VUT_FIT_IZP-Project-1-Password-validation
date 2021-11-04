/*
* File:    pwcheck.C
* Name:    Project 1 - Validation of password
* Author:  Nevoral Leos (xnevor03@vutbr.cz)
* Date:    October 2021
* Course:  Introduction to Programming Systems
* Summary: This program proccesses passwords from stdin based on requirements from the user.
*          The requirements are LEVEL and PARAMETER.
*          Level 1 - at least 1 upper and lower case letter
*          Level 2 - the password contains characters from PARAMETER of groups
*                    (the groups are, lower, upper case letters, numbers, special scharacter)
*          Level 3 - there are no sequences of single character of the length PARAMETER
*          Level 4 - the password doesnt contain two same substrings of the length PARAMETER
*          If the password is strong enough, it gets printed on the standart stdout output.
*          The program also keeps track of password statistics, in case the user wants them.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAXLENGTH 100 //max length of password

typedef struct statistics{
    char used_chars[96];
    int chars_count;
    int shortest_pass;
    int length_count;
    int pass_count;
}stats;

typedef struct password{
    char string[MAXLENGTH + 2];
    int pass_length;
}pass;

/*
* Function: string_length
* -----------------------
* Counts the length of an atring, which is pointed as parameter.
* Return the length of the string.
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

/*
* Function req_filled
* -------------------
* Checks if the required parameters have been set.
* If its true, the function sets error message number, because too many arguments were given.
* Return false if the parameters aren't set yet and true if the are both set.
*/

bool req_filled(int pass_level, int parameter, int *err_num){       // if value MAXLENGTH + 3 is passed to function, it indicates
    if (pass_level == MAXLENGTH + 3 || parameter == MAXLENGTH + 3){ // that only one variable is meant to be checked
        if (pass_level != MAXLENGTH + 2 && pass_level != MAXLENGTH + 3) return true;
        
        else if (parameter != MAXLENGTH + 2 && parameter != MAXLENGTH + 3) return true;
        
        else return false;
    }
    if (pass_level != MAXLENGTH + 2 && parameter != MAXLENGTH + 2){
        *err_num = 406;
        return true;
    }
    return false;
}

/*
* Function: set_level
* -------------------
* Checks if the given argument is suitable to be set as level, that the passwords have to accomplish.
* Returns false, if the argument doesn't fit the requirements and sets the number of error message, that has to be displayed. 
* If the value is set succesfuly, the function returns true.
*/

bool set_level(int *pass_level, int *err_num, int argument){
    if (req_filled(*pass_level, MAXLENGTH + 3, err_num)){ //if the value is already set, the program will display error message
        *err_num = 410;                                   
        return false;
    }

    if (argument > 0 && argument < 5){
        *pass_level = argument;
        return true;
    }

    *err_num = 407;
    return false;
}

/*
* Function: set_paramete
* -------------------
* Checks if the given argument meets the measerues required for parameter.
* If the parameter is set correctly, the function returns true, otherwise it returns false and sets the number of error to be displayed.
*/

bool set_parameter(int *parameter, int *err_num, int argument){
    if (req_filled(MAXLENGTH + 3, *parameter, err_num)){ //if the value is already set, the program will display error message
        *err_num = 410;
        return false;
    }

    if (argument > 0 && argument < MAXLENGTH){
        *parameter = argument;
        return true;
    }

    else if (argument > MAXLENGTH){ //all parameters longer than the password, dont have any effect, so the value is set to max
        *parameter = MAXLENGTH;
        return true;
    }

    *err_num = 408;
    return false;

}

/*
* Function: check_arguments
* -------------------------
* This function takes arguments given to the program, checks their usability and then assigns them to the right variable.
* Returns true, if all arguments were assigned correctly, but in case, that the arguments are invalid, or there is too many
* or too few for the program to run, it returns false and sets ERR_NUM to coresponding error message.
*/

bool check_arguments(int argc, char **argv, int *pass_level, int *parameter, bool *make_stats, int *err_num){
    int i = 1; // the index of argument
    char *remainder;
    int argument;

    if (argc < 3 || argc > 6){
        *err_num = 406;
        return false;
    }

    while (i < argc){
        argument = strtol(argv[i], &remainder, 10);

        if (*remainder == '\0'){ // if argument is just number
            if (req_filled(*pass_level, *parameter, err_num)) return false; // if all parameters are already set, too many args were 
                                                                            // given so the function returns false
            if (i == 1){
                if (!set_level(pass_level, err_num, argument)) return false; 
            }

            else if (i == 2){
                if (!set_parameter(parameter, err_num, argument)) return false;
            }

            else{
                *err_num = 410;
                return false;
            }
        }

        else if (string_comp(remainder, "--stats")){
            *make_stats = true;
        }

        else if (string_comp(remainder, "-l") && argument == '\0'){
            if (req_filled(*pass_level, *parameter, err_num)) return false;

            if (i + 1 < argc){ // if there is another argument after -l, the function checks the value of it
                argument = strtol(argv[i + 1], &remainder, 10);
                if (*remainder == '\0'){
                    if (!set_level(pass_level, err_num, argument)) return false; // if it was number, the program sets it as level
                    i++;                                                         // and moves to another argument
                }

                else if (string_comp(remainder, "-p") || string_comp(remainder, "--stats")){ // if it is -p / --stats, the value is
                    if (!set_level(pass_level, err_num, 1)) return false;                    // set to default value 1
                }

                else {
                    *err_num = 409; // if neither is true, invalid argument was given to the program
                    return false;
                }
            }

            else{
                if (!set_level(pass_level, err_num, 1)) return false; //  if there isn't any other argument, the value is set do default 1
            }
        }

        else if (string_comp(remainder, "-p") && argument == '\0'){ //the logic in this part is same as above, it only sets tha value for parameter
            if (req_filled(*pass_level, *parameter, err_num)) return false;
            
            if (i + 1 < argc){
                argument = strtol(argv[i + 1], &remainder, 10);
                if (*remainder == '\0'){
                    if (!set_parameter(parameter, err_num, argument)) return false;
                    i++;
                }

                else if (string_comp(remainder, "-l") || string_comp(remainder, "--stats")){
                    if (!set_parameter(parameter, err_num, 1)) return false;
                }

                else {
                    *err_num = 409;
                    return false;
                }
            }

            else{
                if (!set_parameter(parameter, err_num, 1)) return false;
            }
        }

        else{
            *err_num = 409;
        }

        if (*err_num != 0){
            return false;
        }
        i++;
    }

    if (!req_filled(*pass_level, *parameter, err_num)) return false;
    return true;
}

/*
* Function is_unique
* ------------------
* Checks if the character has been already used in other passwords.
* If it has been used, the function returns false, and true if it wasn't used.
*/

bool is_unique(char subject, stats *p_stats){
    int i = 0;

    while (p_stats->used_chars[i] != '\0'){
        if (subject == p_stats->used_chars[i] || subject == '\n' || subject == '\0'){
            return false;
        }
        i++;
    }
    return true;
}

/*
* Function: count_stats
* ---------------------
* This function takes in current passwords and statistics then ads the parameters of the password to statistics.
* The function doesnt return anything.
*/

void count_stats(pass *p_pass, stats *p_stats){
    int i = 0;

    p_stats->length_count += p_pass->pass_length;
    if (p_pass->pass_length < p_stats->shortest_pass){ //if the password is shorter than the shortest password, it becomes the shortest password
        p_stats->shortest_pass = p_pass->pass_length; 
    }
    else if (p_stats->pass_count == 1){
        p_stats->shortest_pass = p_pass->pass_length; //if its the first password, it becomes automatically the shortest password
    }

    while (i < p_pass->pass_length){
        if (is_unique(p_pass->string[i], p_stats)){
            p_stats->used_chars[p_stats->chars_count] = p_pass->string[i]; //unique characters are added to the string of unique chars
            p_stats->used_chars[p_stats->chars_count + 1] = '\0'; //next char is set to null terminator, because the program works with strings
            p_stats->chars_count++; //program counts, how many chars have we used
        }
        i++;
    }
}

/*
* Function: in_in_ascii
* ---------------------
* Checks if the given character is in the accepted part of Ascii table.
* IF the character fits the parameter, the function returns true, in the other case, it returns false.
*/

bool is_in_ascii(char subject){
    if (subject >= ' ' && subject <= '~'){
        return true;
    }
    return false;
}

/*
* Function: is_upper
* ------------------
* Checks if the given char is upper case.
* Returns true if it is upper case and false if it isnt.
*/

bool is_upper(char subject){
    if (subject >= 'A' && subject <= 'Z'){
        return true;
    }
    return false;
}

/*
* Function: is_lower
* ------------------
* Checks if the given char is lower case.
* Returns true if it is lower case and false if it isnt.
*/

bool is_lower(char subject){
    if (subject >= 'a' && subject <= 'z'){
        return true;
    }
    return false;
}

/*
* Function: level1
* ----------------
* This function checks, if the password already fits the parameters, to pass the first level.
* In case any of the parameters isn't fulfilled yet, the function checks if the given character fits these parameters.
* If all parameters are filled, the function returns true as the password passed first level, otherwise it returns false.
* To track of how many char types the password contains, it uses the array RULES, true means contains, false means the opposite.
*/

bool level1(char subject, bool rules[]){
    if (!rules[0] && is_upper(subject)){ 
        rules[0] = true;
    }

    else if (!rules[1] && is_lower(subject)){ 
        rules[1] = true;
    }
    if (rules[0] && rules[1]){
        return true;
    }
    return false;
}

/*
* Function: is_num
* ----------------
* Checks if the given char is number.
* Returns true if it is number and false if it isn't.
*/

bool is_num(char subject){
    if (subject >= '0' && subject <= '9'){   
        return true;
    }
    return false;
}

/*
* Function: is_special_char
* -------------------------
* Checks if the given char is special char.
* List of all accepted special chars: 
* space ! " # $ % & ' () * + , - . / : ; < = > ? @ [] \ ^ _ ` {} | ~
* Returns true if it is special char and false if it isnt.
*/

bool is_special_char(char subject){
    if (subject >= 0x20 && subject <= 0x7E && !(subject >= 'a' && subject <= 'z') && !(subject >= 'A' && subject <= 'Z') && !(subject >= '0' && subject <= '9')){
        return true;
    }
    return false;
}

/*
* Function: level2
* ----------------
* This function checks, if the password already fits the parameters, to pass the second level.
* In case any of the parameters isn't fulfilled yet, the function checks if the given character fits these any of the parameters.
* To track of how many char types the password contains, it uses the array RULES, true means contains, false means the opposite.
* If all parameters are filled, the function returns true, otherwise it returns false.
*/

bool level2(char subject, bool rules[], int parameter){
    if (!rules[2] && is_num(subject)){ 
        rules[2] = true;
    }
    else if (!rules[3] && is_special_char(subject)){ 
        rules[3] = true;
    }
    int rules_count = 0;
    int act_parameter = (parameter > 4) ? 4 : parameter; //even if the parameter is more than 4, the password has to fit only 4 groups, so the compared parameter is set to 4 
    if ((rules_count = (rules[0] + rules[1] + rules[2] + rules[3])) >= act_parameter){
        return true;
    }
    return false;
}

/*
* Function is_in_sequence
* -----------------------
* This function compares current char, to the last char. 
* If they are equal, the function increments the count of characters in sequence. 
* The program uses variable CHAR_IN_SEQUENCE to track the last used character 
* On the other hand, if the characters dont match, the count is set back to 1.
* When the length of sequence is equal to PARAMETER, the function returns true, if not it returns false.
*/

bool is_in_sequence(char current_char, int parameter, int *current_sequence, char *char_in_sequence){
    if (parameter == 1){ //if parameter is set to 1, then any character is in sequence, so the function returns true
        return true;
    }
    if (current_char == *char_in_sequence){
        *current_sequence += 1;
        if (*current_sequence == parameter){
            return true;
        }
    }
    else{
        *char_in_sequence = current_char;
        *current_sequence = 1;
    }
    return false;
}

/*
* Function: fill_substring
* ------------------------
* This function creates substring of length PARAMETER from the given password and saves it to the given string.
*/

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
* The function uses variable i to represent beggining of every second substring in password.
* If it finds the same substring, the function returns true, contrarily it returns false.
*/

bool is_in_2_same_strings(int c_position, int parameter, char password[], int pass_length){
    int i = c_position + 1;
    char substring1[MAXLENGTH + 1];
    char substring2[MAXLENGTH + 1];

    fill_substring(substring1, password, parameter, c_position);
    while (i < (pass_length - parameter + 1)){
        fill_substring(substring2, password, parameter, i);
        i++;
        if (string_comp(substring1, substring2)) return true;
    }

    return false;
}

/*
* Function: levels_validation
* ---------------------------
* This function goes threw every character in password and checks every time, if enough levels have been completed.
* If any level isn't filled yet, the program sends the current character to check, if it fills any of the levels.
* To track progress of each level, the program uses array LEVELS, true means, the level has been passed and false means the opossite.
* The value of level 3 and 4 are set to true since the beggining, untill the program proves, 
* that the password contains the right substring or sequence of charater and then are changed to false.
*/

void levels_validation(bool levels[], int pass_level, int parameter, char password[], int pass_length){
    bool rules[4] = {false, false, false, false}; // To track of how many char types the password contains, it uses the array RULES,
    int current_sequence = 1;                     // true means contains, false means the opposite.
    char char_in_sequence = '\0';
    int i = 0;

    while (i < pass_length){

        if (pass_level > 0 && !levels[0]){                      // When some levels aren't required to be checked,
            levels[0] = level1(password[i], rules);             // the program doesn't try to check them, because it first checks, 
        }                                                       // if the required strength of password that is stored
                                                                // in variable PASS_LEVEL is higher than the level it would check
        if (pass_level > 1 && !levels[1]){
            levels[1] = level2(password[i], rules, parameter);
        }

        if (pass_level > 2 && levels[2]){
            levels[2] = !is_in_sequence(password[i], parameter, &current_sequence, &char_in_sequence);
        }

        if (pass_level > 3 && levels[3]){
            levels[3] = !is_in_2_same_strings(i, parameter, password, pass_length);
        }

        i++;
    }
}

/*
* Function: evaluate_strength
* ---------------------------
* This function counts, how many levels has the program passed bassed on value assigned to each level in arra LEVELS
* When the level doesn't need to be passed, the function ignores it. The function knows it from of the value in PASS_LEVEL,
* if it's lower than the index of level, the level is skipped.
*/

int evaluate_strength(bool levels[], int pass_level){
    int ans = 0;
    int i = 0;

    while (i < pass_level){
        ans += levels[i];
        i++;
    }

    return ans;
}

/*
* Function: is_password
* ---------------------
* Checks if the password meats the basic requirements to be proccessed, 
* the password must contatin only characters from the ASCII table and mustn't be longer than 100 characters.
* In process the program also counts the length of each password and stores it in structure PASSWORD in variable pass_length.
* If the password passes the test, function returns true, if any of the test fails, it returns false
* and sets the variable ERR_NUM to match the index of assigned error.
*/

bool is_password(int *err_num, pass *p_pass, stats *p_stats){
    int i = 0;
    while (i < MAXLENGTH + 1){
        if (p_pass->string[i] == '\n' || p_pass->string[i] == '\0'){
            if (i == 0){
                p_pass->pass_length = 0; // empty lines in input are still considered to be a password with length 0
                return true;
            }

            if (p_pass->string[i] == '\0'){ // when the program reaches end of file, the last character is set to be \0
                p_pass->string[i] = '\n';   // but for the password to print properly we set it to new line character
                p_pass->string[i+1] = '\0';
            }

            p_stats->pass_count++;
            p_pass->pass_length = i;       
            return true;
        }
        else if (!(is_in_ascii(p_pass->string[i])))
        {
            *err_num = 405;
            p_pass->pass_length = i;
            return false;
        }
        i++;
    }
    *err_num = 404;
    return false;
}

/*
* Function: error_handling
* ------------------------
* Whenever any problem occures in program the err_num value is set to correct error.
* This function takes care of printing out the correct error message for user.
*/

void error_handling(int err_num, pass *p_pass, stats *p_stats){
    if (err_num == 404){
        fprintf(stderr, "Password is too long!! The program will be terminated.");
        return;
    }
    else if (err_num == 405){
        fprintf(stderr, "Invalid character on position %d in password number: %d", p_pass->pass_length, p_stats->pass_count);
        return;
    }
    else if (err_num == 406){
        fprintf(stderr, "Invalid amount of arguments!!");
        return;
    }
    else if (err_num == 407){
        fprintf(stderr, "Wrong arguments!! Level must be from interval <1, 4>");
        return;
    }
    else if (err_num == 408){
        fprintf(stderr, "Wrong arguments!! Parameter must be positive number");
        return;
    }
    else if (err_num == 409){
        fprintf(stderr, "Invalid argument!!");
        return;
    }
    else if (err_num == 410){
        fprintf(stderr, "Invalid format of arguments, the only accepted formats are:\nLEVEL PARAMETER [--stats]\n[-l]LEVEL [-p]PARAMETER [--stats]\n[] - arguments are optional\nif -l and -p is presented, the postion of each argument is adjustable");
        return;
    }
}

int main(int argc, char **argv){
    pass act_pass;
    stats act_stats;
    act_pass.string[0] = '\0';      // array used to store the password we currently work with
    act_pass.pass_length = 0;       // length of current password
    act_stats.used_chars[0] = '\0'; // array of used characters in all password
    act_stats.chars_count = 0;      // count of all unique characters
    act_stats.length_count = 0;     // length of all passwords all together
    act_stats.pass_count = 0;       // amount of passwords given to the program
    act_stats.shortest_pass = 0;    // length of the shortest password
    int err_num = 0;                // err tracking number if set to 0, means no errors have occured
    bool make_stats = false;        // if set to true from arguments, the program will keep track of stats
    int pass_level = MAXLENGTH + 2; // amount of levels, the password has to pass,  the value that is set cannot be reached by the program, the value is set to know, if the program has assigned any value to it
    int parameter = MAXLENGTH + 2;  // value of parameter used for the program, the value that is set cannot be reached by the program, it's set to know, if the program has assigned any value to it

    if (!(check_arguments(argc, argv, &pass_level, &parameter, &make_stats, &err_num))){ // validation of arguments given to program
        error_handling(err_num, &act_pass, &act_stats);
        return -1;
    }

    while (fgets(act_pass.string, MAXLENGTH + 2, stdin) != NULL){ // reads and stores passwords one by one
        if (!(is_password(&err_num, &act_pass, &act_stats))){     // checks if it is valid password
            error_handling(err_num, &act_pass, &act_stats);
            return -1;
        }
        bool levels[4] = {false, false, true, true};
        levels_validation(levels, pass_level, parameter, act_pass.string, act_pass.pass_length); // checks levels the password passes
        int passed = evaluate_strength(levels, pass_level); // defines the strength of password
        if (make_stats){
            count_stats(&act_pass, &act_stats); // if asked for, tracks statistics
        }
        if (passed >= pass_level){
            printf("%s", act_pass.string); // print strong enough password
        }  
    }

    if (make_stats){
        int divisor;
        divisor = (act_stats.pass_count == 0) ? 1 : act_stats.pass_count; // changes amount of passwords given to program to 1, if no password was given to it
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", act_stats.chars_count);
        printf("Minimalni delka: %d\n", act_stats.shortest_pass);
        printf("Prumerna delka: %0.1lf\n", (double)act_stats.length_count / divisor);
    }

    return 0;
}