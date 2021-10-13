#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define	norw 9			//norw-1个关键字
#define	al 20			//最长的关键字的长度

#define ID 10			//标识符的词法记号
#define inttoken 11		//整数的词法记号
#define comma 12		//逗号的词法记号
#define lparen 13		//左括号的语法记号
#define rparen 14		//右括号的语法记号
#define lbrace 15		//左大括号的语法记号
#define rbrace 16		//右大括号的语法记号
#define becomes 17		//赋值号的语法记号
#define addtoken 18		//加号的语法记号
#define multoken 19		//乘号的语法记号
#define ltctoken 20		//小于的语法记号
#define gtctoken 21		//大于的语法记号
#define eqctoken 22		//等号的语法记号
#define semicolon 23	//分号的语法记号
#define minustoken 24 	//减号的语法记号

#define zhengshu 28	//整数的语法标记
#define xiaoshu 29		//浮点数的语法标记
#define zifu 30		//字符常量的语法标记

using namespace std;

char TOKEN[20];		//字符数组用来依次存放单词词文的各个字符

extern int lookup(char *);	//以TOKEN字符串查保留字表
extern void rreport_error(char);	//报告程序中的词法错误
bool isalpha(char);		//判断接收字符是否为字母
bool isalnum(char);		//判断接收字符是否为字母或者数字
bool isdigit(char);		//判断接收字符是否为数字
bool istermial(char); 		//判断接受字符是否是终结符（运算符或界符）

FILE* fin;
FILE* fout;

void scanner()
{//词法分析的主体程序，对输入的文本文件进行词法分析
	int ch;
	int i,c;
	int error=0;                         //记录文件中词法错误的个数
	ch=fgetc(fin);                        //从输入文件中读取一个字符

	while(ch!=EOF)				//当从输入文件接收的字符不是文件结束符时，执行循环
	{
		if(isalpha(ch))			//如果从输入文件接收的第一个字符是字母（标识符[关键字]）
		{
			int flag=0;
			TOKEN[0]=ch;
			ch=fgetc(fin);i=1;
			while(!istermial(ch))
			{
				if(!isalnum(ch)) flag=1;
				TOKEN[i]=ch;i++;
				ch=fgetc(fin);		
			}
			TOKEN[i]='\0';
			
			if(flag) 
			{
				fseek(fin,-1,1);
				error++;printf("%s is error\n",TOKEN);
			}
			else 
			{
				fseek(fin,-1,1);
				c=lookup(TOKEN);                   //查保留字表
				if(c==0) {fprintf(fout,"(%d,%s)\n", ID,TOKEN);} //输出标识符
				else fprintf(fout,"(%d,%s)\n", c,TOKEN);//输出接收单词为保留字
			}           
		}
		else if(isdigit(ch))	//如果从输入文件接收的第一个字符是数字（整数）
		{
			i=0;
			while(isdigit(ch))
			{
				TOKEN[i]=ch;i++;
				ch=fgetc(fin);
			}
			if(ch=='.')
			{
				TOKEN[i]='.';i++;
				ch=fgetc(fin);
				while(isdigit(ch))
				{
					TOKEN[i]=ch;i++;
					ch=fgetc(fin);
				}
				if(istermial(ch))
				{
					TOKEN[i]='\0';
					fseek(fin,-1,1);
					fprintf(fout,"(%d,%s)\n", xiaoshu, TOKEN);
				}
				else
				{
					while(!istermial(ch))
					{
						TOKEN[i]=ch;i++;
						ch=fgetc(fin);
					}
					TOKEN[i]='\0';
					fseek(fin,-1,1);
					error++;printf("%s is error\n",TOKEN);
				}
				
			}
			else if(istermial(ch)) 
			{
				TOKEN[i]='\0';
				fseek(fin,-1,1);
				fprintf(fout,"(%d,%s)\n", zhengshu, TOKEN);
			}
			else
			{
				while(!istermial(ch))
				{
					TOKEN[i]=ch;i++;
					ch=fgetc(fin);
				}
				TOKEN[i]='\0';
				fseek(fin,-1,1);				
				error++;printf("%s is error\n",TOKEN);
			}
			
		}
		else            //如果从输入文件接收的第一个字符既不是字母又不是数字
		switch(ch)
		{//将所接收到的符号字符进行分类，采取一符一类
			case '=':ch=fgetc(fin);
		        if(ch=='=') fprintf(fout,"(%d,==)\n", becomes);	//输出接收符号为赋值号
				else
				{
					fseek(fin,-1,1);		//文件接收字符回推一个字符
					fprintf(fout,"(%d,'=')\n", eqctoken);
				}
				break;
			case '%':ch=fgetc(fin);
		        if(ch=='%') //输出接收符号为单行注释
				{
					ch=fgetc(fin);
					while(ch!='\n')
					{
						ch=fgetc(fin);
					}
				}	
				else
				{
					i=0;TOKEN[i++]='%';
					while(!istermial(ch))
					{
						TOKEN[i++]=ch;
						ch=fgetc(fin);
					}
					TOKEN[i]='\0';
					fseek(fin,-1,1);	//文件接收字符回推一个字符
					error++;printf("%s is error\n", TOKEN);
				}
				break;
			case '/':ch=fgetc(fin);	//多行注释，若读到末尾都没有就报错
		        if(ch=='*')
				{
					ch=fgetc(fin);
					while(ch!=EOF)
					{
						if(ch=='*') 
						{
							ch=fgetc(fin);
							if(ch=='/') break;
						}
						ch=fgetc(fin);
					}
					if(ch==EOF) {printf("多行注释没有闭合\n");error++;}
				}
				else
				{		
					i=0;TOKEN[i++]='/';
					while(!istermial(ch))
					{
						TOKEN[i++]=ch;
						ch=fgetc(fin);
					}
					TOKEN[i]='\0';
					fseek(fin,-1,1);
					error++;printf("%s is error\n", TOKEN);
				}
				break;
			case '\'' :ch=fgetc(fin);
				if(isalnum(ch))
				{
					char tmp=ch;
					ch=fgetc(fin);
					if(ch=='\'')
						fprintf(fout,"(%d,%c)\n", zifu,tmp);
					else
					{
						i=0;
						TOKEN[i++]='\'';
						TOKEN[i++]=tmp;
						while(!istermial(ch))
						{
							TOKEN[i++]=ch;
							ch=fgetc(fin);
						}
						TOKEN[i]='\0';
						fseek(fin,-1,1);
						error++;printf("%s is error\n", TOKEN);
					}
				}
				else
				{
					i=0;TOKEN[i++]='\'';
					while(!istermial(ch))
					{
						TOKEN[i++]=ch;
						ch=fgetc(fin);
					}
					TOKEN[i]='\0';
					fseek(fin,-1,1);
					error++;printf("%s is error\n", TOKEN);
				}
				break;
			case ',':fprintf(fout,"(%d,',')\n", comma); break;
			case ';':fprintf(fout,"(%d,';')\n", semicolon);break;
			case '(':fprintf(fout,"(%d,'(')\n", lparen);break;
			case ')':fprintf(fout,"(%d,')')\n", rparen);break;
			case '{':fprintf(fout,"(%d,'{')\n", lbrace);break;
			case '}':fprintf(fout,"(%d,'}')\n", rbrace);break;
			case '<':fprintf(fout,"(%d,'<')\n", ltctoken);break;
			case '>':fprintf(fout,"(%d,'>')\n", gtctoken);break;
			case '+':fprintf(fout,"(%d,'+')\n", addtoken);break;
			case '*':fprintf(fout,"(%d,'*')\n", multoken);break;
			case '-':fprintf(fout,"(%d,'-')\n", minustoken);break;
			case ' ':break;
			case '\n':break;
			case '\t':break;
	        default:
				i=0;TOKEN[i++]=ch;
				ch=fgetc(fin);
				while(!istermial(ch))
				{
					TOKEN[i++]=ch;
					ch=fgetc(fin);
				}
				TOKEN[i]='\0';
				fseek(fin,-1,1);
				error++;printf("%s is error\n", TOKEN);		//接收非上述字符程序报告词法错误
				break;
		}
		ch=fgetc(fin);		//继续从文件中读取下一个单词，直到文件结束
	}//while循环结束

	printf("共发现%d 个词法错误!",error);
	return;
}

int lookup(char *token)
{
	int j;
    char word[norw][al];
	strcpy(&(word[1][0]), "int");
	strcpy(&(word[2][0]), "char");
	strcpy(&(word[3][0]), "bool");
	strcpy(&(word[4][0]), "if");
	strcpy(&(word[5][0]), "then");
	strcpy(&(word[6][0]), "else");
	strcpy(&(word[7][0]), "true");
	strcpy(&(word[8][0]), "false");

	for(j=1;j<=norw-1;j++)
		if(strcmp(token,word[j])==0) return j;   //以TOKEN字符串查保留字表，若查到返回保留字类别码
	return 0;                                              //TOKEN不是保留字，返回0
}

bool isalpha(char c)
{ //判断接收字符是否为字母
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))return 1;
	else return 0;
}

bool isalnum(char c)
{//判断接收字符是否为字母或者数字
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))return 1;
	else return 0;
}

bool isdigit(char c)
{//判断接收字符是否为数字
	if(c>='0'&&c<='9')return 1;
	else return 0;
}

bool istermial(char c)
{
	if(c==' '||c=='\n'||c=='\t'||c=='/'||c=='*'||c=='-'||c=='+'||c=='>'||c=='<'||c=='='||c==';'||c==','||c=='('||c==')'||c=='{'||c=='}'||c=='%'/* | c==':' */)
		return true;
	else return false;
}

int main()
{
	char filename[20];
	printf("请输入文件名:");
	scanf("%s",filename);
	if((fin=fopen(filename,"r"))==NULL) //打开要读取的文本文件
	{
		printf("不能打开文件.\n");
		exit(0);
	}

	printf("请输入保存分析结果的文件名:");
	scanf("%s",filename);
	if((fout=fopen(filename,"w"))==NULL)
	{
		printf("不能打开文件.\n");
		exit(0);
	}
	/* fin=fopen("t.txt","r");
	fout=fopen("c.txt","w"); */
	scanner();    //调用词法分析程序
	getchar();
	getchar();

	fclose(fin);
	fclose(fout);
	return 0;
}