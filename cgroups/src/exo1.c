#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    
        int size = atoi(argv[1]);
        if(size >=1)
        {
            for(int i = 0; i< size; i++)
            {
                printf("creation du %d ème octé \n", i);
                void * var = malloc(1048576);
                sleep(1);
            }
        }
}