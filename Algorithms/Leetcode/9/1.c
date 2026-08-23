
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isPalindrome(int x) 
{
    char* buf=(char*)malloc(20*sizeof(char));
    snprintf(buf,20,"%d",x);
    int left=0;
    int right=strlen(buf)-1;
    while(left<right)
    {
        if(buf[left++]!=buf[right--])
        {
            free(buf);
            return false;
        }
    }
    free(buf);
    return true;
}

int main()
{
    bool isPalindrome(int x);
}