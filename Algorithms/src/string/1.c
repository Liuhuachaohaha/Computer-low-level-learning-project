#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define MAXLEN 255

typedef struct
{
    char ch[MAXLEN];
    int length;
}SString;

// 将 C 字符串 chars 赋值给 T（1-based 存储）
// Assign C-string chars to T (1-based storage)
bool StrAssign(SString* T,const char* chars)
{
    int i=0;
    while(chars[i]!='\0' && i<MAXLEN-1)
    {
        T->ch[i+1]=chars[i];
        i++;
    }
    T->length=i;
    return true;
}

// 把字符串 S 复制到 T
// Copy string S to T
bool StrCopy(SString * T,SString S)
{
    for(int i=1;i<=S.length;i++)
    {
        T->ch[i]=S.ch[i];
    }
    T->length=S.length;
    return true;
}

// 判断字符串 S 是否为空
// Check whether string S is empty
bool StrEmpty(SString S)
{
    return S.length==0;
}

// 返回字符串 S 的长度
// Return the length of string S
int StrLength(SString S)
{
    return S.length;
}

// 清空字符串 S（仅置 length=0）
// Clear string S (only set length to 0)
bool ClearString(SString *S)
{
    S->length=0;
    return true;
}

// 拼接 S1 和 S2 到 T，超长则截断 S2，返回是否完整拼接
// Concatenate S1 and S2 into T; truncate S2 if too long, return whether uncut
bool Concat(SString *T,SString S1,SString S2)
{
    bool uncut=true;
    if(S1.length+S2.length<=MAXLEN-1)
    {
        for(int i=1;i<=S1.length;i++)
        {
            T->ch[i]=S1.ch[i];
        }
        for(int i=1;i<=S2.length;i++)
        {
            T->ch[S1.length+i]=S2.ch[i];
        }
        T->length=S1.length+S2.length;
    }
    else {
        for (int i = 1; i <= S1.length; i++)
            T->ch[i] = S1.ch[i];
        for (int i = 1; i <= MAXLEN - 1 - S1.length; i++)
            T->ch[S1.length + i] = S2.ch[i];
        T->length = MAXLEN - 1;
        uncut = false;
    }
    return uncut;
}

// 打印字符串内容及长度（调试用）
// Print string content and length (for debugging)
void printString(const char *name, SString S)
{
    printf("%s = ", name);
    for (int i = 1; i <= S.length; i++)
        printf("%c", S.ch[i]);
    printf("  (length=%d)\n", S.length);
}

// 比较 S 与 T：>0 表示 S>T，=0 相等，<0 表示 S<T
// Compare S and T: >0 means S>T, =0 equal, <0 means S<T
int StrCompare(SString S,SString T)
{
    for(int i=1;i<=S.length&&i<=T.length;i++)
    {
        if(S.ch[i]!=T.ch[i])
        {
            return S.ch[i]-T.ch[i];
        }
    }
    return S.length-T.length;
}

// 从 S 的 pos 位置起取长度为 len 的子串存入 Sub
// Extract a substring of length len starting at pos from S into Sub
bool SubString(SString *Sub, SString S, int pos, int len)
{
    if (pos < 1 || pos > S.length || len < 0 || len > S.length - pos + 1)
        return false;
    for (int i = 1; i <= len; i++)
        Sub->ch[i] = S.ch[pos + i - 1];
    Sub->length = len;
    return true;
}

// 定位：用 SubString+StrCompare 查找 T 在 S 中首次出现的位置
// Index: find the first occurrence of T in S using SubString+StrCompare
int Index(SString S, SString T)
{
    SString sub;
    int n = S.length;
    int m = T.length;
    for (int i = 1; i <= n - m + 1; i++)
    {
        SubString(&sub, S, i, m);
        if (StrCompare(sub, T) == 0)
            return i;
    }
    return 0;
}

// 暴力匹配（朴素模式匹配），时间复杂度 O(n*m)
// Brute-force (naive) pattern matching, time complexity O(n*m)
int Index_bruteforce(SString S,SString T)
{
    int i=1,j=1;
    while(i<=S.length && j<=T.length)
    {
        if(S.ch[i]==T.ch[j])
        {
            i++;j++;
        }
        else {
            i=i-j+2;
            j=1;
        }
    }
    if(j>T.length)
    {return i-T.length;}
    else {
        return 0;
    }
}

// 构造模式串 T 的 next 数组（最长相等前后缀长度）
// Build the next array for pattern T (length of longest equal prefix-suffix)
void get_next(SString T,int* next)
{
    int i=1,j=0;
    next[1]=0;
    while(i<T.length)
    {
        if(j==0 ||T.ch[i]==T.ch[j])
        {
            ++i;++j;
            next[i]=j;
        }
        else {
        j=next[j];
        }
    }
}

// KMP 模式匹配，时间复杂度 O(n+m)，主串指针不回溯
// KMP pattern matching, time complexity O(n+m), no backtracking of main pointer
int Index_KMP(SString S,SString T)
{
    int next[MAXLEN];
    get_next(T, next);
    int i=1,j=1;
    while(i<=S.length && j<=T.length)
    {
        if(j==0 || S.ch[i]==T.ch[j])
        {
            i++;j++;
        }
        else {
            j=next[j];
        }
    }
    if(j>T.length)
    {return i-T.length;}
    else {
        return 0;
    }
}

// 主函数：对各字符串操作进行测试
// Main function: test each string operation
int main()
{
    SString s1, s2, s3;

    StrAssign(&s1, "Hello");
    printString("StrAssign: s1", s1);

    StrCopy(&s2, s1);
    printString("StrCopy: s2", s2);

    printf("StrEmpty: %s\n", StrEmpty(s1) ? "empty" : "not empty");

    printf("StrLength: %d\n", StrLength(s1));

    Concat(&s3, s1, s2);
    printString("Concat: s1+s2", s3);

    ClearString(&s2);
    printString("ClearString: s2", s2);

    StrAssign(&s2, "World");
    printf("StrCompare(s1, s2): %d\n", StrCompare(s1, s2));

    SString sub;
    SubString(&sub, s1, 2, 3);
    printString("SubString(s1,2,3)", sub);

    StrAssign(&s2, "ll");
    printf("Index(s1, 'll'): %d\n", Index(s1, s2));
    printf("Index_bruteforce(s1, 'll'): %d\n", Index_bruteforce(s1, s2));
    printf("Index_KMP(s1, 'll'): %d\n", Index_KMP(s1, s2));
    return 0;
}





