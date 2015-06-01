//
//  vm.c
//  vm
//
//  Created by Michael Labus on 5/30/15.
//
//

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct {
    int op; // opcode
    int l;  // L
    int m;  // M
} instruction;

/*instruction ReadInput(FILE* input, instruction instructions[], int *lineCount);
void PrintInstructions(int lineCount, instruction instructions[]);
void WriteInstructions(FILE* output, int lineCount, instruction instructions[]);
void FetchInstruction(instruction currentInstruction, instruction *instructionRegister);
void ExecuteCycle(FILE* ouput, instruction *instructionRegister, int stack[], int pc, int sp, int bp, int *haltFlag);
void ExecuteOPRInstruction(instruction *instructionRegister, int stack[], int pc, int sp, int bp);
void ExecuteSIOInstruction(FILE* output, instruction *instructionRegister, int stack[], int pc, int sp, int bp, int *haltFlag);
void LIT(instruction *instructionRegister, int stack[], int sp);
void LOD(instruction *instructionRegister, int stack[], int sp, int bp);
void STO(instruction *instructionRegister, int stack[], int sp, int bp);
void CAL(instruction *instructionRegister, int stack[], int pc, int sp, int bp);
void INC(instruction *instructionRegister, int sp);
void JMP(instruction *instructionRegister, int pc);
void JPC(instruction *instructionRegister, int stack[], int pc, int sp);
int Base(int level, int b, int stack[]);
void PrintStack(int stack[], int sp);*/


instruction ReadInput(FILE* input, instruction instructions[], int *lineCount);
void PrintInstructions(int lineCount, instruction instructions[]);
void Output(FILE* output, int lineCount, instruction instructions[]);
void WriteInstructions(FILE* output, int lineCount, instruction instructions[]);
void FetchInstruction(instruction currentInstruction, instruction *instructionRegister);
void ExecuteCycle(FILE* ouput, instruction *instructionRegister, int *haltFlag);
void ExecuteOPRInstruction(instruction *instructionRegister);
void ExecuteSIOInstruction(FILE* output, instruction *instructionRegister, int *haltFlag);
void LIT(instruction *instructionRegister);
void LOD(instruction *instructionRegister);
void STO(instruction *instructionRegister);
void CAL(instruction *instructionRegister);
void INC(instruction *instructionRegister);
void JMP(instruction *instructionRegister);
void JPC(instruction *instructionRegister);
int Base(int level, int b);
void PrintStack(FILE* output, int flag, instruction instructions[]);




int lineCount, pc = 0, sp = 0, bp = 1;
int stack[MAX_STACK_HEIGHT];


int main(int argc, const char * argv[]) {
    
    FILE* input, *output;
    
    instruction instructions[MAX_CODE_LENGTH];
    instruction instructionRegister;
    
    
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    
    int calFlag = 0;
    int haltFlag = 0;
    
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    
    // Test if file exists
    if (input == NULL)
    {
        printf("FILE NOT FOUND\n");
        return -1;
    }
    
    // Read input instructions into array of structs
    *instructions = ReadInput(input, instructions, &lineCount);
    
    //Print instructions to console for debugging
    //PrintInstructions(lineCount, instructions);
    
    // Write instructions to output file
    WriteInstructions(output, lineCount, instructions);
    
    //fprintf(output, "\n");
    
    //Output(output, lineCount, instructions);
    
    //fprintf(output, "Initial values           pc    bp    sp     stack\n");
    
    
    fprintf(output, "STACK");
    // Loop through all instructions
    while (pc < lineCount)
    {
        // Fetch Cycle
        {
            FetchInstruction(instructions[pc], &instructionRegister);
            // Increment program counter
            pc++;
        }
        ExecuteCycle(output, &instructionRegister, &haltFlag);
        if (instructionRegister.op == 5)
        {
            calFlag = 1;
            PrintStack(output, 0, instructions);
        }
        else if (instructionRegister.op == 4 && instructionRegister.l > 0)
        {
            calFlag = 0;
        }
        else
        {
            PrintStack(output, calFlag, instructions);
        }
        //printf("\n");
        fprintf(output, "\n");
        if (haltFlag == 1)
            break;
    }
    
    // Close files
    fclose(input);
    fclose(output);
    
    return 0;
}

instruction ReadInput(FILE* input, instruction instructions[], int *lineCount)
{
    int i = 0;
    while (!feof(input))
    {
        fscanf(input, "%d %d %d", &instructions[i].op, &instructions[i].l, &instructions[i].m);
        if (instructions[i].l >= MAX_LEXI_LEVELS)
            printf("Invalid instruction: L must be 3 levels or less.\n");
        i++;
        if (i == MAX_CODE_LENGTH)
            printf("Max code length of %d reached.\n", MAX_CODE_LENGTH);
    }
    *lineCount = i;
    return *instructions;
}

void Output(FILE* output, int lineCount, instruction instructions[])
{
    //fprintf(output, "Line  OP    L    M\n");
    //fprintf(output, "-------------------\n");
    for (int i = 0; i < lineCount; i++)
    {
        if (i < 9)
            fprintf(output, " ");
        switch (instructions[i].op) {
            case 1:
                fprintf(output, "%d    LIT   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 2:
                fprintf(output, "%d    OPR   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 3:
                fprintf(output, "%d    LOD   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 4:
                fprintf(output, "%d    STO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 5:
                fprintf(output, "%d    CAL   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 6:
                fprintf(output, "%d    INC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 7:
                fprintf(output, "%d    JMP   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 8:
                fprintf(output, "%d    JPC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 9:
                fprintf(output, "%d    SIO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            default:
                fprintf(output, "Invalid instruction.\n");
                break;
        }
    }
    fprintf(output, "\n");
}

void WriteInstructions(FILE* output, int lineCount, instruction instructions[])
{
    fprintf(output, "Line  OP    L    M\n");
    fprintf(output, "-------------------\n");
    for (int i = 0; i < lineCount; i++) {
        if (i < 9)
            fprintf(output, " ");
        switch (instructions[i].op) {
            case 1:
                fprintf(output, "%d    LIT   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 2:
                fprintf(output, "%d    OPR   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 3:
                fprintf(output, "%d    LOD   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 4:
                fprintf(output, "%d    STO   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 5:
                fprintf(output, "%d    CAL   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 6:
                fprintf(output, "%d    INC   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 7:
                fprintf(output, "%d    JMP   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 8:
                fprintf(output, "%d    JPC   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 9:
                fprintf(output, "%d    SIO   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            default:
                fprintf(output, "Invalid instruction.\n");
                break;
        }
    }
    fprintf(output, "\n");
}


void PrintInstructions(int lineCount, instruction instructions[])
{
    printf("Line  OP    L    M\n");
    printf("-------------------\n");
    for (int i = 0; i < lineCount; i++) {
        if (i < 9)
            printf(" ");
        switch (instructions[i].op) {
            case 1:
                printf("%d    LIT   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 2:
                printf("%d    OPR   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 3:
                printf("%d    LOD   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 4:
                printf("%d    STO   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 5:
                printf("%d    CAL   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 6:
                printf("%d    INC   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 7:
                printf("%d    JMP   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 8:
                printf("%d    JPC   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            case 9:
                printf("%d    SIO   %d    %d\n", i+1, instructions[i].l, instructions[i].m);
                break;
            default:
                printf("Invalid instruction.\n");
                break;
        }
    }
    printf("\n");
}

void FetchInstruction(instruction currentInstruction, instruction *instructionRegister)
{
    instructionRegister->op = currentInstruction.op;
    instructionRegister->l = currentInstruction.l;
    instructionRegister->m = currentInstruction.m;
}

void ExecuteCycle(FILE* output, instruction *instructionRegister, int *haltFlag)
{
    // Do OPR instruction based on M
    if (instructionRegister->op == 2)
    {
        ExecuteOPRInstruction(instructionRegister);
    }
    // Do SIO instruction based on M
    else if (instructionRegister->op == 9)
    {
        ExecuteSIOInstruction(output, instructionRegister, haltFlag);
    }
    // Do all other instructions
    else
    {
        switch (instructionRegister->op) {
            case 1:
                LIT(instructionRegister);
                break;
            case 3:
                LOD(instructionRegister);
                break;
            case 4:
                STO(instructionRegister);
                break;
            case 5:
                CAL(instructionRegister);
                break;
            case 6:
                INC(instructionRegister);
                break;
            case 7:
                JMP(instructionRegister);
                break;
            case 8:
                JPC(instructionRegister);
                break;
            default:
                break;
        }
    }
}

void ExecuteOPRInstruction(instruction *instructionRegister)
{
    switch (instructionRegister->m) {
        // RET
        case 0:
            sp = bp - 1;
            pc = stack[sp + 4];
            bp = stack[sp + 3];
            break;
        // NEG
        case 1:
            stack[sp] = -stack[sp];
            break;
        // ADD
        case 2:
            sp = sp - 1;
            stack[sp] = stack[sp] + stack[sp + 1];
            break;
        // SUB
        case 3:
            sp = sp - 1;
            stack[sp] = stack[sp] - stack[sp + 1];
            break;
        // MUL
        case 4:
            sp = sp - 1;
            stack[sp] = stack[sp] * stack[sp + 1];
            break;
        // DIV
        case 5:
            sp = sp - 1;
            stack[sp] = stack[sp] / stack[sp + 1];
            break;
        // ODD
        case 6:
            stack[sp] = stack[sp] % 2;
            break;
        // MOD
        case 7:
            sp = sp + 1;
            stack[sp] = stack[sp] % stack[sp + 1];
            break;
        // EQL
        case 8:
            sp = sp - 1;
            stack[sp] = stack[sp] == stack[sp + 1];
            break;
        // NEQ
        case 9:
            sp = sp - 1;
            stack[sp] = stack[sp] != stack[sp + 1];
            break;
        // LSS
        case 10:
            sp = sp - 1;
            stack[sp] = stack[sp] < stack[sp + 1];
            break;
        // LEQ
        case 11:
            sp = sp - 1;
            stack[sp] = stack[sp] <= stack[sp + 1];
            break;
        // GTR
        case 12:
            sp = sp - 1;
            stack[sp] = stack[sp] > stack[sp + 1];
            break;
        // GEQ
        case 13:
            sp = sp - 1;
            stack[sp] = stack[sp] >= stack[sp + 1];
            break;
        default:
            printf("Invalid M value.\n");
            break;
    }
}

void ExecuteSIOInstruction(FILE* output, instruction *instructionRegister, int *haltFlag)
{
    switch (instructionRegister->m) {
        // Pop stack and print
        case 0:
            printf("%d\n", stack[sp]);
            sp = sp - 1;
            break;
        // Read input and push stack
        case 1:
            sp = sp + 1;
            printf("Enter input: ");
            scanf("%d", &stack[sp]);
            break;
        // Halt
        case 2:
            *haltFlag = 1;
            break;
        default:
            printf("Invalid M value");
            break;
    }
}

void LIT(instruction *instructionRegister)
{
    sp = sp + 1;
    stack[sp] = instructionRegister->m;
}

void LOD(instruction *instructionRegister)
{
    sp = sp + 1;
    stack[sp] = stack[Base(instructionRegister->l, bp)];
}

void STO(instruction *instructionRegister)
{
    stack[Base(instructionRegister->l, bp) + instructionRegister->m] = stack[sp];
    sp = sp - 1;
}

void CAL(instruction *instructionRegister)
{
    // return value
    stack[sp + 1] = 0;
    // static link
    stack[sp + 2] = Base(instructionRegister->l, bp);
    // dynamic link
    stack[sp + 3] = bp;
    // return address
    stack[sp + 4] = pc;
    
    bp = sp + 1;
    pc = instructionRegister->m;
}

void INC(instruction *instructionRegister)
{
    sp = sp + instructionRegister->m;
}

void JMP(instruction *instructionRegister)
{
    pc = instructionRegister->m;
}

void JPC(instruction *instructionRegister)
{
    if (stack[sp] == 0)
        pc = instructionRegister->m;
    sp = sp - 1;
}

// Function to find base, L levels down
int Base(int level, int b)
{
    while (level > 0)
    {
        b = stack[b+2];
        level--;
    }
    return b;
}

void PrintStack(FILE* output, int flag, instruction instructions[])
{
    for (int i = 1; i <= sp/2; i++)
    {
        /*fprintf(output, " ");
        switch (instructions[i].op) {
            case 1:
                fprintf(output, "%d    LIT   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 2:
                fprintf(output, "%d    OPR   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 3:
                fprintf(output, "%d    LOD   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 4:
                fprintf(output, "%d    STO   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 5:
                fprintf(output, "%d    CAL   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 6:
                fprintf(output, "%d    INC   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 7:
                fprintf(output, "%d    JMP   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 8:
                fprintf(output, "%d    JPC   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            case 9:
                fprintf(output, "%d    SIO   %d    %d\n", pc, instructions[sp].l, instructions[sp].m);
                break;
            default:
                fprintf(output, "Invalid instruction.\n");
                break;
        }*/
        
        //printf("%d ", stack[i]);
        fprintf(output, "%d ", stack[i]);
    }
    if (flag == 1)
    {
        //printf("| ");
        fprintf(output, "| ");
    }
    for (int i = sp/2 + 1; i <= sp; i++)
    {
        
        
            /*fprintf(output, " ");
        switch (instructions[i].op) {
            case 1:
                fprintf(output, "%d    LIT   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 2:
                fprintf(output, "%d    OPR   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 3:
                fprintf(output, "%d    LOD   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 4:
                fprintf(output, "%d    STO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 5:
                fprintf(output, "%d    CAL   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 6:
                fprintf(output, "%d    INC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 7:
                fprintf(output, "%d    JMP   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 8:
                fprintf(output, "%d    JPC   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            case 9:
                fprintf(output, "%d    SIO   %d    %d\n", pc, instructions[i].l, instructions[i].m);
                break;
            default:
                fprintf(output, "Invalid instruction.\n");
                break;
        }*/
        //printf("%d ", stack[i]);
        fprintf(output, "%d ", stack[i]);
    }
    //fprintf(output, "\n");
}





