#include "shell.h"
#define TRUE 1

void  INThandler(int sig)
{
     char  c;
     switch (sig){
            case SIGINT:
                 
                 printf("Are you sure? [y/n] ");
                 c = getchar();
                 if (c == 'y' || c == 'Y')
                      exit(0);
                 else
                      longjmp(buff,1); 
                      break;
     }
     return;
}

void proc(void)
{
	int status,i;
	char *command = NULL;
	char **parameters;
	char **temp_parameters;
	int ParaNum;
	char prompt1[MAX_PROMPT];
	struct parse_info info;
    	pid_t ChdPid;
	
	parameters = malloc(sizeof(char *)*(MAXARG+2));
	temp_parameters = malloc(sizeof(char *)*(MAXARG+2));

	buffer = malloc(sizeof(char) * MAXLINE);
	if(parameters == NULL || buffer == NULL)
	{
 		printf("Wshell error:malloc failed.\n");
		return;
	}

	signal(SIGINT, INThandler);

/********************************/
/* 	READ PROFILE FILE 	*/
/********************************/
   	
	FILE *fp;
	char *temp;
	char *home;

	struct path_info* paths = malloc(sizeof(struct path_info));

	int num_line=0;
	int buff_size=64;
    
	fp = fopen("profile","r");
    
	if(fp == NULL)
	{
		printf("Can not open profile!\n");
		exit(1);
	}

	while(fgets(temp, buff_size,fp)!=NULL)
	{
		if(num_line==0)
		{
			temp = temp + 5;
			home = strtok(temp, ";");
			printf("The home directory is: %s\n", home);
			chdir(home);
		}

		if(num_line==1)
		{
			temp = temp + 5;
			paths->path1 = strtok(temp, ";");
			paths->path2 = strtok(strstr(temp, ";"), ";");
			printf("The path directories are: %s & %s\n", paths->path1, paths->path2);
		}
		num_line++;
	}

/****************************/
	//arg[0] is command

	while(TRUE)
	{
		int pipe_i_o[2],in_fd,out;
		prompt(prompt1);
		setjmp(buff);
		ParaNum = read_command(&command,parameters,prompt1);
        	if(-1 == ParaNum)
			continue;
        		ParaNum--;//count of units in buffer
			parsing(parameters,ParaNum,&info);
			if(builtin_command(command,parameters))
				continue;
        	if(info.flag & IS_PIPED) //command2 is not null
        	{                
            		if(pipe(pipe_i_o)<0)
            		{
                		printf("Wshell error:pipe failed.\n");
                		exit(0);
			}
        	}  
		if((ChdPid = fork())!=0) //wshell
        	{
			if(info.flag & IS_PIPED)
            		{
                		if(fork() == 0) //command2
				{
                    			close(pipe_i_o[0]);
                    			close(fileno(stdout)); 
                    			dup2(pipe_i_o[1], fileno(stdout));
                    			close(pipe_i_o[1]);
					execvp(info.command2,info.parameters2);
				}
				else
				{
                    			close(pipe_i_o[0]);
                    			close(pipe_i_o[1]);
                    			waitpid(-1,&status,0); //wait command2
                		}
            		}
      		
			if(info.flag & BACKGROUND)
            			printf("Child pid:%u\n",ChdPid);
            		else
           		{          
            			waitpid(-1,&status,0);//wait command1
            		} 
        	}
		else //command1
        	{				
			if(info.flag & IS_PIPED) //command2 is not null
            		{                
                		if(!(info.flag & OUT_REDIRECT) && !(info.flag & OUT_REDIRECT_APPEND)) // ONLY PIPED
				{
					close(pipe_i_o[1]);
					close(fileno(stdin)); 
					dup2(pipe_i_o[0], fileno(stdin));
					close(pipe_i_o[0]); 
                		}
                		else //OUT_REDIRECT and PIPED
				{
                    			close(pipe_i_o[0]);
                    			close(pipe_i_o[1]);//send a EOF to command2
                    			if(info.flag & OUT_REDIRECT)
					{
    		            			out = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
					}
                    			else
					{
    		            			out = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    				close(fileno(stdout)); 
                    				dup2(out, fileno(stdout));
                    				close(out);
					}
				}
            		}
            		else
            		{
                		if(info.flag & OUT_REDIRECT) // OUT_REDIRECT WITHOUT PIPE
				{
		        		out = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                    			close(fileno(stdout)); 
                    			dup2(out, fileno(stdout));
                    			close(out);
                		}	
                		if(info.flag & OUT_REDIRECT_APPEND) // OUT_REDIRECT_APPEND WITHOUT PIPE
				{
		        		out = open(info.out_file, O_WRONLY|O_APPEND|O_TRUNC, 0666);
                    			close(fileno(stdout)); 
                    			dup2(out, fileno(stdout));
                    			close(out);
               			}
            		}            
			if(info.flag & IN_REDIRECT)
            		{
				in_fd = open(info.in_file, O_CREAT |O_RDONLY, 0666);
                		close(fileno(stdin)); 
                		dup2(in_fd, fileno(stdin));
                		close(in_fd);
            		} 
            		execvp(command,parameters);
        	}
	}
	free(paths);
	free(parameters);
	free(buffer);
}

int main() {
    proc();
    return 0;
}
