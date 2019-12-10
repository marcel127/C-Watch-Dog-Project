#include <stdio.h>
#include <time.h>

#define RESET   	"\033[0m"       

#include "watch_dog.h"

int main(int argc, char const *argv[])
{
    time_t time_to_run = {0};
    
    if(0 != WDKeepAlive(NULL, argc ,argv))
    {
        printf("failed\n");
        return 1;
    }

    time_to_run = time(NULL) + 60;

    /*app work*/
    while (time_to_run != time(NULL));    
    /*app work*/

    WDFree();
    printf(RESET"bye bye app\n");
    
    return 0;
}