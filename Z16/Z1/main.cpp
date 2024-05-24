#include <iostream>
#include <stdexcept>
#include <utility>
using namespace std;

template<typename TipKljuca, typename TipVrijednsti>
class Mapa{
    public:
    Mapa(){
        
    }
    virtual ~Mapa(){
        
    }
    
    virtual TipVrijednsti &operator[](const TipKljuca &kljuc) = 0;
    virtual TipVrijednsti operator[](const TipKljuca &kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;

    
};

template<typename TipKljuca, typename TipVrijednsti>
class NizMapa : public Mapa<TipKljuca, TipVrijednsti>{
    private:
    std::pair<TipKljuca, TipVrijednsti> **dupli = nullptr;
    int broj_elemenata = 0;
    int broj_alociranih = 0;
    static std::pair<TipKljuca, TipVrijednsti> **Alociraj(int brElemenata)
    {
        std::pair<TipKljuca, TipVrijednsti> **temp = nullptr;
        try {
            temp = new std::pair<TipKljuca, TipVrijednsti> *[brElemenata] {};
        } catch(...) {
            delete[] temp;
            temp = nullptr;
            throw;
            //Dealociraj(pokNaNizPokazivaca, brElemenata);
        }
        return temp;

    }

    static void Dealociraj(std::pair<TipKljuca, TipVrijednsti> **pok, int brElemenata)  //za destruktor
    {
        for(int i = 0; i < brElemenata; i++) {
            delete pok[i];
            pok[i] = nullptr;
        }
        delete[] pok;
        pok = nullptr;
    }

    static void Copy(std::pair<TipKljuca, TipVrijednsti> **stari, std::pair<TipKljuca, TipVrijednsti> **novi, int brElemenata)
    {
        for(int i = 0; i <brElemenata; i++) novi[i] = new std::pair<TipKljuca, TipVrijednsti>(*stari[i]);

    }

    bool postojiLi(TipKljuca kljuc) const{
        for(int i = 0; i < broj_elemenata; i++){
            if(std::get<0>(*dupli[i]) == kljuc) return true;
        }
        return false;
    } 
    public:

    NizMapa(){
        dupli = Alociraj(10);
        broj_alociranih = 10;
        broj_elemenata = 0;
    };
    ~NizMapa(){
        Dealociraj(dupli, broj_alociranih);
        broj_elemenata = 0;
        broj_alociranih = 0;
    }
    NizMapa(const NizMapa &novi)
    {
        if(this == &novi) return;
        try {
            auto pom = Alociraj(novi.broj_alociranih);
            Copy(novi.dupli, pom, novi.broj_elemenata);
            dupli = pom;
            broj_alociranih = novi.broj_alociranih;
            broj_elemenata = novi.broj_elemenata;
        } catch(...) {
            throw;
        }

    }

    NizMapa(NizMapa &&novi):broj_alociranih(novi.broj_alociranih), broj_elemenata(novi.broj_elemenata){
        dupli = novi.dupli;
    }

    NizMapa &operator=(const NizMapa &novi)
    {
        try {
            if(this == &novi) return *this;
            auto pom = Alociraj(novi.broj_alociranih);
            Dealociraj(dupli, broj_elemenata);
            //pokNaNizPokazivaca = pom;
            Copy(novi.dupli, pom, novi.broj_elemenata);
            dupli = pom;
            broj_elemenata = novi.broj_elemenata;
        } catch(...) {
            throw;
        }
        return *this;
    }

    NizMapa &operator =(NizMapa &&novi)
    {
        if(this == &novi) return *this;
        std::swap(broj_elemenata, novi.broj_elemenata);
        std::swap(broj_alociranih, novi.broj_alociranih);
        std::swap(dupli, novi.dupli);
        return *this;
    }

    int brojElemenata() const override{
        return broj_elemenata;
    }

    void obrisi() override{
        Dealociraj(dupli, broj_elemenata);
        auto pom = Alociraj(10);
        dupli = pom;
        broj_elemenata = 0;
        broj_alociranih = 10;
    }

    void obrisi(const TipKljuca& kljuc) override{
        if(broj_elemenata ==0 || !postojiLi(kljuc)) throw std::logic_error("Ne postoji vrijednost sa datim kljucem!");
        for(int i = 0; i < broj_elemenata; i++){
            if(std::get<0>(*dupli[i]) == kljuc){
                if(i == broj_elemenata - 1){
                    delete dupli[i];
                    dupli[i] = nullptr;

                }
                else{
                    delete dupli[i];
                    dupli[i] = nullptr;
                    for(int j = i + 1; j < broj_elemenata; j++){
                        dupli[j-1] = dupli[j];
                    }
                }
                break;
            }
        }
        broj_elemenata--;

    }

    TipVrijednsti &operator[](const TipKljuca &kljuc) override{
        if(!postojiLi(kljuc)){
            if(broj_elemenata >= broj_alociranih){
                auto pom = Alociraj(broj_alociranih * 2);
                Copy(dupli, pom, broj_elemenata);
                Dealociraj(dupli, broj_elemenata);
                dupli = pom;
                broj_alociranih *= 2;

            }
            auto par = std::make_pair(kljuc, TipVrijednsti());
            dupli[broj_elemenata++] = new std::pair<TipKljuca, TipVrijednsti>(par);
            return dupli[broj_elemenata - 1]->second;
        }

        else{
            for(int i = 0; i < broj_elemenata; i++){
                if(std::get<0>(*dupli[i]) == kljuc) return dupli[i]->second;
            }
        }
    }

    TipVrijednsti operator[](const TipKljuca &kljuc) const override{
        if(!postojiLi(kljuc)){
            return TipVrijednsti();
        }
        else{
            for(int i = 0; i < broj_elemenata; i++){
                if(std::get<0>(*dupli[i]) == kljuc) return dupli[i]->second;
            }
        }
    }






};

int main(){
NizMapa <int, int> m;
m[100] = 200;
const NizMapa <int, int>& m2(m);
m[200] = 300;
cout << m2.brojElemenata() << " "; // 2
cout << m2[200] << " "; // 300

// Ovdje se sigurno nece kreirati objekat m2[300] jer je m2 const
cout << m2[300] << " "; // 0
cout << m2.brojElemenata() << " "; // 2
m[300] = 400;
cout << m2[300] << " "; // 400
cout << m2.brojElemenata(); // 3
    return 0;
}