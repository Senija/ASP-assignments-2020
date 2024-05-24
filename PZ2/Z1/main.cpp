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
        virtual bool prethodni() = 0;
        virtual bool sljedeci() = 0;
        virtual void pocetak() = 0;
        virtual void kraj() = 0; 
        virtual void obrisi()=0; //da li treba vratiti trenutni????????????? neee
        virtual void dodajIspred(const TipEl& el)=0;
        virtual void dodajIza(const TipEl& el)=0;
        virtual TipEl operator[](int index) const = 0;
        virtual TipEl &operator[](int index) = 0;
        
};
template<typename TipEl>
class NizLista:public Lista<TipEl>{
    private: 
        TipEl **pokNaNizPokazivaca = nullptr;
        int broj_Elemenata = 0;
        int broj_alociranih = 0 ;
        //TipEl *trenutniEl = nullptr;
        int index_trenutnog = -1;
        static TipEl **Alociraj(int brElemenata){
            TipEl **temp = nullptr;
            try{
                temp = new TipEl*[brElemenata]{};
            }
            catch(...){
                delete[] temp;
                temp = nullptr;
                throw;
                //Dealociraj(pokNaNizPokazivaca, brElemenata);
            }
            return temp;
            
        }
        
        static void Dealociraj(TipEl **pok, int brElemenata){ //za destruktor
            for(int i = 0; i < brElemenata; i++){
                delete pok[i];
                pok[i] = nullptr;
            } 
            delete[] pok;
            pok = nullptr;
        }
        
        static void Copy(TipEl **stari, TipEl **novi, int brElemenata){
            for(int i = 0; i <brElemenata; i++) novi[i] = new TipEl(*stari[i]);

        }

        // int dajIndexTrenutnog(){
        //     for(int i = 0; i < broj_Elemenata; i++){
        //         if(trenutniEl == pokNaNizPokazivaca[i]) return i;
        //     }
        //     return -1;//za greske
        // }
        void add(const TipEl &el, int granica){
                //debug
                //int pom = dajIndexTrenutnog();
                //debug
                for(int i = broj_Elemenata - 1; i >= granica; i--){
                    pokNaNizPokazivaca[i+1] = pokNaNizPokazivaca[i];
                }
                //debug
                //int pom = dajIndexTrenutnog();
                // trenutniEl = pokNaNizPokazivaca[granica+1]; //ovo ide samo kod ubacivanja ispred
                //debug
                pokNaNizPokazivaca[granica] = new TipEl(el);

                
                //broj_Elemenata++;
        }
    public:
        NizLista():Lista<TipEl>(){
            pokNaNizPokazivaca = Alociraj(100);
            broj_Elemenata = 0;
            broj_alociranih = 100;
            index_trenutnog = -1;
            //trenutniEl = nullptr;
        }

        NizLista(const NizLista &novi){
            if(this == &novi) return;
            try{
                auto pom = Alociraj(novi.broj_alociranih);
                Copy(novi.pokNaNizPokazivaca, pom, novi.broj_Elemenata);
                pokNaNizPokazivaca = pom;
                broj_alociranih = novi.broj_alociranih;
                broj_Elemenata = novi.broj_Elemenata;
            }
            catch(...){
                throw;
            }

        }

        NizLista(NizLista &&novi):broj_alociranih(novi.broj_alociranih), broj_Elemenata(novi.broj_Elemenata), index_trenutnog(novi.index_trenutnog){
            pokNaNizPokazivaca = novi.pokNaNizPokazivaca;
        }

        NizLista &operator=(const NizLista &novi){
            try{
                auto pom = Alociraj(novi.broj_alociranih);
                Dealociraj(pokNaNizPokazivaca, broj_Elemenata);
                pokNaNizPokazivaca = pom;
                Copy(novi.pokNaNizPokazivaca, pom, novi.broj_Elemenata);
                pokNaNizPokazivaca = pom;
                broj_Elemenata = novi.broj_Elemenata;
                index_trenutnog = novi.index_trenutnog;
            }
            catch(...){
                throw;
            }
            return *this;
        }

        NizLista &operator =(NizLista &&novi){
            std::swap(broj_Elemenata, novi.broj_Elemenata);
            std::swap(broj_alociranih, novi.broj_alociranih);
            std::swap(index_trenutnog,novi.index_trenutnog);
            std::swap(pokNaNizPokazivaca, novi.pokNaNizPokazivaca);
            return *this;
        }
        
        ~NizLista(){
            Dealociraj(pokNaNizPokazivaca, broj_Elemenata);
            broj_Elemenata = 0;
            broj_alociranih= 0;
            //trenutniEl = nullptr;
            index_trenutnog = -1;
        }

        int brojElemenata() const override{
            return broj_Elemenata;
            
        }

        TipEl trenutni() const override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            return *pokNaNizPokazivaca[index_trenutnog];
        }

        TipEl &trenutni() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            return *pokNaNizPokazivaca[index_trenutnog];
        }

        bool prethodni() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            if(index_trenutnog == 0) return false;
            //trenutniEl = pokNaNizPokazivaca[dajIndexTrenutnog() - 1];
            index_trenutnog -= 1;
            return true;
        }

        bool sljedeci() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            if(index_trenutnog == broj_Elemenata-1) return false;
            else index_trenutnog += 1;
            return true;
        }

        void pocetak() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            //trenutniEl = pokNaNizPokazivaca[0];
            index_trenutnog = 0;
        }

        void kraj() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            //trenutniEl = pokNaNizPokazivaca[broj_Elemenata-1];
            index_trenutnog = broj_Elemenata-1;
        }

        void obrisi() override{
            if(broj_Elemenata == 0) throw std::logic_error("Lista je prazna.");
            if(index_trenutnog == broj_Elemenata-1){
                //auto index_trenutnog = dajIndexTrenutnog();
                delete pokNaNizPokazivaca[index_trenutnog];
                pokNaNizPokazivaca[index_trenutnog] = nullptr;
                if(broj_Elemenata != 1) index_trenutnog -= 1;
               
            }
            else
            {
                //auto index_trenutnog =dajIndexTrenutnog();
                delete pokNaNizPokazivaca[index_trenutnog];
                pokNaNizPokazivaca[index_trenutnog] = nullptr;// ide for petlja
                for(int i = index_trenutnog; i < broj_Elemenata - 1; i++){
                    pokNaNizPokazivaca[i] = pokNaNizPokazivaca[i+1];
                }
                //pokNaNizPokazivaca[broj_Elemenata - 1] =nullptr; mozda nije potrebo jer smanjimo velicinu svakako
                //trenutniEl = pokNaNizPokazivaca[index_trenutnog + 1];
                //index_trenutnog += 1; 
            }
             broj_Elemenata--;
        }

        void dodajIspred(const TipEl& el) override{// nije OK
            if(broj_Elemenata == 0){
                pokNaNizPokazivaca[broj_Elemenata] = new TipEl(el);//ako je lista prazna, ako ima mjesta, ako nema mjesta
                //trenutniEl = pokNaNizPokazivaca[0];
                index_trenutnog = 0;
                broj_Elemenata++;
                return;
            } 
            
            else if(broj_Elemenata >= broj_alociranih){
                try
                {
                    //int index_trenutnog = dajIndexTrenutnog();
                    TipEl **noviPokNaNiz = Alociraj(broj_alociranih *2);
                    Copy(pokNaNizPokazivaca, noviPokNaNiz, broj_Elemenata);
                    Dealociraj(pokNaNizPokazivaca, broj_Elemenata);
                    pokNaNizPokazivaca = noviPokNaNiz;
                    //trenutniEl =pokNaNizPokazivaca[index_trenutnog];
                    broj_alociranih *= 2;
                    
                }
                catch(...)
                {
                    throw;
                }   
            }
            //pokNaNizPokazivaca[broj_Elemenata] = new TipEl();
            //auto index_trenutnog =  dajIndexTrenutnog();
            add(el, index_trenutnog);
            broj_Elemenata++;
            index_trenutnog +=1;
            //trenutniEl = pokNaNizPokazivaca[index_trenutnog+1]; //da li ce ovdje biti neke smetnje ako budu nek add vrati granicu!!
        }
        void dodajIza(const TipEl& el) override{
            if(broj_Elemenata == 0){
                pokNaNizPokazivaca[broj_Elemenata++] = new TipEl(el);//ako je lista prazna, ako ima mjesta, ako nema mjesta
                //trenutniEl = pokNaNizPokazivaca[0];
                index_trenutnog = 0;
                return;
            }
            else if(broj_Elemenata >= broj_alociranih){
                TipEl **noviPokNaNiz = Alociraj(broj_alociranih *2);
                Copy(pokNaNizPokazivaca, noviPokNaNiz, broj_Elemenata);
                Dealociraj(pokNaNizPokazivaca, broj_Elemenata);
                pokNaNizPokazivaca = noviPokNaNiz;
                broj_alociranih *= 2;
            }
            if(index_trenutnog == broj_Elemenata - 1){
                
                pokNaNizPokazivaca[broj_Elemenata++] = new TipEl(el);
                return; 
            } 
            add(el, index_trenutnog + 1);
            broj_Elemenata++;
            // ne treba dirati trenutni
        }
        //debug
        // void Ispisi(){
        //     for(int i = 0; i < broj_Elemenata; i++){
        //         std::cout<<*pokNaNizPokazivaca[i]<<std::endl;
        //     }
        // }
        //debug
        TipEl operator[](int index) const{
            if(index < 0 || index >=broj_Elemenata) throw std::range_error("Nevaljan index.");
            return *pokNaNizPokazivaca[index];
        }
        TipEl &operator[](int index){
            if(index < 0 || index >=broj_Elemenata) throw std::range_error("Nevaljan index.");
            return *pokNaNizPokazivaca[index];
        }

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

    Cvor *nadjiIspredTrenutnog(){
        Cvor *element_ispred = nullptr;
        for(auto i = prvi_element; i != nullptr; i = i-> element_nakon ){
            if(i-> element_nakon == trenutni_element){
                    return i;
            }
        }
        return element_ispred;
    }
    Cvor *nadjiElement(int index) const{
        int brojac = 0;
        for(auto i = prvi_element; i != nullptr; i = i->element_nakon){
            if(index == brojac) return i;
            brojac++;
        }
        return nullptr; //IMA GRESKA!!!!!!!!!!!!!!!!!!!!!!
    }
    
    // Cvor *dodaj(Cvor *prethodnik, TipEl element) const{
    //     prethodni->element_nakon = new Cvor{element, nullptr};
    //     return prethodni->element_nakon;
    // }
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
        // auto temp = jednostrukaLista.prvi_element;
        // Cvor  *origigi = nullptr;
        // for(int i = 0; i < jednostrukaLista.broj_elemenata; i++){
        //     if(i == 0){
        //         prvi_element = new Cvor{temp->vrijednost, nullptr};
        //         origigi = prvi_element;
        //         if(temp == jednostrukaLista.trenutni_element) trenutni_element = prvi_element;
        //     }
        //     else{
        //         origigi->element_nakon = new Cvor{temp->vrijednost,nullptr};
        //         if(temp == jednostrukaLista.trenutni_element) trenutni_element = origigi->element_nakon;
        //         if(temp == jednostrukaLista.posljednji_element) posljednji_element = origigi->element_nakon;
        //         origigi = origigi->element_nakon;
        //     }
        //     temp = temp->element_nakon;
        //     broj_elemenata++;
            
        // }
    }
    
    JednostrukaLista &operator=(const JednostrukaLista &jednostrukaLista){
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

    bool sljedeci() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        if(trenutni_element->element_nakon == nullptr) return false;
        trenutni_element = trenutni_element->element_nakon;
        return true;
    }
    bool prethodni() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        if(trenutni_element == prvi_element) return false;
        Cvor *ispred = nadjiIspredTrenutnog();
        // for(auto i = prvi_element; i !=nullptr; i = i->element_nakon){
        //     if(i->element_nakon == trenutni_element){
        //         ispred = i;
        //         break;
        //     }
        // }
        trenutni_element = ispred;
        return true;
    }

    void dodajIza(const TipEl &el) override{
        if(broj_elemenata == 0){
            Cvor *novi = new Cvor{el, nullptr};
            prvi_element = novi;
            posljednji_element = novi;
            trenutni_element = novi;
        }
        else{
            auto iza = trenutni_element->element_nakon;
            Cvor *novi = new Cvor{el, nullptr};
            if(iza) novi->element_nakon = iza;
            if(novi->element_nakon == nullptr) posljednji_element = novi;
            trenutni_element->element_nakon = novi;
            //azurirati krajnji
        }
        broj_elemenata++;
    }

    void pocetak() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        trenutni_element = prvi_element;
    }

    int brojElemenata() const override{
        return broj_elemenata;
    }

    void kraj() override{
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        trenutni_element = posljednji_element;
    }

    void obrisi(){
        if(broj_elemenata ==0) throw std::logic_error("Lista je prazna.");
        auto ispred = nadjiIspredTrenutnog();
        if(trenutni_element == prvi_element){
            auto iza = trenutni_element->element_nakon;
            delete trenutni_element;
            trenutni_element = iza;
            prvi_element = iza;
        }
        else if(ispred){
            auto iza = trenutni_element->element_nakon;
            delete trenutni_element;
            trenutni_element = nullptr;
            ispred->element_nakon = iza;
            if(iza) trenutni_element = iza;
            else trenutni_element = ispred;
            
        }
        else{
            delete trenutni_element;
            trenutni_element = nullptr;
            prvi_element = nullptr;
            posljednji_element = nullptr;
        }
        broj_elemenata--;

    }

    void dodajIspred(const TipEl &el) override{
        if(broj_elemenata == 0){
            Cvor *novi = new Cvor{el, nullptr};
            prvi_element = novi;
            //posljednji_element = novi;
            trenutni_element = novi;
        }
        else{
            Cvor *element_ispred = nadjiIspredTrenutnog();
            // for(auto i = prvi_element; i != nullptr; i = i-> element_nakon ){
            //     if(i-> element_nakon == trenutni_element){
            //         element_ispred = i;
            //         break;
            //     }
            // }
            Cvor *novi = new Cvor{el, trenutni_element};
            if(element_ispred){
                element_ispred->element_nakon = novi;
            }
            else {
                prvi_element = novi;
                posljednji_element = trenutni_element;
            }
        }
        broj_elemenata++;

    }

    TipEl &operator[](int index) override{
        if(index >= broj_elemenata) throw std::range_error("Nevalidan index.");
        Cvor *i_ti = nadjiElement(index);
        if(i_ti){
            return i_ti->vrijednost;
        }
        else{
            throw std::logic_error("nije oke nesto, popravi");
        }
    }

    TipEl operator[] (int index) const override{
        if(index >= broj_elemenata) throw std::range_error("Nevalidan index.");
        Cvor *i_ti = nadjiElement(index);
        if(i_ti){
            return i_ti->vrijednost;
        }
        else{
            throw std::logic_error("nije oke nesto, popravi");
        }
    }

    //debug
    void ispisi() const{
        for(auto i = prvi_element; i != nullptr; i = i->element_nakon){
            std::cout<<i->vrijednost<<std::endl;
        }
    }
    //debug
    // void ispisUnazad(){
    //     Cvor *pom = prvi_element;
    //     pomocna(prvi_element);
    
    // }
    
    // void pomocna(Cvor *cvor){
    //     if(cvor == prvi_element) return;
    //     pomocna
    // }
    

    
};

int main() {
    

JednostrukaLista<int> jednostrukaLista;
    jednostrukaLista.dodajIspred(1);
    jednostrukaLista.dodajIspred(2);
    jednostrukaLista.dodajIspred(7);
    jednostrukaLista.dodajIza(11);
    jednostrukaLista.dodajIza(8);
    jednostrukaLista.dodajIza(9);
    jednostrukaLista.dodajIza(10);
    //std::cout<<jednostrukaLista.trenutni();
    jednostrukaLista.obrisi();
    std::cout<<jednostrukaLista.trenutni()<<std::endl;
    jednostrukaLista.kraj();
    std::cout<<jednostrukaLista.trenutni()<<"\n";
    jednostrukaLista[0] = 33;
    std::cout<<jednostrukaLista[0];
    //jednostrukaLista.ispisi();
    JednostrukaLista<int> lista;
for (int i(1); i<=5; i++)
	lista.dodajIspred(i);
	
	Lista<int>* l;
l = new NizLista<int>;
for (int i(1); i<=5; i++)
	l->dodajIza(i);
do {
	std::cout << l->trenutni() << " ";
} while(l->sljedeci());
delete l;
	
{
    JednostrukaLista<int> lista2(lista);
    JednostrukaLista<int> lista3;
    //lista3=lista;
    lista.obrisi();
    std::cout << lista2.brojElemenata();
    //std::cout << " " << lista3.brojElemenata() << " ";
}
// std::cout << lista.brojElemenata();

return 0;
}
