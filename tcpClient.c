#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 4444
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct node{
  char msg[1024];
  int h,m;
  struct node* next;
};

struct node *head=NULL;

struct node* createNode(int hh , int mm, char mesg[] )
{
  struct node *n= (struct node*)malloc(sizeof(struct node));
  strcpy(n->msg,mesg);
  n->h=hh;
  n->m=mm;
  n->next=NULL;
}

void insertEnd(struct node *n)
{
  struct node *ptr=head;

  if(head!=NULL)
  {
    //traverse till last node
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=n;

  }
  else{
    head=n;
  }
}

void deleteBeg()
{
  struct node *ptr;

  if(head==NULL)
  {
    printf("Empty LIST\n");
  }
  else
  {
    ptr=head;
    head=head->next;
    free(ptr);
  }
}

void display()
{
  struct node *ptr=head;
  if(head!=NULL)
  {
    do{
      printf("%s %d:%d\n",ptr->msg,ptr->h,ptr->m);
      ptr=ptr->next;
    }while(ptr!=NULL);
  }
  else
  {
    printf("Empty list\n");
  }
}

int getH()
{
        time_t tim=time(NULL);
        struct tm *t= localtime(&tim);
        return t->tm_hour;
}


int getM()
{
        time_t tim=time(NULL);
        struct tm *t= localtime(&tim);
        return t->tm_min;
}
void deleteI(int n)
{

  struct node *ptr=head, *temp;
  int i=0;
  if(head!=NULL)
  {
    if(n==0)
    {
      deleteBeg();
    }
    else
    {
      while(i<n-1)
      {
        ptr=ptr->next;
        i++;
      }
      
      temp=(ptr->next);
      ptr->next=temp->next;
      free(temp);

    }
  }
  else
  {
    printf("Empty list in specififed delete\n");
  }
}

char arr[1024];

char*  putMessage()
{
  int ch,br=0;
  
  struct node *n, *ptr;
  int uh=getH(),um=getM();
  
  {
  printf("1. Send message now\n");
//  printf("2. Delete message\n");
//  printf("3. Show messages\n");
  printf("2. Send later\n");
  scanf("%d",&ch);

    switch(ch)
    {
    case 1:
      printf("Enter message: ");
      scanf("%s",arr);
      n=createNode(getH(),getM(),arr);
      insertEnd(n);
	
      break;
    case 3:
      deleteBeg();
      printf("message deleted\n");
      break;
    case 4:
      display();
      break;
    case 2:
      printf("Enter message: ");
      scanf("%s",arr);
      printf("Enter time in  HH:mm format\n");
        scanf("%d:%d",&uh,&um);
        printf("Entered time: %d:%d\n",uh,um);
        
      n=createNode(uh,um,arr);
      insertEnd(n);
      break;
    
    default:
      printf("Invalid choice\n");
	strcpy(arr,"X");
	return arr;
    
    }//end of switch case
  }  // end of do-whille
//  return 0;

//traversing queue to find latest message to send
	ptr=head;
	int i=0;
	do{
		if(ptr->h<=getH() && ptr->m<=getM())
		{
			strcpy(arr, ptr->msg);
			deleteI(i);
			return arr;
		}
		ptr=ptr->next;
		i++;
	}while(ptr!=NULL);
	
	printf("No message to send in queue\n");
	return "";
}//end of main

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		printf("Error no is : %d\n",errno);
		printf("Error description is : %s\n",strerror(errno));
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Client: \t");
		strcpy(buffer,putMessage());
		printf("You entered: %s\n",buffer);
		while(strcmp(buffer,"")==0)
			{
					strcpy(buffer,putMessage());
			}		
			
		
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}

	return 0;
}
