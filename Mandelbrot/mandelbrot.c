// Torbert, 26 November 2013
// 
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "mpi.h"
// 
#define M 1200 
#define N 900 
// 
int count =  0 ; 
int ascii = 48 ; 
int trials = 100;
double xwidth = 4.0;
double ywidth = 3.0;
double pxorigin = 0;
double pyorigin = 0;
//
void genMandel(int xp, int yp, int trials)
{
	double xr = (xp * xwidth/1200.0) - (xwidth/2.0) + pxorigin;
	double yr = -1*((yp * ywidth/900.0) -  (ywidth/2.0))  + pyorigin;
	double a, b;
	//printf("%i\n",trials );
	a= 0.0;
	b = 0.0;
	int magHigh = 0;
	int i;
	 i = 0;

	while(i <= trials)
	{
		i++;
		double anew = a*a - b*b + xr;
		double bnew = 2*a*b + yr;
		a = anew;
		b = bnew;
		if (sqrt(a*a + b*b) > 2)
		{
			magHigh = 1;
			break;
		}
	}
	if(magHigh)
	{
			//glColor3f(.1 *log(i), .2*log(i), .3 *log(i)); //hype
		
			glColor3f(.1 *log(2*i), .2*log(2*i), .3 *log(2*i));
		
	}
	else
	{
		glColor3f(0,0,0);
	}
	glBegin(GL_POINTS);
	glVertex2f(xp, yp);
	glEnd();
}

void displayfunc(void)
{
  
   double xp , yp ;
   xp = 0;
   glClear(GL_COLOR_BUFFER_BIT);
   for( xp = 0 ; xp < M ; xp++ )
   {
      for( yp = 0 ; yp < N ; yp++ )
      { 
         genMandel(xp, yp, trials);
         glEnd();
      }
   }
   glutSwapBuffers() ;
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
   {
      printf("Left mouse clicked.\n");
      pxorigin = pxorigin + (xscr*xwidth/1200.0) -(xwidth/2.0);
      pyorigin = pyorigin + -1*((yscr*ywidth/900.0)- (ywidth/2.0));
      xwidth /= 2.0;
      ywidth /= 2.0;
      displayfunc();
      glutDisplayFunc(displayfunc);
      printf("Successfully Zoomed In\n");

   }
   else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
   {
      printf("Right mouse clicked.\n");
   }
}
void motionfunc(int xscr,int yscr)
{
	double xr = (xscr * xwidth/1200.0) - (xwidth/2.0) +pxorigin;
   double yr = -1*((yscr* ywidth/900.0) -  (ywidth/2.0))  + pyorigin;
   printf("Motion ( %f , %f ).\n" , xr , yr ) ;
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   printf("Key %c pressed.\n" , key);
}
void specialfunc(int key,int xscr,int yscr)
{
   if( key == GLUT_KEY_UP )
   {
      trials = trials +100;
      displayfunc();
      glutDisplayFunc(displayfunc);
      printf("Successfully Increased Resolution\n");
   }
   if( key == GLUT_KEY_DOWN )
   {
      trials = trials -100;
      displayfunc();
      glutDisplayFunc(displayfunc);
      printf("Successfully Zoomed Out\n");
   }
}
void closefunc(void)
{
   printf("Window closed.\n");
}
int main(int argc,char* argv[])
{  
  

   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(M,N);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   glViewport(0,0,(GLsizei)M,(GLsizei)N); // reshape
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*M,1.0*N,0.0); // invert y-coords
   glMatrixMode(GL_MODELVIEW);
   //
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(NULL);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyfunc);
   glutSpecialFunc(specialfunc);
   glutWMCloseFunc(closefunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
        