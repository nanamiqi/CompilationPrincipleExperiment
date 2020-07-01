#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/**
 *  ******实验二  语法分析******
 *  A simple Syntax Analyzer for Arithmetic expression
 *  written by xizijun, @ 2020/5/30
 *  all rights reserved.
 *
 */
const char *FILENAME = "expr.txt"; /// 算术表达式样例

#define ADVANCE() (idx<CNT-1? ++idx : threshold=1)
int threshold = 0;
int ERROR = 0, CNT = 0;
int SYM[100];
int idx = 0;

/** ------------------------------------词法分析器--------------------------------------*/
struct Word {
    int num; ///词所属种类
    char w[10]; ///词的属性值
};

char KEYWORDS[28][10]= {
    "begin","end","if","then","while","do","const","var","call","procedure",
    "+","-","*","/","odd","=", "<>", "<",">","<=",">=",":=","(",")",",",".",";","#"
};

bool IsIDLegal(char* c) { ///判断标识符是否合法
    if(strlen(c)>10)
        return false;
    for(int i=0; i<strlen(c); i++) {
        if(!isalpha(c[i]))
            return false;
    }
    return true;
}

bool IsNumLegal(char* num) { ///判断常数是否合法
    for(int i=0; i<strlen(num); i++) {
        if(!isdigit(num[i]))
            return false;
    }
    return true;
}

void PL0SampleCodeShow() { ///读取并显示PL0示例程序文件
    FILE *fp;
    char ch;
    fp=fopen(FILENAME, "r");
    if(!fp) {
        printf("无法打开文件: %s !\n", FILENAME);
        exit(1);
    }
    printf("算术表达式如下:\n");
    while((ch=fgetc(fp))!=EOF) {
        putchar(ch);
    }
    fclose(fp);
}

void WordsClassTable() { ///单词种类表分类说明
    printf("\n词法分析后, 它所输出的单词符号是:\n");
}

void LexicalAnalysis() { ///词法分析
    char ch, word_temp[20]="";  ///缓存区
    int i=0, j=0, k=0, key=0, flag=0;
    FILE *fp; ///创建文件
    Word word[100]; ///创建词对象
    PL0SampleCodeShow(); ///PL0示例程序展示
    fp=fopen(FILENAME, "r"); ///打开文件
    while((ch=fgetc(fp)) != EOF) { ///独立子程序，每次只读取一个单词，直至文件末尾
        if(isalnum(ch)) { ///判断字母、数字
            word_temp[key++] = ch;  ///添加字母、数字序列
            word_temp[key] = '\0';
            continue;
        } else {
            if(strcmp(word_temp, "")!=0) {
                strcpy(word[i].w, word_temp); ///将字母、数字序列拷贝到结构数组中
                strcpy(word_temp, ""); ///临时数组置空
                key=0; ///回到临时数组的开始位置
                i++; ///结构数组的下标加1
            }
            if(ch==' '|| ch==10 || ch==13 || ch=='	') ///去掉空格、回车、换行和制表符
                continue;
            else { ///判断特殊符号组合、算符/界符
                word_temp[0] = ch, flag = 0;
                if(word_temp[0]==':' || word_temp[0]=='>'|| word_temp[0]=='<')
                    ch=fgetc(fp), flag = 1;
                if((ch=='=' && flag) || (word_temp[0]=='<' && ch=='>'))
                    word_temp[1]=ch, word_temp[2]='\0';
                else
                    word_temp[1]='\0';
                strcpy(word[i].w, word_temp); ///将非字母、数字符号序列拷贝到结构数组中
                strcpy(word_temp, "");
                key=0, i++;
            }
        }
    }
    ///分类别
    for(j=0; j<i; j++) {
        for(k=0; k<28; k++) {
            if((strcmp(word[j].w, KEYWORDS[k]))==0) {
                if(k>=0 && k<10)
                    word[j].num=1; ///关键字
                else if(k>=10 && k<28)
                    word[j].num=k; ///算符/界符
                break;
            } else if(IsIDLegal(word[j].w))
                word[j].num=2; ///标识符
            else if(IsNumLegal(word[j].w))
                word[j].num=3; ///常数
            else
                word[j].num=0; ///无法识别
        }
    }
    WordsClassTable(); ///打印单词种类表
    CNT = i;
    for(j=0; j<i; j++) { ///格式化打印输出, 即(种类, 属性值)
        SYM[j] = word[j].num>=10? word[j].num-6:word[j].num; //将单词种类存进SYM数组
        if(word[j].num == 1)
            printf("(%d,)\t\t%s%s\n", word[j].num, "关键字", word[j].w);
        else if(word[j].num == 2)
            printf("(%d,\'%s\')\t\t%s%s\n", word[j].num, word[j].w, "标识符", word[j].w);
        else if(word[j].num == 3)
            printf("(%d,\'%s\')\t\t%s%s\n", word[j].num, word[j].w, "常数", word[j].w);
        else if(word[j].num == 0)
            printf("(%d,)\t\t%s%s\n", word[j].num, "ERROR: ", word[j].w);
        else {
            if(!(strcmp("+", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "加号", word[j].w);
            else if(!(strcmp("-", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "减号", word[j].w);
            else if(!(strcmp("*", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "乘号", word[j].w);
            else if(!(strcmp("/", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "除号", word[j].w);
            else if(!(strcmp("odd", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "奇数号", word[j].w);
            else if(!(strcmp("=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "等于号", word[j].w);
            else if(!(strcmp("<>", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "不等于号", word[j].w);
            else if(!(strcmp("<", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "小于号", word[j].w);
            else if(!(strcmp(">", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "大于号", word[j].w);
            else if(!(strcmp("<=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "小于等于号", word[j].w);
            else if(!(strcmp(">=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "大于等于号", word[j].w);
            else if(!(strcmp(":=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "赋值号", word[j].w);
            else if(!(strcmp("(", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "左括号", word[j].w);
            else if(!(strcmp(")", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "右括号", word[j].w);
            else if(!(strcmp(",", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "逗号", word[j].w);
            else if(!(strcmp(".", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "终结符", word[j].w);
            else if(!(strcmp(";", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "行末符", word[j].w);
            else if(!(strcmp("#", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "跳转符", word[j].w);
            else
                printf("(%d,)\t\t%s%s\n", 0, "ERROR", word[j].w);
        }

    }
    fclose(fp); ///关闭文件
}
/** ----------------------------------语法分析器--------------------------------*/
void E();
void E1();
void T();
void T1();
void F();

void E() { //E→TE'
    T(); E1();
}
void E1() { //E'→+TE'|-TE'|ε
    if(SYM[idx] == 4) { // 当前SYM为"+"
        ADVANCE();
        T(); E1();
    } else if(SYM[idx] == 5) { // 当前SYM为"-"
        ADVANCE();
        T(); E1();
    } else if(SYM[idx]!=17 && threshold!=1) { //SYM不为")"或idx超过表达式长度
        ERROR = -1; return;
    }
}
void T() { //T→FT'
    F(); T1();
}
void T1() { //T'→*FT'|/FT'|ε
    if(SYM[idx] == 6) { // 当前SYM为"*"
        ADVANCE();
        F(); T1();
    } else if(SYM[idx] == 7) { // 当前SYM为"/"
        ADVANCE();
        F(); T1();
    } else if(SYM[idx]!=4 && SYM[idx]!=5 && SYM[idx]!=17 && threshold!=1) {
        //SYM不为")"或"+"或"-"或idx超过表达式长度
        ERROR = -1; return;
    }
}
void F() { //F→(E)|id|num
    if(SYM[idx]==2 || SYM[idx]==3) { // 当前SYM为标识符, 常数
        ADVANCE();
    } else {
        if(SYM[idx] == 16) { // 当前SYM为"("
            ADVANCE();
            E();
            if(SYM[idx] == 17)  // 当前SYM为")"
                ADVANCE();
            else {
                ERROR = -1; return;
            }
        } else {
            ERROR = -1; return;
        }
    }
}
void SyntaxAnalysis() {
    puts("\n语法分析结果如下：");
    E();
    if(ERROR == -1 || !threshold)
        printf("Wrong: 该表达式语法错误！\n");
    else
        printf("Correct: 该表达式语法正确！\n");
}
int main() {
    LexicalAnalysis();
    SyntaxAnalysis();
    return 0;
}
