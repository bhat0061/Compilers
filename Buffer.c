/*File name: buffer.c
Compiler: MS Visual 2012
Author: Archit Bhatia , 040772687
Course: CST 8152 – Compilers, Lab Section: [011 or 012]
Assignment: [1]
Date: February 5, 2015
Professor: Sv. Ranev
Purpose: The buffer.c depicts all the functions that are necessary to make
		 a successful and working buffer without any errors.
Function list:	Buffer * b_create();
				pBuffer b_addc();
				int b_reset ();
				void b_destroy();
				int b_isfull();
				short b_size();
				short b_capacity ();
				char * b_setmark();
				short b_mark ();
				int b_mode();
				size_t b_inc_factor();
				int b_load();
				int b_isempty();
				int b_eob();
				char b_getc();
				int b_print();
				b_pack();
				char b_rflag();
				short b_retract();
				short b_retract_to_mark();
				short b_getc_offset();

*/
#include "buffer.h"
#include<stdio.h>
#include<limits.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

/*Purpose:This function creates a new buffer in the memory dynamically.
Author: Archit Bhatia
History/Versions: 1.8
Called functions: calloc() , malloc()
Algorithm: It uses the function calloc() to allocate memory for one Buffer
			structure.It sets the buffer operational mode and increment
			factor. If the mode is f or the inc_factor is 0, the mode and
			inc_factor are set to 0; If the mode is f or the inc_factor is
			not 0,the mode and inc_factor are set to 0
Parameters: init_capacity , char inc_factor, char o_mode 
Return value: pointer to buffer
*/
Buffer * b_create(short init_capacity,char inc_factor,char o_mode){/*starting of the function to create a buffer*/

		Buffer * kBuffer = NULL;

	
	kBuffer = (Buffer *)calloc(1,sizeof(Buffer)); /*allocates memory for 1 buffer struct  */
	if(kBuffer == NULL)return NULL;
	kBuffer->ca_head = (char *) malloc(init_capacity*sizeof(char));/*allocates memory for 1 character array */
	if( kBuffer->ca_head ==NULL){
	free(kBuffer);
	
	return NULL;
	}
	if(o_mode=='f'||inc_factor==0){/*checks if the o_mode is f or inc_factor is 0*/
		kBuffer->mode =0; 
		kBuffer->inc_factor =0;
} /*end of if*/

	else if(o_mode=='a')/* checks the o_mode and inc_factor values*/
	{
		kBuffer->mode=1; /*sets mode of buffer to 1*/
		kBuffer->inc_factor = inc_factor; /*buffer inc_factor value is assigned to inc_factor*/
		//kBuffer->capacity=init_capacity;
	//	return kBuffer;
	} /*end of elseif*/
	else if(o_mode=='m' && (inc_factor<=100 && inc_factor>=1)) /*checks if o_mode is m and inc_factor is under 100*/
	{
	kBuffer->mode = -1; /*if the above condition is true, sets the mode of buffer to -1*/
	//inc_factor = kBuffer->inc_factor; /*inc_factor value is assigned to inc_factor of buffer*/
	kBuffer->inc_factor = inc_factor;
	kBuffer->capacity = init_capacity;
	}
	else{
		free(kBuffer->ca_head);
		free(kBuffer);
		return NULL;
	}
	kBuffer->capacity=init_capacity;
	return kBuffer;
	

} 



/*Purpose:This function resets the r_flag to 0. and adds the character symbol
		  in the character array of buffer. It also resizes the buffer capacity
		  to include the char symbol, and returns NULL if fails to do so.It uses
		  realloc to increase the array capacity.
Author:		Archit Bhatia
History/Versions: 1.9
Called functions: realloc() , sizeof()
Parameters: Constant pBD and character Symbol.
Algorithm:Resets the r_flag to 0.
			It tries to increment the capacity of the buffer using realloc.
			the available space is calculated by subtracting the capacity
			from max buffer size.Then the available space is multiplied by
			in crement factor to get the space we need to increment.
			Then new increment is added to currently available capacity
			to get new capacity.
			If inc_mode 1 or -1 is successful, the r_flag is set to 1, and
			the char is added to the array.The value of addc_offset 
			increments by 1, and saves the new capacity in the capacity
			variable in Buffer struct.
Return value: Pointer to buffer structure.
*/

pBuffer b_addc(pBuffer const pBD,char symbol){/*this function adds the character symbol to buffer if it is not full.*/
	short capacity =0;
			char * tempP;

			//	char *nBuffer = (char *)realloc(pBD->ca_head,capacity); /*if 1 and -1 successful.*/

	if(pBD == NULL)return NULL;
	if (pBD->addc_offset == SHRT_MAX)
		return NULL;
	pBD->r_flag=0;/*sets r_flag to 0*/
	if(b_isfull(pBD)!=1) /*checks if the offset is larger than capacity or not*/
	{
		pBD->ca_head[pBD->addc_offset] = symbol;
		++pBD->addc_offset;
		
        return pBD;
	}
	else
	{
		if(pBD->mode ==0) 
			return NULL;
		else if(pBD->mode ==1)
		{
			capacity = (unsigned char)pBD->inc_factor + pBD->capacity; /*if the mode is 1, it increases the capacity of buffer*/
			if(capacity<0) return NULL; 
		}
		else if(pBD->mode == -1) /*checks the mode of the buffer*/
		{
			short av_space = SHRT_MAX - pBD->capacity;/*calculates the left space after adding chars*/
			short new_inc = (short) (av_space * ((double)pBD->inc_factor/100.0)); /*calulates the charactersa dded*/
							capacity = pBD->capacity +new_inc; /*increments the capacity*/

			 
		}
		/*if(capacity >SHRT_MAX || capacity==0 )
				capacity = SHRT_MAX;
		*/	
		if(capacity ==pBD->capacity)
				capacity = SHRT_MAX;

		tempP =(char *)realloc(pBD->ca_head,capacity); /*temporary buffer is allocated increased space and assigned the address of new memory*/
		if(tempP==NULL)
			return NULL;
		pBD->ca_head=tempP;
		pBD->r_flag=SET_R_FLAG;
		pBD->capacity= capacity;
		pBD->ca_head[pBD->addc_offset]=symbol;/*buffer size increased so add the charactor to it*/
		++pBD->addc_offset;/*increase offset as the charater has been added*/
		/*end of else*/
		
	return pBD;/*returning the pointer to buffer*/
	}
}

/*Purpose:This function is used to reset all the values of the buffer.
Author: Archit Bhatia
History/Versions: 1.1
Parameters: Constant Pointer pBD to Buffer 
Return value: int
*/

int b_reset(Buffer * const pBD){
	if (pBD == NULL)
		return -1;
	else
	{
	pBD->addc_offset =0;
	pBD->getc_offset=0;
	pBD->mark_offset =0;
	pBD->r_flag =0;
	pBD->eob =0;
	return 1;
}
}

/*Purpose:This function is used to free the memory dynamically allocated
		  to the buffer and character array.
Author: Archit Bhatia
History/Versions: 2.1
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: void
*/


void b_destroy(Buffer * const pBD){/*this function is used to free the memory we allocated 
								   dynamically to buffer and character array*/
	
	free(pBD->ca_head);
	free(pBD);
}


/*Purpose:This function chekcs if the buffer is full. If buffer is full, it
		  returns 1. Else returns 0.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: int
*/


int b_isfull(Buffer * const pBD){ /*this function is used to check if the buffer is full or not*/
	 if (pBD == NULL)
			return -1;
	 if((pBD->addc_offset==pBD->capacity))
			return 1;
		else
			return 0;
}


/*Purpose:This function is used to return the current size of the buffer.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short
*/

short b_size(Buffer * const pBD ){ /*this function returns the current size of the buffe
								  which is the buffer offset*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return -1;
	return pBD->addc_offset;
}

/*Purpose:This function is used to return the current capacity of the buffer.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short
*/

short b_capacity (Buffer * const pBD){/*this function returns the current capacity of the buffer*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)	/*checks for NULL pointers*/
		return -1;
	return pBD->capacity;
}

/*Purpose:This function is used to check if the mark is within the current size
		  of the buffer and sets the mark 
Author: Archit Bhatia
History/Versions: 1.0
Parameters: Constant Pointer pBD to Buffer and mark of type short
Return value: char
*/

char  *b_setmark(Buffer * const pBD, short mark){/*this function checks that the mark must be within 
												 the current size of the buffer*/
	
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return NULL;

	if(mark>=0){		
		pBD->mark_offset = mark;	
		return pBD->ca_head+mark;
	}/*returns the current position of mark in the buffer*/
	else
		return NULL;
} 

/*Purpose:This function is used to return mar_offset of the buffer
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short
*/


short b_mark (Buffer * const pBD){/*this function returns the mark_offset of the buffer*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
			return -1;

		return pBD->mark_offset;

}

/*Purpose:This function is used to return the value of mode.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: int
*/


int b_mode(Buffer * const pBD){/*returns the value of the mode*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return -2	;
	return (int)pBD->mode;
}

/*Purpose:This function is used to return inc_factor.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: size_t
*/

size_t b_inc_factor(Buffer *const pBD){/*this function returns the increment factor of the buffer to
									   the function and returns 256 on any error*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return 256;
	else
		return (size_t)(unsigned char )pBD->inc_factor;
}
/*Purpose:This function loads the input file and adds the character by character to the
		  buffer.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer and pointer to FILE
Return value: int
*/
int b_load(FILE * const fi, Buffer *const pBD){/*loads the file and its character is added to the
											   buffer byte by byte using the b_addc function*/
int count=0;/*temporary count of data type int to count number of character added to the buffer\ */
	char tchar;/*temporary varaible of type char to hold charater read from file */
 	while ((tchar = fgetc(fi)) != EOF) {/*until you find the needle called endoffile keeping searching the hay */
 		if( b_addc(pBD,tchar)==NULL){/*add characters to the buffer untils its full or something went wrong*/
		return LOAD_FAIL;
	 }
		count++;/*a char was added*/
	}
	return count;/*total number of chars added*/
}

/*Purpose:This function is used to check if the addc_offset is 0 or not.7
		  If its 0, the function returns 0 else 1.
Author: Archit Bhatia
History/Versions: 1.3
Parameters: Constant Pointer pBD to Buffer 
Return value: integer
*/

int b_isempty(Buffer * const pBD){/*this function checks if the buffer is empty or not*/
	if (pBD == NULL )
		return -1;
	else if(pBD->addc_offset ==0)
		return 1;
	else
		return 0;

}

/*Purpose:This function is used to return end of buffer flag variable of the
		  buffer
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short type
*/
int b_eob(Buffer * const pBD){/*this function returns end of buffer flag*/
	
	if(pBD->getc_offset > pBD->addc_offset || pBD->addc_offset > pBD->capacity)
		return 0;
	else
		return pBD->eob;
}


/*Purpose:This function is used to get the character offset of the buffer
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short type
*/

char b_getc(Buffer * const pBD){/*this funcn returns the character at the position of the 
								offset */
	short temp;
	if (pBD == NULL)
		return -2;
	else if(pBD->getc_offset == pBD->addc_offset){
		pBD->eob=1;
		return -1;
	}  
	else{
	pBD->eob=0;/*not end of buffer*/
	  
	return pBD->ca_head[pBD->getc_offset++];
	}
}	

/*Purpose:This function is used for diagnosis purposes
Author: Archit Bhatia
History/Versions: 1.3
Called functions: b_getc()
Parameters: Constant Pointer pBD to Buffer 
Return value: int
*/
	
int b_print(Buffer * const pBD){/*this function only used for diagnosis. 
								this prints on the screen if the buffer is empty
								or prints the contents of the buffer*/
	int temp=0;	/*temporary short to count number of chars*/
	char tchar={'0'};		/*temporary char to store the char read from the buffer*/
	if(pBD->addc_offset==0){		/*nothing to print*/
		printf("The buffer is empty.\n");
		return  R_FAIL_1;
	}/*end of if*/
	else{  
		pBD->getc_offset=0;
		tchar = b_getc(pBD);	/*read char from buffer and assign it to tchar*/

		 /*calling function to set the value to getc to zero*/
		while(b_eob(pBD)==0){/*infinte loop this will generate a warning*/
			printf("%c",tchar);	/*if a valid char was read print it*/
 		++temp;		/*count of chars increasing as it gets printed*/
					tchar = b_getc(pBD);	/*read char from buffer and assign it to tchar*/

		}/*end of else*/
		printf("\n");	
	}
	//pBD->getc_offset=0;
	//pBD->eob=1; /////////asdasdasdasd/////////asdasdasd/////
		return temp;	/*return number of char printed*/

}
/*Purpose:This function is used to change the buffer contents when it shrinks
		  or increases
Author: Archit Bhatia
History/Versions: 1.4
Called functions: realloc()
Parameters: Constant Pointer pBD to Buffer 
Return value: Pointer to buffer
*/
Buffer *b_pack(Buffer * const pBD){/*this function is used for the utilitues that
									happen when the buffer increases or shrinks*/
	short temp;	/*temp variale of type short*/
	char * tBuffer;	/*tBuffer pointer of type char*/
	pBD->r_flag=0;


	if(pBD->addc_offset==SHRT_MAX)return NULL;
	
	
	temp=(pBD->addc_offset +1)*sizeof(char);	/*to pack buffer with one more char*/

	tBuffer =(char *)realloc(pBD->ca_head,temp); /*temporary buffer is allocated increased space and assigned the address of new memory*/
	
	if(tBuffer== NULL)return NULL;

	/*check to see if realloc worked fine*/
	pBD->ca_head=tBuffer;	/*assign tBuffer to ca_head of the buffer*/
	pBD->r_flag=1;/*set r_flag because of realloc new memory address has been assigned to */
	
	pBD->capacity=temp;		/*temp is the new capacity of the buffer*/
	
	return pBD;			/*return pointer descriptor*/
	

} 

/*Purpose:This function returns the r_flag of the Buffer structure.
Author: Archit Bhatia
History/Versions: 1.0
Parameters: Constant Pointer pBD to Buffer 
Return value: char type
*/

char b_rflag(Buffer * const pBD){/*returns r_flag, which is used to check if the buffer has been increased or dec*/
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return -1;
	else
		return pBD->r_flag;

}
/*
Purpose: Decrements the value of getc_offset and returns it.
Author: Archit Bhatia
History/Versions: 1.0
Called functions: No function called.
Parameters: Constant Pointer to Buffer.
Return value: Short type.
Algorithm: Decrements the value of getc_offset everytime
			the function is called .
*/

short b_retract(Buffer *  const pBD){/*this function decrements the offset*/
		if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
			return -1;

	(pBD->getc_offset) --; 
	
		return pBD->getc_offset;
}
/*Purpose: sets the value of getc_offset and returns that value.
Author: Archit Bhatia
History/Versions: 1.3
Called functions: 
Parameters: Constant Pointer to Buffer
Return value: short value.
Algorithm:  Sets the getc_offset equal to mark_offset and returns
			the value of getc_offset.
*/
short b_retract_to_mark(Buffer *  const pBD){
	pBD->getc_offset = pBD->mark_offset;
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return -1;
	else
		return pBD->getc_offset;
}

/*Purpose:This function is used to get the character offset of the buffer
Author: Archit Bhatia
History/Versions: 1.0
Called functions: 
Parameters: Constant Pointer pBD to Buffer 
Return value: short type
*/

short b_getc_offset(Buffer *  const pBD){
	if (pBD == NULL || pBD->addc_offset > pBD->capacity || pBD->getc_offset > pBD->addc_offset)
		return -1;
	else
		return pBD->getc_offset;
}