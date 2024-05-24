#include<iostream>
#include<stdexcept>
#include<vector>
#include<algorithm>

using namespace std;


void countSort(std::vector<int> &niz, int exp){
    std::vector<int> izlazni(niz.size());
    std::vector<int> brojac(10,0);
    for(int i = 0; i<niz.size(); i++){
        brojac[(niz[i]/exp)%10]++;
    }

    for(int i = 1; i < 10; i++){
        brojac[i] += brojac[i-1];
    }

    for(int i = niz.size()-1; i >=0; i--){
        izlazni[brojac[(niz[i]/exp)%10]-1] = niz[i];
        brojac[(niz[i]/exp)%10]--;
    }
    niz = izlazni; //meh
}

void radixSort(std::vector<int> &a){
    int max = *std::max_element(a.begin(), a.end());
    for(int i = 1; max/i > 0; i *= 10){
        countSort(a, i);
    }

}

int roditelj(int index){
    return (index-1)/2;
}
int lijevoDijete(int index){
    return 2*index+1;
}

int desnoDijete(int index){
    return 2*index+2;
}
bool jeLiList(std::vector<int> &a, int index, int &velicina){
    return index >= velicina/2 && index < velicina;
}

void popraviDolje(std::vector<int> &a, int index, int &velicina){
    while(!jeLiList(a, index, velicina)){
        int veci = lijevoDijete(index);
        int dd = desnoDijete(index);
        if(dd < velicina && a[dd] > a[veci]){
            veci = dd;
        }

        if(a[index] > a[veci]) return;
        int tmp = a[index];
        a[index] = a[veci];
        a[veci] = tmp;
        index = veci;
    }
}

void stvoriGomilu(std::vector<int> &a){
    int vel = a.size();
    for(int i = vel/2; i >=0; i--){
        popraviDolje(a, i, vel);
    }

} 

void popraviGore(std::vector<int> &a, int index){
    while(index != 0 && a[index] > a[roditelj(index)]){
        int tmp = a[index];
        a[index] = a[roditelj(index)];
        a[roditelj(index)] = tmp;
        index = roditelj(index);
    }
}

void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina){
    if(a.size() >= velicina){
        velicina +=1;
    }
    a.push_back(umetnuti);
    popraviGore(a, a.size()-1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina){
    if(velicina == 0) throw std::logic_error("Gomila je prazna!");
    velicina--;
    int tmp = a[0];
    a[0] = a[velicina];
    a[velicina] = tmp;
    if(velicina != 0){
        popraviDolje(a, 0, velicina);
    }
    
    return a[velicina];
}

void gomilaSort(std::vector<int> &a){
    stvoriGomilu(a);
    int velicina = a.size();
    for(int i = a.size()-1; i>=1; i--){
        int tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        velicina -= 1;
        popraviDolje(a,0,velicina);
    }
}

int main(){
        vector<int> a;
    a.push_back(546);
    a.push_back(1);
    a.push_back(31);
    a.push_back(1001);
    a.push_back(104351);
    a.push_back(7);
    a.push_back(6);
    a.push_back(60045);
    gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }
   
    return 0;
}