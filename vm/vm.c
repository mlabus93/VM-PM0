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

instruction ReadInput(FILE* input, instruction instructions[], int *lineCount);
void PrintInstructions(int lineCount, instruction instructions[]);
void WriteInstructions(FILE* output, int lineCount, instruction instructions[]);
void FetchInstruction(instruction currentInstruction, instruction instructionRegister);
void ExecuteCycle(instruction instructionRegister, int pc);
void ExecuteOPRInstruction(instruction instructionRegister);
void ExecuteSIOInstruction(instruction instructionRegister);
void LIT(instruction instructionRegister);
void LOD(instruction instructionRegister);
void STO(instruction instructionRegister);
void CAL(int pc, instruction instructionRegister);
void INC(instruction instructionRegister);
void JMP(int pc, instruction instructionRegister);
void JPC(int pc, instruction instructionRegister);

int main(int argc, const char * argv[]) {
    
    FILE* input, *output;
    instruction instructions[MAX_CODE_LENGTH];
    instruction instructionRegister;
    
    int lineCount, pc = 0, sp = 0, bp = 1;
    
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
    PrintInstructions(lineCount, instructions);
    
    // Write instructions to output file
    WriteInstructions(output, lineCount, instructions);
    
    // Loop through all instructions
    while (pc < lineCount)
    {
        // Fetch Cycle
        {
            FetchInstruction(instructions[pc], instructionRegister);
            // Increment program counter
            pc++;
        }
        ExecuteCycle(instructionRegister, pc);
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

// Function for console debugging
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

void FetchInstruction(instruction currentInstruction, instruction instructionRegister)
{
    instructionRegister = currentInstruction;
}

void ExecuteCycle(instruction instructionRegister, int pc)
{
    // Do OPR instruction based on M
    if (instructionRegister.op == 2)
    {
        ExecuteOPRInstruction(instructionRegister);
    }
    // Do SIO instruction based on M
    else if (instructionRegister.op == 9)
    {
        ExecuteSIOInstruction(instructionRegister);
    }
    // Do all other instructions
    else
    {
        switch (instructionRegister.op) {
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
                CAL(pc, instructionRegister);
                break;
            case 6:
                INC(instructionRegister);
                break;
            case 7:
                JMP(pc, instructionRegister);
                break;
            case 8:
                JPC(pc, instructionRegister);
                break;
            default:
                break;
        }
    }
}

void ExecuteOPRInstruction(instruction instructionRegister)
{
    
}

void ExecuteSIOInstruction(instruction instructionRegister)
{
    
}

void LIT(instruction instructionRegister)
{
    
}

void LOD(instruction instructionRegister)
{
    
}

void STO(instruction instructionRegister)
{
    
}

void CAL(int pc, instruction instructionRegister)
{
    
}

void INC(instruction instructionRegister)
{
    
}

void JMP(int pc, instruction instructionRegister)
{
    
}

void JPC(int pc, instruction instructionRegister)
{
    
}





