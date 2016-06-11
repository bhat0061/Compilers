/*File name: Scanner.c
Compiler: MS Visual Studio 2012
Author: [Archit Bhatia 040 772 687]
Course: CST 8152 – Compilers, Lab Section:012
Assignment:2
Date:March 12, 2015
Professor: Sv. Ranev
Purpose: Scanner is used to scan the characters which we are passing to the buffer and
		 tries to match it with the tokens passed by us and assigns a specific attribute and 
		 code to that token. This also generates error lexemes when some illegal strings
		 or lexes are passed on.
Function list: scanner_init()
			   mlwpar_next_token
			   get_next_state
			   char_class
			   aa_func02
			   aa_func03 
			   aa_func05 
			   aa_func08 
			   aa_func11 
			   aa_func12
*/
/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */
#include <math.h>    /*to use pow in the atool function*/
/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG
#define true 1
#define false 0

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern Buffer * str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/

/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */ 
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int iskeyword(char * kw_lexeme); /*keywords lookup functuion */
static long atool(char * lexeme); /* converts octal string to decimal value */
typedef enum {FALSE =0,TRUE} BOOL; 
/*
 * Purpose:				Returns the appropriate column number from the transition table
 * 						based on the input character
 * Author:				Archit Bhatia
 * Version:				2.2 (04-March-2015)
 * Called Functions:	isalpha, isdigit
 * Parameters:			c
 * type: char
 * Returns:				The column number in the transition table(int)
 */
int scanner_init(Buffer * sc_buf) {
  	if(b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_setmark(sc_buf, 0);
	b_retract_to_mark(sc_buf);
	b_reset(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
}
/*
 * Purpose:				This function is used to return a particular token for a
						character 
 * Author:				Archit Bhatia
 * Version:				10.24 (04-March-2015)
 * Called Functions:	getc()
						isspace
						islanum
						b_retract
						b_setmark
						b_getc
						get_next_state()
						b_getc_offset
						b_addc
						free()
						b_retract()
						b_size()
						strncpy
						strcpy
 * Parameters:			Buffer pointer
 * type:				Token
 * Returns:				Token
 */
Token mlwpar_next_token(Buffer * sc_buf)
{
   Token t; /* token to return after recognition */
   unsigned char c; /* input symbol */
   int state = 0; /* initial state of the FSM */
   short lexstart=0;  /*start offset of a lexeme in the input buffer */
   short lexend=0;    /*end   offset of a lexeme in the input buffer */
   int accept = NOAS; /* type of state - initially not accepting */                                     
/* 
lexstart is the offset from the beginning of the char buffer of the
input buffer (sc_buf) to the first character of the current lexeme,
which is being processed by the scanner.
lexend is the offset from the beginning of the char buffer of the
input buffer (sc_buf) to the last character of the current lexeme,
which is being processed by the scanner.

*/		int loop=0;
   				int i=0;
				int lexeme_length;
		char message[]="RUN TIME ERROR: ";/*Error message during runtime error*/
		short capacity =0;
		/* Label for goto continuation, removes need for infinite loop that generates warnings */
while(1){ //this might generate warnings
        c = b_getc(sc_buf);		/*GET THE NEXT SYMBOL FROM THE INPUT BUFFER */
		if (isspace(c))/*Checks for the space or tabs*/
	{
		/* If a new-line character was encountered, increment the line count */
		if (c == '\n' || c == '\r')
			line++;
		/* Skip and get the next character */
		//goto getNextCharacter;
	}
		else if(isalnum(c)){ /*Checks if the character is alphabet or a number*/
		  int k;
			short capacity;					/*capacity for lexeme buffer*/
			b_retract(sc_buf);				/*to get the offset of the first char*/
			b_setmark(sc_buf,b_getc_offset(sc_buf));		/*set mark of the first char of the lexeme */
			state=0;			/*machine state o*/
			 do{
				 c=b_getc(sc_buf);			/*because we retracted the buffer before now get the same char again to runn the machine*/
				 state=get_next_state(state,c,&accept);		/*get next state of the machine depending on te char we sent */
				  if(accept != NOAS) break;
				 } while(accept==NOAS);			/*although while doing the same thing like if but its works with if there*/
			lexstart = b_mark(sc_buf);
			 if(accept == ASWR) b_retract(sc_buf);/*retracts getc_offset if final state is retracting*/

			 lexend= (b_getc_offset(sc_buf));/*sets lexend to getc_offset*/
 			 capacity = lexend - lexstart;
			 b_retract_to_mark(sc_buf);
			 lex_buf =b_create(3000 ,12 ,'f');
			 if (lex_buf == NULL){/*Checks if the buffer hasnt worked properly*/
				  t.code = ERR_T;/*sets the code of the token*/
				  strcpy(t.attribute.err_lex,message); /*Copies the error messag\e in the error lex*/
				  ++scerrnum; /*Increments the runtime error counter*/
			  return t;
			  }
			 for(k=0;k<capacity ;k++){
				 b_addc(lex_buf,b_getc(sc_buf));/*Adds char from the scbuf to lexbuf*/
			 }
			 b_addc(lex_buf,'\0');/*Makes the lexbuf a Ctype string*/
			 t = aa_table[state](b_setmark(lex_buf,0));
			 b_destroy(lex_buf);/*destroys the buffer*/

			return t;
			 
	  }
		
		else {
		switch (c){/*switch case for other cases*/
			case '\0':
				t.code = SEOF_T;/*sets the code of the token*/
				return t;	
			case SEOF:/*SOURCE END OF FILE*/
				t.code=SEOF_T;
				return t;
			case '<':
				c = b_getc(sc_buf);		/*GET THE NEXT SYMBOL FROM THE INPUT BUFFER */
				if(c =='<'){
					t.code =SCC_OP_T;/*sets the code of the token*/
					
				}
				else if(c =='>'){
					t.code= REL_OP_T;/*sets the code of the token*/
					t.attribute.rel_op=NE;
					
				}
				else{
				t.code = REL_OP_T;/*sets the code of the token*/
				t.attribute.rel_op=LT ;
				b_retract(sc_buf);
				}
					return t;		

				
			case '=':
		
		/* Get the next character from the buffer */
		c =  b_getc(sc_buf);

		/* If the next character is also '=' then the scanner has found the equality operator */
		if (c == '=')
		{
			t.code = REL_OP_T;
			t.attribute.rel_op = EQ;
			return t;
		}

		/* Any other character means the token is simply the assignment operator */
		t.code = ASS_OP_T;
		/* Retract the buffer so that we can parse the second character again in the context of a new token */
		b_retract(sc_buf);
		return t;
			case '>':
				t.code = REL_OP_T;/*sets the code of the token*/
				t.attribute.rel_op=GT ;
				return t;

			case ',':
				t.code=COM_T;/*sets the code of the token*/
				return t;

			case '+':
				t.code=ART_OP_T;/*sets the code of the token*/
				t.attribute.arr_op = PLUS;
				return t;
			
			case '-':
				t.code =ART_OP_T;/*sets the code of the token*/
				t.attribute.arr_op=MINUS;
				return t;
			case '*':
				t.code=ART_OP_T;/*sets the code of the token*/
				t.attribute.arr_op= MULT;
				return t;
			case '/':
				t.code=ART_OP_T;/*sets the code of the token*/
				t.attribute.arr_op=DIV;
				return t;

			case ';':
				t.code=EOS_T;/*sets the code of the token*/
				return t;
			case '(' :
				
				t.code=LPR_T;/*sets the code of the token*/
				return t;

			case ')':	
				t.code=RPR_T;/*sets the code of the token*/
				return t;
			case '{':
				t.code=LBR_T;/*sets the code of the token*/
				return t;
			case '}':
				t.code=RBR_T;/*sets the code of the token*/
				return t;

			case '!':
				c = b_getc(sc_buf);		/*GET THE NEXT SYMBOL FROM THE INPUT BUFFER */
				if (c != '<')
					{/*If its not < after ! then it is an error because there is no such thing as ! in PLATYPUS*/
						t.code = ERR_T;		
						t.attribute.err_lex[0] = '!';
						t.attribute.err_lex[1] = (signed char) c;
						t.attribute.err_lex[2] = '\0';

						/* Skip the rest of the input until the next line */
						do
						{
							/* Get the next character from the buffer */
							c =   b_getc(sc_buf);
							/* If SEOF is found retract the buffer and return the token */
							if (c == SEOF)
							{
							b_retract(sc_buf);
							return t;
								}
							} while (c != '\n' && c != '\r' && c != '\0');
									++line;
									return t;
							}

							/* Get the next character from the buffer */
							c =   b_getc(sc_buf);

							/*
							 * Loop through all the characters in the buffer until the scanner encounters a newline ('\n').
							 * If an end of file character (SEOF) is encountered before a new line, issue an
							* error as per grammar rules.
							*/
							while (c != '\n' && c != '\r' && c != '\0' && c != SEOF)
							c =   b_getc(sc_buf);

							/* If a newline was encountered, the scanner has found the end of the comment and will process the next token */
							if (c == '\n' || c == '\r')
							{
								++line;
							//	goto getNextCharacter;
							}

						/* If SEOF has been encountered before a newline, that is an error according to the grammar */
						t.code = ERR_T;
						strcpy(t.attribute.err_lex, "No new Line");
						return t;
/*

IF STRING (FOR EXAMPLE, "text") IS FOUND      
      SET MARK TO MARK THE BEGINNING OF THE STRING
      IF THE STRING IS LEGAL   
         USING b_addc(..)COPY THE text FROM INPUT BUFFER INTO str_LTBL 
         ADD '\0' at the end make the string C-type string 
         SET STRING TOKEN
         (the attribute of the string token is the offset from
         the beginning of the str_LTBL char buffer to the beginning 
         of the string (TEXT in the example)) 
 
         return t;

*/
			case '"':
				
				lexstart = (b_getc_offset(sc_buf));/* mark the beginning of the string*/

				c=b_getc(sc_buf);					/* get next character */
				if(c=='"'){							/* if its an empty string return */
					 t.code=STR_T;
					 t.attribute.str_offset=b_size(str_LTBL);
					 b_addc(str_LTBL,'\0');				/* make it a c-type string */

					}

				else{					/* check for error or a valid string */
					 do{			/* check if we found second  ' " ' */
						if(c=='\n'){
							++line;
						}
						c=b_getc(sc_buf);					/* get next character */

						 if((c=='\0' ||c == 255 ) ){/* if we hit end of file return error */
							 t.code=ERR_T;/*attribute of error lexeme to be set*/
							 lexend=(short)b_getc_offset(sc_buf);/*get the last char of the string */
							 b_retract_to_mark(sc_buf);		/*-1 is to get the " and report them as error */
							 capacity=lexend-lexstart;
							 for(loop=0;loop<=capacity;loop++){	
									c=b_getc(sc_buf);				/*get the next char */
									if((loop)<=(ERR_LEN-3))				/*check to see only 17 chars are added*/
											 t.attribute.err_lex[loop]=(unsigned)c;	/*setting error attribute*/
							 }/*end of for*/
							 t.attribute.err_lex[ERR_LEN-3]='.';	/*ending string */
							 t.attribute.err_lex[ERR_LEN-2]='.';	/*ending string */
							 t.attribute.err_lex[ERR_LEN-1]='.';	/*ending string */
							 t.attribute.err_lex[ERR_LEN]='\0';	/*making a C type string*/
							 return t;
						} // end of if
					}while(c!='"');// end of do-while
				 lexend=b_getc_offset(sc_buf)-1;		/* mark the end of lexeme */
				 b_setmark(sc_buf,lexstart);	/* set the mark to begining of the string  */
				 b_retract_to_mark(sc_buf);
 				 t.code=STR_T;						/*sets the code of the token*/
				 t.attribute.str_offset=b_size(str_LTBL);	/* return the offset where the string begins*/
				 for(loop=lexstart;loop<lexend;loop++){			/* loop to add chars to string buffer */
					 c=b_getc(sc_buf);							/* get next char*/
					 b_addc(str_LTBL,c);					/* add char to string buffer */
				 }/*end of for*/
				 c=b_getc(sc_buf);				/* get next character */
				 b_addc(str_LTBL,'\0');			/* adding \0 to make a ctype string */
				 lexstart=0;					/* reset */
				 lexend=0;						/* reset */

				 printf("GETC %d \n",b_getc_offset(str_LTBL));
					}/*end of else*/
				 return t;
				

	 case '.':
		/*
		 * The '.' character followed by "AND." represents the AND logical operator.
		 * The '.' character followed by "OR." represents the OR logical operator.
		 * Anything else following the '.' character is a syntax error.
		 */

		/* Set the lexstart to the location of the next character in case of error */
		lexstart = b_getc_offset(sc_buf);

		/* Get the next character from the buffer */
		c =  b_getc(sc_buf);

		/*
		 * If the next character is 'A', keep testing to see if the remaining characters
		 * represent the AND logical operator.
		 */
		if (c == 'A')
		{
			/* Get the next character from the buffer */
			c =b_getc(sc_buf);
			/*
			 * If the next character is 'N', keep testing to see if the remaining characters
			 * represent the AND logical operator.
			 */
			if (c == 'N')
			{
				/* Get the next character from the buffer */
				c =b_getc(sc_buf);
				/*
				 * If the next character is 'D', keep testing to see if the remaining characters
				 * represent the AND logical operator.
				 */
				if (c == 'D')
				{
					/* Get the next character from the buffer */
					c =b_getc(sc_buf);
					/*
					 * If the next character is '.' then the token
					 * represents the AND logical operator
					 */
					if (c == '.')
					{
						t.code = LOG_OP_T;
						t.attribute.log_op = AND;
						return t;
					}
				}
			}
		}
		/*
		 * If the next character is 'O', test to see if the remaining characters
		 * represent the OR logical operator.
		 */
		else if (c == 'O')
		{
			/* Get the next character from the buffer */
			c = b_getc(sc_buf);
			/*
			 * If the next character is 'R', keep testing to see if the remaining character
			 * completes the OR logical operator.
			 */
			if (c == 'R')
			{
				/* Get the next character from the buffer */
				c =  b_getc(sc_buf);
				/*
				 * If the next character is '.' then the token
				 * represents the OR logical operator
				 */
				if (c == '.')
				{
					t.code = LOG_OP_T;
					t.attribute.log_op = OR;
					return t;
				}
			}
		}
		/* The '.' is out of context; return error */
		t.code = ERR_T;
		t.attribute.err_lex[0] = '.';
		t.attribute.err_lex[1] = '\0';
		/* Reset the buffer's location to the next char after the dot */
		b_setmark(sc_buf, lexstart);
		return t;
			default : /*if none of the case are present.*/
				t.code =ERR_T;
				t.attribute.err_lex[0]=c;
				t.attribute.err_lex[1]='\0';
				return t;

			}/*end of switch*/
}
}
}
int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
printf("Input symbol: %c Row: %d Column: %d Next: %d \n",c,state,col,next);
#endif
/*
The assert(int test) macro can be used to add run-time diagnostic to programs
and to "defend" from producing unexpected results.
assert() is a macro that expands to an if statement;
if test evaluates to false (zero) , assert aborts the program
(by calling abort()) and sends the following message on stderr:

Assertion failed: test, file filename, line linenum

The filename and linenum listed in the message are the source file name
and line number where the assert macro appears.
If you place the #define NDEBUG directive ("no debugging")
in the source code before the #include <assert.h> directive,
the effect is to comment out the assert statement.
*/
       assert(next != IS);

/*
The other way to include diagnostics in a program is to use
conditional preprocessing as shown bellow. It allows the programmer
to send more details describing the run-time problem. 
Once the program is tested thoroughly #define DEBUG is commented out
or #undef DEBUF is used - see the top of the file.
*/ 
#ifdef DEBUG
	if(next == IS){
	  printf("Scanner Error: Illegal state:\n");
	  printf("Input symbol: %c Row: %d Column: %d\n",c,state,col);
	  exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}
/*
 * Purpose:				Returns the appropriate column number from the transition table
 * 						based on the input character
 * Author:				Archit Bhatia
 * Version:				2.2 (12-March-2015)
 * Called Functions:	isalpha, isdigit
 * Parameters:			c
 * type: char
 * Returns:				The column number in the transition table(int)
 */
int char_class (char c)
{
        int val=0;
		if(isalpha(c)) val = 0;
		else if(isdigit(c)){
			if(c== '0') val =1;
			if(c>'0' && c<='7') val=2;
			if(c=='8'|| c=='9' ) val =3;
		}
		else if(c=='.') val=4;
		else if(c=='#')val=5;
		else val =6;
		return val;
}

/*
 * Purpose:				Returns the token for arithmetic variable identifier
 * 						based on the input character
 * Author:				Archit Bhatia
 * Version:				2.1 (12-March-2015)
 * Called Functions:	iskeyword,strlen
 * Parameters:			lexeme array of chars
 * type:			`   char
 * Returns:				token
 */
Token aa_func02(char lexeme[]){
	Token t;
	int index =0;
 	index=iskeyword(lexeme);/*Return the index of the keyword if the lexeme is keyword*/
	if(index>=0){
		t.code = KW_T;/*Sets the code*/
		t.attribute.kwt_idx=index;/*sETS THE KEYWORD INDEX*/
		return t;
	}
			t.code=AVID_T;
			
		 if(strlen(lexeme) > VID_LEN)/*checks for the length of lexeme*/
		{
			strncpy( t.attribute.vid_lex,lexeme,VID_LEN);/*copies first 8 chars of lexeme to vid_lex*/
			t.attribute.vid_lex[VID_LEN]='\0';/*makes it ctype string*/
			
		}
		else{
			strcpy( t.attribute.vid_lex,lexeme);
			
			t.attribute.vid_lex[strlen(lexeme)]='\0';/*makes it ctype string*/
			
		}
	
		return t;
	}


/*
 * Purpose:				Sets the string variable identifier token
 * Author:				Archit Bhatia
 * Version:				2.2 (12-March-2015)
 * Called Functions:	strcat,strncpy
 * Parameters:			c
 * type: char
 * Returns:				Token
 */
Token aa_func03(char lexeme[]){
	Token t;

	int length;				
		length = strlen(lexeme);	/*getting length of lexeme*/
	if(length > VID_LEN){			/*checking length to be samller the vid_len*/
		strncpy(t.attribute.vid_lex,lexeme,VID_LEN);	/*truncating lexeme to vid_len*/
		t.attribute.vid_lex[VID_LEN-1]='#';				/*adding # for svid*/
		t.attribute.vid_lex[VID_LEN]='\0';
}/*	end of if*/
	else{
		strncpy(t.attribute.vid_lex,lexeme,length+1);	/*copying string */
		t.attribute.vid_lex[length+1]='\0';
	}	/*end of else*/
  t.code=SVID_T;		/*setting token code*/
	return t;
}

/*
 * Purpose:				Accepting function for the floating points
 * Author:				Archit Bhatia
 * Version:				2.2 (12-March-2015)
 * Called Functions:	strcat,strncpy
 * Parameters:			lexeme
 * type: char
 * Returns:				Token
 */
Token aa_func08(char lexeme[]){
	Token t;
	/* Used to store the float value of a valid lexeme */
	double value;

	/* Attempt to convert the string to numeric*/
	value = atof(lexeme);
	/*
	 * If the returned value is out of range,
	 * return error token.
	 */
	if (value> FLT_MAX || (value <FLT_MIN && value >= 0.0) || value < 0.00F ){
		t.code=ERR_T;/*If the value is greater than max float, error is returned*/
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN);
		t.attribute.err_lex[ERR_LEN] = '\0';/*makes it ctype*/
	}
	else{/*the float is correct*/
	t.code = FPL_T;
	t.attribute.flt_value = (float) value;
	}
	return t;
}


/*Purpose:				THE FUNCTION MUST CONVERT THE LEXEME REPRESENTING A DECIMAL CONSTANT AND 0
						TO A DECIMAL INTEGER VALUE, WHICH IS THE ATTRIBUTE FOR THE TOKEN.

Author:					Archit Bhatia
History/ Versions:		2
Called functions:		strncpy,atoi
Parameters:				lexeme
Return value:			Token
*/
Token aa_func05(char lexeme[]){
	long i;
	Token t;
	i = atol(lexeme);
	if(i > MAX2BYTEINT || i < 0 )
	{
		t.code =ERR_T;
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN);
		t.attribute.err_lex[ERR_LEN]='\0';
	}	
	
 
	else{
		t.code=INL_T;
		t.attribute.int_value=i;
	}
	return t;
 
}

/*Purpose:				accepting FUNCTION FOR AN OCTAL CONSTANT
Author:					Archit Bhatia
History/ Versions:		2
Called functions:		strncpy,atool
Parameters:				characyer array
Return value:			TOken
*/
Token aa_func11(char lexeme[]){
Token t;				/* Token to be returned */
	short oct=0;
	oct = (short)atool(lexeme);
	/* Prevent overflow errors */
	if(oct > SHRT_MAX || oct < 0 || oct > 5)
	{
		t.code=ERR_T;
		strncpy(t.attribute.err_lex,lexeme,ERR_LEN);
		t.attribute.err_lex[ERR_LEN]='\0';	
	}	
	else{
	t.code = INL_T;
	t.attribute.int_value = oct;
	}
	return t;
}


/*Purpose:				Adds error string to the error lex and returns a token for that
Author:					Archit Bhatia
History/ Versions:		3
Called functions:		strncpy,strlen
Parameters:				array of chars
Return value:			Token
*/
Token aa_func12(char lexeme[]){
	Token t;
	char errorlex[ERR_LEN+1];
	if(strlen(lexeme)> (ERR_LEN-3)){
		t.code=ERR_T;
		strncpy(errorlex,lexeme,ERR_LEN);
		errorlex[ERR_LEN]='\0';
	}
	else{
	strncpy(errorlex,lexeme,strlen(lexeme));
	errorlex[strlen(lexeme)]='\0';
	}
	t.code=ERR_T;
	strncpy(t.attribute.err_lex,errorlex,ERR_LEN+1);
	return t;
}

/*Purpose:				Converts octal to integer
Author:					Archit Bhatia
History/ Versions:		 2
Called functions:		strcmp(),atoi,strlen
Parameters:				Pointer to character
Return value:			Integer long 
*/
long atool(char * lexeme){
	long total = 0;	/*Store the total value */
	int octalDigit = 0;	/*The current index of the array converted to an int*/
	unsigned int i = 0;		/*Used as an iterator*/
	unsigned int j = 0;		/*Used as an iterator*/

	/*Iterate over the full input array. */
	for(i = 0; i <strlen(lexeme); i++)
    {
		octalDigit = 0;
		/*Convert the current index of the array to an int. */
		octalDigit = lexeme[i] - '0';
		/*Determine the power of the current digit by finding its index in the array. */
		for(j=1; j< strlen(lexeme) -i; j++)
		{
			octalDigit *=8;
		}
		/*Add the current digit to the total. */
		total+= octalDigit;
	}
	return total;
}
/*
 * Purpose:				checks if the lexeme is a keyword or not.
 * Author:				Archit Bhatia
 * Version:				1.1 (12-March-2015)
 * Called Functions:	strcmp
 * Parameters:			POinter to character
 * type:				int
 * Returns:				Index of the ketword in kw_table
 */
int iskeyword(char * kw_lexeme){
int i=0;
int ind=-1;
for(i=0;i<KWT_SIZE;i++){ /*Loop to check if the lexeme is any keyword*/
	if(strcmp(kw_lexeme , kw_table[i])==0){
		ind=i;
	break;
	}
}
if(ind>=0)return ind;
else
		return R_FAIL_1;
	
}
