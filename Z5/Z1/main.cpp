#include <stdio.h>

int izbaci_podniz(int *p1, int *p2, const int *q1, const int *q2)
{
    int *p = p1;
    int *p0 = p1;
    const int *q = q1;
    int k = 0, brojac = 0, brojizbacenih = 0;
    int i=0;
    while (p != p2) {
        if (*p == *q1) {
            if (k == 0) {
                k = 1;
                brojac = 0;
                p0 = p;
            }
            brojac++;
            q++;
        } else {
            if (k == 1) {
                p--;
                k = 0;
                q = q1;
            }
        }
        if (q == q2) {
            p = p0;
            q=q1;
            for (i = 0; i < p2 - p; i++) {
                *p0 = *(p0 + brojac);
                p0++;
            }
            p2 -= brojac;
            brojizbacenih += brojac;

        } else
            p++;
    }
    return brojizbacenih;
}
int main()
{
    // printf("Lekcija 6: Pokazivači, Izbaci podniz");
    // int niz[10] ={1,2,3,4,5,6,7,8,9,10};
    // izbaci_podniz(niz)
    int niz1[10]={1,1,2,3,5,6,1,2,4,10};
    int niz2[3]={1,2,3};
    int k = izbaci_podniz(niz1, niz1+10, niz2, niz2+3);
    int i;
    for(i=0; i<10-k; i++) printf("%d ",niz1[i]);
    return 0;
}
