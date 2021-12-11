#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;
char *keywords[33] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
void expr(void);
void term(void);
void factor(void);
void error(void);
/* Character classes */
#define LETTER 0 //[a-zA-Z]
#define DIGIT 1 //[0-9]
#define SINGLEQUOTE 2 //[\']
#define DOUBLEQUOTE 3 //[\"]
#define DECIMAL 4 //[\.]
#define UNDERSCORE 5
#define UNKNOWN 99 //[^a-zA-Z0-9]

/* Token codes */
#define FLOAT_LIT 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define DECIMAL_POINT 27

/******************************************************/
/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	lex();
	 } while (nextToken != EOF);

    expr();
	 }
	 return 0;
}


/******************************************************/
/* lookup - a function to look up operators and
 *  parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
		 case '=':
			 addChar();
			 nextToken = ASSIGN_OP;
			 break;
		 case '.':
			 addChar();
			 nextToken = DECIMAL_POINT;
			 break;
		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 *  input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
     else if (nextChar == '_')
      charClass = UNDERSCORE;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}

/******************************************************/
/* getNonBlank - a function to call getChar until it
 *  returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 *  expressions */
int lex(void) {
	 lexLen = 0;
	 getNonBlank();
	 switch (charClass) {
		/* User Defined Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE) {
				 addChar();
				 getChar();
			 }
			 nextToken = IDENT;
			 break;
     case UNDERSCORE:
       addChar();
       getChar();
       while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE) {
				 addChar();
				 getChar();
			 }
			 nextToken = IDENT;
			 break;
		/* Integer & Float literals */
		 case DIGIT:
			 addChar();
			 getChar();
			 while (charClass == DIGIT) {
				 addChar();
				 getChar();
         if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
            addChar();
            getChar();
            nextToken = FLOAT_LIT;
            break;
         }
         if(nextChar == 'E' || nextChar == 'e'){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
            if(nextChar == '-'){
              addChar();
              getChar();
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
            } else if(charClass == DIGIT){
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
			      }
         }
       }
       if (charClass == UNKNOWN && nextChar == '.'){
         addChar();
				 getChar();
         while (charClass == DIGIT) {
				  addChar();
				  getChar();
          if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
            addChar();
            getChar();
            nextToken = FLOAT_LIT;
            break;
          }
          if(nextChar == 'E' || nextChar == 'e'){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
            if(nextChar == '-'){
              addChar();
              getChar();
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
            } else if(charClass == DIGIT){
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
            }
          }
         }
         nextToken = FLOAT_LIT;
       } else {
			 nextToken = INT_LIT;
       }
		 	break;
       
		 case UNKNOWN:
        if (nextChar == '.'){
          addChar();
          getChar();
          printf("I am here");
          if(charClass == DIGIT){
            addChar();
            getChar();
            while(charClass == DIGIT){
              addChar();
              getChar();
              if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                addChar();
                getChar();
                nextToken = FLOAT_LIT;
                break;
              }
              else if(nextChar == 'E' || nextChar == 'e'){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
                if(nextChar == '-'){
                  addChar();
                  getChar();
                  while(charClass == DIGIT){
                    addChar();
                    getChar();
                    if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                      addChar();
                      getChar();
                      nextToken = FLOAT_LIT;
                      break;
                    }
                  }
                  nextToken = FLOAT_LIT;
                  break;
                } else if(charClass == DIGIT){
                  while(charClass == DIGIT){
                    addChar();
                    getChar();
                    if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                      addChar();
                      getChar();
                      nextToken = FLOAT_LIT;
                      break;
                    }
                  }
                  nextToken = FLOAT_LIT;
                  break;
			          }
              }
              nextToken = FLOAT_LIT;
            }
          } else {
            nextToken = DECIMAL_POINT;
          } 
        } else {
          lookup(nextChar);
          getChar();
        }
			 break;
			/* EOF */
		case EOF:
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
	 printf("Next token is: %d, Next lexeme is %s\n",
	 nextToken, lexeme);
	 return nextToken;
} /* End of function lex */



void expr(void) {
	 term();
	 while (nextToken == ADD_OP || nextToken == SUB_OP) {
		 lex();
		 term();
	 }
}

/* term
 *  Parses strings in the language generated by the rule:
 *   <term> -> <factor> {(* | /) <factor>}
 *    */
void term(void) {
	 factor();
	 while (nextToken == MULT_OP || nextToken == DIV_OP) {
		 lex();
		 factor();
	 }
}

/* factor
 *  Parses strings in the language generated by the rule:
 *   <factor> -> id | int_constant | ( <expr> )
 *    */
void factor(void) {
	 if (nextToken == IDENT || nextToken == INT_LIT || nextToken == FLOAT_LIT)
		 lex();
	 else { if (nextToken == LEFT_PAREN) {
		 lex();
		 expr();
		 if (nextToken == RIGHT_PAREN)
		 	lex();
		 else
		 	error();
		 }
		 else
		 error();
	 }
	 printf("Exit <factor>\n");
}


void error(void){
 exit(1);
}

