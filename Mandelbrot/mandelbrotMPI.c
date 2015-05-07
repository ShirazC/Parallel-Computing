
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "mpi.h"
// 
#define M 1200 
#define N 900 
int trials = 100;
double xwidth = 4.0;
double ywidth = 3.0;
double pxorigin = 0;
double pyorigin = 0;
MPI_Status status;
int rank   ;
int size   ;
int tag    ;
int j ;
//
int* genMandel(int xp)
{
	double xr = (xp * xwidth/1200.0) - (xwidth/2.0) + pxorigin;
	double a, b;
	a= 0.0;
	b = 0.0;
	int i;

 int array[N];
 int counter = 0;
 for(counter = 0; counter < N; counter++)
 {
   i = 0;
   a=0;
   b = 0;
   while(i <= trials)
   {
      double yr = -1*((counter * ywidth/900.0) -  (ywidth/2.0))  + pyorigin;
      i++;
      double anew = a*a - b*b + xr;
      double bnew = 2*a*b + yr;
      a = anew;
      b = bnew;
      if (sqrt(a*a + b*b) > 2)
      {
         break;
      }
      
   }
   array[counter] = i;
}
MPI_Send(array, N, MPI_INT, 0, tag, MPI_COMM_WORLD);
}

void displayfunc(void)
{

   //double xr = (xp * xwidth/1200.0) - (xwidth/2.0) + pxorigin;
   glClear(GL_COLOR_BUFFER_BIT);
   glFlush();
   //glutSwapBuffers();
   //double xValues[];
   int col = 0;
   double workerDist[size];
   int i;
   for (i = 1; i < size; ++i)
   {
      MPI_Send(&col, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      MPI_Send(&xwidth, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
      MPI_Send(&ywidth, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
      MPI_Send(&pxorigin, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
      MPI_Send(&pyorigin, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
      MPI_Send(&trials, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
      workerDist[j] = col;
       col += 1;
   }
   while(col<M)
   {
      int tval[N];
      MPI_Recv(&tval, N, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
      j = status.MPI_SOURCE;
      int currentWorker = j;
      int currentX = workerDist[currentWorker];
      workerDist[j] = col;

      int ycurr;
      for (ycurr = 0; ycurr < N; ++ycurr)
      {
         int currMaxTrials = tval[ycurr];
         if (currMaxTrials != (trials-1))
         {
            glColor3f(.1 *log(2*currMaxTrials), .2*log(2*currMaxTrials), .3 *log(2*currMaxTrials));
         }
         else
         {
            glColor3f(200,200,200);
         }
         glBegin(GL_POINTS);
         glVertex2f(currentX, ycurr);
         glEnd();

      }
      glFlush();
      MPI_Send(&col, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
      MPI_Send(&xwidth, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
      MPI_Send(&ywidth, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
      MPI_Send(&pxorigin, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
      MPI_Send(&pyorigin, 1, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
      MPI_Send(&trials, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
      col+=1;
   }

   int j;
   for ( j = 1; j < size; ++j)
   {
     int tval[N];
     MPI_Recv(&tval, N, MPI_INT, j, tag, MPI_COMM_WORLD, &status);
     //j = status.MPI_SOURCE;
     int currentWorker = j;
     int currentX = workerDist[currentWorker];
     //workerDist[j] = col;
     int ycurr = 0;
     for (ycurr = 0; ycurr < N; ++ycurr)
     {
      int currMaxTrials = tval[ycurr];
      if (currMaxTrials != (trials-1))
      {
         glColor3f(.7 *log(2*currMaxTrials), .2*log(2*currMaxTrials), .3 *log(2*currMaxTrials));
      }
      else
      {
         glColor3f(200,200,200);
      }
      glBegin(GL_POINTS);
      glVertex2f(currentX, ycurr);
      glEnd();
      //MPI_Send(&col, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
   }
   glFlush();
   
}
glFlush();
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
      //displayfunc();
      //glutDisplayFunc(displayfunc);
      printf("Successfully Zoomed In\n");
      glutPostRedisplay();

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

   //
   // other variables
   //
   int        k , j  ;
   double     result ;
   //
   // boilerplate
   //
   MPI_Init(      &argc          , &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different

   
   //
   int col = 0;

   if( rank == 0 )
   {
      glutInit(&argc,argv);
      glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
      glutInitWindowSize(M,N);
      glutInitWindowPosition(100,50);
      glutCreateWindow("");
      glClearColor(1.0,1.0,1.0,0.0);
      glShadeModel(GL_SMOOTH);
   //
      glViewport(0,0,(GLsizei)M,(GLsizei)N);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0,1.0*M,1.0*N,0.0);
      glMatrixMode(GL_MODELVIEW);

      glutReshapeFunc(NULL);
      glutDisplayFunc(displayfunc);
      glutMouseFunc(mousefunc);
      glutMotionFunc(motionfunc);
      glutKeyboardFunc(keyfunc);
      glutSpecialFunc(specialfunc);
      glutWMCloseFunc(closefunc);

      glutMainLoop();
   }

   else
   {
      while(1)
      {
         int xp =0;
         MPI_Recv( &xp , 1 , MPI_INT , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &xwidth , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &ywidth , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &pxorigin , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &pyorigin , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
         MPI_Recv( &trials, 1 , MPI_INT , 0 , tag , MPI_COMM_WORLD , &status ) ;
        genMandel(xp);
      }
   }
   MPI_Finalize();
   //glutDisplayFunc(displayfunc);

   //
   return 0;
}
// 
// end of file
// 
