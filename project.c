/* CDA 3103 
   Data Path Project
   Authors: Zach Muller
   			Danielle Evans
   			Meredith Pyrich
*/
#include "spimcore.h"
#include "limits.h"

#define MEMSIZE (65536 >> 2)

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

/* instruction fetch */
/* Author: Meredith Pyrich */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{	
	// If the address is not word aligned or if it is trying
    // to access an address beyond the memory, halt.
    if (((PC % 4) != 0) || (PC  > (MEMSIZE << 2))) {
    	printf("instructionFetch Halted\n");
        return 1;
    }
    /* ADDED & TO MEM[PC >> 2]*/
    *instruction = Mem[PC >> 2];
    printf("instruction = %u\n", *instruction);
    // Return zero, for there was no halt.
    return 0;
}


/* instruction partition */
/* Author: Meredith Pyrich */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	// A copy of the 32-bit instruction to manipulate with shifting.
    unsigned instructionCopy = instruction;

    //printf("instructionCopy = %u\n", instructionCopy);

    instructionCopy = instructionCopy >> 26;
    *op = instructionCopy;
    //printf("op = %u\n", *op);
    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 27;
    *r1 = instructionCopy;
    //printf("r1 = %u\n", *r1);
    instructionCopy = instruction;
    instructionCopy = instructionCopy << 11;
    instructionCopy = instructionCopy >> 27;
    *r2 = instructionCopy;
    //printf("r2 = %u\n", *r2);
    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 27;
    *r3 = instructionCopy;
    //printf("r3 = %u\n", *r3);
    instructionCopy = instruction;
    instructionCopy = instructionCopy << 26;
    instructionCopy = instructionCopy >> 26;
    *funct = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 16;
    *offset = instructionCopy;
    //printf("offset = %u\n", *offset);
    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 6;
    *jsec = instructionCopy;
}



/* instruction decode */
/* Author: Meredith Pyrich */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{	

	printf("op = %u\n", op);
	/* ADDED THE EXTRA FUNCTION CONTROL SIGNALS */


	/* RegDest		0 -> use r2						1 -> use r3 
	   jump         0 -> don't Jump         		1 -> jump
	   branch       0 -> don't Branch       		1 -> branch 
	   MemRead		0 -> don't read from memory 	1 -> read from mem
	   MemtoReg		0 -> dont redirect mem to reg 	1 -> redirect mem to reg
	   ALUOp  		is a 3 - bit bus to ALU control
	   MemWrite		0 -> don't write to mem         1 -> write to mem
	   ALUSrc		0 -> use r2						1 -> sign extend
	   RegWrite 	0 -> don't write to reg         1 -> write to reg 
	*/

	// Pulled the opcodes and their cooresponding values from the ppt
    // Set everything to 0 to default.
    controls->RegDst = '0';
    controls->Jump = '0';
    controls->ALUSrc = '0';
    controls->MemtoReg = '0';
    controls->RegWrite = '0';
    controls->MemRead = '0';
    controls->MemWrite = '0';
    controls->Branch = '0';
    controls->ALUOp = '0';

    switch(op) {

    	//R-type
    	case 0:
	    	controls->RegDst = '1';
	    	controls->Jump = '0';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '7';
    		break;

    	//jump
    	case 2:
    		controls->RegDst = '0';
    		controls->Jump = '1';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//beq
    	case 4:
	    	controls->RegDst = '2';
	    	controls->Jump = '0';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '2';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '1';
	        controls->ALUOp = '0';
    		break;

    	//addi
    	case 8:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '1';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//slti
    	case 10:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '2';
    		break;

    	//sltui
    	case 11:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '3';
    		break;

    	//lui
    	case 15:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '6';
    		break;

    	//lw
    	case 35:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '1';
	        controls->MemRead = '1';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//sw
    	case 43:
    		controls->RegDst = '2';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '2';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '1';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;
    }

    if(op != 0 && op != 2 && op != 4 && op != 8 && op != 10 && op != 11 && op != 15 && op != 35 && op != 43) {
    	// Opcode doesn't equal any of the values here.
        // Illegal instruction error, halt.
        printf("instructionDecode Halted\n");
        return 1;
    }
    
    return 0;
}

/* Read Register */
/* Author: Danielle Evans */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* Author: Danielle Evans */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{	
	printf("offset = %u\n", offset);
	int val = offset;
    int sign = 0;
   
    if((val >> 15) == 1)
    {
        sign = 1;
    }

    if(sign == 1)
    {
        *extended_value = val | 0xffff0000;
    }

    else{
        *extended_value = val | 0x00000000;
    }

    printf("extended value = %u\n", *extended_value);
}

/* ALU operations */
/* Author: Zach Muller */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{	
	printf("funct = %u\n", funct);
	printf("ALUOp = %c\n", ALUOp);
	/* check for HALT condition */
	//An illegal instruction appears
	if((ALUOp != '0') && (ALUOp != '2') && (ALUOp != '3') && (ALUOp != '6') && (ALUOp != '7') && (ALUOp != '8')) {
		printf("ALU Op Halted\n");
		return 1;
	}

	//lw/sw
	if(ALUOp == '0') {
		//add
		ALUOp = '0';
	}
	//Branch
	else if(ALUOp == '1') {
		//subtract
		ALUOp = '1';
	}
	//R-type
	else {
		switch(funct) {
			//add
			case 32:
				ALUOp = '0';
				break;
			 
			//subtract
			case 34:
				ALUOp = '1';
				break;

			//AND
			case 36:
				ALUOp = '4';
				break;

			//OR
			case 37:
				ALUOp = '5';
				break;

			//slt signed
			case 42:
				ALUOp = '2';
				break;

			//slt unsigned
			case 43:
				ALUOp = '3';
				break;

			//NOT
			case 12:
				ALUOp = '7';
				break;

			//Shift left B by 16
			case 63:	
				ALUOp = '6';
				break;
		}
	}
	printf("ALUSrc = %c\n", ALUSrc);
	printf("Zero = %c\n", *Zero);
	/* use data 2 or extended_value? (determined by ALU source control signal) */
	//Use extended value (immediate)
	if(ALUSrc == '1') {
		printf("data 1 = %u\ndata 2 = %u\n\n", data1, data2);
		ALU(data1,extended_value,ALUOp,ALUresult,Zero);
		printf("ALUresult = %u\n", *ALUresult);
	}
	//Use r2
	else {
		printf("data 1 = %u\ndata 2 = %u\n\n", data1, data2);
		ALU(data1,data2,ALUOp,ALUresult,Zero);	
	}

	return 0;
}

/* ALU */
/* Author: Zach Muller */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{	
	printf("ALUControl = %c\n", ALUControl);
	switch(ALUControl) {
		//Add
		case '0':
		{	
			/* WE SHOULD GO INTO HERE FOR FIRST AND SECOND INSTRUCTION*/

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
			//printf("ALU result = %u\n", *ALUresult);

			break;
		}
		//SLT signed
		case '2':
		{	
			printf("SLT Signed\n");
			*ALUresult = 0;
			if((int)A < (int)B) {
				*ALUresult = 1;
			}
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//SLT unsigned
		case '3':
		{
			*ALUresult = 0;
			if(A < B) {
				*ALUresult = 1;
			}
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//AND
		case '4':
		{	
			*ALUresult = A & B;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//OR
		case '5':
		{	
			*ALUresult = A | B;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//shift LEFT B by 16 bits
		case '6':
		{	
			*ALUresult = B << 16;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//NOT A
		case '7':
		{	
			*ALUresult = ~A;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
	}

	/* If result is 0, Zero = 1,
	Otherwise Zero = 0 */
	*Zero = '0';
	if(*ALUresult == 0) {
		*Zero = '1';
	}
	//printf("Zero = %c\n", *Zero);
}

/* Read / Write Memory */
/* Author: Danielle Evans */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{	
	unsigned *addressBefore = (unsigned *)malloc(sizeof(unsigned));
	addressBefore = &ALUresult;
	unsigned *address = (unsigned *)malloc(sizeof(unsigned));
	

	*address = (long)addressBefore % 4;
	//printf("address = %lu\n", (long)address);
	*address = (long)address * 4;
	//printf("addressBefore = %p\n", &ALUresult);
	//printf("addressBefore = %lu\n", (long)addressBefore);
	//printf("address = %lu\n", (long)address);
	//printf("MemRead = %c\nMemWrite = %c\n\n", MemRead, MemWrite);
	
	/* Most false positives for halting */
	 if(MemRead == '1')
    {	
    	*memdata = Mem[ALUresult >> 2];
    	printf("Reading from memory...\n");
    	//Make sure result is word-alligned
    	if(addressBefore == address) {
        	*memdata = Mem[ALUresult >> 2];
    	}
    	else {
	    	printf("rw_mem Halted\n");
	    	//We have an improper address
	    	//return 1;
	    }
    }

    if(MemWrite == '1')
    {	
    	Mem[ALUresult >> 2] = data2;
    	printf("Writing to memory...\n");
    	//Make sure result is word-alligned
    	if(*addressBefore == *address) {
        	Mem[ALUresult >> 2] = data2;
    	}
    	else {
	    	printf("rw_mem Halted\n");
	    	//We have an improper address
	    	//return 1;
	    }
    }

	return 0;
}


/* Write Register */
/* Author: Danielle Evans */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if(RegWrite == '1' && MemtoReg == '1' && RegDst == '0')
    {
        Reg[r2] = memdata;

    }
    else if(RegWrite == '1' && MemtoReg == '1' && RegDst == '1')
    {
        Reg[r3] = memdata;

    }
    else if(RegWrite == '1' && MemtoReg == '0' && RegDst == '0')
    {	
    	printf("r2 = %u\n", r2);
    	printf("ALUresult = %u\n", ALUresult);
    	printf("writing register...\n");
        Reg[r2] = ALUresult;

    }
    else if(RegWrite == '1' && MemtoReg == '0' && RegDst == '1')
    {	
        Reg[r3] = ALUresult;

    }
}

/* PC update */
/* Author: Zach Muller */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{	
	/* If instruction is a branch, calculate target address */
	//Use ZERO
	if(Branch == '1' && Zero == '1') {
		*PC = (*PC + 4) + (4 * extended_value);
	}
	else if(Jump == '1') {
		//Jump to instruction at address
		*PC = jsec;
	}
	/* PC+4 */
	else {
		*PC += 4;
	}
}