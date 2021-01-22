// compile gcc 11.c -g -O2 -Wall -I/usr/include/freetype2 -lftgl -lglfw -lGLU -lGL -lX11 -lXrandr -lm -o 11
// if you have a better way or a good idea to improve my code, feel free to contact me

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glfw.h>
#include<FTGL/ftgl.h>

#define MaxName 100
#define CrossingNumber 100
#define MaxPoints 5
#define OpenList 1
#define CloseList 0
#define InitialValue 1000
#define Yes 1
#define No 0

#ifndef FONT_FILENAME
#define FONT_FILENAME "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf"
#endif

FTGLfont *font;

typedef struct structureI
{
	int id;
	double pos_x;
	double pos_y;
	int wait;
	char *jname[MaxName];
	char ename[MaxName];
	int points;
	int next[MaxPoints];
}Crossing;

typedef struct structureII
{
	int id;
	int set;
	int parent;
	double distance; 
}PathSearch;

typedef struct Structure
{
  double dis;
}order;

typedef struct structureIV
{
  order from[CrossingNumber];
}Distance;

typedef struct structureV
{
	int id;
}PathSort;

Crossing cross[CrossingNumber];
PathSearch node[CrossingNumber];
Distance to[CrossingNumber];
PathSort sort[CrossingNumber];

int read_map(const char *filename)
{
	int i,j,crossing_number;
	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		perror(filename);
		exit(1);
	}
	if(fscanf(fp,"%d",&crossing_number));
	for (i=0;i<crossing_number;i++)
	{
	  if(fscanf(fp,"%d,%lf,%lf,%d,%[^,],%[^,],%d",&(cross[i].id), &(cross[i].pos_x), &(cross[i].pos_y), &(cross[i].wait), cross[i].jname, cross[i].ename, &(cross[i].points)));
		for(j=0;j<cross[i].points;j++)
		  {
		    if(fscanf(fp,",%d",&(cross[i].next[j])));
		  }
	}
	fclose(fp);
	return crossing_number;
}

int verify_map(const char *input ,int crossing_number)
{
	int i,result=404;  // 404 means uncnfirmed
	char *name;
	for(i=0;i<crossing_number;i++)
	{
		name=cross[i].ename;
		if(strcmp(name,input)==0)
		{ 
			result=i;
			printf("the verify_map receive as %d name %s id %d\n",crossing_number,input,i);
			return result;
		}
	}
	puts("YOUR SELECT DATA IS NOT EXISTS");
	return result;
}


void find_distance(int crossing_number)  //find the distance between any two connected crossings on the map
{
  double d;
  int i,j,k;
  for(i=0;i<crossing_number;i++)
    {
      for(j=0;j<cross[i].points;j++)
	{
	  k=cross[cross[i].next[j]].id;
	  d=hypot(cross[i].pos_x-cross[k].pos_x,cross[i].pos_y-cross[k].pos_y);
	  to[i].from[k].dis=d;
	}
    }
}

void search_shortest(int crossing_number, int eid)
{
  	int i,j;
  	int currentid;
  	double bypassdis;     // the sum of the distance between current id to end point and current id to its connected crossing
  	int shortestid=-1;      
  	for(i=0;i<crossing_number;i++)
  	{
  		node[i].distance=1e100;
  		node[i].parent=-1;
  		node[i].set=0;
  		node[i].id=cross[i].id;
	}	
	node[eid].distance=0;
	node[eid].set=1;	
	currentid=eid;
	for(j=0;j<crossing_number;j++)
	{
		for(i=0;i<cross[currentid].points;i++)
		{
			if(node[cross[currentid].next[i]].set==0)      // if the connected crossing is unconfirmed
			{
				bypassdis=node[currentid].distance+to[cross[currentid].next[i]].from[currentid].dis;   
				if(bypassdis<node[cross[currentid].next[i]].distance)
				{
					node[cross[currentid].next[i]].distance=bypassdis;
					node[cross[currentid].next[i]].parent=currentid;
				}
			}
		}
		for(i=0;i<crossing_number;i++)
		{
			if((node[i].set==0)&&(node[i].distance<1e100))   // assume the first unconfirmed crossing with valid distance has the shortest distance
			{
				shortestid=i;
				break;
			}
		}
		for(i=0;i<crossing_number;i++)
		{
			if(node[i].set==0)
				shortestid=node[shortestid].distance>node[i].distance?i:shortestid;    // compare the assumption shortest crossing to other unconfirmed crossing, and find the real shortest one
		}
		node[shortestid].set=1;
		currentid=shortestid;
	}
}

int sort_path(int crossing_number, int start_id)
{
	int current_id=start_id;
	int i;
	for(i=0;i<crossing_number;i++)
	{
		sort[i].id=-1;
	}
	for(i=1;node[current_id].distance!=0;i++)
	{
		sort[i].id=node[current_id].parent;
		current_id=node[current_id].parent;
	}
	return i;
}

void encrypt(int *path_pointer,int path_number)
{
	FILE *fp;
	char path_array[CrossingNumber];
	char key_array[CrossingNumber];
	int encryption_array[CrossingNumber];
	int i,f=1;
	for(i=0;i<CrossingNumber;i++) 
	{
		path_array[i]=-1;
	}
	for(i=1;i<=path_number;i++)
	{
		path_array[i-1]=*(path_pointer+i);
	}
	puts("PLEASE SELECT YOUR KEY");
	if(scanf("%s",key_array));
	fp=fopen("save.dat","w");
	for(i=0;i<path_number;i++)
	{
		encryption_array[i]=path_array[i]+(int)key_array[i]+f;
		fprintf(fp,"%d  ",encryption_array[i]);  
		f=-f;
	}
	fclose(fp);	
}

int *decryption()
{
	FILE *fp;
	int input_array[MaxName];
	char key_array[MaxName];
	static int decrypted_array[MaxName];
	int input_length;
	int i,f=1;
	puts("INPUT YOUR KEY");
	if(scanf("%s",key_array));
	fp=fopen("save.dat","r");
	fp==NULL?exit(1):1;
	i=-1;
	do{
		i++;
		input_array[i]=-1;    
		if(fscanf(fp,"%d  ",&input_array[i]));
	}while(input_array[i]!=-1);
	input_length=i;
	for(i=0;i<MaxName;i++)
	{
		decrypted_array[i]=InitialValue;  
	}
	for(i=0;i<input_length;i++)
	{
		decrypted_array[i]=input_array[i]-(int)key_array[i]-f;
		f=-f;
	}
	fclose(fp);
	return decrypted_array;
}

double angle(double x0, double y0, double x1, double y1)  
{
  double ang=0;
  double k;
  double theta,slop,delta_x,delta_y;
  delta_x=x1-x0;
  delta_y=y1-y0;
  slop=delta_y/delta_x;
  k=fabs(slop);
  theta=atan(k)*180/3.1415926535;
  if(delta_x==0)
    {
      if(delta_y>0)
	ang=270;
      if(delta_y<0)
	ang=90;
    }
  if(delta_y==0)
    {
      if(delta_x>0)
	ang=180;
      if(delta_x<0)
	ang=0;
    }
  if((delta_x>0)&&(delta_y>0))
    ang=180.0+theta;
  if((delta_x>0)&&(delta_y<0))
    ang=180.0-theta;
  if((delta_x<0)&&(delta_y>0))
    ang=360.0-theta;
  if((delta_x<0)&&(delta_y<0)) 
    ang=theta;
  return ang;    
}

void circle(double x, double y, double r)
{  
  int const N = 1024; 
  int i;
  glColor3d(1,0,0);   
  glBegin(GL_POLYGON);
  for (i=0;i<N;i++)
    {
      glVertex2d(x+cos(2*M_PI*i/N)*r, y+sin(2*M_PI*i/N)*r);
    }
  glEnd();  
}

void line(double x0, double y0, double x1, double y1)
{
  glColor3d(0,0,0);
  glBegin(GL_LINES);
  glVertex2d(x0,y0);
  glVertex2d(x1,y1);
  glEnd();
}

void outtextxy(double x, double y, char const *text) 
{
  double const scale = 0.005; 
  glPushMatrix();
  glTranslated(x, y, 0.0); 
  glScaled(scale, scale, scale);
  ftglRenderFont(font, text, FTGL_RENDER_ALL); 
  glPopMatrix();
}

void car(double x, double y)
{
  glColor3d(0.0,0.0,0.0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(x,y);
  glVertex2d(x+0.15,y-0.15);
  glVertex2d(x+0.15,y+0.15);
  glEnd();
}

void map_show(int crossing_number)
{
  int i,j;
  double x0,y0,x1,y1,x,y,r;
  for(i=0;i<crossing_number;i++)
    {
      x=cross[i].pos_x;
      y=cross[i].pos_y;
      r=0.06;
      circle(x,y,r);
      glColor3d(1.0, 1.0, 0.0);
      outtextxy(x, y, cross[i].ename); 
      for(j=0;j<cross[i].points;j++)
	{
	  x0=cross[i].pos_x;
	  y0=cross[i].pos_y;
	  x1=cross[cross[i].next[j]].pos_x;
	  y1=cross[cross[i].next[j]].pos_y;
	  line(x0,y0,x1,y1);
	}
    }	 
}

int main()
{
	char input[MaxName];
	char startname[MaxName],endname[MaxName],stopover_name[CrossingNumber][MaxName];
	int *decryption_pointer;
	int i,j,k;
	int verify_result,crossing_number,stopovernum; 
	int startid,endid,stopover_id[CrossingNumber];
	int step[1000],step_number;
	int path[CrossingNumber],path_number,sort_number;
	int status=No; 
	int car_from, car_to, car_goal, car_step;
	double car_x=0.0,car_y=0.0;
	double map_scale=1;
	double map_trans_x=0,map_trans_y=0;
	int window_width,window_height;
	double angle_degree=0;

	FILE *fp;
	fp=fopen("save.dat","r");
	if(fp!=NULL)
	{
		puts("RECORD FOUND, DO YOU WANT TO READ? yes/no");
		if(scanf("%s",input));
		if(strcmp(input,"yes")==0)
			status=Yes;
	}	
	fclose(fp);
	for(;;)
	{
		if(status==Yes)
		{			
			crossing_number=read_map("map2.dat");
			decryption_pointer=decryption();
			i=0;
			do{
				path[i+1]=*(decryption_pointer+i);
				i++;
			}while(path[i]!=InitialValue);
			path_number=i-1;
			startid=path[1];
			endid=path[path_number];
		}
		if(status==No)
		{
			puts("answe is no");
			puts("PLEASE SELECT YOUR MAP NAME, TO EXIT PROGRAM INPUT N/A");
			if(scanf("%s",input));
			if(strcmp(input,"N/A")==0)
				break;
			crossing_number=read_map(input);			
			do{
				puts("PLEASE SELECT YOUR START POINT");
				if(scanf("%s",input));
				verify_result=verify_map(input,crossing_number);		
			}while(verify_result==404);  
			strcpy(startname,input);
			startid=verify_result;
	
			do{
				puts("PLEASE SELECT A DESINTAION");
				if(scanf("%s",input));
				verify_result=verify_map(input,crossing_number);
				if(strcmp(startname,input)==0)
				{
					puts("YOUR DESTINATION AS SAME AS START, PLEASE CHOSE ANOTHER ONE");
					verify_result=404;
				}
			}while(verify_result==404);
			strcpy(endname,input);
			endid=verify_result;
			for(i=1;;i++)
			{
				do{
					printf("PLEASE SELECT YOUR STOPOVER %d IF YOU DO NOT HAVE ANY JUST INPUT N/A\n",i);
					if(scanf("%s",input));
					if(strcmp(input,"N/A")==0)
						break;
					verify_result=verify_map(input,crossing_number);
				}while(verify_result==404);
				if(strcmp(input,"N/A")==0)
					break;
				strcpy(stopover_name[i],input);	
				stopover_id[i]=verify_result;						
			}
			stopovernum=i-1;	
			path[1]=startid;
			for(i=1;i<stopovernum+1;i++)
			{
				path[i+1]=stopover_id[i];
			}
			path[i+1]=endid;
  	        path_number=i+1;
 	        puts("DO YOU WANT TO REGISTER YOUR POINT? yes/no");
 	      	if(scanf("%s",input));
 	      	strcmp(input,"yes")==0?encrypt(path,path_number):1;
       	}
		status=No; 
		for(i=0;i<1000;i++)
		{
			step[i]=100;
		}	
		find_distance(crossing_number);	
		k=1;
		for(i=1;i<path_number;i++)
		{
			search_shortest(crossing_number,path[i+1]);
			sort_number=sort_path(crossing_number,path[i]);
			for(j=1;j<sort_number;j++)
			{
				step[k]=sort[j].id;
				k++;
			}
		}
		step[0]=startid;
		step_number=k;		
		for(i=0;i<step_number;i++)  
		{
			printf("step %d is %d name %s\n",i,step[i],cross[step[i]].ename);
		}			
		glfwInit();
		glfwOpenWindow(757,1100,0,0,0,0,0,0,GLFW_WINDOW);
		glMatrixMode(GL_PROJECTION);
  		glLoadIdentity();
  		glOrtho(-8,12,-12.8,8,-1.0,1.00); 		
  		glMatrixMode(GL_MODELVIEW);
  		glLoadIdentity();		
  		font=ftglCreateExtrudeFont(FONT_FILENAME);
  		if(font==NULL)
  		{
  			perror(FONT_FILENAME);
  			fprintf(stderr,"double not load font\n");
  			exit(1);
		}
		ftglSetFontFaceSize(font, 24, 24);
    	ftglSetFontDepth(font, 0.01); 
    	ftglSetFontOutset(font, 0, 0.1);
    	ftglSetFontCharMap(font, ft_encoding_unicode);	
#if 1
    car_from = 1; 
    car_goal = 2; 
#else
    car_from = 3; 
    car_goal = 4; 
#endif
    car_to = car_from;
    car_step = 0; 		
    	i=0;
    	for(;;)
    	{
      		if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED))
				break;
		glfwGetWindowSize(&window_width, &window_height);
		glViewport(0,0,window_height,window_height);
      		glClearColor(0.38f,0.75f,0.72f,0.3f);  
      		glClear(GL_COLOR_BUFFER_BIT);
      		map_show(crossing_number);
      		glMatrixMode(GL_MODELVIEW);   
  		glLoadIdentity();
      		glScaled(map_scale,map_scale,map_scale);  
      		glTranslated(map_trans_x,map_trans_y,0.0);
      		car_goal=endid;
      		car_from=step[i];
      		if(car_from!=car_goal)
      		{
      			car_to=step[i+1];
      			double x0 = cross[car_from].pos_x; 
            		double y0 = cross[car_from].pos_y; 
            		double x1 = cross[car_to].pos_x; 
            		double y1 = cross[car_to].pos_y; 
            		double two_point_dis = hypot(x1 - x0, y1 - y0); 
            		int steps = (int)(two_point_dis / 0.1);   
            		angle_degree=angle(x0,y0,x1,y1);
            		car_step++;
            		car_x = x0 + (x1 - x0) / steps * car_step; 
            		car_y = y0 + (y1 - y0) / steps * car_step;
            		if (car_step >= steps) 
			{ 
                		car_from = car_to; 
                		car_step = 0;
				i++;
            		}
		}			
		glPushMatrix();    
      		glTranslatef(car_x,car_y,0);
      		glRotatef(angle_degree,0,0,1);
      		glTranslatef(-car_x,-car_y,0);
        	car(car_x, car_y);      
		glPopMatrix();			
		glColor3d(255.0,255.0,255.0);  		
      		circle(cross[car_goal].pos_x, cross[car_goal].pos_y, 0.13);	
      		if(glfwGetKey(GLFW_KEY_UP))    		      
				map_trans_y+=0.5;
      		if(glfwGetKey(GLFW_KEY_DOWN))
				map_trans_y-=0.5;
      		if(glfwGetKey(GLFW_KEY_LEFT))
				map_trans_x-=0.5;
      		if(glfwGetKey(GLFW_KEY_RIGHT))
				map_trans_x+=0.5;     
      		if(glfwGetKey(GLFW_KEY_KP_ADD))
				map_scale+=0.01;     
      		if(glfwGetKey(GLFW_KEY_KP_SUBTRACT))
				map_scale-=0.01;
      		if(map_scale<0)
				map_scale=0;
		usleep(25*1000);
      		glfwSwapBuffers();     		
		}
		glfwTerminate();	
	}	
	return 0;
}