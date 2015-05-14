#include <stdio.h>
#include <omp.h>
#include "math.h"
//
#define ROW 4
#define COL 8

void printMatrix(int matrix[][COL]);
void sum(int* initArray, int sumArray[][COL]);
void prefixsum(int sumArray[][COL], int prefixArray[][COL]);
void compaction(int bits[], int valuesArray[], int prefix[][COL], int outArray[]);
void printArray(int arr[], int size);
void mergeSort(int merge1[], int merge2[], int out[]);
void prefixmax(int sumArray[][COL], int prefixArray[][COL]);
void nearestOne2(int bitsArray[], int productArray[][COL], int prefixArray[][COL], int out[]);
void nearestOne(int prefixSum[][COL], int nearest[]);
void sumInitialMax(int* initArray, int sumArray[][COL]);


void sum(int* initArray, int sumArray[][COL])
{
   int rank;
   omp_set_num_threads(COL);
   #pragma omp parallel private(rank)
   {
      rank = omp_get_thread_num();
      sumArray[0][rank] = initArray[rank];
   }
     
      printf("\n");
   int currRow = 1;
   while(currRow < ROW)
   {
      omp_set_num_threads(COL/ pow(2,currRow));
      #pragma omp parallel private(rank)
      {
         rank = omp_get_thread_num();
         sumArray[currRow][rank] = sumArray[currRow -1][2*rank + 1] + sumArray[currRow -1][2*rank];
      }
      currRow++;
   } 
}

void sumInitialMax(int* initArray, int sumArray[][COL])
{
   int rank;
   omp_set_num_threads(COL);
   #pragma omp parallel private(rank)
   {
      rank = omp_get_thread_num();
      sumArray[0][rank] = initArray[rank];
   }
     
      printf("\n");
   int currRow = 1;
   while(currRow < ROW)
   {
      omp_set_num_threads(COL/ pow(2,currRow));
      #pragma omp parallel private(rank)
      {
         rank = omp_get_thread_num();
         if (sumArray[currRow -1][2*rank + 1] > sumArray[currRow -1][2*rank])
         {
            sumArray[currRow][rank] = sumArray[currRow -1][2*rank + 1];
         }
         else
         {
            sumArray[currRow][rank] = sumArray[currRow -1][2*rank];
         }
      }
      currRow++;
   }
   
}
void printMatrix(int matrix[][COL])
{
   int i;
   int j;

   for (i = 0; i < ROW; ++i)
   {
      for (j = 0; j < COL; ++j)
      {
         printf("%d ", matrix[i][j]);
      }
      printf("\n");
   }
}

void printArray(int arr[], int size)
{
   int i;
   for (i = 0; i < size; ++i)
   {
      printf("%d ", arr[i]);
   }
   printf("\n");
}

void prefixsum(int sumArray[][COL], int prefixArray[][COL])
{
   int rank; 
   int currRow = ROW -1;
   while(currRow >= 0)
   {
      omp_set_num_threads(COL/ pow(2, currRow));
      #pragma omp parallel private(rank)
      {
         rank = omp_get_thread_num();
         if (rank == 0)
         {
            prefixArray[currRow][rank] = sumArray[currRow][rank];
         }
         else if ((rank - 1) % 2 == 0)
         {
            prefixArray[currRow][rank] = prefixArray[currRow + 1][rank/2];
         }
         else if ((rank - 1) % 2 == 1)
         {
            prefixArray[currRow][rank] = prefixArray[currRow + 1][(rank-1)/2] + sumArray[currRow][rank];
         }
      }
      currRow--;
   }
}

void compaction(int bits[], int valuesArray[], int prefix[][COL], int outArray[])
{
   int rank;
   omp_set_num_threads(COL);
   #pragma omp parallel private(rank)
   {
      rank = omp_get_thread_num();
      if (bits[rank]==1)
      {
         int index = prefix[0][rank]-1;
         outArray[index] = valuesArray[rank];
      }
   }
}

void mergeSort(int merge1[], int merge2[], int out[])
{
   int i;
   omp_set_num_threads(COL);
   #pragma omp parallel private(i)
   {
      i = omp_get_thread_num();
      int val = merge1[i];
      int rank;
      if (val < merge2[0])
      {
         rank = 0;
      }
      else if(val > merge2[7])
      {
         rank = 8;
      }
      else
      {
         int left = 0;
         int right = 7;
         while(left != right && left< right)
         {
            // int middle = left + (right - left)/2;
            int middle = (left + right)/2;
            if (val < merge2[middle])
            {
               right = middle -1;
            }
            else
            {
               left = middle + 1;
            }
         }
         rank = left;

      }
      out[i + rank] = val;
   }

   #pragma omp parallel private(i)
   {
      i = omp_get_thread_num();
      int val = merge2[i];
      int rank;
      if (val < merge1[0])
      {
         rank = 0;
      }
      else if(val > merge1[7])
      {
         rank = 8;
      }
      else
      {
         int left = 0;
         int right = 7;
         while(left != right && left< right)
         {
            // int middle = left + (right - left)/2;
            int middle = (left + right)/2;
            if (val < merge1[middle])
            {
               right = middle -1;
            }
            else
            {
               left = middle + 1;
            }
         }
         rank = left;
      }
      out[i + rank] = val;
   }
}

void nearestOne2(int bitsArray[], int productArray[][COL], int prefixArray[][COL], int out[])
{
   omp_set_num_threads(COL);
   int i;
   #pragma omp parallel private(i)
   {
      i = omp_get_thread_num();
      productArray[0][i] = i*bitsArray[i];
   }
   prefixmax(productArray, prefixArray);
}

void nearestOne(int prefixSum[][COL], int nearest[])
{
   omp_set_num_threads(COL);
   int i;
   #pragma omp parallel private(i)
   {
      i = omp_get_thread_num();
      nearest[i] = prefixSum[0][i] - 1;
   }
}

void prefixmax(int sumArray[][COL], int prefixArray[][COL])
{
   int rank; 
   int currRow = ROW -1;
   while(currRow >= 0)
   {
      omp_set_num_threads(COL/ pow(2, currRow));
      #pragma omp parallel private(rank)
      {
         rank = omp_get_thread_num();
         if (rank == 0)
         {
            prefixArray[currRow][rank] = sumArray[currRow][rank];
         }
         else if ((rank - 1) % 2 == 0)
         {
            prefixArray[currRow][rank] = prefixArray[currRow + 1][rank/2];
         }
         else if ((rank - 1) % 2 == 1)
         {
            if (prefixArray[currRow + 1][(rank-1)/2] > sumArray[currRow][rank])
            {
               prefixArray[currRow][rank] = prefixArray[currRow + 1][(rank-1)/2];
            }
            else
            {
               prefixArray[currRow][rank] = sumArray[currRow][rank];
            }
            
         }
      }
      currRow--;
   }
}

int main( int argc , char* argv[] )
{
   //Initialization
   int rank, size;
   int initArray[] = {3, 1, 4, 1, 5, 9, 2, 6};
   int bitsArray[] = {1,0,1,1,0,1,0,0};
   int valsArray[] = {0,1,2,3,4,5,6,7};
   // Sum
   printf("\nSum");
   int sumArray[ROW][COL] = {0};
   sum(bitsArray, sumArray);
   printMatrix(sumArray);

   //Sum Max
   printf("\nSum: Max");
   int sumMaxArray[ROW][COL] = {0};
   sum(bitsArray, sumArray);
   printMatrix(sumMaxArray);

   // Prefix Sum
   printf("\nPrefix Sum\n");
   int prefixArray[ROW][COL] = {0};
   prefixsum(sumArray, prefixArray);
   printMatrix(prefixArray);

   // Prefix Sum
   printf("\nPrefix Max\n");
   int prefixMaxArray[ROW][COL] = {0};
   prefixsum(sumMaxArray, prefixMaxArray);
   printMatrix(prefixMaxArray);

   //Compaction
   printf("\nCompaction\n");
   int bitsOut = 4;
   int compactionArray[bitsOut];
   compaction(bitsArray, initArray, prefixArray, compactionArray);
   printArray(compactionArray , 4);

   //Nearest ONe
   printf("\nNearest One\n");
   int nearestOneOut[COL] = {0};
   nearestOne(prefixArray, nearestOneOut);
   printArray(nearestOneOut, 8);

   //Merge Sort
   printf("\nMerge Sort\n");
   int merge1[] = {1, 2, 3, 4, 9, 10, 11, 12};
   int merge2[] = {5, 6, 7, 8, 13, 14, 15, 16 };
   int mergeOut[16];
   mergeSort(merge1, merge2, mergeOut);
   printArray(mergeOut, 16);
}
