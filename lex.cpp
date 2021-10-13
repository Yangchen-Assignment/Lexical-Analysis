#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define	norw 9			//norw-1���ؼ���
#define	al 20			//��Ĺؼ��ֵĳ���

#define ID 10			//��ʶ���Ĵʷ��Ǻ�
#define inttoken 11		//�����Ĵʷ��Ǻ�
#define comma 12		//���ŵĴʷ��Ǻ�
#define lparen 13		//�����ŵ��﷨�Ǻ�
#define rparen 14		//�����ŵ��﷨�Ǻ�
#define lbrace 15		//������ŵ��﷨�Ǻ�
#define rbrace 16		//�Ҵ����ŵ��﷨�Ǻ�
#define becomes 17		//��ֵ�ŵ��﷨�Ǻ�
#define addtoken 18		//�Ӻŵ��﷨�Ǻ�
#define multoken 19		//�˺ŵ��﷨�Ǻ�
#define ltctoken 20		//С�ڵ��﷨�Ǻ�
#define gtctoken 21		//���ڵ��﷨�Ǻ�
#define eqctoken 22		//�Ⱥŵ��﷨�Ǻ�
#define semicolon 23	//�ֺŵ��﷨�Ǻ�
#define minustoken 24 	//���ŵ��﷨�Ǻ�

#define zhengshu 28	//�������﷨���
#define xiaoshu 29		//���������﷨���
#define zifu 30		//�ַ��������﷨���

using namespace std;

char TOKEN[20];		//�ַ������������δ�ŵ��ʴ��ĵĸ����ַ�

extern int lookup(char *);	//��TOKEN�ַ����鱣���ֱ�
extern void rreport_error(char);	//��������еĴʷ�����
bool isalpha(char);		//�жϽ����ַ��Ƿ�Ϊ��ĸ
bool isalnum(char);		//�жϽ����ַ��Ƿ�Ϊ��ĸ��������
bool isdigit(char);		//�жϽ����ַ��Ƿ�Ϊ����
bool istermial(char); 		//�жϽ����ַ��Ƿ����ս���������������

FILE* fin;
FILE* fout;

void scanner()
{//�ʷ�������������򣬶�������ı��ļ����дʷ�����
	int ch;
	int i,c;
	int error=0;                         //��¼�ļ��дʷ�����ĸ���
	ch=fgetc(fin);                        //�������ļ��ж�ȡһ���ַ�

	while(ch!=EOF)				//���������ļ����յ��ַ������ļ�������ʱ��ִ��ѭ��
	{
		if(isalpha(ch))			//����������ļ����յĵ�һ���ַ�����ĸ����ʶ��[�ؼ���]��
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
				c=lookup(TOKEN);                   //�鱣���ֱ�
				if(c==0) {fprintf(fout,"(%d,%s)\n", ID,TOKEN);} //�����ʶ��
				else fprintf(fout,"(%d,%s)\n", c,TOKEN);//������յ���Ϊ������
			}           
		}
		else if(isdigit(ch))	//����������ļ����յĵ�һ���ַ������֣�������
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
		else            //����������ļ����յĵ�һ���ַ��Ȳ�����ĸ�ֲ�������
		switch(ch)
		{//�������յ��ķ����ַ����з��࣬��ȡһ��һ��
			case '=':ch=fgetc(fin);
		        if(ch=='=') fprintf(fout,"(%d,==)\n", becomes);	//������շ���Ϊ��ֵ��
				else
				{
					fseek(fin,-1,1);		//�ļ������ַ�����һ���ַ�
					fprintf(fout,"(%d,'=')\n", eqctoken);
				}
				break;
			case '%':ch=fgetc(fin);
		        if(ch=='%') //������շ���Ϊ����ע��
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
					fseek(fin,-1,1);	//�ļ������ַ�����һ���ַ�
					error++;printf("%s is error\n", TOKEN);
				}
				break;
			case '/':ch=fgetc(fin);	//����ע�ͣ�������ĩβ��û�оͱ���
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
					if(ch==EOF) {printf("����ע��û�бպ�\n");error++;}
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
				error++;printf("%s is error\n", TOKEN);		//���շ������ַ����򱨸�ʷ�����
				break;
		}
		ch=fgetc(fin);		//�������ļ��ж�ȡ��һ�����ʣ�ֱ���ļ�����
	}//whileѭ������

	printf("������%d ���ʷ�����!",error);
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
		if(strcmp(token,word[j])==0) return j;   //��TOKEN�ַ����鱣���ֱ����鵽���ر����������
	return 0;                                              //TOKEN���Ǳ����֣�����0
}

bool isalpha(char c)
{ //�жϽ����ַ��Ƿ�Ϊ��ĸ
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))return 1;
	else return 0;
}

bool isalnum(char c)
{//�жϽ����ַ��Ƿ�Ϊ��ĸ��������
	if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))return 1;
	else return 0;
}

bool isdigit(char c)
{//�жϽ����ַ��Ƿ�Ϊ����
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
	printf("�������ļ���:");
	scanf("%s",filename);
	if((fin=fopen(filename,"r"))==NULL) //��Ҫ��ȡ���ı��ļ�
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}

	printf("�����뱣�����������ļ���:");
	scanf("%s",filename);
	if((fout=fopen(filename,"w"))==NULL)
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}
	/* fin=fopen("t.txt","r");
	fout=fopen("c.txt","w"); */
	scanner();    //���ôʷ���������
	getchar();
	getchar();

	fclose(fin);
	fclose(fout);
	return 0;
}