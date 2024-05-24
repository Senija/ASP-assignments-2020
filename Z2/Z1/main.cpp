#include <iostream>
#include <stdexcept>
using namespace std;

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
        }
        //  else {
        //     auto temp_ispred = trenutni_element->prethodni_element;
        //     //auto temp_iza = trenutni_element->sljedeci;
        //     trenutni_element->prethodni_element = new Cvor{el, temp_ispred, trenutni_element};
        //     temp_ispred->sljedeci_element = trenutni_element->prethodni_element;
        // } NE TREBA
        broj_elemenata++;
    }

    void dodajIza(const TipEl& el) override
    {
        if(broj_elemenata == 0) {
            prvi_element = new Cvor{el, nullptr, nullptr};
            trenutni_element = prvi_element;
            posljednji_element = prvi_element;
        } 
        else if(trenutni_element == posljednji_element) {
            trenutni_element->sljedeci_element = new Cvor{el, trenutni_element, nullptr};
            posljednji_element = trenutni_element->sljedeci_element;
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
        if(trenutni_element == prvi_element) {
            auto temp_iza = trenutni_element->sljedeci_element;
            delete trenutni_element;
            trenutni_element = temp_iza;
            prvi_element = temp_iza;
        } else if(trenutni_element == posljednji_element) {
            auto temp_ispred = trenutni_element->prethodni_element;
            delete trenutni_element;
            temp_ispred->sljedeci_element = nullptr;
            trenutni_element = temp_ispred;
            posljednji_element = temp_ispred;
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
    void brisiSve(){
        dealociraj(prvi_element);
        broj_elemenata = 0;
        prvi_element = nullptr;
        posljednji_element = nullptr;
        trenutni_element = nullptr;
    }
};
template<typename TipEl>
class DvostraniRed{
    private:
    DvostrukaLista<TipEl> dvostrukaLista;
    public:
    DvostraniRed(){
        
    }
    void brisi(){
        dvostrukaLista.brisiSve();
        
    }

    int brojElemenata(){
        return dvostrukaLista.brojElemenata();
    }

    void staviNaCelo(const TipEl &el){
        if(dvostrukaLista.brojElemenata() != 0) dvostrukaLista.pocetak();
        dvostrukaLista.dodajIspred(el);
    }
    TipEl skiniSaCela(){
        if(dvostrukaLista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        dvostrukaLista.pocetak();
        auto pom = dvostrukaLista.trenutni();
        dvostrukaLista.obrisi();
        return pom;
    }

    void staviNaVrh(const TipEl &el){
        if(dvostrukaLista.brojElemenata() != 0) dvostrukaLista.kraj();
        dvostrukaLista.dodajIza(el);
    }

    TipEl skiniSaVrha(){
        if(dvostrukaLista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        dvostrukaLista.kraj();
        auto pom = dvostrukaLista.trenutni();
        dvostrukaLista.obrisi();
        return pom;
    }

    TipEl &vrh(){
        if(dvostrukaLista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
         dvostrukaLista.kraj();
        return dvostrukaLista.trenutni();
    }

    TipEl &celo(){
        if(dvostrukaLista.brojElemenata() == 0) throw std::logic_error("Red je prazan");
        dvostrukaLista.pocetak();
        return dvostrukaLista.trenutni();
    }
    void ispisi() const{
        dvostrukaLista.ispisi();
    }

};

int main(){
    DvostraniRed<int> dvostaniRed;
    dvostaniRed.staviNaCelo(1);
    dvostaniRed.staviNaCelo(2);
    dvostaniRed.staviNaVrh(3);
    dvostaniRed.staviNaVrh(4);
    dvostaniRed.ispisi();
    dvostaniRed.skiniSaCela();
    dvostaniRed.skiniSaVrha();
    std::cout<<"nakon izbacaja\n";
    dvostaniRed.ispisi();
    std::cout<<dvostaniRed.vrh()<<" ovo je vrh\n";
    std::cout<<dvostaniRed.celo()<<" ovo je celo\n";
    dvostaniRed.brisi();
    dvostaniRed.staviNaCelo(5);
    return 0;
}
