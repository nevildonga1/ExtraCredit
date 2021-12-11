#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOKENS 100
#define MAXTOKENLEN 64

enum type_tag { IDENTIFIER, QUALIFIER, TYPE };
struct token {
    char type;
    char string[MAXTOKENLEN];
};

int top = -1;
struct token stack[MAXTOKENS];    
struct token t;

#define pop stack[top--]
#define push(s) stack[++top]=s

enum type_tag classify_string(void)
{
    char *s = t.string;
    if (!strcmp(s, "const"))
    {
        strcpy(s, "read-only");
        return QUALIFIER;
    }
    if (!strcmp(s, "volatile")) return QUALIFIER;
    if (!strcmp(s, "void")) return TYPE;
    if (!strcmp(s, "char")) return TYPE;
    if (!strcmp(s, "signed")) return TYPE;
    if (!strcmp(s, "unsigned")) return TYPE;
    if (!strcmp(s, "short")) return TYPE;
    if (!strcmp(s, "int")) return TYPE;
    if (!strcmp(s, "long")) return TYPE;
    if (!strcmp(s, "float")) return TYPE;
    if (!strcmp(s, "double")) return TYPE;
    if (!strcmp(s, "struct")) return TYPE;
    if (!strcmp(s, "union")) return TYPE;
    if (!strcmp(s, "enum")) return TYPE;
    return IDENTIFIER;
}

void gettoken(void)
{
    char *p = t.string;

    while ((*p = getchar()) == ' ');

    if (isalnum(*p))
    {
        while (isalnum(*++p = getchar()));
        ungetc(*p, stdin);
        *p = '\0';
        t.type = classify_string();
        return;
    }

    if (*p == '*')
    {
        strcpy(t.string, "pointer to");
        t.type = '*';
        return;
    }
    t.string[1] = '\0';
    t.type = *p;
    return;
}

void read_to_first_identifier()
{
    gettoken();
    while (t.type != IDENTIFIER)
    {
        push(t);
        gettoken();
    }
    printf("%s is ", t.string);
    gettoken();
}

void deal_with_arrays()
{
    while (t.type == '[')
    {
        printf("array ");
        gettoken();
        if (isdigit(t.string[0]))
        {
            printf("0..%d ", atoi(t.string) - 1);
            gettoken();
        }
        gettoken(); 
        printf("of "); 
    }
}

void deal_with_function_args()
{
    while (t.type != ')')
    {
        gettoken();
    }
    gettoken(); 
    printf("function returning ");
}

void deal_with_pointers()
{
    while (stack[top].type == '*')
    {
        printf("%s ", pop.string);
    }
}

void deal_with_declarator()
{
    
    switch (t.type)
    {
    case '[': deal_with_arrays(); break;
    case '(': deal_with_function_args();
    }
    
    deal_with_pointers();

    
    while (top >= 0)
    {    
        if (stack[top].type == '(')
        {
            pop;
            gettoken(); 
            deal_with_declarator(); 
        }
        else
        {
            printf("%s ", pop.string);
        }
    }
}

int main()
{
    read_to_first_identifier();
    
    deal_with_declarator();
    printf("\n");
    return 0;
}
