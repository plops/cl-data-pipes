#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
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
  

  char s[200];
  int count;
  for(count=0;count<10;count++){
    fd_set rfds,efds;
    FD_ZERO(&rfds);
    FD_SET(a,&rfds);
    FD_SET(b,&rfds);
    FD_ZERO(&efds);
    FD_SET(a,&efds);
    FD_SET(b,&efds);
    
    int r=select(max(a,b)+1,&rfds,NULL,&efds,NULL);
    
    printf("%d select returned\n",count);
    fflush(stdout);
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
	printf("fd a\n");
	if(0==fgets(s,200,af)){
	  printf("fgets a returned 0\n");
	  continue;
	}
	printf("1 %s\n",s);
	fflush(stdout);
      }
      if(FD_ISSET(a,&efds))
	break;
      if(FD_ISSET(b,&efds))
	break;
      
      if(FD_ISSET(b,&rfds)){
	printf("fd b\n");
	if(0==fgets(s,200,bf)){
	  printf("fgets b returned 0\n");
	  continue;
	}
	printf("2 %s\n",s);
	fflush(stdout);
      }
    }
  }
  exit(EXIT_SUCCESS);
}
