#include <stdio.h>
#include <math.h>
#include <time.h>
//Размеры и объем изображения
#define WIDTH 1280
#define HEIGHT 1024
#define VOLUME 3932160
size_t fwrite(const void *buf, size_t size, size_t count, FILE *stream);
clock_t clock(void);
int frame(double pcenter, double qcenter, double range);
FILE *fopen(const char *fname, const char *mode);
double Pmax, Pmin, Qmin;
//Хидер bmp файла
char head[]={0x42, 0x4D, 0x36, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char results[VOLUME];
int main(void)
{
  double range, xcenter, ycenter;
  range=0.000000049304;
    xcenter=-0.743643900055;
    ycenter=0.131825890901;
    frame(xcenter, ycenter, range);
  FILE *str;
  str=fopen("output.bmp", "wb");
  fwrite(head, 1, 54, str);
  fwrite(results, 1, VOLUME, str);
  fclose(str);
  // FPS 
  printf("clock %.4f\n", 1.0*CLOCKS_PER_SEC/(1.0*(clock())));
  return 0;
}
int frame(double pcenter, double qcenter, double range)
{
  double dp, p, q, cardio;
  register double x, y, x2, y2, xtemp;
  register int i,j,n;
  int colwidth=666;
  int coloffset=3200;
  int nmax=24000;
  long int black=0;
  char *pres=results;
  
  Pmax=pcenter+range/2;
  Pmin=pcenter-range/2;
  Qmin=qcenter-range/2.5;
  dp=range/(WIDTH-1);
  for (j=0;j<HEIGHT;j++)
  {
  	 q=Qmin+j*dp;
    for (i=0;i<WIDTH;i++)
    {
      p=Pmin+i*dp;
//Исключаем то, что заведомо относится к множеству
      cardio=sqrt((p-0.25)*(p-0.25)+q*q);
      if (p<(cardio-2*cardio*cardio+0.25)||(p+1)*(p+1)+q*q<0.0625)
        {
        	 *pres=black;
          pres+=3;
        }
      else
      {
  	     x=y=x2=y2=n=0;
        while (x2+y2<4&&n<nmax)
        {
          xtemp=x2-y2+p;
          y=2*x*y+q;
          x=xtemp;
          x2=x*x;
          y2=y*y;
          n++;
        }
// Раскрашиваем
        if (n==nmax) 
        {
          *pres=black;
          pres+=3;
        }
        else 
        { 
          n=(n+coloffset)%(3*colwidth);
          if (n/colwidth==0)
          { // Голубой 116.11.0          
            *pres++=116+139*n/colwidth;//B
            *pres++=11+244*n/colwidth;//G
            *pres++=237*n/colwidth;//R
          }
          else if (n/colwidth==1)
          { // Белый 255.255.237
            n-=colwidth;
            *pres++=255-253*n/colwidth;//B
            *pres++=255-123*n/colwidth;//G
            *pres++=238;//R            
          }
          else 
          { // Рыжий 2.132.239
            n-=2*colwidth;
            *pres++=2+114*n/colwidth;//B
            *pres++=132-121*n/colwidth;//G
            *pres++=239-239*n/colwidth;//R
          }
        }
      }
    }
  }  
  return results[VOLUME];
}
