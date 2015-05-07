
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "mpi.h"

#define bool int
#define true 1
#define false 0

double myrand();
char** genForest(int rows, int cols);
char** popForest(int rows, int cols, double prob, char** forest);
void printForest(int rows, int cols, char **forest);
bool totalBurn(int rows, int cols, char **forest);
bool checkForFire(int rows, int cols, char **forest);
char** firstColFire(int rows, int cols, char **forest);
int runSim(int rows, int cols, char **forest);
char** burn(int rows, int cols, char **forest);
char** backToFire(int rows, int cols, char **forest);

int main( int argc , char* argv[] )
{
   int rank;
   int size;
   MPI_Status status;
   int tag;
   MPI_Init(&argc, &argv) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

   int k, j;

   int rseed;
   rseed = time(NULL);
   srand(rseed);
   printf("\n");

   if (rank == 0)
   {
     double finalTrack = 0;
     double prob = 0.00;
     FILE *write = fopen("grapherMPI.txt","a");
     while(prob <= 1)
     {
        for( j = 1 ; j < size ; j++ )
        {
          MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
          prob = prob + 0.001;
        }
        for( k = 1 ; k < size ; k++ )
        {
          double curProb = 0.0;
          MPI_Recv( &curProb , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
          j = status.MPI_SOURCE ;
          MPI_Recv( &finalTrack , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD , &status ) ;
          fprintf(write,"%f %f\n",curProb,finalTrack );
        }
      }
      double bogus = -1;
      for (j = 1; j < size; ++j)
      {
       MPI_Send(&bogus, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
      }
      
    }
   else
   {
    double count = 0.00;
    int rows = 500; 
      int cols = 500; 
    while(true)
    {
      double totalTrack = 0;
      int trials = 200;
      double prob = 0.00;
      int currTrial = 0;
      MPI_Recv( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status );
      if (prob == -1)
      {
        break;
      }
      while(currTrial <= trials)
    {
      

      char **forest = genForest(rows,cols); 
      forest = popForest(rows,cols,prob, forest); 
      bool boolean = checkForFire(rows, cols, forest);
      int track = runSim(rows, cols, forest);
      totalTrack = totalTrack + track;
      int i = 0;
      for(i = 0; i < cols; i++)
      {
        free(forest[i]);
      }
        free(forest);
        currTrial++;
      }
      double finalTrack = totalTrack/trials;
      double burnCount = finalTrack/rows;
      MPI_Send( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
      MPI_Send( &burnCount , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
       count = count + 0.01;
    }
   }
   MPI_Finalize();
   
//    while(prob <= 1.00)
//    {
//     int totalTrack = 0;
//     int trials = 10;
//     int currTrial = 0;
//     while(currTrial <= trials)
//     {
//    int rows = 30; 
//    int cols = 30;   
//    char **forest = genForest(rows,cols); 
//    forest = popForest(rows,cols,prob, forest); 
   
//    //printForest(rows, cols, forest);//prints forest
//    bool boolean = checkForFire(rows, cols, forest);
//    int track = runSim(rows, cols, forest);
//    totalTrack = totalTrack + track;
//    for(int i = 0; i < cols; i++)
//    {
//     free(forest[i]);
//   }
//     free(forest);
//     currTrial++;
//   }
//   int finalTrack = totalTrack/trials;
//   fprintf(write, "%f %i\n",prob,finalTrack );
//    prob = prob + 0.01;

// }
 
   return 0;
}

int runSim(int rows, int cols, char **forest)
{
  //sleep(1);
  //system("clear");
	int track = 0;
	forest = firstColFire(rows, cols, forest);
	//printf("\n\n   Pass %i---------------------------\n\n",track);
  	//printForest(rows, cols, forest);
  if (checkForFire(rows,cols,forest)!=false)
  {
    track++;
  }
  else
  {
    return track;
  }
	while(checkForFire(rows,cols,forest)!=false)
	{
    //sleep(1);
    //system("clear");
		forest = burn(rows, cols, forest);
		//printf("\n\n   Pass %i---------------------------\n\n",track);
  		//printForest(rows, cols, forest);
  		track++;
	}
  return track;
}

char** burn(int rows, int cols, char **forest)
{
	int x =0;
	while(x <rows)
	{
		int y =0;
		while(y < cols)
		{
			if (forest[x][y]=='*')
			{
				forest[x][y]='-';
				if (x>0 && forest[x-1][y]=='X')
				{
					forest[x-1][y] = '&';
				}
				if (x<rows-1 && forest[x+1][y]=='X')
				{
					forest[x+1][y] = '&';
				}
				if (y>0 && forest[x][y-1] == 'X')
				{
					forest[x][y-1] = '&';
				}
				if (y<cols-1 && forest[x][y+1] == 'X')
				{
					forest[x][y+1] = '&';
				}
			}
			y++;
		}
		x++;
	}
	forest = backToFire(rows, cols, forest);
	return forest;
}

char** backToFire(int rows, int cols, char **forest)
{
	int x =0;
	while(x < rows)
	{
		int y =0;
		while(y < cols)
		{
			if (forest[x][y] == '&')
			{
				forest[x][y] = '*';
			}
			y++;
		}
		x++;
	}
	return forest;
}

char** firstColFire(int rows, int cols, char **forest)
{
   int x=0;
   while( x < rows)
   {	
   		if (forest[x][0] == 'X')
   		{
   			forest[x][0] = '*';
   		}
   		
   		x++;
   }
   return forest;
}

bool totalBurn(int rows, int cols, char **forest)
{
	int total = rows * cols;
	int track = 0;
	int x=0;
   while( x < rows)
   {
   		int y =0;
   		while(y <cols)
   		{
   			if (forest[x][y] == '-')
   			{
   				track++;
   			}
   			y++;
   		}
   		x++;
   }
   if (track == total)
   {
   		return true;
   }
   else 
   {
   	return false;
   }
}

bool checkForFire(int rows, int cols, char **forest)
{
	int x=0;
   while( x < rows)
   {
   		int y =0;
   		while(y <cols)
   		{
   			if (forest[x][y] == '*')
   			{
   				return true;
   			}
   			y++;
   		}
   		x++;
   }
  return false;
}

void printForest(int rows, int cols, char **forest)
{
   int x=0;
   while( x < rows)
   {
   		int y =0;
   		printf("    ");
   		while(y <cols)
   		{
   			printf("%c ",forest[x][y] );
   			y++;
   		}
   		printf("\n");
   		x++;
   }
}

char** genForest(int rows, int cols)
{
	char **forest = (char**)malloc(sizeof(char*)*rows);
	int makeForestCount = 0;
	while(makeForestCount<cols)
	{
		forest[makeForestCount] = (char*)malloc(sizeof(char*)*cols);
		makeForestCount++;
	}
    return forest;
}

char** popForest(int rows, int cols, double prob, char** forest)
{
	double deciProb = (prob);
	double theoryPercentage = prob * ((double)cols*rows);
	//printf("The theoretical number of trees in this forest is %i\n", (int)theoryPercentage);
	int x =0;
	int numTrees = 0;
    while( x < rows)
    {
   		int y =0;
   		while(y < cols)
   		{
   			double random = myrand();
   			if (random <= prob)
   			{
   				forest[x][y]= 'X';
   				numTrees++;
   			}
   			else
   			{
   				forest[x][y]= '-';
   			}
   			y++;
   		}
   		x++;
    }
    //printf("The actual number of trees in this forest is %i\n\n", numTrees );
    return forest;
}

double myrand()
{
   return ((rand() % 100000)/100000.0) ;
}