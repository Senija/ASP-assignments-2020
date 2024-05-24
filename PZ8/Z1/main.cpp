#include<iostream>
#include<stdexcept>
#include <utility>
#include<iomanip>
#include<vector>
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
class BSCvor{
    public:
        BSCvor *lijevo = nullptr;
        BSCvor *desno = nullptr;
        BSCvor *roditelj = nullptr;
        TipKljuca kljuc;
        TipVrijednsti vrijednost;
        BSCvor(){
            // lijevo = nullptr;
            // desno = nullptr;
        }

        BSCvor(const TipKljuca &el, const TipVrijednsti &v, BSCvor *r=0, BSCvor *l=0, BSCvor *d = 0){
            kljuc = el;
            vrijednost = v;
            roditelj = r;
            lijevo = l;
            desno = d;
        }

};



template<typename TipKljuca, typename TipVrijednsti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednsti>{
    private:
    BSCvor<TipKljuca, TipVrijednsti> *root;
    int br_elemenata;
    BSCvor <TipKljuca, TipVrijednsti> *Trazi(BSCvor<TipKljuca, TipVrijednsti> *r, const TipKljuca &x) const{
        if(r == nullptr || r->kljuc == x){
            return r;
        }
        else if(x < r->kljuc)
            return Trazi(r->lijevo, x);
        else return Trazi(r->desno, x);
    }

    BSCvor<TipKljuca, TipVrijednsti> * Umetni(BSCvor<TipKljuca, TipVrijednsti>* &r, const TipKljuca &x){
        if(r==nullptr){
            r = new BSCvor<TipKljuca, TipVrijednsti>(x, TipVrijednsti(), nullptr, nullptr, nullptr); //kako saznati roditelja
            br_elemenata++;
            return r;
        }
        if(x < r->kljuc){
            if(r->lijevo == nullptr){
                r->lijevo = new BSCvor<TipKljuca, TipVrijednsti>(x, TipVrijednsti(), r, nullptr, nullptr);
                br_elemenata++;
                return r->lijevo;

            }
            else return Umetni(r->lijevo, x);
        }
        else{
                if(r->desno == nullptr){
                r->desno = new BSCvor<TipKljuca, TipVrijednsti>(x, TipVrijednsti(), r, nullptr, nullptr);
                br_elemenata++;
                return r->desno;

            }
            else return Umetni(r->desno, x);
        }
    }

    void Brisi(BSCvor<TipKljuca, TipVrijednsti> *r){
        if(br_elemenata == 0) return;
        if(r){
            Brisi(r->lijevo);
            Brisi(r->desno);
            delete r;
            r = nullptr;
        }
    }
    void BrisiCvor(BSCvor<TipKljuca, TipVrijednsti>* &korijen, const TipKljuca &k){
        BSCvor<TipKljuca, TipVrijednsti> *p = korijen;
        while(p!= nullptr && k!= p->kljuc){
            if(k < p->kljuc) p = p->lijevo;
            else p = p->desno;
        }
        if(p == nullptr) throw std::logic_error("Ne postoji vrijednost sa datim kljucem!");
        BSCvor<TipKljuca, TipVrijednsti> *roditelj = p->roditelj;
        BSCvor<TipKljuca, TipVrijednsti> *m = nullptr;
        if(p->lijevo == nullptr) m = p->desno;
        else if(p->desno == nullptr) m = p->lijevo;
        else{
                m = p->lijevo;
                BSCvor<TipKljuca, TipVrijednsti> *tmp = m->desno;
                while(tmp != nullptr){
                    m = tmp;
                    tmp = m->desno;
                }
                BSCvor<TipKljuca, TipVrijednsti> *pm = m->roditelj;
                if(pm != p){
                    pm->desno = m->lijevo;
                    m->lijevo = p->lijevo;
                }
                m->desno = p->desno;

        }

        if(roditelj == nullptr){
            korijen = m;
        }
        else if(p==roditelj->lijevo){
            roditelj->lijevo = m;

        }
        else roditelj->desno = m;

        delete p;
        p = nullptr;
    }

    
    void Copy(BSCvor<TipKljuca, TipVrijednsti> *p){
        //cout<<p->kljuc<<" ";
        //novo[p->kljuc] = p->vrijednost;
        Umetni(root, p->kljuc);
        (*this)[p->kljuc] = p->vrijednost;

    }
    void Preorder(BSCvor<TipKljuca, TipVrijednsti> *r){
        if(r != nullptr){
            Copy(r);
            Preorder(r->lijevo);
            Preorder(r->desno);
        }
    }
    
    public:
    BinStabloMapa(){
        root = nullptr;
        br_elemenata = 0;
    }

    ~BinStabloMapa(){
        Brisi(root);
        root = nullptr;
        br_elemenata = 0;
    }
    
    BinStabloMapa(const BinStabloMapa &novi){
        if(this == &novi) return;
        root = nullptr;
        br_elemenata = 0;
        Preorder(novi.root);
    }
    
    BinStabloMapa(BinStabloMapa &&novi):br_elemenata(novi.br_elemenata), root(novi.root){
        novi.root = nullptr;
        novi.br_elemenata = 0;
    }
    
    BinStabloMapa &operator=(const BinStabloMapa &novi){
        if(this == &novi) return *this;
        Brisi(root);
        root = nullptr;
        br_elemenata = 0;
        Preorder(novi.root);
        return *this;
        
    }
    
    BinStabloMapa &operator=(BinStabloMapa &&novi){
        if(this == &novi) return *this;
        std::swap(br_elemenata, novi.br_elemenata);
        std::swap(root, novi.root);
        return *this;
        
    }

    TipVrijednsti &operator[](const TipKljuca &kljuc) override{
        BSCvor<TipKljuca, TipVrijednsti> *novi = Trazi(root, kljuc);
        if(novi){
            return novi->vrijednost;
        }
        else{
            return Umetni(root, kljuc)->vrijednost;
            
        }

    }
    TipVrijednsti operator[](const TipKljuca &kljuc) const override{
        BSCvor<TipKljuca, TipVrijednsti> *novi = Trazi(root, kljuc);
        if(novi){
            return novi->vrijednost;
        }
        else{
            return TipVrijednsti();
            
        }

    }
    int brojElemenata() const override{
        return br_elemenata;
    }
    void obrisi() override{
        Brisi(root);
        root = nullptr;
        br_elemenata = 0;
    }
    void obrisi(const TipKljuca &kljuc) override{
        //broj elemenata --
        BrisiCvor(root, kljuc);
        br_elemenata--;


    }

};


int main(){
   

    return 0;
}