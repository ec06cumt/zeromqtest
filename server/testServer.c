#include "stdio.h"

int GetMax(int iFirst,int iSecond)
{
	return iFirst>iSecond?iFirst:iSecond;
}

int main()
{
	int iFirst = 10;
	int iSecond = 20;
	printf("The max value is %d\n",GetMax(iFirst,iSecond));
	
	return 0;
};