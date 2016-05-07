#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

void allocate();
void MCRun();
void MCStep();
void WalkLeft(int xrand, int yrand);
void WalkRight(int xrand, int yrand);
void WalkUp(int xrand, int yrand);
void WalkDown(int xrand, int yrand);
void react(int oldx, int oldy, int newx, int newy);
void dissociateInto(int x, int y);
void dissociate(int x, int y, int otherx, int othery);
void run();
void flush();

int ground[60][60] = {0};
int complexId[60][60] = {0};
int type[8] = {0,25,83,75,0,0,0,0}; 
int complexCount = 0;
long int MCStepId = 0;
double Pr12 = 0.02;
double Pr13 = 0.2;
double Pr32 = 0.2;
double Pd4 = 0.000002;
double Pd41 = 0.000002; 
double Pd5 = 0.000002;
double Pd6 = 0.000002;
int buffer=0;
/*
type[1] = tBid
type[2] = Bax
type[3] = BCl2
type[4] = tBid-Bax
type[5] = BCl2-tBid
type[6] = BCl2-Bax
type[7] = Bax*

*/

int main()
{
	srand(time(NULL));
	/*for(int iter = 1; iter<=10;iter++)
	{
		run();
	}*/
	run();
	return 0;
}
void run()
{
	flush();
	//printf("Initial values are %d %d %d\n",type[1],type[2],type[3]);
	allocate();
	//printf("Final values after allocation are %d %d %d\n",type[1],type[2],type[3]);
	type[0] = 0;
	type[1] = 25;
	type[2] = 83;
	type[3] = 75;
	type[4] = 0;
	type[5] = 0;
	type[6] = 0;
	type[7] = 0;
	/*for(int i = 0;i<60;i++)
		{
			for(int j = 0;j<60;j++)
			{
				printf("%d",ground[i][j]);}printf("\n");
			}*/
	//printf("tBid=%d Bax=%d BCl2=%d tBid-Bax=%d BCl2-tBid=%d BCl2-Bax=%d Bax*%d\n",type[1],type[2],type[3],type[4],type[5],type[6],type[7]);
	MCRun();
}
void flush()
{
	for(int i = 0;i<=59;i++)
		{
			for(int j = 0;j<=59;j++)
			{
				ground[i][j] = 0;
				complexId[i][j] = 0;
			}
		}
	type[0] = 0;
	type[1] = 25;
	type[2] = 83;
	type[3] = 75;
	type[4] = 0;
	type[5] = 0;
	type[6] = 0;
	type[7] = 0;
	complexCount = 0;
	MCStepId = 0;
	buffer=0;
}

void allocate(){
	while(type[1]!=0||type[2]!=0||type[3]!=0)
	{
		int xrand = rand()%60;
		int yrand = rand()%60;
		int typerand = rand()%4;
		if(ground[xrand][yrand]==0)
		{
			if(type[typerand]!=0)
			{
				ground[xrand][yrand] = typerand;
				type[typerand] = type[typerand]-1;
				//printf("Allocate %d to point %d %d\n",typerand,xrand,yrand);
			}
		}
	}
}
void MCRun()
{
	for(int i = 0;i<100000000;i++)
	{
		if(type[7]==20) break;
		if(buffer!=type[7])
		{
			printf("%d tBid=%d Bax=%d BCl2=%d tBid-Bax=%d BCl2-tBid=%d BCl2-Bax=%d Bax*=%d\n",MCStepId,type[1],type[2],type[3],type[4],type[5],type[6],type[7]);
		}
		buffer = type[7];
		MCStepId++;
		MCStep();
		//printf("%d tBid=%d Bax=%d BCl2=%d tBid-Bax=%d BCl2-tBid=%d BCl2-Bax=%d Bax*=%d\n",MCStepId,type[1],type[2],type[3],type[4],type[5],type[6],type[7]);
	}
	//printf("%d tBid=%d Bax=%d BCl2=%d tBid-Bax=%d BCl2-tBid=%d BCl2-Bax=%d Bax*=%d\n",MCStepId,type[1],type[2],type[3],type[4],type[5],type[6],type[7]);
	/*for(int i = 0;i<60;i++)
		{
			for(int j = 0;j<60;j++)
			{
				printf("%d",ground[i][j]);}printf("\n");
				}*/
}
void MCStep()
{
	//printf("MCStepId = %d \n",MCStepId);
	for(int i=0;i<183;i++)
	{
		//printf("%d tBid=%d Bax=%d BCl2=%d tBid-Bax=%d BCl2-tBid=%d BCl2-Bax=%d Bax*=%d\n",MCStepId,type[1],type[2],type[3],type[4],type[5],type[6],type[7]);
		//printf("%f",prob);
		int xrand = rand()%60;
		int yrand = rand()%60;
		if(ground[xrand][yrand]==1||ground[xrand][yrand]==2||ground[xrand][yrand]==3)
		{
			//printf("Molecule at %d %d selected",xrand,yrand);
			double prob10_2 = rand()%100;
			double prob_2 = prob10_2/100;
			//if(prob_2 < 0.2) continue;
			if(prob_2 >= 0.2 && prob_2 < 0.4) WalkLeft(xrand,yrand);
			if(prob_2 >= 0.4 && prob_2 < 0.6) WalkRight(xrand,yrand);
			if(prob_2 >= 0.6 && prob_2 < 0.8) WalkUp(xrand,yrand);
			if(prob_2 >= 0.8) WalkDown(xrand,yrand);
		}
		if(ground[xrand][yrand]==4||ground[xrand][yrand]==5||ground[xrand][yrand]==6)
		{
			//printf("Complex at %d %d selected",xrand,yrand);
			dissociateInto(xrand,yrand);
		}
	}
	//printf("MCStepId = %d completed\n",MCStepId);
}

void WalkLeft(int xrand, int yrand)
{
	if(xrand > 0)
	{
		if(ground[xrand-1][yrand]!=0)
		{
			react(xrand,yrand,xrand-1,yrand);
		}
		else{
			ground[xrand-1][yrand]=ground[xrand][yrand];
			//printf("%d",ground[xrand-1][yrand]);
			ground[xrand][yrand]=0;
		}
	}
}
void WalkRight(int xrand, int yrand)
{
	if(xrand < 59)
	{
		if(ground[xrand+1][yrand]!=0)
		{
			react(xrand,yrand,xrand+1,yrand);
		}
		else{
			ground[xrand+1][yrand]=ground[xrand][yrand];
			//printf("%d",ground[xrand+1][yrand]);
			ground[xrand][yrand]=0;
		}
	}
}
void WalkUp(int xrand, int yrand)
{
	if(yrand > 0)
	{
		if(ground[xrand][yrand-1]!=0)
		{
			react(xrand,yrand,xrand,yrand-1);
		}
		else{
			ground[xrand][yrand-1]=ground[xrand][yrand];
			//printf("%d",ground[xrand][yrand-1]);
			ground[xrand][yrand]=0;
		}
	}
}
void WalkDown(int xrand, int yrand)
{
	if(yrand < 59)
	{
		if(ground[xrand][yrand+1]!=0)
		{
			react(xrand,yrand,xrand+1,yrand);
		}
		else{
			ground[xrand][yrand+1]=ground[xrand][yrand];
			//printf("%d",ground[xrand][yrand+1]);
			ground[xrand][yrand]=0;
		}
	}
}
void react(int oldx, int oldy, int newx, int newy)
{
	//double prob10_r = rand()%10000;
	double prob_r = rand()/1000;//prob10_r/10000;	
	if((ground[oldx][oldy]==1&&ground[newx][newy]==2)||(ground[oldx][oldy]==2&&ground[newx][newy]==1))
	{
		if(prob_r<=Pr12)
		{
			//printf("tBid %d %d selected",xrand,yrand);
			ground[oldx][oldy]=4;
			ground[newx][newy]=4;
			int complexCount1=++complexCount;
			complexId[oldx][oldy]=complexCount1;
			complexId[newx][newy]=complexCount1;
			type[1]--;
			type[2]--;
			type[4]++;
		}
	}
	if((ground[oldx][oldy]==1&&ground[newx][newy]==3)||(ground[oldx][oldy]==3&&ground[newx][newy]==1))
	{
		if(prob_r<Pr13)
		{	
			ground[oldx][oldy]=5;
			ground[newx][newy]=5;
			int complexCount1=++complexCount;
			complexId[oldx][oldy]=complexCount1;
			complexId[newx][newy]=complexCount1;
			type[1]--;
			type[3]--;
			type[5]++;
		}
	}
	if((ground[oldx][oldy]==3&&ground[newx][newy]==2)||(ground[oldx][oldy]==2&&ground[newx][newy]==3))
	{
		if(prob_r<Pr32)
		{	
			ground[oldx][oldy]=6;
			ground[newx][newy]=6;
			int complexCount1=++complexCount;
			complexId[oldx][oldy]=complexCount1;
			complexId[newx][newy]=complexCount1;
			type[3]--;
			type[2]--;
			type[6]++;
		}
	}
}
void dissociateInto(int x, int y)
{
	int complexIdent = complexId[x][y];
	if(x>0)
	{
		if(complexId[x-1][y]==complexIdent)
		{
			dissociate(x,y,x-1,y);
		}
	}
	if(x<59)
	{
		if(complexId[x+1][y]==complexIdent)
		{
			dissociate(x,y,x+1,y);
		}
	}
	if(y>0)
	{
		if(complexId[x][y-1]==complexIdent)
		{
			dissociate(x,y,x,y-1);
		}
	}
	if(y<59)
	{
		if(complexId[x][y+1]==complexIdent)
		{
			dissociate(x,y,x,y+1);
		}
	}
}
void dissociate(int x, int y, int otherx, int othery)
{
	//double prob10_d = rand()%10000;
	double prob_d = rand()/1000;//prob10_d/10000;
	//printf("dissociate");
	if(ground[x][y]==4)
	{
		double prob10_3 = rand()%10;
		double prob_3 = prob10_3/10;
		if(prob_3<=0.5)
		{
			if(prob_d<Pd4)
			{
				ground[x][y] = 1;
				ground[otherx][othery] = 7;
				type[4]--;
				type[1]++;
				type[7]++;
				complexId[x][y] = 0;
				complexId[otherx][othery] = 0;
			}
		}
		else{
			if(prob_d<Pd41)
			{
				ground[x][y] = 1;
				ground[otherx][othery] = 2;
				type[4]--;
				type[1]++;
				type[2]++;
				complexId[x][y] = 0;
				complexId[otherx][othery] = 0;
			}
		}
	}
	if(ground[x][y]==5)
	{
		if(prob_d<Pd5)
		{
			ground[x][y] = 3;
			ground[otherx][othery] = 1;
			type[5]--;
			type[1]++;
			type[3]++;
			complexId[x][y] = 0;
			complexId[otherx][othery] = 0;
		}
	}
	if(ground[x][y]==6)
	{
		if(prob_d<Pd6)
		{
			ground[x][y] = 3;
			ground[otherx][othery] = 2;
			type[6]--;
			type[3]++;
			type[2]++;
			complexId[x][y] = 0;
			complexId[otherx][othery] = 0;
		}
	}
}
