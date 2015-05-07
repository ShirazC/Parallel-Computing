#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct lNode
{
	char letter;
	int count;
	struct lNode* left;
	struct lNode* right;
}lNode;
int symbCount;
lNode** letterFreq(lNode **allLetters, FILE *file);
lNode** minLetterFreq(lNode **minLetters, lNode **allLetters);
lNode** sortByCount(lNode **minLetters);
lNode* makeTree(lNode **minLetters);
void walk(char letterChar, lNode *tree,int track, char *code, FILE *write);
void writeToFile(lNode **minLetters,lNode *tree);
char codeWord[256][50];
void encode(FILE *write);
int main()
{
	FILE *file;
	lNode **letterCount;
	lNode **minimizedLetterCount;
	char text[10000];
	lNode **otherminimizedLetterCount = (lNode**)malloc(sizeof(minimizedLetterCount));
	lNode *myTree = (lNode*)malloc(sizeof(lNode*));
	

	file = fopen("test.txt", "r");
	FILE *write = fopen("encoding.txt", "a");
	
	letterCount = letterFreq(letterCount, file);
	minimizedLetterCount = minLetterFreq(minimizedLetterCount, letterCount);
	minimizedLetterCount = sortByCount(minimizedLetterCount);
	otherminimizedLetterCount = minimizedLetterCount;

	myTree = makeTree(minimizedLetterCount);
	writeToFile(letterCount, myTree);
	encode( write);
	

	return 0;
}
void encode(FILE *write)
{

	FILE *file;

	file = fopen("test.txt", "r");
	
	if (file)
	{

		char ch;
		while(( ch=getc(file))!=EOF)
		{
			int ascii = (int)ch;
			int w =0;
			while(codeWord[ascii][w]!='\0')
			{

				fprintf(write, "%c",codeWord[ascii][w] );

				w++;
			}
		}
	}

}
void writeToFile(lNode **minLetters,lNode *tree)
{
	FILE *write;
	write = fopen("encoding.txt","a");
	fprintf(write, "%d\n", symbCount);
	int step =0;			
	printf("%i\n",minLetters[83]->count );
	while(step < 256)
	{
		if (minLetters[step]->count!=0)
		{
			char letterChar = minLetters[step]->letter;
			char code[70];
			walk(letterChar, tree,0, code, write);
		}
		step++;
	}
}
void walk(char letterChar, lNode *tree,int track, char *code, FILE *write)
{
	int i;
	if (tree->letter == letterChar)
	{
		code[track]='\0';
		track++;
		fprintf(write, "%c", letterChar);
		for (i = 0; i < track-1; ++i)
		{
			fprintf(write, "%c",code[i] );
		}
		fprintf(write, "\n");
		int ascii = (int)letterChar;
		for (i = 0; i < track; ++i)
		{
			codeWord[ascii][i]= code[i];
		}
	}
	if (tree->letter == '*')
	{
		code[track]= '0';
		walk(letterChar, tree->left,track+1, code, write);
		code[track]= '1';
		walk(letterChar, tree->right,track+1, code, write);
	}
	
}
lNode* makeTree(lNode **minLetters)
{
	int steps = symbCount;
	while(steps!=1)
	{
		printf("Node Count is: %d\n",steps );
		minLetters = sortByCount(minLetters);

		lNode* newNode = (lNode*)malloc(sizeof(lNode));
		newNode->letter = '*';
		int firstSmall = minLetters[steps-1]->count;
		int secondSmall = minLetters[steps-2]->count;

		newNode->count = firstSmall+secondSmall;
		newNode->left = minLetters[steps-1];
		newNode->right = minLetters[steps-2];
		
		minLetters[steps-2] = newNode;
		steps = steps-1;
	}

	return minLetters[0];
}
lNode** sortByCount(lNode **minLetters)
{
	int i,j;
	lNode* temp;
	for(i=0;i<symbCount;++i)
    {
        for(j=i;j<symbCount;++j)
        {
            if((minLetters[i]->count) < (minLetters[j]->count)){
                temp=minLetters[i];
                minLetters[i]=minLetters[j];
                minLetters[j]=temp;
            }
        }
    }
    return minLetters;
}
lNode** minLetterFreq(lNode **minLetters, lNode **allLetters)
{
	int x =0;
	symbCount = 0;
	minLetters = (lNode**)malloc((0)*sizeof(lNode*));
	while(x < 256)
	{
		if ((allLetters[x]->count)>0)
		{
			symbCount++;
			minLetters = (lNode**)realloc(minLetters,(symbCount)*sizeof(lNode*));
			lNode* n;
			minLetters[symbCount-1] = allLetters[x];
		}
		x ++;
	}
	return minLetters;
}
lNode** letterFreq(lNode **allLetters, FILE *file)
{
	char letters;
	allLetters = (lNode**)malloc(256*sizeof(lNode*));
	unsigned int fillCount =0;
	while(fillCount<256)
	{
		lNode* n;
		unsigned char charInt = (char)fillCount;
		n = (lNode*)malloc(sizeof(lNode*));
		n->count = 0;
		n->letter = charInt;
		allLetters[fillCount] = n;
		fillCount++;
	}
	while((letters = getc(file))!=EOF)
	{
		 unsigned intChar = (int)letters;
		 allLetters[intChar] -> count ++;
		 allLetters[intChar] -> letter = letters; 
	}

	return allLetters;
}
