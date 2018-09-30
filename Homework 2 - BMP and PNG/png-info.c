#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdint.h>

void *read_image(const char *filepath);

int main(int argc, char **argv)
{

    char *simo = read_image(argv[1]);
	if(simo==NULL)
	{
	printf("Unable to open file");
	return 0;
	}
	int png=0;
	char *P = simo+1;
	char *N = simo+2;
	char *G = simo+3;
	if(*P=='P' && *N=='N' && *G=='G') png=1;
	if(png==0)
	{
	printf("Unable to parse file");
	return 0;
	}
        uint32_t *a=simo+16;
        uint32_t *b=simo+20;
		uint32_t width=ntohl(*a);
        uint32_t height=ntohl(*b);

        printf("PNG image width: %d\n",width);
        printf("PNG image height: %d\n",height);
        
	return 0;

}

