#include <iostream>
#include <stdlib.h>

using namespace std;

class Pocisk
{
public:
    virtual int obrazenia()=0;
    virtual ~Pocisk(){};
    virtual Pocisk * Clone()=0;
};

class Zapalajacy:public Pocisk
{
public:
    virtual int obrazenia()
    {
        cout<<"Zapalajacy::obrazenia()\n";
        return 50;
    }
    Zapalajacy(){}
    virtual ~Zapalajacy(){}
    virtual Zapalajacy* Clone()
    {
        cout<<"Zapalajacy::Clone()\n";
        return new Zapalajacy;
    }
};

class Odlamkowy:public Pocisk
{
public:
    virtual int obrazenia()
    {
        cout<<"Odlamkowy::obrazenia()\n";
        return 80;
    }
    Odlamkowy(){}
    virtual ~Odlamkowy(){}
    virtual Odlamkowy* Clone()
    {
        cout<<"Odlamkowy::Clone()\n";
        return new Odlamkowy;
    }
};

class Smog:public Pocisk
{
    int biezaceObrazenia;
    public:
     Smog()
     {
        biezaceObrazenia = 10;
     }
     virtual int obrazenia()
    {
        cout<<"Smog::obrazenia()\n";
        return biezaceObrazenia;
    }
    virtual ~Smog(){}
    virtual Smog* Clone()
    {
        cout<<"Smog::Clone()\n";
        Smog* kopia = new Smog;
        (*kopia).biezaceObrazenia= (*this).biezaceObrazenia;
        return kopia;
    }
};

class Magazynek
{
    Pocisk *naboje[30];
    int numerPocisku,lacznie;

    public:
    Magazynek()
    {
        for(int i=0;i<30;i++)
        {
          naboje[i]=NULL;
        }
        lacznie=0;
        numerPocisku=0;
    }
    //=============================================================================================
    Magazynek(Magazynek &wzor)                 ///////////////KONSTRUKTOR KOPIUJACY////////////////
    {
        lacznie=0;
        for(int i=0;i<30;i++)
        {
           naboje[i]=NULL;
        }
        for(int i=0;i<wzor.numerPocisku;i++)
        {
           naboje[i]=(*(wzor.naboje[i])).Clone();
        }
        numerPocisku = wzor.numerPocisku;
    }
    //=============================================================================================
    void Wybierz()
    {
        int wybor;
        do
        {
            cout<<"Wybierz rodzaj naboi\n1 Zapalajacy\n2 Smog\n3 Odlamkowy\n";
            cin>>wybor;
            if(wybor==1)
              {
                naboje[numerPocisku] = new Zapalajacy;
                numerPocisku++;
              }

            if(wybor==2)
              {
                naboje[numerPocisku]=new Smog;
                numerPocisku++;
              }

            if(wybor==3)
              {
                naboje[numerPocisku]=new Odlamkowy;
                numerPocisku++;
              }
          }while(wybor>3);
    }
    //=============================================================================================
    int obrazenia_seri()
    {
        for (int i=0;i<numerPocisku;i++)
        {
            lacznie=lacznie+naboje[i]->obrazenia();
        }
        return lacznie;
    }
    //=============================================================================================
    virtual~ Magazynek()                                     ////////////////DESTRUKTOR////////////
    {
        for(int i=0;i<numerPocisku;i++)
        {
                delete naboje[i];

                cout<<"\nDestruktor "<<i<<" dziala! ";
        }
    }
    //=============================================================================================
    Magazynek& operator=(Magazynek &wzor)                   /////////OPERATOR PRZYPISANIA//////////
    {
       if(this!=&wzor)
        {
           lacznie=0;
           for(int i=0;i<numerPocisku;i++)
           {
              delete naboje[i];
              naboje[i] = NULL;
              cout<<"\nDestruktor "<<i<<" dziala! ";
           }
           for(int i=0;i<wzor.numerPocisku;i++)
           {
               naboje[i]=(*(wzor.naboje[i])).Clone();
           }

            numerPocisku=wzor.numerPocisku;
           }
          return *this;
    }
};

class Karabin
{
    int liczbaPociskow;
    Magazynek *wskaz_magazynek;
    public:
    //=============================================================================================
    Karabin(int ilePociskow)
    {
        liczbaPociskow = ilePociskow;
        if(30<liczbaPociskow)
        {
            cout << "\nMagazybek miesci maksymalnie 30 naboi!\n";
            cout<<" \nWybierz liczbe naboi, max 30 \n  ";
            cin >> ilePociskow;
            liczbaPociskow=ilePociskow;
        }
        wskaz_magazynek=new Magazynek;
    }
    //=============================================================================================
    void Wybierz()
    {
        for(int i=0; i<liczbaPociskow;i++)
        {
            (*wskaz_magazynek).Wybierz();
        }
    }
    //=============================================================================================
    int obrazenia_seri()
    {
        return (*wskaz_magazynek).obrazenia_seri();
    }
    //=============================================================================================
    virtual ~Karabin()                                       ////////////////DESTRUKTOR////////////
    {
        delete wskaz_magazynek;
        cout<<"\nDestruktor Karabinu dziala! ";
    }
    //=============================================================================================
    Karabin(Karabin &wzor)                                   ///////////KONSTRUKTOR KOPIUJACY//////
    {
        cout<<"\noperator karabn\n";
        wskaz_magazynek=new Magazynek;
        *wskaz_magazynek=*(wzor.wskaz_magazynek);
    }
    //=============================================================================================
    Karabin&operator=(Karabin &wzor)                         /////////oPERATOR PRZYPISANIA/////////
    {
        if(this!=&wzor)
        {
            cout<<"\noperator karabn\n";
            delete wskaz_magazynek;
            wskaz_magazynek=new Magazynek;
            *wskaz_magazynek=*(wzor.wskaz_magazynek);
        }
    return *this;
    }
};

int main()
{
    int ile;
    cout<<"\nStworz swoj wlasny Karabin ! \nWybierz liczbe naboi, max 30 \n  ";
    cin>>ile;
    Karabin wskaz_magazynek(ile);
    wskaz_magazynek.Wybierz();
    cout<< " \nZadales "<<wskaz_magazynek.obrazenia_seri()<<" obrazen!";
    cout<<"\nII - Stworz swoj wlasny Karabin ! \nWybierz liczbe naboi, max 30 \n  ";
    cin>>ile;
    Karabin obj1(ile);
    obj1.Wybierz();
    cout<< " \nOBJ1::Zadales "<<obj1.obrazenia_seri()<<" obrazen!";
    obj1 = wskaz_magazynek;
    cout<< " \nOBJ1_II::Zadales "<<obj1.obrazenia_seri()<<" obrazen!";
    return 0;
}
