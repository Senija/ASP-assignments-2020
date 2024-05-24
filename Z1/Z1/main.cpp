#include <iostream>
#include <stdexcept>
#include<string>
using namespace std;
template<typename TipEl>
class Iterator;

template<typename TipEl>
class Lista
{
private:
    //konstruktor kopirajuci i pomjerajuci
public:
    Lista() {}
    //copy costruktor
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual  TipEl trenutni() const = 0;
    virtual  TipEl &trenutni()=0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi()=0; //da li treba vratiti trenutni????????????? neee
    virtual void dodajIspred(const TipEl& el)=0;
    virtual void dodajIza(const TipEl& el)=0;
    virtual TipEl operator[](int index) const = 0;
    virtual TipEl &operator[](int index) = 0;
    friend class Iterator<TipEl>;

};

template<typename TipEl>
class DvostrukaLista:public Lista<TipEl>
{
private:
    struct Cvor {
        TipEl vrijednost;
        Cvor *prethodni_element;
        Cvor *sljedeci_element;
    };

    Cvor *prvi_element = nullptr;
    Cvor *posljednji_element = nullptr;
    Cvor *trenutni_element = nullptr;
    int broj_elemenata =0;
    static void dealociraj(Cvor *pocetak)
    {
        for(auto i = pocetak; i != nullptr;) {
            auto novo_i = i->sljedeci_element;
            delete i;
            i=novo_i;
        }
    }
    static void copy(DvostrukaLista &trenutna, const DvostrukaLista &original)
    {
        Cvor *pom_trenutna = nullptr;
        Cvor *temp_original = original.prvi_element;
        for(int i = 0; i<original.broj_elemenata; i++) {
            if(i == 0) {
                trenutna.prvi_element = new Cvor{temp_original->vrijednost, nullptr, nullptr};
                if(temp_original == original.trenutni_element) trenutna.trenutni_element = trenutna.prvi_element;
                pom_trenutna = trenutna.prvi_element;
            } else {
                pom_trenutna->sljedeci_element = new Cvor{temp_original->vrijednost, pom_trenutna, nullptr};
                if(temp_original == original.trenutni_element) trenutna.trenutni_element = pom_trenutna->sljedeci_element;
                if(temp_original == original.posljednji_element) trenutna.posljednji_element = pom_trenutna->sljedeci_element;
                pom_trenutna = pom_trenutna->sljedeci_element;

            }
            temp_original = temp_original->sljedeci_element;
        }
    }
    static Cvor *nadji_iti(Cvor *prvi, int index)
    {
        int brojac = 0;
        for(auto i = prvi; i != nullptr; i = i->sljedeci_element) {
            if(brojac == index) return i;
            brojac++;
        }
        return nullptr;

    }
public:
    DvostrukaLista()
    {
        prvi_element = nullptr;
        posljednji_element = nullptr;
        trenutni_element = nullptr;
        broj_elemenata =0;
    }

    DvostrukaLista(const DvostrukaLista &DL)
    {
        copy(*this, DL);
        broj_elemenata = DL.broj_elemenata;
    }

    DvostrukaLista(DvostrukaLista &&DL) : prvi_element(DL.prvi_element), posljednji_element(DL.posljednji_element), trenutni_element(DL.trenutni_element), broj_elemenata(DL.broj_elemenata)
    {
        DL.trenutni_element = nullptr;
        DL.prvi_element = nullptr;
        DL.posljednji_element = nullptr;
        DL.broj_elemenata = 0;

    }

    DvostrukaLista &operator=(const DvostrukaLista &dl)
    {
        DvostrukaLista pomocna;
        copy(pomocna, dl);
        dealociraj(prvi_element);
        prvi_element = pomocna.prvi_element;
        posljednji_element = pomocna.posljednji_element;
        trenutni_element = pomocna.posljednji_element;
        broj_elemenata = dl.broj_elemenata;
        pomocna.prvi_element = nullptr;
        pomocna.trenutni_element = nullptr;
        pomocna.posljednji_element = nullptr;
        pomocna.broj_elemenata = 0;
        return *this;
    }

    DvostrukaLista &operator = (DvostrukaLista &&DL)
    {
        std::swap(trenutni_element, DL.trenutni_element);
        std::swap(prvi_element, DL.prvi_element);
        std::swap(posljednji_element, DL.posljednji_element);
        std::swap(broj_elemenata, DL.broj_elemenata);
        return *this;
    }
    ~DvostrukaLista()
    {
        // for(auto i = prvi_element; i != nullptr;){
        //     auto novo_i = i->sljedeci;
        //     delete i;
        //     i=novo_i;
        // }
        dealociraj(prvi_element);
        broj_elemenata = 0;
        prvi_element = nullptr;
        trenutni_element = nullptr;
        posljednji_element = nullptr;
    }
    TipEl trenutni() const override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        return trenutni_element->vrijednost;
    }
    TipEl &trenutni() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        return trenutni_element->vrijednost;
    }

    void dodajIspred(const TipEl& el) override
    {
        if(broj_elemenata == 0) {
            prvi_element =new Cvor{el,nullptr, nullptr};
            trenutni_element = prvi_element;
            posljednji_element = prvi_element;

        } else if(trenutni_element == prvi_element) {

            trenutni_element->prethodni_element = new Cvor{el, nullptr,trenutni_element};
            prvi_element = trenutni_element->prethodni_element;
        } else {
            auto temp_ispred = trenutni_element->prethodni_element;
            //auto temp_iza = trenutni_element->sljedeci;
            trenutni_element->prethodni_element = new Cvor{el, temp_ispred, trenutni_element};
            temp_ispred->sljedeci_element = trenutni_element->prethodni_element;
        }
        broj_elemenata++;
    }

    void dodajIza(const TipEl& el) override
    {
        if(broj_elemenata == 0) {
            prvi_element = new Cvor{el, nullptr, nullptr};
            trenutni_element = prvi_element;
            posljednji_element = prvi_element;
        } else if(trenutni_element == posljednji_element) {
            trenutni_element->sljedeci_element = new Cvor{el, trenutni_element, nullptr};
            posljednji_element = trenutni_element->sljedeci_element;
        } else {
            auto temp_iza = trenutni_element->sljedeci_element;
            if(temp_iza) {
                trenutni_element->sljedeci_element = new Cvor{el, trenutni_element, temp_iza};
                temp_iza->prethodni_element = trenutni_element->sljedeci_element;
            }
            //else trenutni_element->sljedeci_element = new Cvor{el, trenutni_element, nullptr};

        }
        broj_elemenata++;
    }

    bool prethodni() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        if(trenutni_element == prvi_element) return false;
        trenutni_element = trenutni_element->prethodni_element;
        return true;
    }
    bool sljedeci() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        if(trenutni_element == posljednji_element) return false;
        trenutni_element = trenutni_element->sljedeci_element;
        return true;
    }
    void pocetak() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        trenutni_element = prvi_element;
    }
    void kraj() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        trenutni_element = posljednji_element;
    }
    void obrisi() override
    {
        if(broj_elemenata == 0) throw std::logic_error("Lista je prazna.");
        auto temp_iza = trenutni_element->sljedeci_element;
        auto temp_ispred = trenutni_element->prethodni_element;
        // delete trenutni_element;
        // trenutni_element = nullptr;
        // temp_ispred->sljedeci_element = temp_iza;
        // if(temp_iza){
        //     trenutni_element = temp_iza;
        // }
        // else{
        //     trenutni_element = temp_ispred;
        // }
        if(trenutni_element == prvi_element) {
            delete trenutni_element;
            trenutni_element = temp_iza;
            prvi_element = temp_iza;
        } else if(trenutni_element == posljednji_element) {
            delete trenutni_element;
            temp_ispred->sljedeci_element = nullptr;
            trenutni_element = temp_ispred;
            posljednji_element = temp_ispred;
        } else {
            delete trenutni_element;
            temp_ispred->sljedeci_element = temp_iza;
            temp_iza->prethodni_element = temp_ispred;
            trenutni_element = temp_iza;

        }
        broj_elemenata--;
    }

    int brojElemenata() const override
    {
        return broj_elemenata;
    }
    void ispisi() const
    {
        for(auto i = prvi_element; i != nullptr; i = i->sljedeci_element) {
            std::cout<<i->vrijednost<<std::endl;
        }
    }
    TipEl operator[](int index) const
    {
        if(index >= broj_elemenata) throw std::logic_error("Nepostojeci index.");
        auto i_ti = nadji_iti(prvi_element, index);
        if(i_ti) {
            return i_ti->vrijednost;
        } else throw std::logic_error("Nepostojeci index.");
    }

    TipEl &operator[](int index)
    {
        if(index >= broj_elemenata) throw std::logic_error("Nepostojeci index.");
        auto i_ti = nadji_iti(prvi_element, index);
        if(i_ti) {
            return i_ti->vrijednost;
        } else throw std::logic_error("Nepostojeci index.");
    }
    bool palindrom() const{
        Cvor *pocetni = prvi_element;
        Cvor *krajnji  = posljednji_element;
        for(int i = 0; i < broj_elemenata/2; i ++){
            if(pocetni->vrijednost != krajnji->vrijednost) return false;
            else{
                pocetni = pocetni->sljedeci_element;
                krajnji = krajnji->prethodni_element;
            }
        }
         return true;
    }
    friend class Iterator<TipEl>;
};
template<typename TipEl>
class Iterator
{
private:
    const DvostrukaLista<TipEl> *dvostrukaLista;
    typename DvostrukaLista<TipEl>::Cvor *trenutniLista;
public:
    Iterator(const DvostrukaLista<TipEl> *DL)
    {
        dvostrukaLista = DL;
        trenutniLista = DL->trenutni_element;
    }
    TipEl &trenutni()
    {
        if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
        return trenutniLista->vrijednost;
    }

    TipEl trenutni() const
    {
        if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
        return trenutniLista->vrijednost;
    }

    // bool prethodni(){
    //     if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
    //     if(trenutniLista == dvostrukaLista->prvi_element) return false;
    //     trenutniLista->prethodni();
    //     return true;
    // }
    bool sljedeci()
    {
        if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
        if(trenutniLista == dvostrukaLista->posljednji_element) return false;
        trenutniLista = trenutniLista->sljedeci_element;
        return true;
    }

    void pocetak(){
        if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
        trenutniLista = dvostrukaLista->prvi_element;
    }

    // void kraj(){
    //     if(dvostrukaLista->broj_elemenata == 0) throw std::logic_error("Lista je prazna");
    //     trenutniLista->;
    // }
};

bool brojElemenataTest(){
    DvostrukaLista<int> dl;
    int i;
    for(i = 1; i <= 5; i++){
        dl.dodajIza(i);
    }
    
    return i-1 == dl.brojElemenata();
    
}

bool sljedeciTest(){
    DvostrukaLista<int> dl;
    int i;
    for(i = 1; i <=5; i++){
        dl.dodajIza(i);
    }
    dl.sljedeci();
    return i-1 == dl.trenutni();
}
bool krajTest(){
    DvostrukaLista<int> dl;
    int i;
    for(i = 1; i <=5; i++){
        dl.dodajIza(i);
    }
    dl.kraj();
    return 2 == dl.trenutni();
}
bool pocetakTest(){
    DvostrukaLista<int> dl;
    for(int i = 1; i <=5; i++){
        dl.dodajIza(i);
    }
    dl.pocetak();
    return 1 == dl.trenutni();
}
bool dodajIzaTest(){
    DvostrukaLista<int> dl;
    for(int i = 1; i <=5; i++){
        dl.dodajIza(i);
    }
    dl.kraj();
    dl.dodajIza(10);
    dl.kraj();
    return dl.trenutni() == 10;
}

template<typename TipEl>
TipEl dajMaksimum(const Lista<TipEl>&n)
{
    if(const DvostrukaLista<TipEl> *dl = dynamic_cast<const DvostrukaLista<TipEl>*>(&n)) {
        Iterator<TipEl> it(dl);
        it.pocetak();
        TipEl max = it.trenutni();
        while(it.sljedeci()) {
            if(it.trenutni() > max) {
                max = it.trenutni();
            }
        }
        return max;
    }
    else throw std::logic_error("Poslali ste nevalidan objekat.");

}
void dajMaksimumTest(){
    DvostrukaLista<int> listaInt;
    listaInt.dodajIspred(5);
    listaInt.dodajIza(6);
    listaInt.dodajIza(7);
    listaInt.dodajIza(8);
   if(dajMaksimum(listaInt) == 8) std::cout<<1<<"\n";
    DvostrukaLista<std::string> listaString;
    listaString.dodajIspred("5");
    listaString.dodajIza("6");
    listaString.dodajIza("7");
    listaString.dodajIza("8");
   if(dajMaksimum(listaString) == "8") std::cout<<1<<"\n";
    
}
int main()
{
        DvostrukaLista<char> dl;
    dl.dodajIza('R');
    dl.kraj();
    dl.dodajIza('A');
        dl.kraj();
    dl.dodajIza('T');
        dl.kraj();
    dl.dodajIza('A');
        dl.kraj();
    dl.dodajIza('R');
    dl.ispisi();
    std::cout<<"Je li polindrom "<<dl.palindrom();
    return 0;
}
