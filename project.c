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

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];

}


/* Sign Extend */
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
    if(MemRead == '1')
    {
        *memdata = Mem[ALUresult];
    }

    if(MemWrite == '1')
    {
        Mem[ALUresult] = data2;
    }

    return 0;
}


/* Write Register */
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
        Reg[r2] = ALUresult;

    }
    else if(RegWrite == '1' && MemtoReg == '0' && RegDst == '1')
    {
        Reg[r3] = ALUresult;

    }

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

