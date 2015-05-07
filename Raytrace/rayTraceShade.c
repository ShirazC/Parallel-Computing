
#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480

#define true 1
#define false 0
//
double ex = 0.5;
double ey = 0.5;
double ez = -1.0;
double lx=0.000000;
double ly=1.000000;
double lz=-0.500000;
int sphereCount = 3;
double floorY = 0.3333333;
double spheres[3][7] = {{0.500000,0.500000,0.166667,0.166667,0,0,255},
{0.833333,0.500000,0.500000,0.166667,0,255,0},
{0.333333,0.666667,0.666667,0.333333,255,0,0}};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

double* abcGen(double cx, double cy, double cz, double R, double rx, double ry, double rz, double ox, double oy, double oz)
{
   double a = 1;
   double b = -2*cx*rx -2*cy*ry - 2*cz*rz +2*ox*rx +2*oy*ry + 2*oz*rz;
   double c = cx*cx - 2*cx*ox + ox*ox + cy*cy - 2*cy*oy + oy*oy + cz*cz - 2*cz*oz + oz*oz - R*R;
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
   int rgb[HIGHT][WIDTH][3] ;
   int y , x ;
   FILE* fout ;
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         //Scale pizels to coordinates
         double px = (x + 0.5)/HIGHT;
         double py = 1- (y +0.5)/HIGHT;
         double pz = 0;
         //create eye to pixel ray
         double rx = px - ex;
         double ry = py - ey;
         double rz = pz - ez;
         double rmag = sqrt((rx*rx) + (ry*ry) + (rz*rz));
         rx = rx/rmag;
         ry = ry/rmag;
         rz = rz/rmag;

         double T;
         //Check the spheres the ray hits
         int sphereNum = 0;
         double tValArray[sphereCount];
         int lightIntersect = false;
         while(sphereNum < sphereCount)
         {
            double cx = spheres[sphereNum][0];
            double cy = spheres[sphereNum][1];
            double cz = spheres[sphereNum][2];
            double R = spheres[sphereNum][3];
            //Solve for T
            double* abcArr = abcGen(cx,cy,cz,R,rx,ry,rz,ex,ey,ez);
            double tVal = tQuadValue(abcArr);
            //Find current point using T
            tValArray[sphereNum] = tVal;
            sphereNum ++;
         }
         //Find min T value
         double* array = minFinder(tValArray);
         int currMinTVal = array[0];
         int minSphere = array[1];
         double currentX = ex + currMinTVal*rx;
         double currentY = ey + currMinTVal*ry;
         double currentZ = ez + currMinTVal*rz;
            //Create light ray from sphere to light
         double rlx = lx - currentX;
         double rly = ly - currentY;
         double rlz = lz - currentZ;

         double rlmag = sqrt((rlx*rlx) + (rly*rly) + (rlz*rlz));
         rlx = rlx/rlmag;
         rly = rly/rlmag;
         rlz = rlz/rlmag;

         int sphereNum2 = 0;
         double tValArray2[sphereCount];
         while(sphereNum2<sphereCount)
         {
            double clx = spheres[sphereNum2][0];
            double cly = spheres[sphereNum2][1];
            double clz = spheres[sphereNum2][2];
            double Rl = spheres[sphereNum2][3];
               //Solve for T again, this time it is the length of the ray that hits the light
            double* abcArrLight = abcGen(clx,cly,clz,Rl,rlx,rly,rlz,currentX , currentY, currentZ);
            double tValLight = tQuadValue(abcArrLight);

            tValArray2[sphereNum2] = tValLight;
            sphereNum2++;
         }
            //If the light intersects any sphere then the point is in shadow
         int w = 0;
         for (w = 0; w < sphereCount; ++w)
         {
               if (tValArray[w] > 0 && w != sphereNum) //!= -1)
{
   lightIntersect = true;
   break;
}
}
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
   if(lightIntersect==true)
   {
      rgb[y][x][0] = (int)(255 * 0.5);
      rgb[y][x][1] = (int)(255 * 0.5);
      rgb[y][x][2] = (int)(255 * 0.5); 
   }
   else
   {
      rgb[y][x][0] = 255;
      rgb[y][x][1] = 255;
      rgb[y][x][2] = 255; 
   }
}
}
else
{
   if(lightIntersect==true)
   {
      rgb[y][x][0] = (int)(spheres[minSphere][4]*0.5);
      rgb[y][x][1] = (int)(spheres[minSphere][5]*0.5);
      rgb[y][x][2] = (int)(spheres[minSphere][6]*0.5); 
   }
   else
   {
      rgb[y][x][0] = spheres[minSphere][4];
      rgb[y][x][1] = spheres[minSphere][5];
      rgb[y][x][2] = spheres[minSphere][6]; 
   }

}

}
}
fout = fopen( "shade.ppm" , "w" ) ;
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
