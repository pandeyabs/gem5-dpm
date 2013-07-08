#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  char buffer[1000];
  char *cmds[5000];

  FILE *fp;
  char *tmp;
  char *fileToRead = (char *) "testfile";
  char *outputFile = (char *) "testfile";
  int fd1;

  int num_cmds = 0, count = 0;
  fileToRead = argv[1];
  fp = fopen(fileToRead, "r");
  if (fp==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

  outputFile = argv[2];
  fd1 = open(outputFile, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
  if (fd1==NULL) 
    {
      char error_message[30] = "Can't open file.\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

  while(!feof(fp))
    {
      fgets(buffer, 1000, fp);
      strtok(buffer, "\n\r");
      
      tmp = strtok(buffer, " ");
    
      num_cmds = 0;
      while (tmp) 
	{
	  cmds[num_cmds] = tmp;
	  num_cmds++;
	  tmp = strtok(NULL, " ");
	}
      count++;

      //power delay int_alu_accesses float_alu_accesses committed_instructions committed_int_instructions committed_float_instructions load_instrutions store_instructions

      //printf("Frequency: %s \t Power: %s \t Delay: %s \t Int ALU Accesses: %s \t Float ALU Accesses: %s \t Committed Ins: %s \tCommitted Int Ins: %s \tCommitted Float Ins: %s \t Load Instr: %s \tStore Ins: %s\n", cmds[0], cmds[1], cmds[2], cmds[3], cmds[4], cmds[5], cmds[6], cmds[7], cmds[8], cmds[9]);  

      //7 1:11.1678928469434 2:2.56395513369111 3:1.58001065576954 4:2.56359226682869 5:1.7399072151527 6:2.10291930103123 7:6.65093108248809 8:11.8921292358571 9:4.71937871005353 10:2.68109242807978 13:0.844904909906825 46:0.674199862463242


      //during training & testing, power and delay are not needed

      write(fd1, cmds[0], strlen(cmds[0]));
      write(fd1, " 1:", strlen(" 1:"));
      write(fd1, cmds[3], strlen(cmds[3]));
      //write(fd1, cmds[1], strlen(cmds[1]));
      write(fd1, " 2:", strlen(" 1:"));
      write(fd1, cmds[4], strlen(cmds[4]));
      //write(fd1, cmds[2], strlen(cmds[2]));
      write(fd1, " 3:", strlen(" 1:"));
      write(fd1, cmds[5], strlen(cmds[5]));
      write(fd1, " 4:", strlen(" 1:"));
      write(fd1, cmds[6], strlen(cmds[6]));
      write(fd1, " 5:", strlen(" 1:"));
      write(fd1, cmds[7], strlen(cmds[7]));
      write(fd1, " 6:", strlen(" 1:"));
      write(fd1, cmds[8], strlen(cmds[8]));
      write(fd1, " 7:", strlen(" 1:"));
      write(fd1, cmds[9], strlen(cmds[9]));
      write(fd1, " 8:", strlen(" 1:"));
      //write(fd1, " 9:", strlen(" 1:"));
      //write(fd1, " 10:", strlen(" 10:"));
      write(fd1, cmds[10], strlen(cmds[10]));

      write(fd1, "\r\n", strlen("\r\n"));
    }
}

