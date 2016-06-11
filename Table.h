/*File name: table.h
* Compiler: MS Visual Studio 2012
* Author: Archit Bhtia 040772687
* Course: CST 8152 – Compilers, Lab Section: 012
* Assignment: 2 (Scanner)
* Date: March 12, 2105
* Professor: Sv. Ranev
* Purpose: Header file for the table contents
*/
#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif
#define MAX2BYTEINT 32767
#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
 *    '\0' or only one of the folowing constants: 255, 0xFF , EOF
 */
 

#define ES  12 /* Error state */
#define IS -1    /* Inavalid state */
#define SEOF 255
#define TABLE_COLUMNS 7 /*7 cloumns*/
/*transition table - type of states defined in separate table */
int  st_table[ ][TABLE_COLUMNS] = {
/* State 0 */  {1,6,4,4,IS,IS,IS},
/* State 1 */  {1,1,1,1,2,3,2},
/* State 2 */  {IS,IS,IS,IS,IS,IS},
/* State 3 */  {IS,IS,IS,IS,IS,IS},
/* State 4 */  {ES,4,4,4,7,5,5},
/* State 5 */  {IS,IS,IS,IS,IS,IS,IS},
/* State 6 */  {ES,10,9,ES,7,ES,5},
/*State 7*/		{8,7,7,7,8,8,8},
/*sTATE 8*/		{IS,IS,IS,IS,IS,IS,IS},
/*STATE 9*/		{11,9,9,ES,ES,11,11},
/*STATE 10*/	{11,ES,11,11,11,11,11},
/*STATE 11*/	{IS,IS,IS,IS,IS,IS,IS},
/*STATE 12*/	{IS,IS,IS,IS,IS,IS,IS}
};
/* Accepting state table definition */
#define ASWR     2  /* accepting state with retract */
#define ASNR     1  /* accepting state with no retract */
#define NOAS     0  /* not accepting state */

int as_table[ ] = {NOAS,NOAS,ASWR,ASNR,NOAS,ASWR,NOAS,NOAS,ASWR,NOAS,NOAS,ASWR,ASNR};
/* Accepting action function declarations */
Token aa_func02(char *lexeme); 
Token aa_func03(char *lexeme); 
Token aa_func05(char *lexeme); 
Token aa_func08(char *lexeme); 
Token aa_func11(char *lexeme); 
Token aa_func12(char *lexeme); 
/* defining a new type: pointer to function (of one char * argument) 
   returning Token
*/  

typedef Token (*PTR_AAF)(char *lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[ ] ={NULL,NULL,aa_func02,aa_func03,NULL,aa_func05,NULL,NULL,aa_func08,NULL,NULL,aa_func11,aa_func12};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  8

char * kw_table []= {
                      "ELSE",
                      "IF",
                      "INPUT",
                      "OUTPUT",
                      "PLATYPUS",
                      "REPEAT",
                      "THEN",
                      "USING"   
                     };

#endif
  