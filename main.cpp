#include <stdlib.h>
#include <stdio.h>
#include <cstring>

//there are variables in our pseudocode and we need to interpret them too.
//use this struct to define the data
struct data {
    int value;          //value of the variable
    char name[16];      //the name of our variable. max 16 chars
    bool isVar = false; //check if the spot is taken
} variable[255];        //an array for the variables. max 255 

//creates a new variable in the struct above
void CreateInteger(int value, char name[]) {
    int i = 0;
    while (variable[i].isVar && i < 255) {
        if (i == 254) {
            printf("Error. Max number of variables already entered.");
            exit(400);
        }
        if (strcmp(variable[i].name, name) == 0) {
            printf("Warning - variable %.*s is already defined. Setting value to 0.\n\n", 
                strlen(name), name);
            variable[i].value = 0;
            return ;
        }

        i++;
    }

    value = 0;
    variable[i].isVar = true;
    variable[i].value = value;
    for (int k = 0; k < strlen(name); k++)
        variable[i].name[k] = name[k];

    printf("Created variable %.*s of int type on position %d in struct. Value %d.\n\n", 
        strlen(name), name, i, value);
}

//function to check if a certain key is the first part of the line
int FirstChars(char location[], char key[]) {
    char *p = strstr(location, key);
    if (!p)
        return 0; //if the line is empty, return 0
    else {
        int setIndex = p - location; //calculate the start index of the pointer

        for (int i = 0; i < setIndex; i++) { //go through the line until the start of pointer
            if (location[i] != ' ')
                return 0; //if there is a different character than ' ' before the key, return 0
        }
    }

    //printf(location);
    
    return 1;
}

//function that counts lines. for debug purposes
//use it to see if FirstChars() counts commented lines correctly
void CountLines(int &totalLine, int &codeLine, int &commentLine, char line[]) {
    totalLine++;
    if (FirstChars(line, "//"))
        commentLine++;
    else
        codeLine++;
}

int CheckVar(data vars[], char key[]) {
    //printf("/////////////////////////////////\n");
    for (int i = 0; i < 6; i++) {
        //printf("name in struct: <%s>\n", vars[i].name);

        if (strcmp(vars[i].name, key) == 0)
            return i;
    }
    //printf("///////////////////////////////////\n");
    return 0;
}

//function to calculate a given expression
int CalculateExpr(char expression[]) {
    /*
        an expression is composed of variables or numbers and operands, such as:
            ~ multiplication - *
            ~ division - /
            ~ modulo - %
            ~ addition - +
            ~ subtraction - -
    
        to calculate the expressions, we will use this large function 
    which will return or provide the data to other functions (or itself)
        we will use functions:
            ~ add()
            ~ subtract()
            ~ divide()
            ~ modulo()
            ~ multiply()
    */
    int value = 0;

    //check if the expression is a number
    int i = 0;
    char numbers[] = {"0123456789"};
    bool isNum = true;
    bool negative = false;

    if (expression[0] == '-') {
        i++;
        negative = true;
    }
    while (i < strlen(expression)) {
        if (!strchr(numbers, expression[i++])) {
            isNum = false;
            break;
        }
        i++;
    }
    if (isNum) {
        for (int i = strlen(expression)-2; i >= 0; i--) {
            value = value * 10 + (expression[i] - 48); 
        }
        if (negative)
            value = value * (-1);

        //printf("Assigned value %d.\n", value);
        return value;
    }

    //if it is not a number, it is certainly an expression
    if (!isNum) {
        /*
            to calculate an equation, we have to split it as such: var1, operand, var2
            var1 and var2, if not already a single variable, will become expressions

            EXAMPLE: x * (y + 1)
            - becomes var1 = x, operand *, var2 = y + 1
            - feed var2 as a new expression to the function
            - equation is y + 1
            - var1 is y, operand +, var2 is 1
            - function returns the value of the sum and goes back to the main equation, 
                and var2 becomes the value above
        */
        printf("Not a number. Expression: %.*s\n", strlen(expression)-1, expression);
    }

    return -1;
}

int main() {
    FILE *input = fopen("inputs/input.txt", "r");

    if (!input) {
        printf("Error - input file empty");
        exit(1);
    }

    char line[1023];
    int totalLine = 0, codeLine = 0, commentLine = 0;
    while (fgets(line, sizeof(line), input)) {
        int var = 0;

        //count the lines for debug
        CountLines(totalLine, codeLine, commentLine, line);

        if(strlen(line) == 1) {}
        else if (FirstChars(line, "//")) {
        }//check if an int variable is defined
        else if (FirstChars(line, "int ")) {
            printf("Found an int\n");

            //extract the variable name
            char name[16] = {0};
            //strcpy(name, line + 4);
            int i = 4, k = 0;
            while(i < strlen(line)-1) {
                name[k++] = line[i++];
            }

            CreateInteger(1, name);
        }//check if the line is an IF check
        else if (FirstChars(line, "if ")) {
            //printf("Found an if\n");
        }
        else if (FirstChars(line, "{")) {}
        else if (FirstChars(line, "}")) {}
        else if (FirstChars(line, "fopen")) {}
        else if (FirstChars(line, "for")) {}
        else if (FirstChars(line, "while")) {}
        else if (FirstChars(line, "printc")) {}
        else if (FirstChars(line, "printf")) {
        }//check if we are modifying a variable
        else if (!var) {
            /*
                an expression should be of form: x = a + b
                so we have a var, a space, an equal, a value, a space, 
            the operation, a space, and the other value

                first, extract the variable name, until the first space.
                check if the name is found in the struct
                if it is, "var" becomes true
                if true, go past the space, 
            past the equal and past the other space
                after that, we have the expression
                save the expression in another char array, which we will
            feed into a function to calculate everything
            */
            
            //count until we hit a char != ' '
            int i = 0, k = 0;
            char name[16] = {0};
            //reset char array
            for (int i = 0; i < 16; i++)
                name[i] = '\0';
            while (line[i++] == ' ' && i < strlen(line)) {} //will end at i+1
            i--;
            while (line[i] != ' ' && i < strlen(line)) {
                name[k++] = line[i++];
            }//will end at i+1 (on the first space after the name)
            //printf("name %s\n", name);

            //now we need to check if the name is found in the struct
            var = CheckVar(variable, name);
            if (var >= 0) {
                printf("Variable %.*s is defined.\n", strlen(name), name);//after the name, we have " = " so 3 chars. 
                //after that, we are in the expression
                i += 3; //MIGHT BE 3 NEED TO CHECK

                //we are now in the expression and we will save it in an array
                k = 0;
                char expression[1023] = {0};
                while(i < strlen(line)) {
                    expression[k++] = line[i++];
                }
                printf("Found expression %.*s = %.*s on line %d.\n", 
                    strlen(name), name, 
                    strlen(expression)-1, expression, 
                    totalLine);
                
                int value = CalculateExpr(expression);
                if (value != -1) {
                    variable[var].value = value;
                    printf("Assigned value %d to variable %.*s.\n", 
                        variable[var].value, 
                        strlen(variable[var].name), variable[var].name);
                }

                printf("\n");
            }
            else {
                printf("Variable %.*s is not defined. Error on line %d.\n",
                    strlen(name), name, totalLine);

                printf("name <%s>\n", name);
                //exit(200);
            }
        
        }//if the line does not match any of the defined operations and is NOT empty and NOT a comment, error 1000
        else {
            if (!FirstChars(line, "//") && !FirstChars(line, "\n")) {
                printf("Error on line %d.", totalLine);
                exit(1000);
            }
        }
        
    }

    //for debug
    printf("Total lines: %d\n", totalLine);
    printf("Code lines: %d\n", codeLine);
    printf("Comment lines: %d\n", commentLine);

    return 0;
}