#include <iostream>
#include <stdexcept>
using std::cout;
int fib_petlja (int n)
{
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    for (int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}

int fib2_0(int n, int pocetnaVrijednosti = 1){
    if(n == 0) return 0;
    if(n == 1 || n == 2) return 1;
    return fib2_0(n-1) + fib2_0(n-2);
    
}

int nzd(int x, int y){
     if(y < 0) throw std::logic_error("Nije definisano za ovaj slucaj!");
     if(y == 0) return x;
     return nzd(y, x % y);
 }
int main()
{
    for(int i = 1; i < 20; i ++)
        std::cout<<(fib2_0(i) == fib_petlja(i))<<"\n";
    cout << fib2_0(20);
    return 0;
}
