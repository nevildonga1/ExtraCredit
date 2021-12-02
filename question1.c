#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


/* Global declarations */ /* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int lineNumber = 0;
int currentIndexCount;
FILE *fp, *fopen();


/* Function declarations */ 
void addChar();
void getChar();
void getNonBlank();
int lex();
void term();
void factor();
void expr();
void error();
void ifstmt();
void while_loop();
void for_loop();
void block();
void retur();
void assignment();
void do_while();
void swit();

int getErrorCharacter();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define NEWLINE 101
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define FOR_CODE 30
#define IF_CODE 31
#define WHILE_CODE 33
#define SEMICOLON 13
#define ELSE_CODE 14
#define LEFT_BRACE 27
#define RIGHT_BRACE 28
#define RETURN 29
#define DO 32
#define SWITCH 49
#define CASE 50
#define DEFAULT 51
#define COLON 52
#define BREAK 53


/* Global variables */
size_t len = 0;
ssize_t read;
char * line = NULL;

/******************************************************/
/* main driver */
int main() {
/* Open the input data file and process its contents */ 
  fp = fopen("text.in", "r");
  if (fp == NULL) {
    fprintf(stderr,"fopen() failed in file %s at line # %d", __FILE__,__LINE__);
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &len, fp)) != -1) {
    lineNumber += 1;
    currentIndexCount = 0;
    getChar(); 
    do {
      lex();
      expr();
    } while (nextToken != EOF);
    printf("\n\n");
  }
}

/*****************************************************/ 
/* lookup - a function to lookup operators and parentheses
 *  * and return the token */ 
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

    case '{':
      addChar();
      nextToken = LEFT_BRACE; 
      break;

    case '}':
      addChar();
      nextToken = RIGHT_BRACE; 
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
	case ';':
      addChar(); 
      nextToken = SEMICOLON; 
      break;

    case '\n':
      addChar();
      nextToken = NEWLINE;
      break;

    case ':':
      addChar();
      nextToken = COLON;
      break;

    default: 
      addChar(); 
      nextToken = EOF; 
      break;
  }
  return nextToken; 
}

/*****************************************************/ 
/* addChar - a function to add nextChar to lexeme */ 
void addChar() {
  if (lexLen <= 98) { 
    lexeme[lexLen++] = nextChar; 
    lexeme[lexLen] = 0;
  } 
  else
    printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of
 *  * input and determine its character class */ 
void getChar() {
  if (line[currentIndexCount] == '\n' || line[currentIndexCount] == '\0') {
    charClass = EOF;
  } else {
    nextChar = line[currentIndexCount];
    currentIndexCount += 1;
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else charClass = UNKNOWN;
  }
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
 *  * returns a non-whitespace character */ 
void getNonBlank() {
  while (isspace(nextChar)) 
    getChar();
} 

/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 *  * expressions */ 
int lex() {
  lexLen = 0; 
  getNonBlank();
  switch (charClass) {
  /* Parse identifiers */ 
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar(); 
      }
      nextToken = IDENT; 
      break;
  /* Parse integer literals */ 
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar(); 
      }
      nextToken = INT_LIT; 
      break;
    /* Parentheses and operators */ 
    case UNKNOWN:
      lookup(nextChar); 
      getChar();
      break;
    /* EOF */
    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
    /* NEWLINE */
    case NEWLINE:
      nextToken = NEWLINE;
      lexeme[0] = 'N';
      lexeme[1] = 'E';
      lexeme[2] = 'W';
      lexeme[3] = 'L';
      lexeme[4] = 'I';
      lexeme[5] = 'E';
  } 
      /* End of switch */
  printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme); 
  return nextToken;
}  
/* End of function lex */

/* expr
 *  * Parses strings in the language generated by the rule:
 *   * <expr> -> <term> {(+ | -) <term>}
 *    * */
void expr() { 
  printf("Enter <expr>\n");
 /* Parse the first term */
  term();
 /* As long as the next token is + or -, get
 *  *     the next token and parse the next term */
  while (nextToken == ADD_OP || nextToken == SUB_OP) { 
    lex();
    term(); 
  }
  printf("Exit <expr>\n");
}  /* End of function expr */


/* term
 *  * Parses strings in the language generated by the rule:
 *   * <term> -> <factor> {(* | /) <factor>)
 *    * */

void term() { 
  printf("Enter <term>\n");
/* Parse the first factor */
  factor();
/* As long as the next token is * or /, get the
 *  *    next token and parse the next factor */
  while (nextToken == MULT_OP || nextToken == DIV_OP) { 
    lex();
    factor(); 
  }
  printf("Exit <term>\n");
}  /* End of function term */

/* factor
 *  * Parses strings in the language generated by the rule:
 *   * <factor> -> id | int_constant | ( <expr )
 *    * */
void factor() {
  printf("Enter <factor>\n");
  /* Determine which RHS */
  if (nextToken == IDENT || nextToken == INT_LIT)
  /* Get the next token */
  lex();
 /* If the RHS is ( <expr>), call lex to pass over the
 *  *     left parenthesis, call expr, and check for the right
 *   *         parenthesis */
  else {
    if (nextToken == LEFT_PAREN) {
      lex();
      expr();
      if (nextToken == RIGHT_PAREN)
        lex();
      else
        error();
      }  /* End of if (nextToken == ... */
 /* It was not an id, an integer literal, or a left
 *  *     parenthesis */
  else
    error();
   }  /* End of else */
  printf("Exit <factor>\n");;
}  /* End of function factor */

void error() {
  currentIndexCount -= 1;
  printf("--------------Syntax error occured in line %d in %d %c", lineNumber, getErrorCharacter(), line[currentIndexCount]);
  printf("------------------\n");
}

int getErrorCharacter() {
  return currentIndexCount;
}

/* ifstmt
 *  *  Parses strings in the language generated by the rule:
 *   *   <ifstmt> -> if '(' <boolexpr> ')' <statement> [else <statement>]
 *    *   */
void ifstmt(void) {
 if (nextToken != IF_CODE)
 	error();
 else {
	lex();
 	if (nextToken != LEFT_PAREN)
 		error();
 	else {
 		lex();
 		expr();
 		if (nextToken != RIGHT_PAREN)
 			error();
 		else {
 			lex();
		 	factor();
 			if (nextToken == ELSE_CODE) {
 				lex();
 				factor();
 			}
 		}
 	}
 }
}

/* while_loop
 *  *  Parses strings in the language generated by the rule:
 *   *   <while_loop> -> if '(' <boolexpr> ')' <statement>
 *    *   */
void while_loop() {
	if (nextToken != WHILE_CODE)
 		error();
 	else {
		lex();
 		if (nextToken != LEFT_PAREN)
 			error();
 		else {
 			lex();
 			expr();
 			if (nextToken != RIGHT_PAREN)
 				error();
 			else {
 				lex();
		 		factor();
 			}
 		}
 	}
}

/* for_loop
 *  *  Parses strings in the language generated by the rule:
 *   *   <for_loop> -> for '(' [<expression>]';'[<expression>]';'[<expression>] ')' <statement>
 *    *   */
void for_loop() {
	if (nextToken != FOR_CODE)
 		error();
 	else {
		lex();
 		if (nextToken != LEFT_PAREN)
 			error();
 		else {
 			lex();
 			expr();
 			if (nextToken != SEMICOLON)
 				error();
 			else {
 				lex();
		 		expr();
				if (nextToken != SEMICOLON)
 					error();
 				else {
 					lex();
		 			expr();
					if (nextToken != RIGHT_PAREN)
 						error();
 					else {
 						lex();
		 				factor();
 					}
 				}
 			}
 		}
 	}
}

/* block
 *  *  Parses strings in the language generated by the rule:
 *   *  <block> ‘{‘ { <statement> ; } ‘}’
 *    */
void block(void){
    if (nextToken != LEFT_BRACE)
 		  error();
    else {
        while (nextToken != RIGHT_BRACE) {
            expr();
            if (nextToken != SEMICOLON)
                error();
            lex();
        }
        if (nextToken != RIGHT_BRACE)
 		      error();
        lex();
    }
}

/* retur
 *  * Parses strings in the language generated by the rule:
 *   * <retur> -> return [<expr>]; 
 *   */
void retur() {
    if (nextToken != RETURN)
 		  error();
    else {
        while (nextToken != SEMICOLON) {
            expr();
        }
        if (nextToken != SEMICOLON)
 		      error();
        lex();
    }
}

/* assignment
 *  * Parses strings in the language generated by the rule:
 *   * <assignment> -- identifier -> <expr>
 *   */
void assignment() {
    expr();
}

/* do while
 * * Parses strings in the language generated by the rule:
 * * <do_while> -> do '{' [statement()] ';' '}' while '(' <expr> ')' ';'
 * */
void do_while(){
  if(nextToken != DO)
    error();
  else {
    if(nextToken != LEFT_BRACE)
      error();
    else {
      while (nextToken != RIGHT_BRACE){
        if (nextToken != SEMICOLON)
          error();
        else {
          expr();
          if (nextToken != WHILE_CODE)
            error();
          else {
            if (nextToken != LEFT_PAREN)
              error();
            else {
              while (nextToken != RIGHT_PAREN)
                expr();
                if (nextToken != SEMICOLON)
                  error();
                lex();
            }
          }
        }

      }
    }
  }
}

/* switch
 * * Parses strings in the language generated by the rule:
 * * <switch> -> switch '(' <expr> ')' '{ case <expr> ':' <statement> ';' <break> ';' '}'
 * */

void swit(){
  if (nextToken != SWITCH)
    error();
  else {
    if (nextToken != LEFT_PAREN)
      error();
    else{
      while(nextToken != RIGHT_PAREN)
        expr();
        if(nextToken != LEFT_PAREN)
          error();
        else {
          if (nextToken != CASE)
            error();
          else {
            while(nextToken != COLON) {
              expr(); 
              while(nextToken != SEMICOLON)
                expr();
                if (nextToken != BREAK)
                  error();
                else {
                  if (nextToken != SEMICOLON)
                    error();
                  else {
                    if (nextToken != RIGHT_BRACE)
                      error();
                    lex();
                  } 
                }
                
            }
          }
        }
    }
  }
}
