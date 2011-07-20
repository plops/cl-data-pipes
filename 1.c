#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

/*
cat - > 3-to-1
cat - > 2-to-1
*/

int
max(int a,int b)
{
  if(a>b)
    return a;
  return b;
}

char s[200];

int
main()
{
  int a=open("2-to-1",O_RDONLY);
  int b=open("3-to-1",O_RDONLY);


  int count;
  for(count=0;count<10;count++){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(a,&rfds);
    FD_SET(b,&rfds);
    
    int r=select(max(a,b)+1,&rfds,NULL,NULL,NULL);
    
    //    printf("%d select returned\n",count);
    //    fflush(stdout);
    if(r==-1 && errno==EINTR){
      printf("eintr\n");
      continue;
    }
    if(r==-1){
      perror("select()");
      exit(EXIT_FAILURE);
    }
    if(r){
      if(FD_ISSET(a,&rfds)){
	int n=read(a, s, 200);
	s[n-1]=0;
	printf("fd a %s\n", s);
      }
      if(FD_ISSET(b,&rfds)){
	int n=read(b, s, 200);
	s[n-1]=0;
	printf("fd b %s\n", s);
      }
    }
  }
  exit(EXIT_SUCCESS);
}
