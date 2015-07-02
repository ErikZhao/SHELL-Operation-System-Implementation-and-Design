//#include "wshell.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

int number();

void calculator()
{

	int num1;
	int num2;
	int length,v2_length;
	char str[20]={' '};
	char v1[10]={' '};
	char v2[10]={' '};
	double result=0;
	char op;
	char wt='$';
	char exitcal='n';
	char *oper;
	//jmp_buf buff;
	char temp[10]={' '};
	 //do{
    //setjmp(buff);
	printf("insert\n");
	fgets(str,20,stdin);
	length=strlen(str);
    printf("length:");
	printf("%d\n",length);
	puts(str);
	if(str[0]==wt)
    {

        printf("this is write mode\n");
        for(int i=0;i<length-1;i++)
        {
            temp[i]=str[i+1];
        }
        oper=strchr(temp,'=');
        //get variable name
        for(int i=0;i<(oper-temp);i++)
        {
            v1[i]=(temp[i]);
        }
        //get variable value

        for(int i=(oper-temp)+1,j=0;i<length-2;i++)
        {
            v2[j++]=(temp[i]);
        }
        v2_length=strlen(v2);
        if(v2_length>=9)
        {
            printf("too long!!");
        }else{
        FILE *fp;
        fp=fopen("calculator.txt","a");
        fprintf(fp,"%s %s \n",v1,v2);
        fclose(fp);
        printf("finish writing!\n");
        exit(0);
        //break;
        }
    }else{
	oper=strchr(str,'=');
	if(oper==NULL)
    {
        oper=strchr(str,'+');
        if(oper==NULL)
        {
            oper=strchr(str,'-');
            if(oper==NULL)
            {
                oper=strchr(str,'*');
                if(oper==NULL)
                {
                    oper=strchr(str,'/');
                }
            }
        }
    }
	for(int i=0;i<(oper-str);i++)
    {
        v1[i]=(str[i]);
    }
    for(int i=(oper-str)+1,j=0;i<length-1;i++)
    {
        v2[j++]=(str[i]);
    }
    op=str[(oper-str)];
    //printf("first number is ");
    //puts(v1);
    //printf("second number is ");
    //puts(v2);
    //puts(&op);
    printf("unknown");
    num1=number(v1);
    num2=number(v2);
    if(num1==99999998||num2==99999998)
    {
        printf("variable dose not exist!\n");
    }else{

	//scanf("%d%c%d",&num1,&op,&num2);
	switch(op)
	{
	case '+':
		result=num1+num2;
		break;
	case '-':
		result=num1-num2;
		break;
	case '*':
		result=num1*num2;
		break;
	case '/':
	    if(num2==0)
        {
            printf("error!\n");
        }else{
		result=num1/num2;
        }
		break;
	default:
		break;
    }
	//printf("%d%c%d\n",num1,op,num2);
	printf("the result is %f\n",result);
	//printf("exit?(y/n)");
	//getchar();
	//scanf("%c",&exitcal);
	//printf("%c",exitcal);
	}//end of $if
	//}while(exitcal=='n');
	 }
}


int number(char *v)
{
    puts(v);
    putchar('.');
    int i=0;
    int result=99999998,value=0;
    char a[20]={' '};//record
    //char *oper;
    //char vari[20]={' '};//variable name
    //int length;//record lenght
    //char actualvalue[20]={' '};//value
    if(isdigit(v[0]))
    {
        result=atoi(v);
        return result;
    }
    FILE *fp=fopen("calculator.txt","r");
    //rewind(fp);
    //while(fscanf(fp,"%s %d",temp,value)!=EOF)
    while(!feof(fp))
    {
        fscanf(fp,"%s",a);
        fscanf(fp,"%d",&value);
        /*oper=strchr(a,'=');
        length=strlen(a);
        for(int i=0;i<(oper-a);i++)
        {
            vari[i]=(a[i]);//get variable name
        }*/
        if(strcmp (v,a) == 0) //compare strings
        {
            /*for(int i=(oper-a)+1,j=0;i<length;i++)
            {
                actualvalue[j++]=(a[i]);
            }*/
            //get actual value
            //printf("\n actual value:");
            //puts(actualvalue);
            //result=atoi(actualvalue);
            //fclose(fp);
            result=value;
        }
        //printf("\n v:");
        //puts(v);
        //printf("\n vari:");
        //puts(a);
        //puts(vari);
    }
    fclose(fp);
    return result;
}


int main(){
    while(1){
	calculator();
    }
	return 0;
}
