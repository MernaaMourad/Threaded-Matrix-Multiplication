#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define  M 300
int row1,column1,row2,column2;
int matrix1[M][M];
int matrix2[M][M];
int resultMatrix1[M][M];
int resultMatrix2[M][M];
//create struct for arguments used in creation of threads
typedef struct arguments{ 
int i;//row
int j;// column
}arguments;
//method 1 thread for each element
void* multiMethod1(void* arg)
{
   
   arguments *data=(arguments *)arg;//casting
      for (int k = 0; k < row2; k++)
        resultMatrix1[data->i][data->j] += matrix1[data->i][k] * matrix2[k][data->j];
}

//method 2 thread for each row
void* multiMethod2(void* arg)
{
   arguments *data=(arguments *)arg;//casting
   for (int j=0;j<column2;j++)
   {
      for (int k = 0; k < row2; k++)
      {
        resultMatrix2[data->i][j] += matrix1[data->i][k] * matrix2[k][j];
      }
   }
}

int main()
{
int i,j,k;
clock_t start1,end1,start2,end2;
double timeElapsed1,timeElapsed2;
char filename[1024] ;
printf("Please enter file name: ");
fgets(filename,1024,stdin);//gets filename from user
strtok(filename,"\n");//remove new line due to usage of fgets
FILE *fptr;
fptr=fopen(filename,"r"); //open file to read data
if(fptr==NULL)
{
printf("Error can not open the file\n");//NO file with such name
        exit(1);
}
fscanf(fptr,"%d%d",&row1,&column1) ;
for(i=0;i<row1;i++)
{ for (j=0;j<column1;j++)
      {
      fscanf(fptr,"%d",&matrix1[i][j]) ;
      }
}
fscanf(fptr,"%d%d",&row2,&column2) ;
if(column1!=row2)
{
printf("Mutiplication of matrix is not possibe\n");
exit(1);
}
for(i=0;i<row2;i++)
{ for (j=0;j<column2;j++)
      {
      fscanf(fptr,"%d",&matrix2[i][j]) ;
      }
}
fclose(fptr);


//method 1 thread for each element
int Noth1=row1*column2;//for all elements of output
pthread_t th1[Noth1];//creating thread for every element in output
start1=clock();//determining start time for method1
//create row1*column2 threads
int count=0;
for(i=0;i<row1;i++)
{
 for (j=0;j<column2;j++)
 {
 
  arguments *data1=(arguments *)malloc(sizeof(arguments));
  data1->i=i;
  data1->j=j;
  pthread_create(&th1[count],NULL,multiMethod1,(void *)data1);
  count++;
 }

}
//wait 
for(i=0;i<Noth1;i++)
{
 pthread_join(th1[i],NULL);
 }

end1=clock(); //determining end time for method1
timeElapsed1=((double)(end1-start1))/CLOCKS_PER_SEC;//time for method1


//method 2 thread for each element
int Noth2=row1;//for all rows of output
pthread_t th2[Noth2];//creating thread for every row in output
int id[Noth2];//so each thread passes a pointer to seperate row in matrix
start2=clock();//determining start time for method2
//create #row1 threads
for(i=0;i<row1;i++)
{
 
  arguments *data2=(arguments *)malloc(sizeof(arguments));
  data2->i=i;
  pthread_create(&th2[i],NULL,multiMethod2,(void *)data2);
 

}
//wait 
for(i=0;i<Noth2;i++)
{

 pthread_join(th2[i],NULL); 
 }

end2=clock(); //determining end time for method2
timeElapsed2=((double)(end2-start2))/CLOCKS_PER_SEC;//time for method2

FILE *fp;
fp=fopen("output-matrix.txt","w"); //open file to rite data
if(fptr==NULL)
{
printf("Error can not create the file\n");
        exit(1);
}
//printing in file output of first method
  fprintf(fp,"By Element\n");
for(i=0; i<row1; i++) 
{ for(j=0; j<column2; j++) 
  { 
  fprintf(fp,"%d\t ", resultMatrix1[i][j]); 
  } 
  fprintf(fp,"\n");
} 
fprintf(fp,"END1 %f s \n", timeElapsed1);
//printing in file output of second method
  fprintf(fp,"By Row\n");
for(i=0; i<row1; i++) 
{ for(j=0; j<column2; j++) 
  { 
  fprintf(fp,"%d\t ", resultMatrix2[i][j]); 
  } 
  fprintf(fp,"\n");
} 
fprintf(fp,"END2 %f s \n", timeElapsed2);
fclose(fp);
return 0;
}