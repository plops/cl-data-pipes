#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int
max(int a,int b)
{
  if(a>b)
    return a;
  return b;
}

int
main()
{
  FILE*af=fopen("2-to-1","r");
  FILE*bf=fopen("3-to-1","r");
  
  int a=fileno(af);
  int b=fileno(bf);
  
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(a,&rfds);
  FD_SET(b,&rfds);

  char s[200];

  for(;;){
    int r=select(max(a,b)+1,&rfds,NULL,NULL,NULL);
    
    if(r==-1 && errno==EINTR)
      continue;
    if(r==-1){
      perror("select()");
      exit(EXIT_FAILURE);
    }
    
    if(FD_ISSET(a,&rfds)){
      if(0==fgets(s,200,af))
	continue;
      printf("1 %s\n",s);
      fflush(stdout);
    }
    if(FD_ISSET(b,&rfds)){
      if(0==fgets(s,200,bf))
	continue;
      printf("2 %s\n",s);
      fflush(stdout);
    }
  }
  exit(EXIT_SUCCESS);
}
