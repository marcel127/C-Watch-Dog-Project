#include <stdio.h>
#include <unistd.h>

#include "watch_dog.h"

#define RESET   	"\033[0m"       

int main(int argc, char const *argv[])
{
    if (0 != WDKeepAlive(NULL, argc ,argv))
    {
        return 1;
    }
    
    printf(RESET"WD FINISH\n");

    return 0;
}
