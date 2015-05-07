//Karthik Maiya

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int globCount;
int compBits;
char* freqList;
int* freqListNum;
int indexOf(char freqChar);
int codeCounter(char textArr[]);
char* createTree(int counter,char textArray[]);
int translate(char textArray[], char tree[]);
double shannon(int bitsEnc, int total);
int main()
{	
	globCount =0;
	int allCount = 0;
	int codeCount = 0;
	char *allText = (char*)malloc(allCount +1);
	FILE *file;
	char ch;
	file = fopen("decodeME.txt","r");
	while((ch = getc(file)) != EOF)
	{
		if(ch =='\n')
		{
			allText[allCount] = '#';
		}
		else
		{
			allText[allCount] = ch;
		}
		allCount++;	
		allText = (char*)realloc(allText,allCount+1);
	}
	allText[allCount] = 'x';
	fclose(file);
	codeCount = codeCounter(allText);
	freqList = (char*)malloc(codeCount+5);
	freqListNum = (int*)malloc((codeCount+5)*sizeof(int));
	int fillCount =0;
	while(fillCount < codeCount+5)
	{
		freqList[fillCount]='^';
		freqListNum[fillCount]=-1;
		fillCount++;
	}
	freqList[codeCount] = '^';
	char *tree;
	tree = createTree(codeCount,allText);
	tree[1]='~';
	int bitsEnc = translate(allText, tree);
	double shannonNum = shannon(bitsEnc, (bitsEnc/8));
	printf("\nThe original message was originally %i bits.\nThe compressed message is %d bits.",bitsEnc,compBits);
	printf("\nThe Shannon Number for this message is %i\n",(int)round(shannonNum));
	double ratio = (bitsEnc-(double)compBits)/bitsEnc;
	printf("Huffman was %i bits away from the best possible compression.\n",compBits-(int)round(shannonNum) );
	printf("The compression ratio is %f.\n", ratio);
	printf("The message was compressed to %f%% of its original size.\n", ratio*100);
	return 0;
}
double shannon(int bitsEnc, int total)
{
	int index =0;
	double sBits =0;
	while(freqListNum[index]!= -1)
	{
		double prob = freqListNum[index]/(double)total;
		int freq = freqListNum[index];
		double fina = (double)freq * -log2(prob);
		sBits += fina;
		index++;
	}
	return sBits;
}
int indexOf(char freqChar)
{
	int index =0;
	while(freqList[index]!='^')
	{	
		if(freqList[index]==freqChar)
			return index;
		index++;
	}
	return -99;
}
int translate(char textArray[], char tree[])
{
	compBits=0;
	int bitsEnc =0;
	int tracker =1;
	while(textArray[globCount]!='x')
	{
		if (textArray[globCount]=='1')
		{
			compBits++;
			tracker = 2*tracker +1;
			if (tree[tracker]!='~')
			{
				int index = indexOf(tree[tracker]);
				int freq = freqListNum[index];
				freq ++;
				freqListNum[index]=freq;
				printf("%c",tree[tracker]);
				tracker =1;
				bitsEnc+=8;
			}
		}
		else
		{
			compBits++;
			tracker = 2*tracker;
			if (tree[tracker]!='~')
			{
				int index = indexOf(tree[tracker]);
				int freq = freqListNum[index];
				freq ++;
				freqListNum[index]=freq;
				printf("%c",tree[tracker] );
				tracker =1;
				bitsEnc+=8;
			}
		}
		globCount++;
	}
	return bitsEnc;
}
char* createTree(int counter,char textArray[])
{
	int keepTrack = 1;
	char *tree = (char*)malloc(9999);
	char c;
	tree[0] = '%';
	int numNew;
	numNew =0;
	int freqCount=0;
	while(numNew<counter)
	{
		if(textArray[globCount]=='0')
		{
			keepTrack = 2*keepTrack;
			tree[keepTrack]='~';
			globCount++;
		}
		else if(textArray[globCount]=='1')
		{
			keepTrack = 2*keepTrack +1;
			tree[keepTrack]='~';
			globCount++;
		}
		else if(textArray[globCount]=='#')
		{
			numNew++;
			globCount++;
			tree[keepTrack] = c;
			keepTrack =1;
		}
		else if((textArray[globCount]!='0')&&(textArray[globCount]!='1')&&(textArray[globCount]!='#'))
		{
			c = textArray[globCount];
			globCount++;
			 freqList[freqCount]=c;
			 freqListNum[freqCount]=0;
			 freqCount++;
		}
	}
	return tree;
}
int codeCounter(char textArr[])
{
	int x = 0;
	int arraySize = 1;
	int *codeCount = (int*)malloc(0);
	while(textArr[x] != '#')
	{
			codeCount = (int*)realloc(codeCount,x+1);
			codeCount[x] = textArr[x] - '0';
			x++;
			arraySize++;
	}
	x++;
	globCount = x;
	int value;
	int scien =0;
	value =0;
	int i = arraySize-1;
	while(i>-1)
	{
		if (codeCount[i]>0 && codeCount[i]<9)
		{
		value += codeCount[i] * pow(10, scien);
		scien ++;
		}
		i = i-1;
	}
	free(codeCount);
	return value;
}