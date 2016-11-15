/* CDA 3103 
   Data Path Project
   Authors: Zach Muller
   			Danielle Evans
   			Meredith Pyrich
*/
#include "spimcore.h"
#include "limits.h"

//Data structure for Words in each register
typedef struct Word {
	//Dynamically allocated array to hold the digits (registers)
	//of an, stored in reverse order
	int *registers;
	//The number of registers in the integer
	//(which is approximately equal to the length of the array)
	int numRegisters;
} Word;


/* Auxillary functions to deal with each A and B 
   as 32-bit word registers */
//Tell the program which operands to use (main menu)
void operation();
//Get length of word
int getLength(int n);
//Turn integers into words
Word *parseInt(int integer);
/* Convert the INTEGER represented by 'p' to a dynamically allocated 
unsigned int, and return a POINTER to that value. */
unsigned int *toUnsignedInt(Word *p);
//Add words
Word *hugeAdd(Word *firstWord, Word *secondWord);
//Converto words to integers
unsigned wordToInt(Word *savedWord);
//Negate an integer
int NOT(unsigned B);


//Gets the number of elements in a Word
int getLength(int n) {
   
   //Get number of digits in int
   int intLength = 0;
   int i = n;

   if(n == 0) {
      intLength = 1;
   }
   while(i != 0) {

   	i /= 10;

   	intLength++;

   }

   return intLength;
}

//Turns an int into a Word
Word *parseInt(int n) {

   //Allocate memory for local struct
	Word *parseInt = (Word *)malloc(sizeof(Word));
   
   int intLength;
   intLength = getLength(n);

   //Put it in struct
   parseInt->numRegisters = intLength;

   //Allocate space for array of digits
   parseInt->registers = (int*)malloc(sizeof(int) * (intLength == 0 ? 1 : intLength));

   //Defense if malloc fails...
   if(parseInt == NULL) {
      printf("ERROR: parseInt failed to allocate memory.\n");
      return NULL;
   }

   //Pointer to each element in the unsigned int
   int *current = (int *)malloc(sizeof(int));
   //Assign it to struct element digits
   current = parseInt->registers;

   //If given a '0' put it in array inside struct
   //change length of array from 0 to 1
   if(n == 0) {
      *current = n;
      parseInt->numRegisters = 1;
   }

   //Get each digit of the integer and put it into array buffer
   while(n != 0) {

    //This is where decimal point is currently on the integer
    *current++ = n % 10;
    //printf("current = %d\n", n);

    //n is whatever is to the right of the decimal
    n /= 10;
   }

	return parseInt;
}

/* PRECONDITION: newly instantiated struct p passed to the function

   NOTE: The only reason that this function returns a POINTER
   insted of an UNSIGNED INT is so that we can return a NULL
   to signify failure in cases where 'p' cannot be represented
   by an unsigned int

   POSTCONDITION: Pointer returned

   RETURNS: A POINTER to a newly allocated unsigned integer, or
   NULL if value cannot be represented as an unsigned int
   (including the case where 'p' is NULL) */	
unsigned int *toUnsignedInt(Word *p) {

   //Allocate memory for unsigned int
	unsigned int *Uint = (unsigned int *)malloc(sizeof(unsigned int));
   
   //If passed HugeInteger is NULL, return NULL
   if(p == NULL) {
      return NULL;
   }

   //Put the backwards digit array back together in the right order
   //to be compared to UINT_MAX
   int i, k = 0;
   for(i = (p->numRegisters - 1); i >= 0; i--) {
      k = 10 * k + p->registers[i];
   }
   
   //If the number is greater than or equal to the max number that can be stored as an
   //unsigned int, add one so that it will be displayed as an
   //unsigned int
   if(k > UINT_MAX) {
      return NULL;
   }

   //If integer is too large to be displayed as an unsigned int
   //Display error and return NULL
   if(k <= UINT_MAX) {

      //Convert all digits in int array to unsigned ints
      int j;
      for(j = (p->numRegisters - 1); j >= 0; j--) {
         *Uint = 10 * (*Uint) + p->registers[j];
      }
   }

	return Uint;
}

//Adds two Words
Word *hugeAdd(Word *p, Word *q) {

   //Create space for struct
   Word *sum = (Word*)malloc(sizeof(Word));

    //If p or q is NULL, return NULL
   if(p == NULL || q == NULL) {
      return NULL;
   }

   //If p is bigger 
   if(p->numRegisters > q->numRegisters) {
      
      //malloc space for size of struct * p's length
      sum->registers = (int *)malloc(sizeof(int) * (p->numRegisters + 1));
      sum->numRegisters = 0;

      //Create a buffer to hold array of smaller number with padded 0s
      int *buffer = (int *)malloc(sizeof(int) * p->numRegisters);
      int i;
      for(i = 0; i < q->numRegisters; i++) {
         buffer[i] = q->registers[i];
      }

      //Loop through smaller array and pad 0's until it's the same
      //length as the bigger array
      for(i = (q->numRegisters); i < (p->numRegisters); i++) {
         buffer[i] = 0;
      }

      //Loop through the two arrays and add them
      //Store them in sum->digits
      int j;
      int carry = 0;
      for(j = 0; j < p->numRegisters; j++) {

         sum->registers[j] = p->registers[j] + buffer[j] + carry;

         //If there's a carry
         if(sum->registers[j] > 9) {
            carry = sum->registers[j] / 10;
            sum->registers[j] %= 10;
         }

         else {
            carry = 0;
         }
      }
      
      if(carry == 1) {
         sum->registers[p->numRegisters] = carry;
         sum->numRegisters++;
      }

      //store length of sum
      sum->numRegisters += p->numRegisters;  
   }
   
   //If q is bigger 
   if(q->numRegisters > p->numRegisters) {

      //malloc space for size of struct * q's length
      sum->registers = (int *)malloc(sizeof(int) * (q->numRegisters + 1));
      sum->numRegisters = 0;

      //Create a buffer to hold array of smaller number with padded 0s
      int *buffer = (int *)malloc(sizeof(int) * q->numRegisters);
      int i;
      for(i = 0; i < p->numRegisters; i++) {
         buffer[i] = p->registers[i];
      }

      //Loop through smaller array and pad 0's until it's the same
      //length as the bigger array
      for(i = (p->numRegisters); i < (q->numRegisters); i++) {
         buffer[i] = 0;
      }

      //Loop through both arrays and add them
      //store their sum in sum->digits
      int j;
      int carry = 0;
      for(j = 0; j < q->numRegisters; j++) {

         sum->registers[j] = q->registers[j] + buffer[j] + carry;

         //If there's a carry
         if(sum->registers[j] > 9) {
            carry = sum->registers[j] / 10;
            sum->registers[j] %= 10;
         }

         else {
            carry = 0;
         }
      }

      if(carry == 1) {
         sum->registers[q->numRegisters] = carry;
         sum->numRegisters++;
      }

      //Store length of sum
      sum->numRegisters += q->numRegisters;
   }

   //If they're the same length
   if(p->numRegisters == q->numRegisters) {

      //malloc space for size of struct * p's length
      sum->registers = (int *)malloc(sizeof(int) * (p->numRegisters + 1));
      sum->numRegisters = 0;

      //Loop through the two arrays and add them
      //Store them in sum->digits
      int j;
      int carry = 0;
      for(j = 0; j < p->numRegisters; j++) {
         
         sum->registers[j] = p->registers[j] + q->registers[j] + carry;

         //If there's a carry
         if(sum->registers[j] > 9) {
            carry = sum->registers[j] / 10;
            sum->registers[j] %= 10;
         }

        else {
           carry = 0;
        }
      }

      if(carry == 1) {
         sum->registers[p->numRegisters] = 1;
         sum->numRegisters++;
      }
      //store length of sum
      sum->numRegisters += p->numRegisters;
   }

   //If malloc fails, return NULL
   if(sum == NULL) {
      return NULL;
   }

  	return sum;
}

//Converts a Word struct to an int
unsigned wordToInt(Word *savedWord) {

	unsigned integer = 0;

	int i;
	for(i = savedWord->numRegisters - 1; i >= 0; i--) {
		integer = (10 * integer) + savedWord->registers[i];
	}

	return integer;
}

//Function that negates a given variable
int NOT(unsigned B) {

	int negated$s2;

	negated$s2 = 0 - B;

	return negated$s2;
}

/* ALU */
/* Author: Zach Muller */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{	
	ALUresult = (unsigned *)malloc(sizeof(unsigned));

	switch(ALUControl) {
		//Add
		case '0':
		{
			//Create space for all words involved in add operation
			//Source 1
			Word *$t0 = (Word *)malloc(sizeof(Word));
			//Source 2
			Word *$t1 = (Word *)malloc(sizeof(Word));
			//Destination register
			Word *$s0 = (Word *)malloc(sizeof(Word));

			$t0 = parseInt(A);
			$t1 = parseInt(B);

			$s0 = hugeAdd($t0, $t1);

			*ALUresult = wordToInt($s0);
			printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//Subtract
		case '1':
		{	
			/* CONFUSION: dealing with signed values when inputs 
			and output are unsigned (need to fix later) */

			//Create space for all words involved in add operation
			//Source 1
			Word *$t0 = (Word *)malloc(sizeof(Word));
			//Source 2
			Word *$t1 = (Word *)malloc(sizeof(Word));
			//Destination Word
			Word *$s0 = (Word *)malloc(sizeof(Word));

			$t0 = parseInt(A);
			$t1 = parseInt(NOT(B));

			$s0 = hugeAdd($t0, $t1);

			*ALUresult = wordToInt($s0);
			printf("ALU result = %u\n", *ALUresult);

			break;
		}
		//SLT signed
		case '2':
		{
			*ALUresult = 0;
			if(A < B) {
				*ALUresult = 1;
			}
			printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//SLT unsigned
		case '3':
		{
			*ALUresult = 0;
			if(A < B) {
				*ALUresult = 1;
			}
			printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//AND
		case '4':
		{	
			*ALUresult = A & B;
			printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//OR
		case '5':
		{	
			*ALUresult = A | B;
			printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//shift LEFT B by 16 bits
		case '6':
		{	
			*ALUresult = B << 16;
			printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//NOT A
		case '7':
		{	
			*ALUresult = ~A;
			printf("ALUresult = %u\n", *ALUresult);
			break;
		}
	}

	/* If result is 0, Zero = 1,
	Otherwise Zero = 0 */
	*Zero = '0';
	if(*ALUresult == 0) {
		*Zero = '1';
	}
	printf("Zero = %c\n", *Zero);
}

/* instruction fetch */
/* Author: Meredith Pyrich */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	return 0;
}


/* instruction partition */
/* Author: Meredith Pyrich */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	/* Compiled Meredith's code, threw errors saying I needed to put '&' 
	in front of instructionCopy assigns, so I did. */

	// A copy of the 32-bit instruction to manipulate with shifting.
    unsigned instructionCopy = instruction;

    /* The following line threw an error saying result unused
    So I assigned the result to itself */
    instructionCopy = instructionCopy >> 26;
    op = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 27;
    r1 = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 11;
    instructionCopy = instructionCopy >> 27;
    r2 = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 27;
    r3 = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 26;
    instructionCopy = instructionCopy >> 26;
    funct = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 16;
    offset = &instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 6;
    jsec = &instructionCopy;
}



/* instruction decode */
/* Author: Meredith Pyrich */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	return 0;
}

/* Read Register */
/* Author: Danielle Evans */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* Author: Danielle Evans */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	int val = offset;
    int sign = 0;

    if((val >> 15) == 1)
    {
        sign = 1;
    }

    if(sign == 1)
    {
        val = val >> 16;
        *extended_value = val | 0xffff0000;
    }

    else
        *extended_value = val >> 16;
    printf("extended value = %u\n", *extended_value);
}

/* ALU operations */
/* Author: Zach Muller */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{	
	/* check for HALT condition */
	//An illegal instruction appears
	if((ALUOp != '0') && (funct != 0)) {
		return 1;
	}

	printf("ALUOp = %c\n", ALUOp);
	/* If opcode is 000000, then we have an R-type */
	if(ALUOp == '0') {
		switch(funct) {
			//add
			case 32:
				ALUOp = '0';
				break;
			//subtract
			case 34:
				ALUOp = '1';
				break;
			//slt signed
			case 42:
				ALUOp = '2';
				break;
			//slt unsigned
			case 43:
				ALUOp = '3';
				break;
			//AND
			case 36:
				ALUOp = '4';
				break;
			//OR
			case 37:
				ALUOp = '5';
				break;
			//Shift left B by 16
			case 0:
				ALUOp = '6';
				break;
			//NOT
			case 39:
				ALUOp = '7';
				break;
		}
	}
	
	/* use data 2 or extended_value? (determined by ALU source control signal) */
	if(ALUSrc == '1') {
		ALU(data1,extended_value,ALUOp,ALUresult,Zero);
	}
	else {
		ALU(data1,data2,ALUOp,ALUresult,Zero);	
	}

	/* check for HALT condition */
	//Address is not word-alligned
	if(*ALUresult % 4 != 0) {
		return 1;
	}
	//Accessing data that is beyond memory
	else if(ALUresult == NULL) {
		return 1;
	}

	return 0;
}

/* Read / Write Memory */
/* Author: Danielle Evans */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	return 0;
}


/* Write Register */
/* Author: Danielle Evans */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* Author: Zach Muller */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}