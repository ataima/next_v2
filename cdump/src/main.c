#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LENGTH 60


#define LINE 16


static  void dump_as_c_array(uint8_t * data_in, uint32_t data_size)
{
    uint32_t i,u,start;
    for(i=0; i<data_size;)
    {
        start=i;
        for(u=0; u<LINE && i<data_size; u++,i++)
        {
            if(i<data_size-1){
            printf("0x%02X,",data_in[i]);
            }else{
                printf("0x%02X",data_in[i]);
            }
        }
        if(i==data_size && u<LINE)
        {
            for(; u<LINE; u++)
            {
                printf(" ");
            }
        }
        i=start;
        printf(" // ");
        for(u=0; u<LINE && i<data_size; u++,i++)
        {
            if(data_in[i]>0x20)
            {
                printf("%c",data_in[i]);
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int main (void)
{

    uint8_t buff[0x1ffff];
    uint32_t count;

    while(!feof(stdin))
    {
        count = fread(buff, sizeof(char), sizeof(buff) / sizeof(char), stdin);
        dump_as_c_array(buff,count);
    }

    return EXIT_SUCCESS;
}
