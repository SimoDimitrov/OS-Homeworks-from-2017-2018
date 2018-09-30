#include<stdio.h>
#include<string.h>
#include<stdint.h>

void *read_image(const char *filepath);

int main(int argc, char **argv)
{

        char *simo = read_image(argv[1]);
        uint32_t *r=simo+18;
	uint32_t *p=simo+22;

        printf("BMP image width: %d\n",*r);
        printf("BMP image height: %d\n",*p);
        return 0;

}


