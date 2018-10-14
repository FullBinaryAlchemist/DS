#include<time.h>
#include<stdio.h>
#include<unistd.h>

int main()
{
	time_t tim=time(NULL);;
	struct tm *t= localtime(&tim);
	int h,m,uh,um;
	printf("HH:mm %d:%d\n",h=t->tm_hour,m=t->tm_min);
	printf("Enter time in  HH:mm format\n");
	scanf("%d:%d",&uh,&um);
	printf("Entered time: %d:%d",uh,um);
	while(1){
	
		if(t->tm_hour>=uh && t->tm_min>=um)
		{
		printf("yes\n");
		break;
		}

		sleep(5);
		tim=time(NULL);
		t=localtime(&tim);
		printf("HH:mm:ss %d:%d:%d\n",h=t->tm_hour,m=t->tm_min,t->tm_sec);

	}
	return 0;
}
