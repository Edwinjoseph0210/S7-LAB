#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char keywords[8][10] = {
    "int", "float", "char", "if", "else", "while", "for", "return"
};

char operators[] = "+-*/%=<>&|!";
char punctuators[] = "();{},[]";

int isKeyword(char *buffer) {
    for (int i = 0; i < 8; i++) {
        if (strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int main() {
    char ch, buffer[15];
    FILE *fp;
    int i;
    fp = fopen("sample.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    i = 0;
    while ((ch = fgetc(fp)) != EOF) {
        // Handle comments starting with //
        if (ch == '/') {
            char next = fgetc(fp);
            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            } else {
                ungetc(next, fp);  // Not a comment, put back
            }
        }

        // Check if ch is operator
        if (strchr(operators, ch) != NULL) {
            // First, print any token buffered so far
            if (i != 0) {
                buffer[i] = '\0';
                if (isKeyword(buffer))
                    printf("Keyword: %s\n", buffer);
                else
                    printf("Identifier: %s\n", buffer);
                i = 0;
            }
            printf("Operator: %c\n", ch);

        // Check if ch is punctuator (including brackets)
        } else if (strchr(punctuators, ch) != NULL) {
            // Print buffered token first
            if (i != 0) {
                buffer[i] = '\0';
                if (isKeyword(buffer))
                    printf("Keyword: %s\n", buffer);
                else
                    printf("Identifier: %s\n", buffer);
                i = 0;
            }
            printf("Punctuator: %c\n", ch);

        } else if (isalnum(ch)) {
            // Build token
            buffer[i++] = ch;

        } else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == ';' || ch == ',') {
            // Token delimiters
            if (i != 0) {
                buffer[i] = '\0';
                if (isKeyword(buffer))
                    printf("Keyword: %s\n", buffer);
                else
                    printf("Identifier: %s\n", buffer);
                i = 0;
            }
        }
    }

    // Print any remaining token in buffer at EOF
    if (i != 0) {
        buffer[i] = '\0';
        if (isKeyword(buffer))
            printf("Keyword: %s\n", buffer);
        else
            printf("Identifier: %s\n", buffer);
    }

    fclose(fp);
    return 0;
}

