#include <iostream>
#include <stdexcept>
using namespace std;
#include <iostream>
#include <stdexcept>
template<typename TipEl>
class Lista{
    private:
    //konstruktor kopirajuci i pomjerajuci
    public:
        Lista(){}
        //copy costruktor
        virtual ~Lista(){}
        virtual int brojElemenata() const = 0;
        virtual  TipEl trenutni() const = 0;
        virtual  TipEl &trenutni()=0;
        virtual void pocetak() = 0;
        virtual void obrisi()=0; //da li treba vratiti trenutni????????????? neee
        virtual void dodajIspred(const TipEl& el)=0;
};

template<typename TipEl>
class  JednostrukaLista:public Lista<TipEl>{
    private:
    struct Cvor{
        TipEl vrijednost;
        Cvor *element_nakon;
    };
    int broj_elemenata = 0;
    Cvor *prvi_element = nullptr;
    Cvor *posljednji_element = nullptr;
    Cvor *trenutni_element = nullptr;


    static void Dealloc(Cvor *pocetni){
        for(auto i = pocetni; i!= nullptr; ){
            auto next= i->element_nakon;
            delete i;
            i = nullptr;
            i = next;
        }
    }
    
    static void copy(JednostrukaLista &trenutna, const JednostrukaLista &originalna){
        auto temp = originalna.prvi_element;
        Cvor  *origigi = nullptr;
        for(int i = 0; i < originalna.broj_elemenata; i++){
            if(i == 0){
                trenutna.prvi_element = new Cvor{temp->vrijednost, nullptr};
                origigi = trenutna.prvi_element;
                if(temp == originalna.trenutni_element) trenutna.trenutni_element = trenutna.prvi_element;
            }
            else{
                origigi->element_nakon = new Cvor{temp->vrijednost,nullptr};
                if(temp == originalna.trenutni_element) trenutna.trenutni_element = origigi->element_nakon;
                if(temp == originalna.posljednji_element) trenutna.posljednji_element = origigi->element_nakon;
                origigi = origigi->element_nakon;
            }
            temp = temp->element_nakon;
            
        }
    }
    public:

    JednostrukaLista():Lista<TipEl>(){
        broj_elemenata = 0;
        prvi_element = nullptr;
        posljednji_element=nullptr;
        trenutni_element = nullptr;
    }
    
    JednostrukaLista(const JednostrukaLista &jednostrukaLista){
        copy(*this, jednostrukaLista);
        broj_elemenata = jednostrukaLista.broj_elemenata;
    }
    
    JednostrukaLista &operator=(const JednostrukaLista &jednostrukaLista){
        if(this != &jednostrukaLista){
            JednostrukaLista pomocna;
            copy(pomocna, jednostrukaLista);
            if(prvi_element) Dealloc(prvi_element);
            prvi_element = pomocna.prvi_element;
            posljednji_element = pomocna.posljednji_element;
            trenutni_element = pomocna.trenutni_element;
            broj_elemenata = jednostrukaLista.broj_elemenata;
            pomocna.prvi_element = nullptr;
            pomocna.trenutni_element = nullptr;
            pomocna.posljednji_element = nullptr;
            pomocna.broj_elemenata = 0;
        }
        return *this;
    }
    
    JednostrukaLista(JednostrukaLista &&jl){
        std::swap(prvi_element, jl.prvi_element);
        std::swap(posljednji_element, jl.posljednji_element);
        std::swap(trenutni_element, jl.trenutni_element);
        std::swap(broj_elemenata, jl.broj_elemenata);
        
    }
    ~JednostrukaLista(){
        Dealloc(prvi_element);
        trenutni_element = nullptr;
        prvi_element = nullptr;
        posljednji_element = nullptr;
        broj_elemenata = 0;
        // for(auto i = prvi_element; i!= nullptr; ){
        //     auto next= i->element_nakon;
        //     delete i;
        //     i = nullptr;
        //     i = next;
        // }
    }
    TipEl trenutni() const override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        return trenutni_element->vrijednost;
    }
    TipEl &trenutni() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        return trenutni_element->vrijednost;
    }
    void pocetak() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        trenutni_element = prvi_element;
    }

    int brojElemenata() const override{
        return broj_elemenata;
    }


    void obrisi(){
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
       
        if(trenutni_element == prvi_element){
            auto iza = trenutni_element->element_nakon;
            delete trenutni_element;
            trenutni_element = iza;
            prvi_element = iza;
        }
        broj_elemenata--;

    }

    void dodajIspred(const TipEl &el) override{
        if(broj_elemenata == 0){
            Cvor *novi = new Cvor{el, nullptr};
            prvi_element = novi;
            posljednji_element = novi;
            trenutni_element = novi;
        }
        else{

            Cvor *novi = new Cvor{el, trenutni_element};
            prvi_element = novi;
            //posljednji_element = trenutni_element;
        }
        broj_elemenata++;

    }
    
    void obrisiSve(){
        Dealloc(prvi_element);
        prvi_element = nullptr;
        trenutni_element = nullptr;
        posljednji_element = nullptr;
        broj_elemenata = 0;
    }
};

template<typename TipEl>
class Stek{
    private:
        JednostrukaLista<TipEl> stek;
    public:
    Stek(){
        
    }
    void brisi(){
        stek.obrisiSve();
    }
    TipEl skini(){
        if(stek.brojElemenata() == 0) throw std::logic_error("Stek je prazan.");
        auto pom = stek.trenutni();
        stek.obrisi();
        //stek.pocetak();
        return pom;
    }

    void stavi(const TipEl &el){
        stek.dodajIspred(el);
        stek.pocetak();
    }

    TipEl &vrh(){
        if(stek.brojElemenata() == 0) throw std::logic_error("Stek je prazan.");
        return stek.trenutni();
    }
    int brojElemenata() const{
        return stek.brojElemenata();
    }

};
template<typename TipEl>
void brisiTest(Stek<TipEl> stek){
    try{
        stek.brisi();
        std::cout<<stek.brojElemenata();
    }
    catch(std::logic_error e){
        std::cout<<e.what();
    }
}

bool vrhTest(){
    Stek<int> s;
    for (int i = 0; i<5; i++)
        s.stavi(i);
    
    return s.vrh() == 4;
}
bool skiniTest(){
    Stek<int> s;
    for (int i = 0; i<5; i++)
        s.stavi(i);
    auto pom = s.skini();
    return pom == 4;
}

bool staviTest(){
    Stek<int> s;
    for (int i = 0; i<5; i++)
        s.stavi(i);
    return s.brojElemenata() == 5;
}

int main() {

    return 0;
}