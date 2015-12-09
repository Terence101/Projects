/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Tokenizer struct
 */

struct TokenizerT_ {
    char *token;
    int start;
    int end;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.  95
 * Else it returns NULL.
 *
 */

TokenizerT *TKCreate( char * ts ) { /*The create token function creates a structure. The structure holds the token string, type string,
and the positions for the start and end of a token.*/
    TokenizerT *tok = malloc(sizeof(struct TokenizerT_));
    tok->token = ts;

  return tok;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 */

void TKDestroy( TokenizerT * tk ) { /*The destroy token function frees the memory of the token that was created from the user
input.*/
  free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 */

char *TKGetNextToken( TokenizerT * tk ) { /*The get next token traverses the token created from the user input and breaks it down into
specific token types and sends it to the main function for outputting. This function uses finite
state machines implementation. The finite state machines include words, decimal constant,
octal constant, hex constant, floats, and all c operators.*/
    int i;
    char *keep;
    char *type;
    char *string = tk->token;
    int length = strlen(string);
    tk->start = tk->end;
    type=malloc(30);

    typedef enum {identify,alpha,digit,hex,octal,floating,operators} state;
    state currentState = identify;

    typedef enum {True,False} boolean;
    boolean stop = False;

    for(i=tk->start;i<length; i++){
        
        switch(currentState){
            case identify:
                if(isblank(string[i])!=0){
                    if(i==length-1){
                        tk->end++;
                        return NULL;
                    }
                    tk->end++;
                    tk->start++;
                    continue;
                }
                else if(isalpha(string[i])!=0){
                    currentState = alpha;
                    i--;
                    continue;
                }
                else if(isdigit(string[i])!=0){
                    currentState = digit;
                    i--;
                    continue;
                }
                else if(ispunct(string[i])!=0){
                    currentState = operators;
                    i--;
                    continue;
                }
            case alpha:
                  if(isalnum(string[i])!=0){
                    tk->end++;
                    currentState = alpha;

                    if(i==length-1){
                        stop = True;
                        currentState=alpha;
                        break;
                    }

                    continue;
                }else{
                    stop = True;
                    break;
                }

            case digit:

                if(string[i]=='.'){
                    i++;
                    if(isdigit(string[i])!=0){
                        currentState=floating;
                        tk->end++;
                        i--;
                        continue;
                     }

                     i-=2;
                     stop=True;
                     break;
                }

                if(isdigit(string[i])!=0){
                    tk->end++;
                    currentState = digit;
                    
                    if(string[i]=='0'){
                            if(tk->start!=i){
                                i--;
                                tk->end--;
                                stop=True;
                                break;
                            }

                            i++;
                            if(string[i]=='x' || string[i]=='X'){
                                if(i==length-1){
                                    currentState=digit;
                                    stop=True;
                                    i--;
                                    break;
                                }
                                tk->end++;
                                currentState=hex;
                            }
                            else if(string[i]>48 && string[i]<56){
                                currentState= octal;
                                tk->end++;
                            }else{
                                i--;
                            }
                    }

                    if(i==length-1){
                        stop = True;
                        break;
                    }

                    continue;
                }
                else{
                    stop = True;
                    break;
                } 

            case floating:
                if(i==length-1){
                    if(string[i]=='e'||string[i]=='E'){
                        stop=True;
                        break;
                    }
                    if(string[i]=='-'){
                        i--;
                        tk->end--;
                        stop=True;
                        break;
                    }
                }

                if(isdigit(string[i])!=0){
                    tk->end++;
                    currentState=floating;

                    if(i==length-1){
                        stop = True;
                        break;
                    }

                    continue;
                }
                else if(string[i]=='E' || string[i]=='e' || string[i]=='-'){
                    if(string[i+1]=='-'){
                        tk->end++;
                        currentState=floating;
                        continue;
                    }
                    if((isdigit(string[i+1]==0)) || (isblank(string[i+1])) || (string[i+1]=='+') || (string[i+1]=='*') || (string[i+1]=='/') || (string[i+1]=='/') || (string[i+1]=='!') || (string[i+1]=='@') || (string[i+1]=='#') || (string[i+1]=='$') || (string[i+1]=='%') || (string[i+1]=='^')|| (string[i+1]=='&') || (string[i+1]=='(') || (string[i+1]==')') || (string[i+1]=='`') || (string[i+1]=='~') || (string[i+1]=='|') || (string[i+1]=='[') || (string[i+1]==']') || (string[i+1]=='{') || (string[i+1]=='}') || (string[i+1]==';') || (string[i+1]==':') || (string[i+1]=='"') || (string[i+1]==',') || (string[i+1]=='.') || (string[i+1]=='?') || (string[i+1]=='<') || (string[i+1]=='>')){
                        if((string[i]=='e' || string[i]=='E') && string[i+1]){
                            i--;
                            stop=True;
                            break; 
                        }
                        if(string[i]=='-')
                            i--;
                            tk->end--;
                            stop=True;
                            break; 
                    }
                    tk->end++;
                    currentState=floating;
                    continue;
                }
                else{
                    stop = True;
                    break;
                }


            case hex:
                if(isxdigit(string[i])!=0){
                    tk->end++;
                    currentState = hex;
                    
                    if(i==length-1){
                        stop=True;
                        break;
                    }
                    continue;
                }
                    else{
                        stop=True;
                        break;
                    }
                
             case octal:
                if(string[i]>48 && string[i]<56){
                    tk->end++;
                    currentState = octal;
                    
                    if(i==length-1){
                        stop = True;
                        break;
                    }
                    continue;
                }else{
                    stop = True;
                    break;
                } 

            case operators:

                switch(string[i]){
                    case '+':
                        tk->end++;
                        i++;
                        if(string[i]=='+'){
                            tk->end++;
                            strcpy(type,"increment \"");
                            stop = True;
                            break;
                        }else if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"plusequals \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"plus \"");
                            stop = True;
                            break;
                        }
                    case '-':
                        tk->end++;
                        i++;
                        if(string[i]=='-'){
                            tk->end++;
                            strcpy(type,"decrement \"");
                            stop = True;
                            break;
                        }else if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"minusequals \"");
                            stop = True;
                            break;
                        }else if(string[i]=='>'){
                            tk->end++;
                            strcpy(type,"structurepointer \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"minus \"");
                            stop = True;
                            break;
                        }
                    case '*':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"multiplyequals \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"asterick \"");
                            stop = True;
                            break;
                        }
                    case '/':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"divideequals \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"forward slash \"");
                            stop = True;
                            break;
                        }
                    case '=':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"equality \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"equal\"");
                            stop = True;
                            break;
                        }
                    case '&':
                        tk->end++;
                        i++;
                        if(string[i]=='&'){
                            tk->end++;
                            strcpy(type,"and \"");
                            stop = True;
                            break;
                        }
                        else if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"ampersandeqauls \"");
                            stop = True;
                            break;
                        }
                        else{
                            i--;
                            strcpy(type,"ampersand \"");
                            stop = True;
                            break;
                        }
                    case '|':
                        tk->end++;
                        i++;
                        if(string[i]=='|'){
                            tk->end++;
                            strcpy(type,"or \"");
                            stop = True;
                            break;
                        }
                        else if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"vertical barequals \"");
                            stop = True;
                            break;
                        }
                        else{
                            i--;
                            strcpy(type,"vertical bar \"");
                            stop = True;
                            break;
                        }
                    case '!':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"not equal \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"exclamation mark \"");
                            stop = True;
                            break;
                        }
                     case '^':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"caretequals \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"caret\"");
                            stop = True;
                            break;
                        }    

                     case '>':
                        tk->end++;
                        i++;
                        if(string[i]=='>'){
                            tk->end++;
                            i++;
                            if(string[i]=='='){
                                tk->end++;
                                strcpy(type,"right shiftequals \"");
                                stop=True;
                                break;
                            }
                            else{
                                i--;
                                strcpy(type,"right shift \"");
                                stop = True;
                                break;
                             }
                        }
                        else{
                            i--;
                            strcpy(type,"great than\"");
                            stop = True;
                            break;
                        }
                        case '<':
                        tk->end++;
                        i++;
                        if(string[i]=='<'){
                            tk->end++;
                            i++;
                            if(string[i]=='='){
                                tk->end++;
                                strcpy(type,"left shiftequals \"");
                                stop=True;
                                break;
                            }
                            else{
                                i--;
                                strcpy(type,"left shift \"");
                                stop = True;
                                break;
                             }
                        }
                        else{
                            i--;
                            strcpy(type,"less than\"");
                            stop = True;
                            break;
                        }
                        case '%':
                        tk->end++;
                        i++;
                        if(string[i]=='='){
                            tk->end++;
                            strcpy(type,"muoduloequals \"");
                            stop = True;
                            break;
                        }else{
                            i--;
                            strcpy(type,"modulo \"");
                            stop = True;
                            break;
                        }


                    case '[':
                        tk->end++;
                        strcpy(type,"left bracket \"");
                        stop = True;
                        break;
                    case ']':
                        tk->end++;
                        strcpy(type,"right bracket \"");
                        stop = True;
                        break;
                    case '(':
                        tk->end++;
                        strcpy(type,"left parenthesis \"");
                        stop = True;
                        break;
                    case ')':
                        tk->end++;
                        strcpy(type,"right parenthesis\"");
                        stop = True;
                        break;
                    case '{':
                        tk->end++;
                        strcpy(type,"left brace \"");
                        stop = True;
                        break;
                    case '}':
                        tk->end++;
                        strcpy(type,"right brace \"");
                        stop = True;
                        break;
                    case '.':
                        tk->end++;
                        strcpy(type,"period \"");
                        stop = True;
                        break;
                    case ',':
                        tk->end++;
                        strcpy(type,"comma \"");
                        stop = True;
                        break;
                    case ':':
                        tk->end++;
                        strcpy(type,"colon \"");
                        stop = True;
                        break;
                    case ';':
                        tk->end++;
                        strcpy(type,"semi-colon\"");
                        stop = True;
                        break;
                    case '$':
                        tk->end++;
                        strcpy(type,"dollar sign \"");
                        stop = True;
                        break;
                    case '#':
                        tk->end++;
                        strcpy(type,"hashtag \"");
                        stop = True;
                        break;
                    case '?':
                        tk->end++;
                        strcpy(type,"question mark \"");
                        stop = True;
                        break;
                    case '~':
                        tk->end++;
                        strcpy(type,"tilde \"");
                        stop = True;
                        break;
                    case '@':
                        tk->end++;
                        strcpy(type,"at sign \"");
                        stop = True;
                        break;
                    

                }
        }

            if(stop==True){
                switch(currentState){
                  case alpha:
                    strcpy(type,"word \"");
                    break;
                  case digit:
                    strcpy(type,"decimal constant \"");
                    break;
                  case hex:
                    strcpy(type,"hex constant \"");
                    break;
                  case octal:
                    strcpy(type,"octal constant \"");
                    break;
                  case floating:
                    strcpy(type,"float \"");
                    break;
                  case identify:
                    break;
                  case operators:
                    break;
                }
                break;
            }

    }

  string = NULL;
  string = malloc(sizeof(length)*10);
  keep=malloc(sizeof(length));

  strcpy(string,type);
  strncpy(keep,tk->token+tk->start,tk->end-tk->start);
  strcat(string,keep);
  strcat(string,"\"");

  free(keep);
  free(type);

  return string;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) { /*The main function receives user input and then outputs the types and tokens contained in the
user input.*/

    if (argc != 2){
        printf("%s\n","User either entered no arguements or too many");
        exit(0);
    }
    int length = strlen(argv[1]);

    if(length==0){
        printf("%s\n", "empty string");
        return 0;
    }

    int i;
    int count;
    count=0;

    for(i=0;i<length;i++){
        if(isblank(argv[1][i])!=0){
            count++;
        }
    }

    if(count==length){
        printf("%s\n","string only contains blanks" );
        return 0;
    }


    char *answer;
    TokenizerT *temp = TKCreate(argv[1]);
    temp->end = 0;
  

    while(temp->end<length){
        answer = TKGetNextToken(temp);
        if(temp!=NULL){
            if(answer!=NULL){
                printf("%s\n", answer);
                free(answer);
            }
            else{
                continue;
            }
        }
    }
    TKDestroy(temp);

  return 0;
}
