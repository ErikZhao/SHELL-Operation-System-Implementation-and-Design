#include "shell.h"

int parse_info_init(struct parse_info *info)
{
    info->flag = 0;
    info->in_file = NULL;
    info->out_file = NULL;
    info->command2 = NULL;
    info->parameters2 = NULL;
    return 0;
}

int parsing(char **parameters,int ParaNum,struct parse_info *info)
{
    int i;
    parse_info_init(info);
    if(strcmp(parameters[ParaNum-1],"&") ==0)
    {
        info->flag |= BACKGROUND;
        parameters[ParaNum-1] = NULL;
        ParaNum--;
    }
    
    /*if(strcmp(parameters[0],"$") ==0)
    {	
        info->command2 = parameters[1];
	info->parameters2 = &parameters[1];
	for(i=1;i<ParaNum;)
	{
		if(strcmp(parameters[i],"$")==0)
		{
			parameters[0]=parameters[i+1];
			parameters[1]=parameters[i+2];
			parameters[2]=parameters[i+3];
			parameters[3]=NULL;
			parameters[4]=NULL;
			parameters[5]=NULL;

		}
	}			

    }*/

    for(i=0;i<ParaNum;)
    {
        if(strcmp(parameters[i],"<<")==0 || strcmp(parameters[i],"<")==0)
        {
            info->flag |= IN_REDIRECT;
            info->in_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        else if(strcmp(parameters[i],"=>")==0)
        {
            info->flag |= OUT_REDIRECT;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        else if(strcmp(parameters[i],">>")==0)
        {
            info->flag |= OUT_REDIRECT_APPEND;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            i+=2;
        }
        else if(strcmp(parameters[i],"|")==0)
        {
            char* pCh;
			info->flag |= IS_PIPED;
            parameters[i] = NULL;
            info->command2 = parameters[i+1];
            info->parameters2 = &parameters[i+1];
            for(pCh = info->parameters2[0]+strlen(info->parameters2[0]);
                    pCh!=&(info->parameters2[0][0]) && *pCh!='/';pCh--)
                ;
            if(*pCh == '/')
                pCh++;
	    printf("%s",pCh);
            info->parameters2[0] = pCh;
            break;
        }

        else if(strcmp(parameters[i],"$")==0)
        {
            char* pCh;
			info->flag |= IS_PIPED;
            parameters[i] = NULL;
            info->command2 = parameters[i+1];
            info->parameters2 = &parameters[i+1];
            for(pCh = info->parameters2[0]+strlen(info->parameters2[0]);
                    pCh!=&(info->parameters2[0][0]) && *pCh!='/';pCh--)
                ;
            if(*pCh == '/')
                pCh++;
	    printf("%s",pCh);
            info->parameters2[0] = pCh;
            break;
        }


        else
            i++;
    }
	return 1;
}
