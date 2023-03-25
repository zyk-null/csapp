#include <stdio.h>
int main(){
    int fac[5000];
    double f=1231.1231;
    fac[0]=fac[1]=1;
    for(int i=2;i<50;i++){
    	for(int j=0;j<i;j++){
	    fac[i]+=fac[j];
	    f*=fac[i];
	    f/=fac[i];
	}
    }
    printf("ok\n");
    return 0;
}
