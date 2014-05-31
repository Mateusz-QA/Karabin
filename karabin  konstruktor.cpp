#include <iostream>
#include <sstream>
#include <windows.h>

using namespace std;

class Pocisk
{
public:
    virtual int obrazenia()=0;
    virtual ~Pocisk(){};
    virtual Pocisk * Clone()=0;
    virtual int typ(){return 0;}
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
    virtual int typ(){return 1;}
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
    virtual int typ(){return 2;}
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
    virtual int typ(){return 3;}
};

enum Typ_bledu {
    Critical , Fatal , Warning
};

class Blad {
      int Typ_bledu_wybor;
  public:
      Blad(Typ_bledu Typ_bledu_wybor) {
          this->Typ_bledu_wybor = Typ_bledu_wybor;
      }
      virtual string Powod_bledu() = 0;
      virtual int Kod_bledu() = 0;
      virtual string Rodzaj_bledu() {
          return "##";
      }
      virtual string Wystapil_blad() {
          stringstream stream;
          switch ( Typ_bledu_wybor ) {
            case Critical:
                stream << "\nCritical ERROR 0x";
                break;
            case Fatal:
                stream << "\nFatal 0x";
                break;
            case Warning:
                stream << "\nWarning 0x";
                break;
            default:
                throw "\nUnknown Typ_bledu!";
        }
        return stream.str();
      }
      virtual string Inf_bledu() {
          stringstream stream;
          stream << Rodzaj_bledu() << Wystapil_blad() << std::hex<<Kod_bledu() << Rodzaj_bledu()
                 << "\n" << Powod_bledu() << "\n\n";
          return stream.str();
      }
};
class Blad_amunicji : public Blad {
  public:
      Blad_amunicji() : Blad(Critical) {}
      int Kod_bledu() {
          return 12;
      }
      string Powod_bledu() {
          return "Twoj numer naboju nie istnieje.";
      }
};
class Blad_znak : public Blad {
  public:
      Blad_znak() : Blad(Fatal) {}
      int Kod_bledu() {
          return 13;
      }
      string Powod_bledu() {
          return "Nieprawidlowy znak.";
      }
};
class Blad_pojemnosc : public Blad {
  public:
      Blad_pojemnosc() : Blad(Warning) {}
      int Kod_bledu() {
          return 14;
      }
      string Powod_bledu() {
          return "Magazynek nie moze byc pusty, moze miec max 30 kul.";
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
        cout<<"\noperator magazynek\n";
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
        int wybor=0;
        do
        {
            cout<<"Wybierz rodzaj naboi\n1 Zapalajacy\n2 Smog\n3 Odlamkowy\n";
            cin>>wybor;
            if ( wybor > 3 )
                throw Blad_amunicji();

            if(wybor==1)
              {
                naboje[numerPocisku] =new Zapalajacy;
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
        if(wybor<=0)
            throw Blad_znak();
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
        if ( ( ilePociskow > 30 ) or ( ilePociskow <= 0 ) )
            throw Blad_pojemnosc();

        liczbaPociskow=ilePociskow;
        wskaz_magazynek=new Magazynek;
    }
    //=============================================================================================
    Karabin(Karabin &wzor)                                   ///////////KONSTRUKTOR KOPIUJACY//////
    {
        cout<<"\noperator karabn\n";
        wskaz_magazynek=new Magazynek;
        *wskaz_magazynek=*(wzor.wskaz_magazynek);
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
    try
    {
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
    }
    catch (Blad & msg)
    {
        cout<< msg.Inf_bledu();
        return msg.Kod_bledu();
         //return 3;cout <<"BLAD!"<<msg.Kod_bledu()<<'\n';
    }
}

