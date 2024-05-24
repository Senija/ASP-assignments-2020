#include<iostream>
#include<stdexcept>
#include <fstream>
#include <string>
using std::cout;
using std::string;
using std::endl;

template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    for(int i = vel - 1; i>0; i--){
        for(int j = 1; j <= i; j++){ //nije potreban zadnji prolaz jer je ostala samo jedna pozicija
            if(niz[j-1] > niz[j]){
                Tip tmp = niz[j];
                niz[j] = niz[j-1];
                niz[j-1] = tmp;
            } 

        }
    }
}

template<typename Tip> //basic sa uup-a
void selection_sort(Tip* niz, int vel){
    for(int i = 0; i < vel - 1; i++){
        Tip minEl = niz[i];
        int indexMin = i;
        for(int j = i+1; j < vel; j++){
            if(niz[j] < minEl){
                minEl = niz[j];
                indexMin = j;
            }
        }

        niz[indexMin] = niz[i];
        niz[i] = minEl;
    }
}
template<typename Tip>
void merge(Tip* niz, int poc1podniz, int kraj1podniz, int poc2podniz, int kraj2podniz){
    int nerasp1 = 0;
    int nerasp2 = poc2podniz - poc1podniz;
    int trenutni = poc1podniz;
    Tip *nizB = new Tip[kraj2podniz - poc1podniz + 1];
    for(int m = 0; m <= kraj2podniz -poc1podniz; m++){
        nizB[m] = niz[poc1podniz + m];
    }

    while(nerasp1 <= kraj1podniz - poc1podniz && nerasp2 <= kraj2podniz - poc1podniz){
        if(nizB[nerasp1] < nizB[nerasp2]){
            niz[trenutni] = nizB[nerasp1];
            nerasp1++;
        }
        else{
            niz[trenutni] = nizB[nerasp2];
            nerasp2++;
        }
        trenutni++;
    }

    while(nerasp1 <= kraj1podniz - poc1podniz){
        niz[trenutni] = nizB[nerasp1];
        trenutni++;
        nerasp1++;
    }

    while(nerasp2 <= kraj2podniz - poc1podniz){
        niz[trenutni] = nizB[nerasp2];
        trenutni++;
        nerasp2++;
    }
    delete[] nizB;
}
template<typename Tip>
 void mergeSort(Tip *niz, int poc, int kraj){
     if(kraj > poc){
         int kraj1podniz = (kraj + poc)/2;
         int poc2podniz = kraj1podniz +1;
         mergeSort(niz, poc, kraj1podniz);
         mergeSort(niz, poc2podniz, kraj);
         merge(niz, poc, kraj1podniz, poc2podniz, kraj); 
     }
 }

template<typename Tip> 
void merge_sort(Tip* niz, int vel){
    mergeSort(niz, 0, vel-1);
}

template<typename Tip> 
void zamijeni(Tip* niz, int i, int j){
    Tip tmp = niz[i];
    niz[i] = niz[j];
    niz[j] = tmp;
}


template<typename Tip> 
int particija(Tip* niz, int prvi, int zadnji){
    Tip pivot = niz[prvi];
    int p = prvi + 1;
    while(p <= zadnji){
        if(niz[p] <= pivot) p++;
        else break;
    }

    for(int i = p +1; i <= zadnji; i++){
        if(niz[i] < pivot){
            zamijeni(niz, p, i);
            p++;
        }
    }
    zamijeni(niz, prvi, p-1);
    return p-1;

}

template<typename Tip> 
void quickSort(Tip* niz, int prvi, int zadnji){
    if(prvi < zadnji){
        int j = particija(niz, prvi, zadnji);
        quickSort(niz, prvi, j-1);
        quickSort(niz, j+1, zadnji);
    }
}

template<typename Tip> 
void quick_sort(Tip* niz, int vel){
    quickSort(niz, 0, vel-1);
}

void ucitaj(std::string filename, int*& niz, int &vel){
    std::ifstream ulazni_tok(filename);
    if(!ulazni_tok) std::cout << "Otvaranje datoteke nije uspjelo!\n";
    else {
        int element;
        int i = 0;
        while(ulazni_tok >> element){
            i++;
        }
        ulazni_tok.close();
        std::ifstream ulazni_tok2(filename);
        niz = new int[i];
        vel = i;
        int brojac = 0;
        while(ulazni_tok2 >> niz[brojac]){
            char znak;
            ulazni_tok>>znak;
            brojac++;
        }
        
    }
    
}

void generisi(std::string filename, int vel){
    std::ofstream izlazni_tok(filename);
    for(int i = 0; i < vel; i++)
    //izlazni_tok <<rand()<<std::endl;
    izlazni_tok <<rand()<<" ";
}

int main(){

    // int A[] = {4,18, 99, 66, 3, 2 ,-1 , 3};
    // quick_sort(A, 8);
    // for(int i = 0; i < 8; i++){
    //     std::cout<<A[i]<<std::endl;
    // }
    
    generisi("brojevi.txt", 8);
    int *niz = nullptr;
    int velicina = 0;
    ucitaj("brojevi.txt", niz,velicina);
    for(int i = 0; i < velicina; i++){
        std::cout<<niz[i]<<",";
    }
    delete[] niz;


    return 0;
}