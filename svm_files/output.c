#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  char *buffer = (char*) malloc(1000*sizeof(char));
  char *buffer1= (char*) malloc(1000*sizeof(char));
  char *buffer2= (char*) malloc(1000*sizeof(char));
  char *buffer3= (char*) malloc(1000*sizeof(char));
//char *buffer[10000];
//  char *buffer1[10000];
//  char *buffer2[10000];
//  char *buffer3[10000];


  int *vals= (int *) malloc(1000000*sizeof(int));;
  //int vals[10000];

  FILE *fp, *fp1, *fp2, *fp3;
  char *tmp;
  char *fileToRead = (char *) "testfile"; //gurobi binary
  char *firstFile = (char *) "testfile"; //1GHz file
  char *secondFile = (char *) "testfile"; //1.6GHz file
  char *thirdFile = (char *) "testfile"; //2GHz file

  int num_cmds = 0, count = 0;
  fileToRead = argv[1];
  fp = fopen(fileToRead, "r");
  if (fp==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

  firstFile = argv[2];
  fp1 = fopen(firstFile, "r");
  if (fp1==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

  secondFile = argv[3];
  fp2 = fopen(secondFile, "r");
  if (fp2==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

  thirdFile = argv[4];
  fp3 = fopen(thirdFile, "r");
  if (fp3==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }


  int i = 0;
  int j;
  int first, second, third;
  while(!feof(fp))
    {
      fgets(buffer, 1000, fp);
      strtok(buffer, "\n\r");

      vals[i] = atoi(buffer);
      printf("Value obtained: %d\n", vals[i]);
      i++;
    }

  for(j = 0 ; j < i ; j = j + 3)
    {
      first = 0; second = 0;
      if(vals[j] == 1)
	{
	  //printf("Position: %d, Value obtained: %d\n", j, vals[j]);
	  //printf("Acquiring date from the first file\n");
	  first = 1;
	}
      else if(vals[j+1] == 1)
	{
	  //printf("Position: %d, Value obtained: %d\n", j+1, vals[j+1]);
	  //printf("Acquiring date from the second file\n");
	  second = 1;
	}
      else if(vals[j+2] == 1)
	{
	  //printf("Position: %d, Value obtained: %d\n", j+1, vals[j+1]);
	  //printf("Acquiring date from the second file\n");
	  third = 1;
	}
  
      if(first == 1)
	{
	 fp1 = fopen(firstFile, "r");
	  int line = j/3;
	  int count = 0;
	  if(line == 0)
	    {
	      fgets(buffer1, 1000, fp1);
	      strtok(buffer1, "\n\r");
	     printf("%s\n", buffer1);
	    }	    
	  else
	    {
	      while(count < line)
		{
		  fgets(buffer1, 1000, fp1);
		  strtok(buffer1, "\n\r");
		  count++;
		}
	      fgets(buffer1, 1000, fp1);
	      strtok(buffer1, "\n\r");
	      
	      printf("%s\n", buffer1);
	    }
	}
      else if(second == 1)
	{
	 //fp1 = fopen(firstFile, "r");
	 fp2 = fopen(secondFile, "r");
	  int line = j/3;//(j+1)/2;
	  int count = 0;
	  if(line == 0)
	    {
	      fgets(buffer2, 1000, fp2);
	      strtok(buffer2, "\n\r");
	      printf("%s\n", buffer2);
	    }	    
	  else
	    {
	      while(count < line)
		{
		  fgets(buffer2, 1000, fp2);
		  strtok(buffer2, "\n\r");
		  //printf("***Second File: %s\n\n", buffer2);
		  count++;
		}
	      fgets(buffer2, 1000, fp2);
	      strtok(buffer2, "\n\r");
	      printf("%s\n", buffer2);
	    }
	}
      else if(third == 1)
	{
	 //fp1 = fopen(firstFile, "r");
	 fp3 = fopen(thirdFile, "r");
	  int line = j/3;//(j+1)/2;
	  int count = 0;
	  if(line == 0)
	    {
	      fgets(buffer3, 1000, fp3);
	      strtok(buffer3, "\n\r");
	      printf("%s\n", buffer3);
	    }	    
	  else
	    {
	      while(count < line)
		{
		  fgets(buffer3, 1000, fp3);
		  strtok(buffer3, "\n\r");
		  //printf("***Second File: %s\n\n", buffer3);
		  count++;
		}
	      fgets(buffer3, 1000, fp3);
	      strtok(buffer3,"\n\r");
	      printf("%s\n", buffer3);
	    }
	}
    }
}
  
