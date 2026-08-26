#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findmissmin(int* A,int n)
{
    int i,*B;
    B=(int*)malloc(sizeof(int)*n);
    memset(B,0,sizeof(int)*n);
    for(int i=0;i<n;i++)
    {
        if(A[i]>0 && A[i]<=n)
        {
            B[A[i]-1]=1;
        }
    }
    for(i=0;i<n;i++)
        {
            if(B[i]==0)
            {
                break;
            }
        }
        return i+1;
}

int main()
{
    int A[3]={1,2,3};
    printf("%d\n",findmissmin(A,3));
    int B[4]={-5,3,2,3};
    printf("%d\n",findmissmin(B,4));
}