#include<iostream>
#include<stdexcept>
#include<string>
#include<functional>
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
class HashMapa : public Mapa<TipKljuca, TipVrijednsti>{
    private:
    
    std::pair<TipKljuca, TipVrijednsti> **dupli = nullptr;;
    
    std::pair<TipKljuca, TipVrijednsti> *delPokazivac = nullptr;
    int broj_elemenata = 0;
    int broj_alociranih = 1000;
    std::function<unsigned int(TipKljuca, unsigned int)> hashFunkcija = nullptr;

    static std::pair<TipKljuca, TipVrijednsti> **Alociraj(int brElemenata)
    {
        std::pair<TipKljuca, TipVrijednsti> **temp = nullptr;
        try {
            temp = new std::pair<TipKljuca, TipVrijednsti> *[brElemenata] {};
        } catch(...) {
            delete[] temp;
            temp = nullptr;
            throw;
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

    void prosiri(){
        auto novi = Alociraj(broj_alociranih *2);
        int brojac = 0;
        for(int i = 0; i < broj_alociranih; i++){
            if(dupli[i] != nullptr &&  dupli[i] != delPokazivac){
                int pozicija = hashFunkcija(dupli[i]->first, broj_alociranih*2);
                if(novi[pozicija] == nullptr){
                    novi[pozicija] = new std::pair<TipKljuca,TipVrijednsti>(std::make_pair(dupli[i]->first, dupli[i]->second));
                    
                }
                else{
                    bool smjesten = false;
                    for(int j = pozicija +1; j < broj_alociranih*2; j++){
                        if(novi[j] == nullptr){
                            novi[j] = new std::pair<TipKljuca,TipVrijednsti>(std::make_pair(dupli[i]->first, dupli[i]->second));
                            smjesten = true;
                            break;
                        }
                    }
                    if(!smjesten){
                        for(int j = 0; j < pozicija; j++){
                            if(novi[j] == nullptr){
                            novi[j] = new std::pair<TipKljuca,TipVrijednsti>(std::make_pair(dupli[i]->first, dupli[i]->second));
                            smjesten = true;
                            break;
                            }
                        }
                    }
                }
                brojac++;
            }
            if(brojac == broj_elemenata) break;
        }
        Dealociraj(dupli, broj_alociranih);  
        broj_alociranih *= 2;
        dupli = nullptr;
        dupli = novi;
        
    }


    public:
    HashMapa(){
        dupli = Alociraj(broj_alociranih);
        broj_elemenata = 0;
        delPokazivac = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(TipKljuca(), TipVrijednsti()));
    }

    ~HashMapa(){
        Dealociraj(dupli,broj_alociranih);
        broj_alociranih = 0;
        broj_elemenata = 0;
        dupli = nullptr;
        delete delPokazivac;
        delPokazivac = nullptr;
    }

    HashMapa(const HashMapa &hashMap){
        if(this == &hashMap) return;
        dupli = Alociraj(hashMap.broj_alociranih);
        for(int i = 0; i < hashMap.broj_alociranih; i++){
            if(hashMap.dupli[i] != nullptr && hashMap.dupli[i] != delPokazivac){
                dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(hashMap.dupli[i]->first, hashMap.dupli[i]->second));
            }
            else if(hashMap.dupli[i] == hashMap.delPokazivac ) dupli[i] = hashMap.delPokazivac;
        }
        
        broj_alociranih = hashMap.broj_alociranih;
        broj_elemenata = hashMap.broj_elemenata;
        hashFunkcija = hashMap.hashFunkcija;
        delPokazivac = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(hashMap.delPokazivac->first, hashMap.delPokazivac->second));

    }

    HashMapa(HashMapa &&hashMap){
        dupli = hashMap.dupli;
        broj_alociranih = hashMap.broj_alociranih;
        broj_elemenata = hashMap.broj_elemenata;
        delPokazivac = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(hashMap.delPokazivac->first, hashMap.delPokazivac->second));
        hashFunkcija = hashMap.hashFunkcija;

    }

    HashMapa &operator=(const HashMapa &hashMap){
        if(this == &hashMap) return *this;
        
        Dealociraj(dupli, broj_alociranih);
        delete delPokazivac;
        delPokazivac = nullptr;
        dupli = nullptr;
        
        auto dupli_novi = Alociraj(hashMap.broj_alociranih);
        for(int i = 0; i < hashMap.broj_alociranih; i++){
            if(hashMap.dupli[i] != nullptr && hashMap.dupli[i] != delPokazivac){
                dupli_novi[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(hashMap.dupli[i]->first, hashMap.dupli[i]->second));
            }
            else if(hashMap.dupli[i] == hashMap.delPokazivac ) dupli_novi[i] = hashMap.delPokazivac;
        }
        


        dupli = dupli_novi;
        broj_alociranih = hashMap.broj_alociranih;
        broj_elemenata = hashMap.broj_elemenata;
        hashFunkcija = hashMap.hashFunkcija;
        delPokazivac = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(hashMap.delPokazivac->first, hashMap.delPokazivac->second));
        
        
        return *this;
    }

    // HashMapa &operator =(HashMapa &&novi){
        
    //     return *this;
    // }

    void definisiHashFunkciju(std::function<unsigned int(TipKljuca, unsigned int)> hashFunkcija){
        this->hashFunkcija = hashFunkcija;
    }

    int brojElemenata() const override{
        return broj_elemenata;
    }

    TipVrijednsti &operator[](const TipKljuca &kljuc) override{
        if(hashFunkcija == nullptr) throw std::logic_error("Nije definisana hash funkcija.");
        if(broj_alociranih/100 == broj_elemenata)
            prosiri();
        int pozicija = hashFunkcija(kljuc, broj_alociranih);

        for(int i = pozicija; i  != broj_alociranih; i++){
            if(dupli[i] != nullptr && dupli[i]->first == kljuc){
                return dupli[i]->second;
            }
            else if(dupli[i] != delPokazivac && dupli[i] == nullptr){
                dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                broj_elemenata++;
                return dupli[i]->second;
                //break;
            }
            else if(dupli[i] == delPokazivac){
                bool postojiLi = false;
                int index = -1;
                for(int j = i+1; j < broj_alociranih; j++){
                    if(dupli[j] != nullptr && dupli[j]->first == kljuc){
                        postojiLi = true;
                        index = j;
                        return dupli[j]->second;
                        //break;
                    }
                }
                if(!postojiLi){
                    for(int j = 0; j < i; j++){
                        if(dupli[j] != nullptr && dupli[j]->first == kljuc){
                            postojiLi = true;
                            index = j;
                            return dupli[j]->second;
                            //break;
                        }
                    }
                }
                if(!postojiLi){
                    dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                    broj_elemenata++;
                    return dupli[i]->second;
                } 
            } 
        }

        for(int i = 0; i < pozicija; i++){
            if(dupli[i] != nullptr && dupli[i]->first == kljuc){
                return dupli[i]->second;
            }
            else if(dupli[i] != delPokazivac && dupli[i] == nullptr){
                dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                broj_elemenata++;
                return dupli[i]->second;
                //break;
            }
            else if(dupli[i] == delPokazivac){
                bool postojiLi = false;
                int index = -1;
                for(int j = i+1; j < broj_alociranih; j++){
                    if(dupli[j] != nullptr && dupli[j]->first == kljuc){
                        postojiLi = true;
                        index = j;
                        return dupli[j]->second;
                        //break;
                    }
                }
                if(!postojiLi){
                    for(int j = 0; j < i; j++){
                        if(dupli[j] != nullptr && dupli[j]->first == kljuc){
                            postojiLi = true;
                            index = j;
                            return dupli[j]->second;
                            //break;
                        }
                    }
                }
                if(!postojiLi){
                    dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                    broj_elemenata++;
                    return dupli[i]->second;
                } 
            } 
            
        }
        throw std::logic_error("NIJE OK IMPLEMENTACIJA");
    }

    TipVrijednsti operator[](const TipKljuca &kljuc) const override{
        if(hashFunkcija == nullptr) throw std::logic_error("Nije definisana hash funkcija.");
        // if(broj_alociranih/100 == broj_elemenata)
        //     prosiri();
        int pozicija = hashFunkcija(kljuc, broj_alociranih);

        for(int i = pozicija; i  != broj_alociranih; i++){
            if(dupli[i] != nullptr && dupli[i]->first == kljuc){
                return dupli[i]->second;
            }
            else if(dupli[i] != delPokazivac && dupli[i] == nullptr){
                return TipVrijednsti();
                //break;
            }
            else if(dupli[i] == delPokazivac){
                bool postojiLi = false;
                int index = -1;
                for(int j = i+1; j < broj_alociranih; j++){
                    if(dupli[i] != nullptr && dupli[j]->first == kljuc){
                        postojiLi = true;
                        index = j;
                        return dupli[j]->second;
                        //break;
                    }
                }
                if(!postojiLi){
                    for(int j = 0; j < i; j++){
                        if(dupli[i] != nullptr && dupli[j]->first == kljuc){
                            postojiLi = true;
                            index = j;
                            return dupli[j]->second;
                            //break;
                        }
                    }
                }
                if(!postojiLi){
                    
                    return TipVrijednsti();
                } 
            } 
        }

        for(int i = 0; i < pozicija; i++){
            if(dupli[i] != nullptr && dupli[i]->first == kljuc){
                return dupli[i]->second;
            }
            else if(dupli[i] != delPokazivac && dupli[i] == nullptr){
                // dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                // broj_elemenata++;
                return TipVrijednsti();
                //break;
            }
            else if(dupli[i] == delPokazivac){
                bool postojiLi = false;
                int index = -1;
                for(int j = i+1; j < broj_alociranih; j++){
                    if(dupli[i] != nullptr && dupli[j]->first == kljuc){
                        postojiLi = true;
                        index = j;
                        return dupli[j]->second;
                        //break;
                    }
                }
                if(!postojiLi){
                    for(int j = 0; j < i; j++){
                        if(dupli[i] != nullptr && dupli[j]->first == kljuc){
                            postojiLi = true;
                            index = j;
                            return dupli[j]->second;
                            //break;
                        }
                    }
                }
                if(!postojiLi){
                    // dupli[i] = new std::pair<TipKljuca, TipVrijednsti>(std::make_pair(kljuc, TipVrijednsti()));
                    // broj_elemenata++;
                    return TipVrijednsti();
                } 
            } 
            
        }
        throw std::logic_error("NIJE OK IMPLEMENTACIJA");

    }

    void obrisi() override{
        for(int i = 0; i < broj_alociranih; i++) {
            if(dupli[i] != delPokazivac){
                delete dupli[i];
                dupli[i] = nullptr;
            }
            else dupli[i] = nullptr;
        }
        broj_elemenata = 0;

    }
    virtual void obrisi(const TipKljuca &kljuc) override{
        int index = hashFunkcija(kljuc, broj_alociranih);
        bool postojiLi = false;;
        for(int i = index; i <broj_alociranih; i++){
            if(dupli[i]->first == kljuc){
                postojiLi = true;
                delete dupli[i];
                dupli[i] = nullptr;
                dupli[i] = delPokazivac;
                broj_elemenata--;
                break;
            }
        }

        if(!postojiLi)
            for(int i = 0; i < index; i++){
                    if(dupli[i]->first == kljuc){
                    postojiLi = true;
                    delete dupli[i];
                    dupli[i] = nullptr;
                    dupli[i] = delPokazivac;
                    broj_elemenata--;
                    break;
                }
            }
        if(!postojiLi) throw std::logic_error("Ne postoji zadani kljuc.");

    }

};


int main(){



    return 0;
}