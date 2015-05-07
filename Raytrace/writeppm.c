
#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
//
double ex = 0.5;
double ey = 0.5;
double ez = -1.0;
int sphereCount = 3;
double spheres[3][7] = {{0.500000,0.500000,0.166667,0.166667,0,0,255},
                        {0.833333,0.500000,0.500000,0.166667,0,255,0},
                        {0.333333,0.666667,0.666667,0.333333,255,0,0}};
double floorY = 0.3333333;

double* abcGen(double cx, double cy, double cz, double R, double rx, double ry, double rz);
double tValue(double* array);
double tLinearValue();
double* minFinder(double* array);
double* abcGen(double cx, double cy, double cz, double R, double rx, double ry, double rz)
{
   double a = 1;
   double b = -2*cx*rx -2*cy*ry - 2*cz*rz +2*ex*rx +2*ey*ry + 2*ez*rz;
   double c = cx*cx - 2*cx*ex + ex*ex + cy*cy - 2*cy*ey + ey*ey + cz*cz - 2*cz*ez + ez*ez - R*R;
   double array[3];
   array[0] = a;
   array[1] = b;
   array[2] = c;
   return array;
}

double tQuadValue(double* array)
{
   double a = array[0];
   double b = array[1];
   double c = array[2];
   double discrim = b*b - 4*a*c;
   if (discrim < 0)
   {
      return -1;
   }
   else if (discrim > 0)
   {
      double solution = (-b + sqrt(discrim))/(2*a);
      return solution;
   }
}

double* minFinder(double* array)
{
   double currMinTVal = -100;
   int minSphere;
   int sphereNum = 0;
   while(sphereNum<sphereCount)
   {
      double temp = array[sphereNum];
      if (temp < 0)
      {
         sphereNum ++;
      }
      else
      {
         if (currMinTVal == -100)
         {
            currMinTVal = temp;
            minSphere = sphereNum;
         }
         else
         {
            if (temp < currMinTVal)
            {
               currMinTVal = temp;
               minSphere = sphereNum;
            }  
         }
          sphereNum ++;
      }
   }
   double result[2];
   result[0] = currMinTVal;
   result[1] = minSphere;
   return result;
}

double tLinearValue()
{

}

int main(void)
{
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   //
   FILE* fout ;
   //
    
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         double px = (x + 0.5)/HIGHT;
         double py = 1- (y +0.5)/HIGHT;
         double pz = 0;
         double rx = px - ex;
         double ry = py - ey;
         double rz = pz - ez;
         double rmag = sqrt((rx*rx) + (ry*ry) + (rz*rz));
         rx = rx/rmag;
         ry = ry/rmag;
         rz = rz/rmag;

         double T;

         int sphereNum = 0;
         double tValArray[sphereCount];
        
         while(sphereNum < sphereCount)
         {
            double cx = spheres[sphereNum][0];
            double cy = spheres[sphereNum][1];
            double cz = spheres[sphereNum][2];
            double R = spheres[sphereNum][3];

            double* abcArr = abcGen(cx,cy,cz,R,rx,ry,rz);
            double tVal = tQuadValue(abcArr);
            tValArray[sphereNum] = tVal;
            sphereNum ++;
         }
         double* array = minFinder(tValArray);
         int currMinTVal = array[0];
         int minSphere = array[1];
         if (currMinTVal == -100)
         {
            if (ry>=0)
            {
             rgb[y][x][0] = 0;
             rgb[y][x][1] = 0;
             rgb[y][x][2] = 0; 
            }
            else
            {
               rgb[y][x][0] = 255;
               rgb[y][x][1] = 255;
               rgb[y][x][2] = 255; 
            }
            

         }
         else
         {
            rgb[y][x][0] = spheres[minSphere][4];
            rgb[y][x][1] = spheres[minSphere][5];
            rgb[y][x][2] = spheres[minSphere][6]; 
         }
         
      }
   }
   //
   fout = fopen( "2dsphere.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , WIDTH , HIGHT ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   close( fout ) ;
   //
   return 0 ;
}




//
// end of file
//
