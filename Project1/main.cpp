#include"stdio.h"
#include"stdlib.h" 
#include"string.h"
#include"math.h"

#define   TRUE	          1
#define   FALSE            0
#define   OK	              1
#define   ERROR		      0
#define   INFEASIBLE	     -1

typedef  int   Status;
typedef  char  SElemType;              //元素类型

#define INITSIZE 20
#define INCREMENT 10
#define MAX 100

/*栈的动态分配存储结构*/
#define  STACK_INIT_SIZE     100   //顺序栈存储空间的初始分配量
#define  STACKINCREMENT      10    //顺序栈存储空间的分配增量

typedef  struct
{
    SElemType* base;    //栈底指针  
    SElemType* top;     //栈顶指针
    int  stacksize;      //当前分配的存储容量
}SqStack;

//函数声明 
Status InitStack(SqStack& S);
Status Push(SqStack& S, SElemType e);
int StackLength(SqStack* S);
Status GetTop(SqStack S, SElemType& e);
Status Pop(SqStack& S, SElemType& e);
Status Check(SElemType str);
Status Change(SqStack& S, SElemType str[]);

//初始化栈
Status InitStack(SqStack& S)
{
    S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType)); //判断申请是否成功

    if (!S.base)
        exit(OVERFLOW);
    S.top = S.base; //设置初始栈顶指针 
    S.stacksize = STACK_INIT_SIZE; //设置顺序栈容量 
    return OK;
}

//顺序入栈操作
Status Push(SqStack& S, SElemType e)
{
    if (S.top - S.base >= S.stacksize) //若栈满，追加存储空间 
    {
        S.base = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S.base)
            exit(OVERFLOW); //判断是否成功 
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT; //设置新的栈顶指针、栈容量 
    }
    *S.top++ = e; //入栈 
    return OK;
}

//取栈顶元素
Status GetTop(SqStack S, SElemType& e)
{
    if (S.top == S.base) //判断是否栈空 
        return ERROR;
    e = *(S.top - 1); //取栈顶元素
    return OK;
}

//求栈长
int StackLength(SqStack S)
{
    return (S.top - S.base); //栈顶指针-栈底指针 
}

//顺序出栈操作
Status Pop(SqStack& S, SElemType& e)
{
    if (S.top == S.base) //判断是否栈空 
        return ERROR;
    e = *--S.top; //出栈栈顶元素 
    return OK;
}

//检查是否输入错误
Status Check(SElemType str[])
{
    int i, count1 = 0, count2 = 0;
    for (i = 0;i < strlen(str);i++)
    {
        //只允许小写字符和加减乘除符号允许输入 
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '(' || str[i] == ')')))
        {
            printf("\n输入格式错误！\n");
            printf("请重新输入！\n");
            return(ERROR);
        }
    }
    for (i = 0;i < strlen(str);i++)
    {
        //不允许连续两个符号输入 
        if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
        {
            if (str[i + 1] == '+' || str[i + 1] == '-' || str[i + 1] == '*' || str[i + 1] == '/')
            {
                printf("\n输入格式错误！\n");
                printf("请重新输入！\n");
                return(ERROR);
            }
            i++;
        }
        else
            i++;
    }
    //不允许最后一个字符是符号 
    if (str[strlen(str) - 1] == '+' || str[strlen(str) - 1] == '-' || str[strlen(str) - 1] == '*' || str[strlen(str) - 1] == '/')
    {
        printf("\n输入格式错误！\n");
        printf("请重新输入！\n");
        return(ERROR);
    }
    //判断左右括号数量是否相等，不相等则输入错误 
    for (i = 0;i < strlen(str);i++)
    {
        if (str[i] == '(')
            count1++;
        if (str[i] == ')')
            count2++;
    }
    if (count1 != count2)
    {
        printf("\n输入格式错误！\n");
        printf("请重新输入！\n");
        return(ERROR);
    }
    //判断是否有括号内为空的括号对出现，若出现则错误 
    for (i = 0;i < strlen(str);i++)
    {
        if (str[i] == '(' && str[i + 1] == ')')
        {
            printf("\n输入格式错误！\n");
            printf("请重新输入！\n");
            return(ERROR);
        }
    }
    //判断是否有符号直接加右括号的错误情况 
    for (i = 0;i < strlen(str);i++)
    {
        if ((str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') && (str[i + 1] == ')'))
        {
            printf("\n输入格式错误！\n");
            printf("请重新输入！\n");
            return(ERROR);
        }
    }
    return(OK);
}


//中缀转后缀函数
Status Change(SqStack& S, SElemType str[])
{
    int i = 0;
    SElemType e, t;
    InitStack(S);
    while (str[i] != '\0')
    {
        while ((str[i] >= 'a' && str[i] <= 'z'))
        {
            printf("%c", str[i++]); //字符串小写字符直接输出，直到下一位不是小写字符跳出循环 
        }
        if (str[i] == '+' || str[i] == '-')
        {
            if (!StackLength(S))
            {
                Push(S, str[i]); //加减运算符优先级最低，如果栈顶元素为空则直接入栈
            }
            else
            {
                do
                {
                    Pop(S, e);
                    if (e == '(')
                    {
                        Push(S, e); //如果不为空栈将栈中存储的运算符全部出栈 
                    }
                    else
                    {
                        printf("%c", e);
                    }
                } while (StackLength(S) && e != '('); //如果碰到左括号停止出栈 

                Push(S, str[i]); //将刚出栈的左括号重新入栈，
            }
        }
        else if (str[i] == ')') //当遇到右括号，把栈中左括号之上的为剩余的运算符出栈
        {
            Pop(S, e);
            while (e != '(')
            {
                printf("%c", e);
                Pop(S, e); //左括号只出栈不打印 
            }
        }

        else if (str[i] == '*' || str[i] == '/') //乘、除、左括号都是优先级高的，直接入栈 
        {
            GetTop(S, t); //取栈顶元素
            if (t == '*' || t == '/') //如果碰到两个乘除，则先出栈一个符号，再入栈
            {
                Pop(S, e);
                printf("%c", e);
                Push(S, str[i]);
            }
            else
                Push(S, str[i]);
        }

        else if (str[i] == '(') //碰到左括号，优先级最高，直接压栈 
        {
            Push(S, str[i]);
        }
        else if (str[i] == '\0') //扫描结束，跳出循环 
        {
            break;
        }
        i++;
    }

    while (StackLength(S))
    {
        Pop(S, e);
        printf("%c", e);  //最后把栈中剩余的运算符依次出栈打印
    }
    printf("\n");
    return(OK);
}

int main()
{
    SElemType str[MAX];
    SqStack S;
    while (1)
    {
        printf("请输入想转换的表达式(仅限小写字符和英文括号)\n");
        printf("如果想退出程序，请输入0!\n");
        gets(str);
        if (str[0] == '0')
            break;
        if (Check(str) == OK)
            Change(S, str);

        system("pause");
        system("cls");
    }
    return 0;
}
