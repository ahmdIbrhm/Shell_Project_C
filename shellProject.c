	#include <stdio.h>
	#include <stdlib.h>
	#include<string.h>
	#include <signal.h>
	#include<unistd.h>
	#include <fcntl.h>
	#include <curses.h>
	#include "type_list.h"
	#define buffer 4096
//begin of stack structure//

//end of stacklist
int child;
char *directory;
char *removeEndl(char *s)
{
	char *res;
	res=(char *)malloc(sizeof(char)*buffer);
	int i;
	for (i = 0; i < buffer; ++i)
	{
		if(s[i]=='\n')
			break;
		res[i]=s[i];
	}
	return res;
}
int countSpaces(char *s)
{
	int i=0;
	int count=0;
	for(i=0;i<buffer;i++)
	{
		if(s[i]==' ' && i<strlen(s)-1)
			count++;
	}
	return count;
}
int countAmprecend(char *s)
{
	int i=0;
	int count=0;
	for(i=0;i<buffer;i++)
	{
		if(s[i]=='&' && s[i+1]=='&' && i<strlen(s)-1)
			count++;
	}
	return count;
}

int countPipes(char *s)
{
	int i=0;
	int count=0;
	for(i=0;i<buffer;i++)
	{
		if(s[i]=='|' && i<strlen(s)-1)
			count++;
	}
	return count;
}
char **splitBySpace(char *s)
{
	char *p;
	char **res;
	res=(char **)malloc(sizeof(char *)*buffer);
	int i=0;
	for(p=strtok(s," ");p!=NULL;p=strtok(NULL," "))
	{
			//printf("word:%s\n",p);
		fflush(stdout);
		res[i]=p;
		i++;
	}
	res[i]=NULL;
	return res;
}
char **splitByPipe(char *s)
{
	char *p;
	char **res;
	res=(char **)malloc(sizeof(char *)*buffer);
	int i=0;
	for(p=strtok(s,"|");p!=NULL;p=strtok(NULL,"|"))
	{
		res[i]=p;
		i++;
	}
	return res;
}
char *removeAmpercend(char *s)
{
	char *res;
	res=(char *)malloc(sizeof(char)*buffer);
	int i;
	for (i = 0; i < buffer; ++i)
	{
		if(s[i]=='&')
			break;
		res[i]=s[i];
	}
	return res;
}
char *removeDotSlash(char *s)
{
	char *res;
	res=(char *)malloc(sizeof(char)*buffer);
	int i;
	int c=0;
	for (i = 0; i < buffer; ++i)
	{
		if(i!=0 && i!=1)
			res[c++]=s[i];
	}
	return res;
}
int endsWithAmpercend(char *s)
{
	if(s[strlen(s)-1]=='&')
	{
		return 1;
	}
	return 0;
}
char **splitByTwoAmpercand(char *s)
{
	char *p;
	char **res;
	res=(char **)malloc(sizeof(char **)*(2));
	int i=0;
	for(p=strtok(s,"&&");p!=NULL;p=strtok(NULL,"&&"))
	{
		res[i]=p;
		i++;
	}
	return res;
}

int checkSign(char *s)
{
	int i;
	int count=0;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]=='>')
			count++;
	}
	return count;
}
char **splitBySign(char *s)
{
	char *p;
	char **res;
	res=(char **)malloc(sizeof(char **)*2);
	int i=0;
	for(p=strtok(s,">");p!=NULL;p=strtok(NULL,">"))
	{
		res[i]=p;
		i++;
	}
	return res;
}
int parrallelProcesses(char *s)
{
	int count=0;
	int i;
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]=='&' && s[i+1]=='&' && i!=strlen(s)-1)
			return 1;
	}
	return 0;
}
list commandsInBack;
char *getCommandNameFromPsFile(int pid)
{
	FILE *fp=fopen("ps","r");
	char s[buffer];
	fgets(s,buffer,fp);
	int p;
	char s2[buffer];
	char s3[buffer];
	char *s4;
	s4=(char *)malloc(sizeof(char)*buffer);

	while(fscanf(fp,"  %d %s    %s %s",&p,s2,s3,s4)==4)
	{
		if(p==pid)
			return s4;
	}
	return NULL;
}
void functionKillParent(int s) //this function is to when pressing ctrl+c make as in shell
{
	printf("\n");
	fflush(stdout);
}
pid_t child3;
int ch;
char *commandToStop;
void functionStopParent(int s)
{
	element e;
	e.id=commandsInBack.size+1;
	e.pid=ch;
	e.name=commandToStop;
	e.status="Stopped";
	if(commandToStop!=NULL)
		insertTail(&commandsInBack,e);
	
	fflush(stdout);
}
void functionKillChild(int s)
{
	signal(SIGINT, SIG_DFL);
	//printf("Kill child: \n");
	fflush(stdout);
	kill(getpid(),SIGINT);
}
void functionStopChild(int s)
{
	signal(SIGTSTP, SIG_DFL);
	//printf("Stop Child\n");
	fflush(stdout);
	exit(0);
}

int isexist(list l,int pid)
{
	node *temp;
	temp=l.head;
	while(temp!=NULL)
	{
		if(temp->e.pid==pid)
			return 1;
	}
	return 0;
}
void printHistory(list l)
{
	node *temp;
	temp=l.head;
	while(temp!=NULL)
	{
		printf("%d       %s\n",temp->e.id,temp->e.name);	
		temp=temp->next;
	}
}
//check running process
int checkPs(char *command)
{
	FILE *fp=fopen("ps","r");
	char s[buffer];
	fgets(s,buffer,fp);
	char s1[buffer];
	char s2[buffer];
	char s3[buffer];
	char s4[buffer];
	while(fscanf(fp,"  %s %s    %s %s",s1,s2,s3,s4)==4)
	{
		if(strcmp(s4,command)==0)
			return 1;
	}
	return 0;
}
void updateJobs()
{
	node *temp;
	temp=commandsInBack.head;
	while(temp!=NULL)
	{
		if(checkPs(temp->e.name)==0)
			removeItem(&commandsInBack,temp->e);
		temp=temp->next;
	}
}
int status=0;
char **cmnd;
pid_t child1,child2,wpid;
int f[2];


void function1Pipe(char *array)
{
	if((child1=fork())==0)
			{
				close(0);
				close(f[1]);
				dup(f[0]);
				char **innerCommand;
				innerCommand=splitBySpace(cmnd[1]);
				int ex=execvp(innerCommand[0],innerCommand);
				if(ex==-1)
					printf("%s: command not found\n",cmnd[1]);
				fflush(stdout);
				exit(0);	
			}
			else if((child2=fork())==0)
			{
				close(1);
				close(f[0]);
				dup(f[1]);
				char **innerCommand;
				innerCommand=splitBySpace(cmnd[0]);
				int ex=execvp(innerCommand[0],innerCommand);
				if(ex==-1)
					fflush(stdout);
				exit(0);
			}
			else
			{
				close(f[0]);
				close(f[1]);
				while ((wpid = waitpid(-1,&status,WUNTRACED)) == WIFSTOPPED(status));
			}
}
int childsCount=0;
int commandExecute(char *cmnds,int input, int first, int last)
{
	int pip[2];
	pipe(pip);	
	if (fork()== 0) 
	{
		if (first == 1 && last == 0 && input == 0) 
		{
			// First command
			dup2( pip[1], 1 );
		} 
		else if (first == 0 && last == 0 && input != 0) 
		{
			// Middle command
			dup2(input, 0);
			dup2(pip[1], 1);
		} 
		else 
		{
			// Last command
			dup2( input, 0 );
		}
 		char **cmndWithNoSpace=splitBySpace(cmnds);
		if (execvp( cmndWithNoSpace[0], cmndWithNoSpace) == -1)
			printf("error..unknown commands !\n");
	}
 
	if (input != 0) 
		close(input);
 	
 	close(pip[1]);
 	
 	if (last == 1)
		close(pip[0]);
 	
 	childsCount++;
	
	return pip[0];
}
void functionManyPipes(char *array)
{
	int i;
	int n=countPipes(removeEndl(array))+1;
	char **cmnds=splitByPipe(removeEndl(array));
	int input = 0;
	int first = 1;
	int c=0;
	for(i=0;i<n;i++)
	{
		if(i==n-1)
			commandExecute(cmnds[i],input,first,1);
		else
			input=commandExecute(cmnds[i],input,first,0);
		first=0;
	}
	for(i=0;i<childsCount;i++)
		wait(NULL);
	
	childsCount=0;
}
void functionSpecial(char *array)
{
	int boo=0;
	int st;
	if((ch=fork())==0)
	{
		signal(SIGTSTP,functionStopChild);
		signal(SIGINT,functionKillChild);
		int x=system(array);
		fflush(stdout);
		if(x==-1)
			printf("%s: command not found",removeEndl(array));
		else
		{
			boo=1;
			printf("[1] %d\n",getpid());
		}
		exit(boo);
	}
	wait(&st);
	if(WEXITSTATUS(st)==1)
	{
				system("ps > ps");//every time put running process in ps file
				char *commonadtojob;
				if(array[0]=='.')
				{
					commonadtojob=removeEndl(removeAmpercend(removeDotSlash(array)));
				}
				else
					commonadtojob=removeEndl(removeAmpercend(array));
				commandToStop=commonadtojob;
				printf("command:%sxxx",commonadtojob);
				if(checkPs(commonadtojob))
				{	
					element command;
					command.name=commonadtojob;
					command.id=commandsInBack.size+1;
					command.status="Running";
					command.pid=ch;
					insertTail(&commandsInBack,command);
				}
			}
}
void functionCommandWriteToFile(char * array)
{
	int c1,c2;
	char **innerCommand=splitBySign(removeEndl(array));
	if(fork()==0)
	{
		close(1);
		FILE *fp;
		fp=fopen(innerCommand[1],"w");
		int fd=open(innerCommand[1],O_WRONLY,S_IWUSR);
		char **in=splitBySpace(innerCommand[0]);
		dup2(fd,1);
		execvp(in[0],in);
		exit(0);
	}

}
void functionParallelProcesses(char *array)
{
	char **arrayOfCommands=splitByTwoAmpercand(removeEndl(array));
	int length=countAmprecend(removeEndl(array))+1;
	int i;
	if(fork()==0)
	{
		for(i=0;i<length;i++)
		{
			if(fork()==0)
			{
				system(arrayOfCommands[i]);
				exit(0);
			}
		}
		exit(0);
	}
	else
		while ((wpid = waitpid(-1,&status,WUNTRACED)) && WIFSTOPPED(status));

}
void functionExec(char *array)
{
	char **innerCommand;
	innerCommand=splitBySpace(removeEndl(array));
	if(strcmp(innerCommand[0],"cd")==0)
	{
		chdir(innerCommand[1]);
	}
	else
	{
		if((child3=fork())==0)
		{
			signal(SIGTSTP,functionStopChild);
			signal(SIGINT,functionKillChild);
			if(execvp(innerCommand[0],innerCommand)==-1)
				printf("%s: command not found\n",removeEndl(array));
			exit(0);
		}
		else
		{
			signal(SIGTSTP,functionStopParent);
			signal(SIGINT,functionKillParent);
		}
	}
}
void functionInSystem(char *array)
{
	if(fork()==0)
	{
		if(system(array)==-1)
			printf("command:%s not found\n",array);
		exit(0);
	}
}
void init()
{
	directory=(char *)malloc(sizeof(char)*buffer);
	commandsInBack=createList();
	list history=createList();
	while(1)
	{
		signal(SIGTSTP,functionStopParent);
		signal(SIGINT,functionKillParent);
		
		while ((wpid = waitpid(-1,&status,WUNTRACED)) == WIFSTOPPED(status));//wait for every child to finish
		
		pipe(f);

		getcwd(directory,buffer);				//get cuurent "path directory" and store it in directory
		printf("%s/wehbi# ",directory);
		
		system("ps>ps");
		
		updateJobs(); //update jobs list reads from ps and check each item if still in ps

		char *pointerToArray;
		char *array;//take input from keyboard
		array=(char *)malloc(sizeof(char)*buffer);
		pointerToArray= fgets(array,buffer,stdin);
		
		if(array[0]=='.')
			commandToStop=removeEndl(removeAmpercend(removeDotSlash(array))); //save the command inorder to add it to jobs if the command is stopped
		else
			commandToStop=removeEndl(array);
		element his;
		his.name=removeEndl(array);
		his.id=history.size+1;
		cmnd=splitByPipe(removeEndl(array));
		fflush(stdout);
		
		int nbOfPipes=countPipes(array);
		
		if(array[0]!='\n')
		insertTail(&history,his);
		
		if(pointerToArray==NULL)
		{
			printf("\n");
			exit(0);
		}
		
		if(array[0]=='\n')
		{
			//when pressing enter without a command
		}
		else if(strcmp(removeEndl(array),"history")==0)
		{
			printHistory(history);
		}
		else if(nbOfPipes==1)
		{
			function1Pipe(array);
		}
		else if (nbOfPipes>1)
		{
			fflush(stdout);
			functionManyPipes(array);
		}
		else if(checkSign(array)>1 )
		{
			functionInSystem(removeEndl(array));
		}
		else if(endsWithAmpercend(removeEndl(pointerToArray)))
		{
			functionSpecial(array);
		}
		else if(array[0]=='.')
		{
			if(fork()==0)
			{
				signal(SIGTSTP,functionStopChild);
				signal(SIGINT,functionKillChild);
				system(removeEndl(array));
				exit(0);
			}
		}
		else if(strcmp(cmnd[0],"jobs")==0)
		{
			printList(commandsInBack);
		}
		else if(checkSign(array)==1)
		{
			functionCommandWriteToFile(array);
		}
		else if(parrallelProcesses(cmnd[0]))
		{	
			functionParallelProcesses(cmnd[0]);
		}
		else
		{
			functionExec(array);
		}
	}
}
void main()
{
	init();
}
