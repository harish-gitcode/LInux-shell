#include <stdio.h>
#include "shell.h"

void print_prompt1(void)
{
    fprintf(stderr, "@My_Shell>|$| ");
}

void print_prompt2(void)
{
    fprintf(stderr, "==> ");
}
