#include <stdio.h>
#include "inc/rom.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s rom.gb\n", argv[0]);
        return 1;
    }

    load_rom(argv[0]);

    return 0;
}
