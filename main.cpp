/**
    Created by Md. Mahmudur Rahman
    Military Institute of Science & Technology
    CSE
    ID: 201314014
**/
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <bits/stdc++.h>
using namespace std;
#define PI acos(-1.0)
#define    rep(i,n)          for(int i = 1 ; i<=(n) ; i++)
#define    repI(i,n)          for(int i = 0 ; i<(n) ; i++)
#ifdef mamun
#define debug(args...) {cerr<<"*: "; dbg,args; cerr<<endl;}
#else
#define debug(args...)  // Just strip off all debug tokens
#endif

struct debugger
{
    template<typename T> debugger& operator, (const T& v)
    {
        cerr<<v<<" ";
        return *this;
    }
} dbg;
/******************/

int carx1=-600;
int cary1=-300;

int carx2=-600;
int cary2=-560;

int k=0;
char kb=' ';
int rn=0;

double cldx=1;
double cldy=1;
double cldz=1;

bool showsun=1;
int timer_day_night=0;
float sky=0.4, cloud_tx=-600.0,cloud_ty=250.0, sun_ty=600.0,sun_tx=-350;
float car_tx=-600.0, car_ty=-160.0;
float car_color_r=1.0, car_color_g=0.0, car_color_b=0.0;
float lights_r=1.0, lights_g=1.0, lights_b=0.0;
float l_r=0.92, l_g=.89, l_b=0.0;
float sky_r=.22,sky_g=.69,sky_b=.87;

double sunx=1;
double suny=.9;
double sunz=0;
bool thunderAlpha = 0;
float weatherAlpha = 0.5;
float thunderPosition[10];
int ThunderTime = 300;
int thunderflag=0;
void rain();
/// for divider
int colblack=1,colwhite=0,dividercnt=0;
/// for firing bullet
int curactv=0,curdeactv=1;
int firedvar=0;
int bullet_org=-255;
int bullet_x ;
int bullet_orgy[2] = {-250,-500};
int bullet_y;
/// for object
int mark[2][2];
double object_x[2][2],object_y[2][2],object_val[2][2];
/// rain
int raincount=0;
int gameover=0;
char buffer [33]= {"0"};
char score[33] = {"Score: 0"};
int scoreval=0;
void drawString(void *font,float x,float y, char *str)
{

    char *ch;
    glRasterPos3f(x,y,0.0);
    for(ch=str; *ch; ch++)
        glutBitmapCharacter(font,(int)*ch);
    glFlush();
}
void viewscore()
{
    glColor3f(1.0, 0.0, 0.0);
    itoa (scoreval,buffer,10);
    strcpy(score,"Score: ");
    strcat(score,buffer);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24,-600,500,score);

}
void createobject(int posx,int posy)
{
    glPushMatrix();
    glColor3f(0.545098,0.270588,0.0745098);
    glRectf(posx-40, posy-80, posx+40, posy+80);
    glPopMatrix();
}
void checkfiredestruction()
{
    if(!firedvar)return;
    repI(row,2)
    {
        repI(col,2)
        {
            if(mark[row][col])
            {
                if(bullet_x>=object_x[row][col]-40&&bullet_x<=object_x[row][col]+40&&bullet_y>=object_y[row][col]-80&&bullet_y<=object_y[row][col]+80)
                {
//                    PlaySound("BOMB.WAV", NULL, SND_ASYNC|SND_FILENAME);
//                    debug(row,col);
                    scoreval++;
                    mark[row][col]=0;
                    firedvar=0;
                    break;
                }
            }

        }
        if(firedvar==0)break;
    }
}
void checkcardestruction()
{
    repI(row,2)
    {
        repI(col,2)
        {
            if(mark[row][col])
            {
//                debug(object_y[row][col],cary1);
                if(object_x[row][col]<=carx1+380&&object_x[row][col]>=-450&&(row!=curactv))
                {
                    printf("\n\nYour score: %d\n",scoreval);
                    gameover=1;
//                    exit(0);
                }
            }
        }
    }
}
void createobjectfunc()
{
    checkfiredestruction();
    checkcardestruction();
    glPushMatrix();
    repI(row,2)
    {
        repI(col,2)
        {
            if(mark[row][col]==0)
            {
                if(mark[row][1-col]==0)
                {
                    mark[row][col]=1;
                    object_x[row][col]=600;
                    if(row==1)object_y[row][col]=-210;
                    else object_y[row][col]=-450;
                    createobject(object_x[row][col],object_y[row][col]);
                    object_val[row][1-col]=80+rand()%1100;
                    object_val[row][1-col]=600-object_val[row][1-col];

                }
                else
                {
//                    int dif=100+(rand()%800);
//                    cout<<dif<<" "<<600-object_x[row][1-col]<<endl;
                    if(object_x[row][1-col]<=object_val[row][1-col])
                    {
//                        cout<<"r"<<endl;
                        object_x[row][col]=600;
                        if(row==1)object_y[row][col]=-210;
                        else object_y[row][col]=-450;
                        createobject(object_x[row][col],object_y[row][col]);
                        mark[row][col]=1;
                    }
                }
            }
            else
            {
                object_x[row][col]-=.3;
                if(object_x[row][col]<-600)
                {
                    mark[row][col]=0;
                    continue;
                }
                createobject(object_x[row][col],object_y[row][col]);
            }
        }
    }

    glPopMatrix();
}

GLvoid drawCircle(GLdouble rad)
{
    GLint points = 50;
    GLdouble delTheta = (2.0 * PI) / (GLdouble)points;
    GLdouble theta = 0.0;

    glBegin(GL_POLYGON);
    {
        for( GLint i = 0; i <=50; i++, theta += delTheta )
        {
            glVertex2f(rad * cos(theta),rad * sin(theta));
        }
    }
    glEnd();
}
void DrawCircle(float cx, float cy, float rx,float ry, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);///get the current angle

        float x = 2*rx * cosf(theta);///calculate the x component
        float y = 2*ry * sinf(theta);///calculate the y component

        glVertex2f(x + cx, y + cy);///output vertex

    }
    glEnd();
}
void createCloud(int x,int y,int z)
{

    /// middle circle
    glPushMatrix();

    glColor3f(cldx, cldy, cldz);
    glTranslatef(x+350,y+400,z+400);
    drawCircle(70);

    glPopMatrix();

    ///lower left circle
    glPushMatrix();

    glTranslatef(x+420,y+400,z+400);
    drawCircle(45);

    glPopMatrix();

    ///right circle
    glPushMatrix();

    glTranslatef(x+290,y+400,z+400);
    drawCircle(40);
    glPopMatrix();

    ///lower left left circle
    glPushMatrix();

    glTranslatef(x+280,y+370,z+400);
    drawCircle(40);

    glPopMatrix();

    ///upper right right circle
    glPushMatrix();

    glTranslatef(x+410,y+450,z+400);
    drawCircle(36);

    glPopMatrix();

}
void cloud()
{
    glPushMatrix();
    createCloud(50,170,0);
    glPopMatrix();

    glPushMatrix();
    createCloud(-450,140,0);
    glPopMatrix();

    glPushMatrix();
    createCloud(-800,130,0);
    glPopMatrix();

}
void CreateCar(int cx, int cy)
{
    ///car
    glPushMatrix();

    glBegin(GL_POLYGON);
    glVertex2f(cx+150, cy+135);
    glVertex2f(cx+150, cy+160);
    glVertex2f(cx+85, cy+160);
    glVertex2f(cx+80, cy+195);
    glVertex2f(cx-80, cy+195);
    glVertex2f(cx-115, cy+160);
    glVertex2f(cx-150,cy+160);
    glVertex2f(cx-150, cy+135);

    glEnd();
    ///glass
    glColor3f(0.69, 0.90, .99);

    glBegin(GL_POLYGON);
    glVertex2f(cx+80, cy+190);
    glVertex2f(cx+80, cy+160);
    glVertex2f(cx-80,cy+160);
    glVertex2f(cx-80, cy+190);

    glEnd();

    ///chaka
    glColor3f(0, 0, 0);
    glTranslatef(cx-85,cy+135,0);
    drawCircle(17);

    glTranslatef(170,0,0);
    drawCircle(17);

    glPopMatrix();
}
void day_night(void)
{
    timer_day_night++;

    if(timer_day_night>=30000)
    {
        timer_day_night=0;
        sun_ty=600;
        sun_tx=-350;
    }
    if(timer_day_night>20000)
    {
        sky_r=sky_g=sky_b=0;
        sun_tx=700;
    }
    else if(timer_day_night<5000||timer_day_night>15000)
    {
        sky_r=.22;
        sky_g=.69;
        sky_b=.87;
        sun_tx+=.05;
        sun_ty-=.05;
    }
    else
    {
        sky_r=sky_g=0;
        sky_b=1;

    }
    if(timer_day_night>20000)
    {
//        sky_r=sky_g=sky_b=0;
        lights_r=1.0;
        lights_g=1.0;
        lights_b=0.0;

        l_r=0.92;
        l_g=0.89;
        l_b=0;
    }
    else
    {
//        sky_r=.22;sky_g=.69;sky_b=.87;
        lights_r=.95;
        lights_g=0.95;
        lights_b=0.95;

        l_r=0.2;
        l_g=l_b=0;
    }

    cloud_tx+=.25;
    if(cloud_tx>1200)
        cloud_tx=-1200;
    glutPostRedisplay();
}
void createsky()
{
    glPushMatrix();
    glColor3f(sky_r,sky_g,sky_b);
    glRectf(1000.0, 700.0, -1000.0, 60.0);
    glPopMatrix();
}
void createsun()
{
    if(showsun==1)
    {
        glColor3f(1, 0.9, 0);
        glPushMatrix();
        glTranslatef(sun_tx,sun_ty,0) ;
        DrawCircle(300,200,40,38,500);
        glPopMatrix();
    }
}
void createfootpath()
{
    glPushMatrix();
    glColor3f(0.419608, 0.556863,0.137255);
    glRectf(1000.0, 60.0, -1000.0, -50.0);
    glPopMatrix();
    /// footpath er design
    glPushMatrix();
    glColor3f (1,1,1);
    glRectf(-1000.0, -50.0, 1000.0, -60.2);
    glColor3f (1,0,0);
    glRectf(-300.0, -50.0, 300.0, -60.2);
    glPopMatrix();
}
void createroad()
{
    glPushMatrix();
    glColor3f(.5,.5,.5);
    glRectf(1000.0, -60.0, -1000.0, -700.0);
    glPopMatrix();
}
void createdivider()
{
    dividercnt++;
    if(dividercnt>1000)
    {
        if(!gameover)swap(colblack,colwhite);
        dividercnt=0;
    }
    glPushMatrix();

    glColor3f (colblack,colblack,colblack);
    glRectf(150.0, -250.0, -90.0, -300.2);
    glRectf(-320.0, -250.0, -570.0, -300.2);
    glRectf(380.0, -250.0, 600.0, -300.2);

    glColor3f (colwhite,colwhite,colwhite);
    glRectf(-90.0, -250.0, -320.0, -300.2);
    glRectf(-570.0, -250.0, -600.0, -300.2);
    glRectf(150.0, -250.0, 380.0, -300.2);

    glPopMatrix();
}
void createbuilding()
{
    /// bulding 1
    glPushMatrix();

    glColor3f(0.91, 0.76,  0.75);
    glBegin (GL_POLYGON);
    glVertex3d(-100,400,0);
    glVertex3d(-330,400,0);
    glVertex3d(-330,60,0);
    glVertex3d(-100,60,0);

    glEnd();

    glColor3f(l_r, l_g, l_b);

    glRectf(-120.0,370.0,-190.0,300.0);
    glRectf(-120.0, 280.0, -190.0, 200.0);


    glRectf(-240.0,370.0,-310.0,300.0);
    glRectf(-240.0, 280.0, -310.0, 200.0);


    glRectf(-170.0, 150.0, -260.0, 60.0);
    glPopMatrix();

    /// bulding 2
    glPushMatrix();

    glColor3f(.8, .8, 1);
    glBegin (GL_POLYGON);
    glVertex3d(70,350,0);
    glVertex3d(-90,350,0);
    glVertex3d(-90,60,0);
    glVertex3d(70,60,0);

    glEnd();

    glColor3f(l_r, l_g, l_b);

    glRectf(50.0,290.0,-70.0,330.0);
    glRectf(50.0, 240.0, -70.0, 280.0);
    glRectf(50.0, 190.0, -70.0, 230.0);

    glRectf(30.0, 130.0, -35.0, 60.0);

    glPopMatrix();

    /// bulding 3
    glPushMatrix();

    glColor3f(.2,.4,.4);
    glBegin (GL_POLYGON);
    glVertex3d(310,365,0);
    glVertex3d(155,365,0);
    glVertex3d(155,60,0);
    glVertex3d(310,60,0);

    glEnd();

    glColor3f(l_r, l_g, l_b);

    glRectf(300.0,355.0,250.0,240.0);
    glRectf(220.0,355.0,166.0,240.0);
    glRectf(300.0, 200.0, 166.0, 160.0);

    glRectf(200.0, 140.0, 270.0, 60.0);

    glPopMatrix();

    /// bulding 4
    glPushMatrix();

    glColor3f(.9, 0, .34);
    glBegin (GL_POLYGON);
    glVertex3d(590,480,0);
    glVertex3d(320,480,0);
    glVertex3d(320,60,0);
    glVertex3d(590,60,0);

    glEnd();


    glColor3f(l_r, l_g, l_b);

    glRectf(570.0,460.0,500.0,350.0);
    glRectf(570.0,340.0,500.0,230.0);
    glRectf(570.0, 220.0, 500.0, 110.0);

    glPopMatrix();
}
void createlamppost()
{
    ///lamp post 1st
    glPushMatrix();
    glColor3f(0.839216,0.839216,0.839216);
    glBegin(GL_POLYGON);
    glVertex3f (-420,-47, 0.0);
    glVertex3f (-420,320, 0.0);
    glVertex3f (-400,320, 0.0);
    glVertex3f (-400,-47, 0.0);
    glEnd();

    glColor3f(lights_r, lights_g, lights_b);

    glRectd(-430,320,-390,400);
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3f (-430,320, 0.0);
    glVertex3f (-430,400, 0.0);
    glVertex3f (-390,400, 0.0);
    glVertex3f (-390,320, 0.0);
    glEnd();
    glPopMatrix();

    ///lamp post 2nd
    glPushMatrix();
    glColor3f(0.839216,0.839216,0.839216);
    glBegin(GL_POLYGON);
    glVertex3f (120,-47, 0.0);
    glVertex3f (120,320, 0.0);
    glVertex3f (100,320, 0.0);
    glVertex3f (100,-47, 0.0);
    glEnd();

    glColor3f(lights_r, lights_g, lights_b);

    glRectd(90,320,130,400);
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3f (130,320, 0.0);
    glVertex3f (130,400, 0.0);
    glVertex3f (90,400, 0.0);
    glVertex3f (90,320, 0.0);
    glEnd();
    glPopMatrix();
}
void createbullet()
{
//    cout<<firedvar<<endl;
    if(firedvar)
    {
        glPushMatrix();
        glColor3f(1, 1, 0);
        glRectf(bullet_x, bullet_y, bullet_x+20, bullet_y+20);
        bullet_x+=1;
        if(bullet_x>600)
        {
            firedvar=0;
            bullet_x=bullet_org;
        }
        glPopMatrix();
    }
}
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(0,-70,0);

    /// day_night
    day_night();

    /// sky
    createsky();

    /// Sun
    createsun();

    ///footpath
    createfootpath();

    ///road
    createroad();

    /// road er divider
    createdivider();

    /// building
    createbuilding();

    /// lamppost
    createlamppost();

    /// Cloud
    cloud();

    /// Car
    glColor3f(.2,.3,.8);
    CreateCar(carx1+200,cary1-0);
    glTranslatef(0,70,0);
    if(gameover)
    {
        glColor3f(1,0.0,0);
        itoa (scoreval,buffer,10);
        strcpy(score,"Score: ");
        strcat(score,buffer);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24,0,-210,score);
        return;
    }

    ///bullet
    createbullet();

    /// object
    createobjectfunc();

    /// rain
    rain();

    /// score
    glColor3f(1,0.0,0);
    viewscore();

    glFlush();
}
void ThunderTimer(int value)
{
    if(thunderflag == 1)
    {
        weatherAlpha = 0.5;
        thunderflag = 0;
        thunderAlpha = 0;
    }
    glutPostRedisplay();    /// Post a paint request to activate display()
    glutTimerFunc(ThunderTime, ThunderTimer, 0); /// subsequent timer call at milliseconds
}
void drawThunder()
{
    thunderPosition[0] = -450;
    thunderPosition[1] = 250;
    thunderPosition[2] = 450;

    int index = rand()%3;
    glPushMatrix();
    glTranslatef(thunderPosition[index],550,-20);
    thunderAlpha = 1;
    glRotatef(30, 0.0, 0.0, 1.0);
    glColor4f(1.0,1.0,1.0,thunderAlpha);
    glBegin(GL_POLYGON);
    glVertex2f(-10,0);
    glVertex2f(-50,-110);
    glVertex2f(-10,-100);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-20,-100);
    glVertex2f(-20,-200);
    glVertex2f(20,-90);
    glEnd();
}

void rain()
{
    raincount++;
//    cout<<raincount<<" "<<rn<<endl;
    if(raincount>=20000)
    {
        if(rn==0)
        {
//            rn=1;
            cldx=cldy=cldz=.8;
            sunx=.22;
            suny=.69;
            sunz=.87;
            showsun=0;
            glutIdleFunc(display);
        }
        else
        {
//            rn=0;
            cldx=1;
            cldy=1;
            cldz=1;
            showsun=1;
            sunx=1;
            suny=.9;
            sunz=0;
            glutIdleFunc(NULL);
        }
        rn=1-rn;
        raincount=0;
    }
    if(rn==0)return;

    glPushMatrix();
    glColor3f(.7,.7,.7);
    int rain=80;
    for(int i = 1; i<= rain ; i++)
    {
        float a = rand() % 1300 - 600;
        float b = rand() % 1300 - 600;

        glBegin(GL_LINE_STRIP);
        glVertex3f (a,b, 0.0);
        glVertex3f (a+10,b+25, 0.0);
        glEnd();
    }
    glPopMatrix();
    if(thunderflag>=1 && rain >=30)
    {
        weatherAlpha = 0;
        thunderflag++;
        thunderflag=thunderflag%100+1;
        drawThunder();
    }
    glutPostRedisplay();
    glPopMatrix();
}

//void keyboard_arrow(int key, int x, int y)
//{
//    ///right car
//    int move_unit=10;
//    switch (key)
//    {
//    case GLUT_KEY_RIGHT:
//        carx1+=move_unit;
//        break;
//    case GLUT_KEY_LEFT:
//        carx1-=move_unit;;
//        break;
//    case GLUT_KEY_UP:
//        cary1+=move_unit;;
//        break;
//    case GLUT_KEY_DOWN:
//        cary1-=move_unit;;
//        break;
//    default:
//        break;
//    }
//    glutPostRedisplay();
//}


void keyboard_key(unsigned char key, int x, int y)
{
//    int numeros;
    int move_unit=10;
    kb=key;

    switch ( key )
    {
    case 's':
        scoreval+=5;
        swap(cary1,cary2);
        swap(curactv,curdeactv);
//        swap(bullet_tmpy,bullet_y);
        break;
    ///fire
    case 'f':
        if(!firedvar)
        {
            PlaySound("bullet.WAV", NULL, SND_ASYNC|SND_FILENAME);
            bullet_x=bullet_org;
            bullet_y=bullet_orgy[curactv];
            firedvar=1;
        }
        glutPostRedisplay();
        break;
    ///rain
    case 'd':
        rn=1;
        cldx=cldy=cldz=.8;
        sunx=.22;
        suny=.69;
        sunz=.87;
        showsun=0;
        glutIdleFunc(display);
        break;

    case 'x':
        rn=0;
        cldx=1;
        cldy=1;
        cldz=1;
        showsun=1;
        sunx=1;
        suny=.9;
        sunz=0;
        glutIdleFunc(NULL);
        break;
        ///thunder
//    case 't':
//        if(thunderflag==0)thunderflag=1;
//        else thunderflag=0;
//        break;

        ///day and night
//    case 'd':
//        glutIdleFunc(day_night);
//        break;

    }


    glutPostRedisplay();
}

void init(void)
{
    thunderflag=1;
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glOrtho(-600,600,-600,600,-600,600);
}
int main()
{

    srand(time(NULL));
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (700,650);
    glutInitWindowPosition (350, 50);
    glutCreateWindow ("MIST CAR RACING");
    init();

    printf("Switch Car Lan: \"Press S\"\n");
//    printf("To Rain : \"Press D\"\n");
//    printf("To Stop or Start Thunder: \"Press T\"\n");
//    printf("To Stop Rain: \"Press X\"\n");
    printf("To Fire: \"Press F\"\n");

    printf("\n\n1.If you destroy a object you will get 1 point\n");
    printf("2.If you Switch lan you will get 5 point\n");

    glutDisplayFunc(display);
    glutPostRedisplay();

//    glutSpecialFunc(keyboard_arrow);
    glutKeyboardFunc(keyboard_key);
    glutMainLoop();
    return 0;
}
