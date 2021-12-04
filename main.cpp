#include <stdlib.h>
#include <stdio.h>
#include <cstring>

//there are variables in our pseudocode and we need to interpret them too.
//use this struct to define the data
struct data {
    int value;          //value of the variable
    char name[16];      //the name of our variable. max 16 chars
    bool isVar = false; //check if the spot is taken
    bool isNeg = false; //check if nr is negative
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
void CountLines(int &totalLine, int &codeLine, int &commentLine, char line[]) {
    totalLine++;
    if (FirstChars(line, "//"))
        commentLine++;
    else
        codeLine++;
}

//check if a var is part of the struct. returns pos in struct if found, (-1) if not found.
int CheckVar(data vars[], char key[]) {
    //printf("/////////////////////////////////\n");
    for (int i = 0; i < 255; i++) {
        //printf("name in struct: <%s>\n", vars[i].name);

        if (strcmp(vars[i].name, key) == 0)
            return i;
    }
    //printf("///////////////////////////////////\n");
    return -1;
}

//check if a var is a number and return its positive value. return -1 if not
int IsNumber(char expression[]) {
    char numbers[] = {"0123456789"};
    bool isNum = true;
    int i = 0;
    int value = 0;

    if (expression[0] == '-') {
        i++;
    }
    while (i < strlen(expression)) {
        if (!strchr(numbers, expression[i++])) {
            isNum = false;
            return -1;
        }
        i++;
    }
    if (isNum) {
        for (int i = 0; i < strlen(expression); i++) {
            value = value * 10 + (expression[i] - 48); 
        }

        //printf("Assigned value %d.\n", value);
        return value;
    }

    return -1;
}

int GetValue(char var[]) {
    int value;
    if (IsNumber(var) >= 0) {
        value = IsNumber(var);
    }
    else if (CheckVar(variable, var) >= 0) {
        int poz = CheckVar(variable, var);
        value = variable[poz].value;
        if (variable[poz].isNeg)
            value *= (-1);
    }

    return value;
}

//function to calculate a given expression
int CalculateExpr(char expression[]) {
    int value = 0;
    int valueVar1 = 0;
    int valueVar2 = 0;
    char op;
    char var1[255] = {0};
    char var2[255] = {0};
    bool isVal1 = false;
    bool isVal2 = false;

    //get var1
    int i = 0;
    int k = 0;
    int leftPar1 = 0, rightPar1 = 0;
    if (expression[i] == '(') {
        while (i < strlen(expression)) {
            if (expression[i] == '(')
                leftPar1++;
            if (expression[i] == ')')
                rightPar1++;
            if (leftPar1 == rightPar1)
                break;
            i++;
        }
        i++;
    }
    if (leftPar1 && rightPar1) {
        for (int j = 1; j < i-1; j++)
            var1[j-1] = expression[j];
        printf("Paranth: %s\n", var1);
    }
    else {
        while (i < strlen(expression) && expression[i] != ' ') {
            var1[i] = expression[i];
            i++;
        }
        if (IsNumber(var1) || CheckVar(variable, var1)) {
            valueVar1 = GetValue(var1);
            isVal1 = true;
            //printf("isVal1\n");
            if (strlen(var1) == strlen(expression)) {
                return valueVar1;
            }
        }
    }

    //get operand
    while (expression[i] == ' ')
        i++;
    op = expression[i];
    
    while (expression[i] != ' ')
        i++;
    i++;
    //if operand is * or / we need an intermediate value
    if (op == '*' || op == '/') {
        while (op == '*' || op == '/') {
            //printf("helpmeee\n");
            //first, if var2 is just a number or simple var, do a simple calc
            char aux[255] = {0};
            strcat(aux, expression+i);
            if (IsNumber(aux) >= 0 || CheckVar(variable, aux) >= 0) {
                strcpy(var2, aux);
                valueVar2 = GetValue(var2);
                isVal2 = true;
                if (IsNumber(var1) == -1 && CheckVar(variable, var1) == -1) {
                    valueVar1 = CalculateExpr(var1);
                    isVal1 = true;
                }
                //printf("Var1 is %s, value %d. Var2 is %s, value %d\n", var1, valueVar1, var2, valueVar2);
                if (op == '*')
                    return valueVar1 * valueVar2;
                else if (op == '/')
                    return valueVar1 / valueVar2;
            }

            while(expression[i] == ' ')
                i++;
            char varAux[255] = {0};
            int leftParAux = 0, rightParAux = 0;
            k = i;
            if (expression[i] == '(') {
                while (i < strlen(expression)) {
                    if (expression[i] == '(')
                        leftParAux++;
                    if (expression[i] == ')')
                        rightParAux++;
                    if (leftParAux == rightParAux)
                        break;
                    i++;
                }
                if (leftParAux && rightParAux) {
                    int j = 0;
                    while (k < i) {
                        varAux[j++] = expression[k++]; 
                    }
                    i++;

                    strcat(var1, varAux);
                    isVal1 = false;
                }
            }
            else {
                char varAux[255] = {0};
                //printf("REST1 %s\n", expression+i);
                while (expression[i] == ' ')
                    i++;
                int k = 0;
                while (expression[i] != ' ') {
                    varAux[k] = expression[i];
                    k++;
                    i++;
                }
                char help[1];
                help[0] = op;
                strcat(var1, " ");
                var1[strlen(var1)] = op;
                strcat(var1, " "); 
                strcat(var1, varAux);
                i++;
                isVal1 = false;
                //printf("HELP %s\n", var1);
            }
            while (expression[i] != ' ')
                i++;
            i--;
            //printf("%c%c%c\n", expression[i-1], expression[i], expression[i+1]);
            op = expression[i];
            i++;
            while (expression[i] != ' ')
                i++;
            i++;
        }
    }
    printf("First var: %s\n", var1);
    printf("Operand: %c\n", op);

    strcat(var2, expression+i);
    printf("Second var: %s\n", var2);

    if (!isVal1) {
        valueVar1 = CalculateExpr(var1);
        //printf("var1?\n");
    }
    if (!isVal2) {
        valueVar2 = CalculateExpr(var2);
    }
    //printf("Value var1: %d\n", valueVar1);
    //printf("Value var2: %d\n", valueVar2);
    switch (op) {
        case '*':
            value = valueVar1 * valueVar2;
            return value;
        case '/':
            value = valueVar1 * valueVar2;
            return value;
        case '+':
            value = valueVar1 + valueVar2;
            return value;
        case '-':
            value = valueVar1 - valueVar2;
            return value;
    }

    return 0;
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
                printf("Variable %s is defined and is equal to %d.\n", 
                    name, variable[var].value);//after the name, we have " = " so 3 chars. 
                //after that, we are in the expression
                i += 3; //MIGHT BE 3 NEED TO CHECK

                //we are now in the expression and we will save it in an array
                k = 0;
                char expression[1023] = {0};
                while(i < strlen(line)-1) {
                    expression[k++] = line[i++];
                }
                printf("Found expression %.*s = %.*s on line %d.\n", 
                    strlen(name), name, 
                    strlen(expression), expression, 
                    totalLine);
                
                int value = CalculateExpr(expression);
                if (value != -1) {
                    variable[var].value = value;
                    printf("Assigned value %d to variable %.*s.\n", 
                        variable[var].value, 
                        strlen(variable[var].name), variable[var].name);
                }

                printf("\n\n");
            }
            else {
                printf("Variable %.*s is not defined. Error on line %d.\n",
                    strlen(name), name, totalLine);

                printf("name <%s>\n", name);
                exit(200);
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