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
    // If the address is not word aligned or if it is trying
    // to access an address beyond the memory, halt.
    if (PC % 4 != 0 || (PC >> 2) >= sizeof(Mem))
        return 1;

    instruction = Mem[PC >> 2];

    // Return zero, for there was no halt.
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // A copy of the 32-bit instruction to manipulate with shifting.
    unsigned instructionCopy = instruction;

    //Thanks for catching my syntax errors, Zach. :)

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
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // Pulled the opcodes and their cooresponding values from the ppt

    // Set everything to 0 to default.
    controls.RegDst = '0';
    controls.Jump = '0';
    controls.ALUSrc = '0';
    controls.MemtoReg = '0';
    controls.RegWrite = '0';
    controls.MemRead = '0';
    controls.MemWrite = '0';
    controls.Branch = '0';
    controls.ALUOp = '0';

    // R-format
    if (op == 0)
    {
        controls.RegDst = '1';
        controls.ALUSrc = '0';
        controls.MemtoReg = '0';
        controls.RegWrite = '1';
        controls.MemRead = '0';
        controls.MemWrite = '0';
        controls.Branch = '0';
        controls.ALUOp = '2';
    }
    // lw
    else if (op == 35)
    {
        controls.RegDst = '0';
        controls.ALUSrc = '1';
        controls.MemtoReg = '1';
        controls.RegWrite = '1';
        controls.MemRead = '1';
        controls.MemWrite = '0';
        controls.Branch = '0';
        controls.ALUOp = '0';
    }
    // sw
    else if (op == 43)
    {
        controls.RegDst = '2';
        controls.ALUSrc = '1';
        controls.MemtoReg = '2';
        controls.RegWrite = '0';
        controls.MemRead = '0';
        controls.MemWrite = '1';
        controls.Branch = '0';
        controls.ALUOp = '0';
    }
    // beq
    else if (op == 4)
    {
        controls.RegDst = '2';
        controls.ALUSrc = '0';
        controls.MemtoReg = '2';
        controls.RegWrite = '0';
        controls.MemRead = '0';
        controls.MemWrite = '0';
        controls.Branch = '1';
        controls.ALUOp = '1';
    }
    else
    {
        // Opcode doesn't equal any of the values here.
        // Illegal instruction error, halt.
        return 1;
    }

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

