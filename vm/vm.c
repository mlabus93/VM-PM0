//
//  main.c
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
void FetchCycle();
void ExecuteCycle();

int main(int argc, const char * argv[]) {
    
    FILE* input, *output;
    instruction instructions[MAX_CODE_LENGTH];
    int lineCount;
    
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if (input == NULL)
    {
        printf("FILE NOT FOUND\n");
        return -1;
    }
    *instructions = ReadInput(input, instructions, &lineCount);
    PrintInstructions(lineCount, instructions);
    WriteInstructions(output, lineCount, instructions);
    
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
        //printf("%d %d %d\n", instructions[i].op, instructions[i].l, instructions[i].m);
        i++;
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
}

void FetchCycle()
{
    
}

void ExecuteCycle()
{
    
}
