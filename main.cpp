#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <windows.h>
#include <glut.h>
#include<queue>

#define pi (2*acos(0.0))

using namespace std ;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double ux,uy,uz,rx,ry,rz,lx,ly,lz,posx,posy,posz ;
double q_w_angle ;
double e_r_angle;
double a_s_angle ;
double d_f_angle ;
double board_distance ;

struct point
{
	double x,y,z;
};

point pos2, u2,l2,r2 ;
point q[2000];
int queue_size;

void drawAxes()
{
	if(drawaxes==1)
	{
		//glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    /*
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			glColor3f(0, 1.0, 0);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
			*/
			glColor3f(1.0, 1.0, 1.0);             //red --X //green --> y blue --> z
			glVertex3f( 1000,0,0);
			glVertex3f(0,0,0);

			glColor3f(1, 1, 1.0);
			glVertex3f(0,0,0);
			glVertex3f(-1000,0,0);

			glColor3f(1.0, 1, 1.0);
			glVertex3f(0,1000,0);
			glVertex3f(0,0,0);

			glColor3f(1, 1, 1.0);
			glVertex3f(0,0,0);
			glVertex3f(0, -1000,0);

			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0, 0);

			glColor3f(1,1,1.0);
			glVertex3f(0,0, 0);
			glVertex3f(0,0,-1000);




			//for gun
			/*pos2.x = 0; pos2.y = 40; pos2.z = 0;
            l2.x = 0; l2.y = 140; l2.z = 0;
            u2.x = 0; u2.y = 40; u2.z = 100;
            r2.x = 100; r2.y = 40; r2.z = 0;*/

            /*
            glColor3f(1,1,1);

            glVertex3f(pos2.x,pos2.y, pos2.z);
			glVertex3f(l2.x,l2.y,l2.z);

			glColor3f(0.6,0.7,1); // akashi
			glVertex3f(pos2.x,pos2.y, pos2.z);
			glVertex3f(u2.x,u2.y,u2.z);

			glColor3f(1,0.4,0.5);  //pink
			glVertex3f(pos2.x,pos2.y, pos2.z);
			glVertex3f(r2.x,r2.y,r2.z);
			*/

			//cout << "u --->" << u2.x << " " << u2.y << "  " << u2.z << endl;
			//cout << "r --->" << r2.x << " " << r2.y << "  " << r2.z << endl;



		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
            if(j%2==0){
                glColor3f(255, 255, 255);
		    }
		    else{
                glColor3f(0,0,0);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere


                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawHalfSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{

            if(j%2==0){
                glColor3f(255, 255, 255);
		    }
		    else{
                glColor3f(0,0,0);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawHalfSphereReverse(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{

            if(j%2==0){
                glColor3f(255, 255, 255);
		    }
		    else{
                glColor3f(0,0,0);
		    }
			glBegin(GL_QUADS);{
			    //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h= height/i;     //radius*tan(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
            if(j%2==0){
                glColor3f(255, 255, 255);
		    }
		    else{
                glColor3f(0,0,0);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
/*
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
*/
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}


void drawRound(double radius1,double radius2, double height,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	r = radius1;
	for(i=0;i<=stacks;i++)
	{
		//h= i*(height/stacks);     //radius*tan(((double)i/(double)stacks)*(pi/2));
		//r = radius1 + (radius2-radius1)*(i*i)/(stacks*45);
		h=radius1*sin(((double)i/(double)stacks)*(pi/2));
		r=radius1*cos(((double)i/(double)stacks)*(pi/2));
		r = 2*radius1 - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
            if(j%2==0){
                glColor3f(255, 255, 255);
		    }
		    else{
                glColor3f(0,0,0);
		    }
			glBegin(GL_QUADS);{
			    //upper hemisphere
/*
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                */
                //lower hemisphere

                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

			}glEnd();
		}
	}
}

void drawSS()
{
    glColor3f(1,0,0);
    //drawSquare(20);
    glPushMatrix();
    {
        //glRotatef(angle,0,0,1);
        glTranslatef(0,board_distance,0);
        glRotatef(90,1,0,0);
        glColor3f(0.7,0.7,0.7);
        drawSquare(100);

        int i = 0;
        while(i < queue_size)
        {
            point p = q[i];
            glColor3f(1,0,0);
            glPushMatrix();
            {
                glTranslatef(p.x,p.z,1);
                drawSquare(10);
            }
            glPopMatrix();

            i++;
        }
    }
    glPopMatrix();

    glRotatef(90,1,0,0);
    glRotatef(q_w_angle,0,1,0);
    //glRotatef(e_r_angle,1,0,0);
    //drawSphere(40,98,98);
    drawHalfSphere(40,98,98);
    glRotatef(e_r_angle,1,0,0);
    drawHalfSphereReverse(40,98,98);

    glTranslatef(0,0,-50);
    glRotatef(a_s_angle,1,0,0);
    glRotatef(d_f_angle,0,0,1);

    drawHalfSphere(10,98,98);

    glTranslatef(0,0,2);

    drawCylinder(10,100,98,98);

    glTranslatef(0,0,-100);
    drawRound(10,15,5,98,98);



    /*
    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
    */
}

void keyboardListener(unsigned char key, int x,int y){
	double theta = 0.05 ;
	double sin_theta = sin(theta);
	double cos_theta = cos(theta);
	//cout << sin_theta << " " << cos_theta << endl;
	switch(key){
		case '1':
		    {
		        double prevlx = lx;
                double prevly = ly;
                double prevlz = lz;
                //drawgrid=1-drawgrid;
                lx = -rx* sin_theta + lx*cos_theta;
                ly = -ry* sin_theta + ly*cos_theta;
                lz = -rz* sin_theta + lz*cos_theta;

                rx = prevlx*sin_theta + rx*cos_theta;
                ry = prevly*sin_theta + ry*cos_theta;
                rz = prevlz*sin_theta + rz*cos_theta;

                //cout << lx << " " << ly << " "<< lz << endl;
                //cout << rx << " " << ry << "  " << rz << endl;
                break;
		    }
        case '2':
		    {
		        double prevlx = lx;
                double prevly = ly;
                double prevlz = lz;
                //drawgrid=1-drawgrid;
                lx = rx* sin_theta + lx*cos_theta;
                ly = ry* sin_theta + ly*cos_theta;
                lz = rz* sin_theta + lz*cos_theta;

                rx = -prevlx*sin_theta + rx*cos_theta;
                ry = -prevly*sin_theta + ry*cos_theta;
                rz = -prevlz*sin_theta + rz*cos_theta;

                //cout << lx << " " << ly << " "<< lz << endl;
                //cout << rx << " " << ry << "  " << rz << endl;
                break;
		    }
        case '3':
		    {
		        double prevlx = lx;
                double prevly = ly;
                double prevlz = lz;
                //drawgrid=1-drawgrid;
                lx = ux* sin_theta + lx*cos_theta;
                ly = uy* sin_theta + ly*cos_theta;
                lz = uz* sin_theta + lz*cos_theta;

                ux = -prevlx*sin_theta + ux*cos_theta;
                uy = -prevly*sin_theta + uy*cos_theta;
                uz = -prevlz*sin_theta + uz*cos_theta;

                break;
		    }
        case '4':
		    {
		        double prevlx = lx;
                double prevly = ly;
                double prevlz = lz;
                //drawgrid=1-drawgrid;
                lx = -ux* sin_theta + lx*cos_theta;
                ly = -uy* sin_theta + ly*cos_theta;
                lz = -uz* sin_theta + lz*cos_theta;

                ux = prevlx*sin_theta + ux*cos_theta;
                uy = prevly*sin_theta + uy*cos_theta;
                uz = prevlz*sin_theta + uz*cos_theta;

                break;
		    }
        case '5':
		    {
		        double prevUx = ux;
                double prevUy = uy;
                double prevUz = uz;
                //drawgrid=1-drawgrid;

                ux = -rx*sin_theta + ux*cos_theta;
                uy = -ry*sin_theta + uy*cos_theta;
                uz = -rz*sin_theta + uz*cos_theta;

                rx = prevUx* sin_theta + rx*cos_theta;
                ry = prevUy* sin_theta + ry*cos_theta;
                rz = prevUz* sin_theta + rz*cos_theta;

                break;
		    }
        case '6':
		    {

		        double prevUx = ux;
                double prevUy = uy;
                double prevUz = uz;
                //drawgrid=1-drawgrid;

                ux = rx*sin_theta + ux*cos_theta;
                uy = ry*sin_theta + uy*cos_theta;
                uz = rz*sin_theta + uz*cos_theta;

                rx = -prevUx* sin_theta + rx*cos_theta;
                ry = -prevUy* sin_theta + ry*cos_theta;
                rz = -prevUz* sin_theta + rz*cos_theta;


                break;
		    }
        case 'q':
            {
                if(q_w_angle <= 45){
                    q_w_angle += 5;



                    double sin_theta = sin(5*3.1416/180) ;//sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180); //sin(5*3.1416/180);

                    l2.x -= pos2.x;
                    l2.y -= pos2.y ;

                    u2.x -= pos2.x;
                    u2.y -= pos2.y ;

                    r2.x -= pos2.x;
                    r2.y -= pos2.y ;

                    pos2.x = -40*sin(q_w_angle*3.1416/180) ;
                    pos2.y = 40*cos(q_w_angle*3.1416/180) ;

                                    //cout << "=================>" << pos2.x << "  " << pos2.y << endl;

                    l2.x += pos2.x;
                    l2.y += pos2.y ;
                    //l2.z += pos2.z ;

                    u2.x += pos2.x;
                    u2.y += pos2.y ;
                    //u2.z += pos2.z ;

                    r2.x += pos2.x;
                    r2.y += pos2.y ;
                    //r2.z += pos2.z ;

                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;
                    //drawgrid=1-drawgrid;

                    l2.x = -r2.x* sin_theta + l2.x*cos_theta;
                    l2.y = -r2.y* sin_theta + l2.y*cos_theta;
                    l2.z = -r2.z* sin_theta + l2.z*cos_theta;

                    r2.x = prevlx*sin_theta + r2.x*cos_theta;
                    r2.y = prevly*sin_theta + r2.y*cos_theta;
                    r2.z = prevlz*sin_theta + r2.z*cos_theta;


                }

                break;
            }
        case 'w':
            {
                if(q_w_angle >= -45){
                    q_w_angle -= 5;



                    l2.x -= pos2.x;
                    l2.y -= pos2.y ;

                    u2.x -= pos2.x;
                    u2.y -= pos2.y ;

                    r2.x -= pos2.x;
                    r2.y -= pos2.y ;

                    double sin_theta = sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180);

                    pos2.x = -40*sin(q_w_angle*3.1416/180) ;
                    pos2.y = 40*cos(q_w_angle*3.1416/180) ;

                    //cout << "=================>" << pos2.x << "  " << pos2.y << endl;

                    l2.x += pos2.x;
                    l2.y += pos2.y ;
                    //l2.z += pos2.z ;

                    u2.x += pos2.x;
                    u2.y += pos2.y ;
                    //u2.z += pos2.z ;

                    r2.x += pos2.x;
                    r2.y += pos2.y ;
                    //r2.z += pos2.z ;

                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;
                    //drawgrid=1-drawgrid;
                    l2.x = r2.x* sin_theta + l2.x*cos_theta;
                    l2.y = r2.y* sin_theta + l2.y*cos_theta;
                    l2.z = r2.z* sin_theta + l2.z*cos_theta;

                    r2.x = -prevlx*sin_theta + r2.x*cos_theta;
                    r2.y = -prevly*sin_theta + r2.y*cos_theta;
                    r2.z = -prevlz*sin_theta + r2.z*cos_theta;

                }
                break;
            }
        case 'e':
            {
                if(e_r_angle <= 45){
                    e_r_angle += 5;


                    l2.y -= pos2.y;
                    l2.z -= pos2.z ;

                    u2.y -= pos2.y;
                    u2.z -= pos2.z ;

                    r2.y -= pos2.y;
                    r2.z -= pos2.z ;

                    pos2.y = 40*cos(e_r_angle*3.1416/180) ;
                    pos2.z = 40*sin(e_r_angle*3.1416/180) ;

                    //cout << "=================>" << pos2.x << "  " << pos2.y << "  "<< pos2.z << endl;

                    l2.y += pos2.y;
                    l2.z += pos2.z ;

                    u2.y += pos2.y;
                    u2.z += pos2.z ;

                    r2.y += pos2.y;
                    r2.z += pos2.z ;

                    double sin_theta = sin(5*3.1416/180) ;//sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180); //sin(5*3.1416/180);


                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;
                    //drawgrid=1-drawgrid;

                    l2.x = u2.x* sin_theta + l2.x*cos_theta;
                    l2.y = u2.y* sin_theta + l2.y*cos_theta;
                    l2.z = u2.z* sin_theta + l2.z*cos_theta;

                    u2.x = -prevlx*sin_theta + u2.x*cos_theta;
                    u2.y = -prevly*sin_theta + u2.y*cos_theta;
                    u2.z = -prevlz*sin_theta + u2.z*cos_theta;


                }
                break;
            }
        case 'r':
            {
                if(e_r_angle >= -45){
                    e_r_angle -= 5;


                    l2.y -= pos2.y;
                    l2.z -= pos2.z ;

                    u2.y -= pos2.y;
                    u2.z -= pos2.z ;

                    r2.y -= pos2.y;
                    r2.z -= pos2.z ;

                    pos2.y = 40*cos(e_r_angle*3.1416/180) ;
                    pos2.z = 40*sin(e_r_angle*3.1416/180) ;

                                    //cout << "=================>" << pos2.x << "  " << pos2.y << endl;

                    l2.y += pos2.y;
                    l2.z += pos2.z ;

                    u2.y += pos2.y;
                    u2.z += pos2.z ;

                    r2.y += pos2.y;
                    r2.z += pos2.z ;


                    double sin_theta = sin(5*3.1416/180) ;//sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180); //sin(5*3.1416/180);


                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;

                    l2.x = -u2.x* sin_theta + l2.x*cos_theta;
                    l2.y = -u2.y* sin_theta + l2.y*cos_theta;
                    l2.z = -u2.z* sin_theta + l2.z*cos_theta;

                    u2.x = prevlx*sin_theta + u2.x*cos_theta;
                    u2.y = prevly*sin_theta + u2.y*cos_theta;
                    u2.z = prevlz*sin_theta + u2.z*cos_theta;


                }
                break;
            }
        case 'a':
            {
                if(a_s_angle <= 45){
                    a_s_angle += 5;



                    double sin_theta = sin(5*3.1416/180) ;//sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180); //sin(5*3.1416/180);


                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;
                    //drawgrid=1-drawgrid;

                    l2.x = u2.x* sin_theta + l2.x*cos_theta;
                    l2.y = u2.y* sin_theta + l2.y*cos_theta;
                    l2.z = u2.z* sin_theta + l2.z*cos_theta;

                    u2.x = -prevlx*sin_theta + u2.x*cos_theta;
                    u2.y = -prevly*sin_theta + u2.y*cos_theta;
                    u2.z = -prevlz*sin_theta + u2.z*cos_theta;
                }
                break;
            }
        case 's':
            {
                if(a_s_angle >= -45){
                    a_s_angle -= 5;


                    double sin_theta = sin(5*3.1416/180) ;//sin(5*3.1416/180);
                    double cos_theta = cos(5*3.1416/180); //sin(5*3.1416/180);


                    double prevlx = l2.x;
                    double prevly = l2.y;
                    double prevlz = l2.z;

                    l2.x = -u2.x* sin_theta + l2.x*cos_theta;
                    l2.y = -u2.y* sin_theta + l2.y*cos_theta;
                    l2.z = -u2.z* sin_theta + l2.z*cos_theta;

                    u2.x = prevlx*sin_theta + u2.x*cos_theta;
                    u2.y = prevly*sin_theta + u2.y*cos_theta;
                    u2.z = prevlz*sin_theta + u2.z*cos_theta;
                }
                break;
            }
        case 'd':
            {
                if(d_f_angle <= 45){
                    d_f_angle += 5;
                }
                break;
            }
        case 'f':
            {
                if(d_f_angle >= -45){
                    d_f_angle -= 5;
                }
                break;
            }
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			posx -=10*lx;
			posy -=10*ly;
			posz -=10*lz;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			posx +=10*lx;
			posy +=10*ly;
			posz +=10*lz;
			break;

		case GLUT_KEY_RIGHT:
			posx += 10*rx;
			posy += 10*ry;
			posz += 10*rz;
			//posy +=2*posy;
			//posz +=2*posz;
			break;
		case GLUT_KEY_LEFT:
			posx -= 10*rx;
			posy -= 10*ry;
			posz -= 10*rz;
			break;

		case GLUT_KEY_PAGE_UP:
		    posx += 10*ux;
			posy += 10*uy;
			posz += 10*uz;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    posx -= 10*ux;
			posy -= 10*uy;
			posz -= 10*uz;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//drawaxes=1-drawaxes;
				cout << "mouse : "<< endl;

				 double t = (board_distance-pos2.y)/(pos2.y - l2.y);
				 double x = pos2.x + t*(pos2.x-l2.x);
				 double z = pos2.z + t*(pos2.z-l2.z);

				 point p ;
				 p.x = x ; p.y = board_distance; p.z = z ;
				 if(p.x < 100 && p.x >-100 && p.z < 100 && p.z > -100){
                    q[queue_size] = p ;
                    queue_size++ ;
                    cout << "point -->" << p.x << "  " << p.y << "  " << p.z << endl;
				 }
			}
			break;

		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){
                drawaxes=1-drawaxes;
		    }
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
	//cout << "In display : " << posx+lx << "  " << posy+ly << "  "<< posz+lz <<endl;
    gluLookAt(posx,posy,posz,	posx+lx,posy+ly,posz+lz,	ux,uy,uz);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	ux = 0,uy=0,uz=1;
	rx= (-1/sqrt(2)),ry=(1/sqrt(2)),rz=0;
	lx=(-1/sqrt(2)),ly=(-1/sqrt(2)),lz=0;
	posx=100,posy=100,posz=0 ;
	q_w_angle = 0;
    e_r_angle = 0;
    a_s_angle = 0;
    d_f_angle = 0;
    queue_size = 0;
    board_distance = 300;

    //point pos2, u2,l2,r2 ;
    pos2.x = 0; pos2.y = 40; pos2.z = 0;
    l2.x = 0; l2.y = 340; l2.z = 0;
    u2.x = 0; u2.y = 40; u2.z = 300;
    r2.x = 300; r2.y = 40; r2.z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
