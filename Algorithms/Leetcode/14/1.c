#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* longestCommonPrefix(char** strs, int strsSize) 
{
    if(strsSize==0)
    {
        char* ans=(char*)malloc(sizeof(char));
        ans[0]='\0';
        return ans;
    }
    int len=strlen(strs[0]);
    for(int i=0;i<len;i++)
    {
        char c=strs[0][i];
        for(int j=1;j<strsSize;j++)
        {
            if(strs[j][i]=='\0' || strs[j][i]!=c)
            {
                char* ans=(char*)malloc((i+1)*sizeof(char));
                strncpy(ans,strs[0],i);
                ans[i]='\0';
                return ans;
            }
        }
    }
    char* ans=(char*)malloc((len+1)*sizeof(char));
    strncpy(ans,strs[0],len);
    ans[len]='\0';
    return ans;
}

int main()
{
    char* strs1[]={"flower","flow","flight"};
    char* r1=longestCommonPrefix(strs1,3);
    printf("%s\n",r1);
    free(r1);

    char* strs2[]={"dog","racecar","car"};
    char* r2=longestCommonPrefix(strs2,3);
    printf("%s\n",r2);
    free(r2);

    char* strs3[]={""};
    char* r3=longestCommonPrefix(strs3,1);
    printf("%s\n",r3);
    free(r3);

    char* strs4[]={"ab","a"};
    char* r4=longestCommonPrefix(strs4,2);
    printf("%s\n",r4);
    free(r4);
    return 0;
}