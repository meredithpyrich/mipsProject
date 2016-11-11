#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    instruction = Mem[PC];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // A copy of the 32-bit instruction to manipulate with shifting.
    unsigned instructionCopy = instruction;

    instructionCopy >> 26;
    op = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 27;
    r1 = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 11;
    instructionCopy = instructionCopy >> 27;
    r2 = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 27;
    r3 = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 26;
    instructionCopy = instructionCopy >> 26;
    funct = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 16;
    offset = instructionCopy;

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 6;
    jsec = instructionCopy;
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

