#include "shell.h"
#ifdef READLINE_ON
#include <readline/readline.h>
#include <readline/history.h>
#endif

//return value: number of parameters
//0 represents only command without any parameters
//-1 represents wrong input
int read_command(char **command,char **parameters,char *prompt)
{
#ifdef READLINE_ON
	buffer  = readline(prompt);
	if(feof(stdin) == 0)
	{
		printf("\n");
		exit(0);
	}
#else
	printf("%s",prompt);
    	char* Res_fgets = fgets(buffer,MAXLINE,stdin);
	if(Res_fgets == NULL)
	{
		printf("\n");
		exit(0);
	}		
#endif
    if(buffer[0] == '\0')
        return -1;
    char *pStart,*pEnd;
    int count = 0;
    int isFinished = 0;
    pStart = pEnd = buffer;
    while(isFinished == 0)
    {
        while((*pEnd == ' ' && *pStart == ' ') || (*pEnd == '\t' && *pStart == '\t'))
        {
            pStart++;
            pEnd++;
        }

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            if(count == 0)
                return -1;
            break;
        }

        while(*pEnd != ' ' && *pEnd != '\0' && *pEnd != '\n')
            pEnd++;


        if(count == 0)
        {
            char *p = pEnd;
            *command = pStart;
            while(p!=pStart && *p !='/')
                p--;
            if(*p == '/')
                p++;
            //else //p==pStart
            parameters[0] = p;
            count += 2;
#ifdef DEBUG
            printf("\ncommand:  %s\n",*command);
#endif
        }
        else if(count <= MAXARG)
        {
            parameters[count-1] = pStart;
            count++;
        }
        else
        {
            break;
        }

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            *pEnd = '\0';
            isFinished = 1;
        }
        else
        {
            *pEnd = '\0';
            pEnd++;
			pStart = pEnd;
        }
    }

    /*printf("Display input:\n");
    printf("command:%s\nparameters:\n",*command);
    int i;
    for(i=0;i<count-1;i++)
        printf("paramerters[%d]: %s\n", i, parameters[i]);
    parameters[count-1] = NULL;*/
    return count;
}
