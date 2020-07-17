#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define UPDATEBLOCK 1024
int D[16];
void binRead(char *fileName);
void bitconverter(unsigned char byte, int *bit);
void crcgenerator(int *bit);


int main(int argc, char * argv[] )
{
    switch(argc)
    {
        case 0:
        case 1:
        	printf("enter the reading file.\n");
        	exit(1);
            break;
        case 2:
            break;
        default:
            printf("Too many arguments.\n");
            exit(1);
    }
    int i;
    //initialise D to all zeros
    for(i=0;i<16;i++)   D[i]=0;
	binRead(argv[1]);
	printf("\nchecksum : ");

	int checksum=0;
	for(i=15; i>=0; i--)
	{
		printf("%d",D[i]);
		checksum += D[i]*pow(2,i);
	}
	printf("\nchecksum : %04XH\n",checksum);
	return 0;
}

void binRead(char *fileName)
{
	FILE *file;
	file = fopen(fileName, "rb");
	if(!file)
	{
		printf("error in opening of the %s file\n",fileName);
		exit(1);
	}

	//finding size of file
	unsigned long int fileSize;
	unsigned long int fileSizeKb;
    fseek(file,0,SEEK_END);
    fileSize = ftell(file);
    fileSizeKb = fileSize / 1024 ;
    if(fileSize%1024) fileSizeKb++;
    rewind(file);
    printf("size of file = %d bytes.(%d KiloBytes)\n",fileSize,fileSizeKb);
	unsigned char *hexArray;
	//calloc automatically sets all bytes to zero after allocating
	hexArray = (unsigned char*) calloc(fileSize + 16 * sizeof(char), sizeof(char));
	if(!hexArray)
	{
		printf("memory allocation of hexArray failed!!!\n");
		exit(1);
	}
	unsigned int readSize;
	readSize = fread(hexArray, sizeof(char), fileSize ,file);
	if(readSize != fileSize)
	{
		printf("error in reading of the %s file\n", fileName);
		printf("read size = %d\n", readSize);
		exit(1);
	}
	else printf("Reading success!!!\n");
	fclose(file);

	unsigned long int i;
	int bitArray[8];

	float percent;
	for(i=0; i< fileSize + 16; i++)
    {
        if(i%UPDATEBLOCK)
        {
            percent = ((float)i/(fileSize+16))*100.00;
            printf("Hashing %d/%d KiloBytes.(%02.02f%% complete)\r",i/1024,fileSizeKb,percent);
        }
	   	bitconverter(hexArray[i],bitArray); //function call
	   	crcgenerator(bitArray); //function call
	}
	printf("Hashing 100%% complete.                                    \n");
    free(hexArray);
}

void bitconverter(unsigned char byte,int *bit)
{
    //converting byte to bits
	//printf("%02XH : ",byte);
	int i;
	for ( i = 7; i >= 0; i--) //i =7 becoz ,8 bit are creating from 1 byte
	{
	    bit[i] = (byte>>i)&1;
	    //printf("%d",bit[i]);
	}
    //printf("\n");
}

void crcgenerator(int *bit)
{
	int feedback;
	int i;
	for(i =0; i<8; i++)
	{
		feedback = bit[i] ^ D[15];
		D[15] = D[14] ^ feedback;
		D[14] = D[13];
		D[13] = D[12];
		D[12] = D[11];
		D[11] = D[10] ^ feedback;
		D[10] = D[9];
		D[9] = D[8];
		D[8] = D[7];
		D[7] = D[6];
		D[6] = D[5];
		D[5] = D[4];
		D[4] = D[3] ^ feedback;
		D[3] = D[2];
		D[2] = D[1];
		D[1] = D[0];
		D[0] = feedback;
	}
}


