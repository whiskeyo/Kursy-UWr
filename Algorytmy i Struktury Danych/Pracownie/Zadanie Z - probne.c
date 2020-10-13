#include <stdio.h>

int main(){
    int a, b;
    scanf("%d %d", &a, &b);

    if (a > b){
        int temp = b;
        b = a;
        a = temp;
    }

    for (int i = a; i <= b; i++)
        printf("%d\n", i);
}
