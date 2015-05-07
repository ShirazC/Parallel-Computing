
#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
#define true 1
#define false 0
typedef struct vector
{
    double a;
    double b;
    double c;
} Vector;

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
int floorC[3] = {255,205,0};
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    double* abcGen(double cx, double cy, double cz, double R, double rx, double ry, double rz, double ox, double oy, double oz, double* array)
    {
        double a = 1;
        double b = -2*cx*rx -2*cy*ry - 2*cz*rz +2*ox*rx +2*oy*ry + 2*oz*rz;
        double c = cx*cx - 2*cx*ox + ox*ox + cy*cy - 2*cy*oy + oy*oy + cz*cz - 2*cz*oz + oz*oz - R*R;
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
            double solution = (-b - sqrt(discrim))/(2*a);
            return solution;
        }
    }
    double* minFinder(double* array, double* result)
    {
        double currMinTVal = -100;
        double minSphere = -1;
        int sphereNum = 0;
        while(sphereNum<sphereCount)
        {
            double temp = array[sphereNum];
            if (temp < 0)
            {
                sphereNum = sphereNum + 1;
            }
            else
            {
                if (currMinTVal == -100 && minSphere == -1)
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
        //double result[2];
        result[0] = currMinTVal;
        result[1] = minSphere;
        return result;
    }
    double tLinearValue(double rx, double ry, double rz, double ey, double floorY)
    {
        double minTFloor = (floorY-ey)/ry;
        return minTFloor;
    }
    double dot(double x1,double x2,double y1,double y2,double z1,double z2)
    {
        return (x1*x2) + (y1*y2) + (z1*z2);
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
                    double abcArr[3];
                    abcGen(cx,cy,cz,R,rx,ry,rz,ex,ey,ez, abcArr);
                    double tVal = tQuadValue(abcArr);
                    //Find current point using T
                    tValArray[sphereNum] = tVal;
                    sphereNum ++;
                }
                //Find min T value
                double answer[2];
                minFinder(tValArray, answer);
                double currMinTVal = answer[0];
                if(answer[1]>-0.5) answer[1] += 0.5;
                int minSphere = (int)answer[1];
                //printf("%d\n", minSphere );

                

               
                //Create light ray from sphere to light
                double currentX;
                double currentY;
                double currentZ;
                double normalX;
                double normalY;
                double normalZ;
                double nMag;
                if (minSphere== -1)
                {
                double floorT = tLinearValue(rx, ry, rz, ey,  floorY );
                currentX = ex + (floorT)*rx;
                currentY = ey + (floorT)*ry;
                currentZ = ez + (floorT)*rz;

                normalX = 0;
                normalY = 1;
                normalZ = 0;
                
            }
            else
            {
                currentX = ex + (currMinTVal)*rx;
                currentY = ey + (currMinTVal)*ry;
                currentZ = ez + (currMinTVal)*rz;

                normalX = (currentX  - spheres[minSphere][0])/spheres[minSphere][3];
                normalY = (currentY - spheres[minSphere][1])/spheres[minSphere][3];
                normalZ = (currentZ - spheres[minSphere][2])/spheres[minSphere][3];
                nMag = sqrt((currentX*currentX) + (currentY*currentY) + (currentZ*currentZ));
                normalX /= nMag;
                normalY /= nMag;
                normalZ /= nMag;

                currentX += 0.001*normalX;
                currentY += 0.001*normalY;
                currentZ += 0.001*normalZ;
            }
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
                     printf("%f %f %f\n",clx, cly, clz );
                    double Rl = spheres[sphereNum2][3];
                    //Solve for T again, this time it is the length of the ray that hits the light
                    double abcArrLight[3];
                    abcGen(clx,cly,clz,Rl,rlx,rly,rlz,currentX , currentY, currentZ, abcArrLight);
                    double tValLight = tQuadValue(abcArrLight);
                    tValArray2[sphereNum2] = tValLight;
                    sphereNum2++;
                }
                //If the light intersects any sphere then the point is in shadow
                int w = 0;
                for (w = 0; w < sphereCount; ++w)
                {
                    if (tValArray2[w] > 0 && w != minSphere) //!= -1)
                    {
                        lightIntersect = true;
                        break;
                    }
                }
                if (currMinTVal == -100 && minSphere == -1)
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
                            rgb[y][x][0] = (int)(floorC[0] * 0.5);
                            rgb[y][x][1] = (int)(floorC[1] * 0.5);
                            rgb[y][x][2] = (int)(floorC[2] * 0.5);
                        }
                        else
                        {
                            rgb[y][x][0] = floorC[0];
                            rgb[y][x][1] = floorC[1];
                            rgb[y][x][2] = floorC[2];
                        }
                    }
                }
                else
                {
                     double shadeDotProduct = dot(normalX, rlx, normalY, rly, normalZ, rlz);

                    if(lightIntersect==true)
                    {
                        rgb[y][x][0] = (int)(spheres[minSphere][4]*0.1);
                        rgb[y][x][1] = (int)(spheres[minSphere][5]*0.1);
                        rgb[y][x][2] = (int)(spheres[minSphere][6]*0.1);
                    }
                    else
                    {   
                        if (shadeDotProduct>0)
                        {
                        rgb[y][x][0] = (int)(spheres[minSphere][4] *shadeDotProduct);
                        rgb[y][x][1] = (int)(spheres[minSphere][5] *shadeDotProduct);
                        rgb[y][x][2] = (int)(spheres[minSphere][6] *shadeDotProduct);
                        }
                        else
                        {
                        rgb[y][x][0] = (int)(spheres[minSphere][4] *-shadeDotProduct);
                        rgb[y][x][1] = (int)(spheres[minSphere][5] *-shadeDotProduct);
                        rgb[y][x][2] = (int)(spheres[minSphere][6] *-shadeDotProduct);
                        }
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
                if (rgb[y][x][0]>255)
                {
                   rgb[y][x][0]=255;
                }
                if (rgb[y][x][1]>255)
                {
                   rgb[y][x][1]=255;
                }
                if (rgb[y][x][2]>255)
                {
                   rgb[y][x][2]=255;
                }
                fprintf( fout , "%d %d %d\n" ,
                rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
            }
        }
        close( fout ) ;
        //
        return 0 ;
    }
