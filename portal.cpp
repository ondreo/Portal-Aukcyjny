#include<bits/stdc++.h>
#include<conio.h>
#define ff first
#define ss second
using namespace std;

struct aukcja
{
    int numer;
    string nazwa;
    string kategoria;
    bool status;//0-sprzedane, 1-dostepne
    double cena;
    string opis;
    pair<string,string>wlasc;
    pair<string,string>kupujacy;
    aukcja *pnast;
};
struct klient
{
    string imie;
    string nazwisko;
    double kw_kupna;
    double kw_sprzedazy;
    int il_kupionych;
    int il_sprzedanych;
    vector<int>lista_kupionych;
    vector<int>lista_sprzedanych;
};
map<string,klient>klienci;//nazwisko+" "+imie
map<string,klient>::iterator it1;
pair<string,int>kategorie[10000];
int likat=0;
aukcja *d_przedmiot_wg_nazwy(aukcja *pocz_wg_nazwy,aukcja *auk)
{
    //aukcja *nowy=(aukcja*)malloc(sizeof(aukcja));
    aukcja *nowy=new aukcja;
    nowy->nazwa=auk->nazwa;
    nowy->numer=auk->numer;
    nowy->kategoria=auk->kategoria;
    nowy->status=auk->status;
    nowy->cena=auk->cena;
    nowy->opis=auk->opis;
    nowy->wlasc=auk->wlasc;
    nowy->kupujacy=auk->kupujacy;
    nowy->pnast=NULL;
    if(pocz_wg_nazwy==NULL)return nowy;
    if(auk->nazwa<pocz_wg_nazwy->nazwa)
    {
        nowy->pnast=pocz_wg_nazwy;
        return nowy;
    }
    aukcja *tmp=pocz_wg_nazwy;
    while(tmp->pnast&&(tmp->pnast)->nazwa<auk->nazwa)tmp=tmp->pnast;
    nowy->pnast=tmp->pnast;
    tmp->pnast=nowy;
    return pocz_wg_nazwy;
}
aukcja *d_przedmiot_wg_ceny(aukcja *pocz_wg_ceny,aukcja *auk)
{
    aukcja *nowy=new aukcja;
    nowy->nazwa=auk->nazwa;
    nowy->numer=auk->numer;
    nowy->kategoria=auk->kategoria;
    nowy->status=auk->status;
    nowy->cena=auk->cena;
    nowy->opis=auk->opis;
    nowy->wlasc=auk->wlasc;
    nowy->kupujacy=auk->kupujacy;
    nowy->pnast=NULL;
    if(pocz_wg_ceny==NULL)return nowy;
    if(auk->cena<pocz_wg_ceny->cena)
    {
        nowy->pnast=pocz_wg_ceny;
        return nowy;
    }
    aukcja *tmp=pocz_wg_ceny;
    while(tmp->pnast&&(tmp->pnast)->cena<auk->cena)tmp=tmp->pnast;
    nowy->pnast=tmp->pnast;
    tmp->pnast=nowy;
    return pocz_wg_ceny;
}
pair<int,pair<double,string> >kup_produkt(aukcja *pocz_wg_nazwy,aukcja *pocz_wg_ceny,aukcja *auk)
{
    if(pocz_wg_nazwy==NULL)return make_pair(0,make_pair(0,""));
    aukcja *tmp=pocz_wg_nazwy;
    while(tmp->pnast&&tmp->numer!=auk->numer)tmp=tmp->pnast;
    if(tmp->numer!=auk->numer)return make_pair(0,make_pair(0,""));
    else if(tmp->status==0)return make_pair(1,make_pair(0,""));
    tmp->status=0;
    tmp->kupujacy=auk->kupujacy;
    auk->kategoria=tmp->kategoria;
    //
    tmp=pocz_wg_ceny;
    while(tmp->pnast&&tmp->numer!=auk->numer)tmp=tmp->pnast;
    tmp->status=0;
    tmp->kupujacy=auk->kupujacy;
    auk->kategoria=tmp->kategoria;
    return make_pair(2,make_pair(tmp->cena,tmp->wlasc.ss+" "+tmp->wlasc.ff));
}
aukcja *usun_produkt_wg_nazwy(aukcja *pocz_wg_nazwy,int numer)
{
    if(pocz_wg_nazwy==NULL)return pocz_wg_nazwy;
    aukcja *tmp=pocz_wg_nazwy;
    if(pocz_wg_nazwy->numer==numer)
    {
        for(int i=1;i<=likat;++i)
        {
            if(kategorie[i].ff==pocz_wg_nazwy->kategoria)
            {
                --kategorie[i].ss;
                break;
            }
        }
        tmp=pocz_wg_nazwy->pnast;
        if(klienci[pocz_wg_nazwy->kupujacy.ss+" "+pocz_wg_nazwy->kupujacy.ff].imie!="")
        {
            cout<<"Nie mozna usunac produktu - w bazie istnieje klient, ktory go kupil"<<endl;
            return pocz_wg_nazwy;
        }
        delete pocz_wg_nazwy;
        pocz_wg_nazwy=tmp;
        cout<<"Produkt usuniety pomyslnie"<<endl;
        return pocz_wg_nazwy;
    }
    while(tmp->pnast)
    {
        if((tmp->pnast)->numer==numer)
        {
            aukcja *tmp2=tmp->pnast;
            if(klienci[tmp2->kupujacy.ss+" "+tmp2->kupujacy.ff].imie!="")
            {
                cout<<"Nie mozna usunac produktu - w bazie istnieje klient, ktory go kupil"<<endl;
                return pocz_wg_nazwy;
            }
            for(int i=1;i<=likat;++i)
            {
                if(kategorie[i].ff==tmp2->kategoria)
                {
                    --kategorie[i].ss;
                    break;
                }
            }
            tmp->pnast=(tmp->pnast)->pnast;
            delete tmp2;
            cout<<"Produkt usuniety pomyslnie"<<endl;
            return pocz_wg_nazwy;
        }
        tmp=tmp->pnast;
    }
    cout<<"Nie ma produktu o podanym numerze"<<endl;
    return pocz_wg_nazwy;
}
aukcja *usun_produkt_wg_ceny(aukcja *pocz_wg_ceny,int numer)
{
    if(pocz_wg_ceny==NULL)return pocz_wg_ceny;
    aukcja *tmp=pocz_wg_ceny;
    if(pocz_wg_ceny->numer==numer)
    {
        tmp=pocz_wg_ceny->pnast;
        if(klienci[pocz_wg_ceny->kupujacy.ss+" "+pocz_wg_ceny->kupujacy.ff].imie!="")return pocz_wg_ceny;
        delete pocz_wg_ceny;
        pocz_wg_ceny=tmp;
        return pocz_wg_ceny;
    }
    while(tmp->pnast)
    {
        if((tmp->pnast)->numer==numer)
        {
            aukcja *tmp2=tmp->pnast;
            if(klienci[tmp2->kupujacy.ss+" "+tmp2->kupujacy.ff].imie!="")return pocz_wg_ceny;
            tmp->pnast=(tmp->pnast)->pnast;
            delete tmp2;
            return pocz_wg_ceny;
        }
        else tmp=tmp->pnast;
    }
    return pocz_wg_ceny;
}
void wypisz_wg_nazwy(aukcja *pocz_wg_nazwy,int nrkat)
{
    aukcja *tym=pocz_wg_nazwy;
    while(tym!=NULL)
    {
        if((nrkat==0||tym->kategoria==kategorie[nrkat].ff)&&tym->status==1)
        {
            cout<<"Numer: "<<tym->numer<<endl;
            cout<<"Nazwa: "<<tym->nazwa<<endl;
            cout<<"Kategoria: "<<tym->kategoria<<endl;
            cout<<"Cena: "<<fixed<<setprecision(2)<<tym->cena<<endl;
            cout<<"Opis:"<<endl<<tym->opis<<endl;
            cout<<"Wlasciciel produktu: "<<tym->wlasc.ff<<" "<<tym->wlasc.ss<<endl<<endl;
        }
        tym=tym->pnast;
    }
    return;
}
void wypisz_wg_ceny(aukcja *pocz_wg_ceny,int nrkat)
{
    aukcja *tym=pocz_wg_ceny;
    while(tym!=NULL)
    {
        if((nrkat==0||tym->kategoria==kategorie[nrkat].ff)&&tym->status==1)
        {
            cout<<"Numer: "<<tym->numer<<endl;
            cout<<"Nazwa: "<<tym->nazwa<<endl;
            cout<<"Kategoria: "<<tym->kategoria<<endl;
            cout<<"Cena: "<<fixed<<setprecision(2)<<tym->cena<<endl;
            cout<<"Opis:"<<endl<<tym->opis<<endl;
            cout<<"Wlasciciel produktu: "<<tym->wlasc.ff<<" "<<tym->wlasc.ss<<endl<<endl;
        }
        tym=tym->pnast;
    }
}
void wyszukaj_aukcje(aukcja *pocz_wg_nazwy,string fraza)
{
    aukcja *tym=pocz_wg_nazwy;
    while(tym!=NULL)
    {
        for(int i=0;i<fraza.length();++i)
        {
            if(i>=tym->nazwa.length()||fraza[i]!=tym->nazwa[i])break;
            if(i==fraza.length()-1)
            {
                cout<<"Numer: "<<tym->numer<<endl;
                cout<<"Nazwa: "<<tym->nazwa<<endl;
                cout<<"Kategoria: "<<tym->kategoria<<endl;
                cout<<"Status: "<<tym->status<<endl;
                cout<<"Cena: "<<fixed<<setprecision(2)<<tym->cena<<endl;
                cout<<"Opis:"<<endl<<tym->opis<<endl;
                cout<<"Wlasciciel produktu: "<<tym->wlasc.ff<<" "<<tym->wlasc.ss<<endl;
            }
        }
        tym=tym->pnast;
    }
}
void wypisz_do_pliku(aukcja *pocz_wg_nazwy)
{
    fstream p;
    p.open("aukcje.txt",ios::out);
    aukcja *tym=pocz_wg_nazwy;
    while(tym!=NULL)
    {
        p<<tym->numer<<endl;
        p<<tym->nazwa<<" /nazwa"<<endl;
        p<<tym->kategoria<<endl;
        p<<tym->status<<endl;
        p<<fixed<<setprecision(2)<<tym->cena<<endl;
        p<<tym->opis<<"/opis"<<endl;
        p<<tym->wlasc.ff<<" "<<tym->wlasc.ss<<endl;
        p<<tym->kupujacy.ff<<" "<<tym->kupujacy.ss<<endl;
        p<<"/przedmiot"<<endl<<endl;
        tym=tym->pnast;
    }
    p.close();
}
int main()
{
    aukcja *pocz_wg_nazwy=NULL;
    aukcja *pocz_wg_ceny=NULL;
    klient kli;
    aukcja auk;
    vector<pair<double,string> >kupno_sprzedaz;

    string wyr;
    int x,y;
    bool czy;
    char c;
    pair<int,pair<double,string> >sprzedaz;//udalo_sie?,cena,wlasc
    int linr=0;
    fstream p;
    p.open("klienci.txt",ios::in);//import klientow z pliku
    if(p)
    {
        while(p>>kli.imie>>kli.nazwisko>>kli.kw_kupna>>kli.kw_sprzedazy>>kli.il_kupionych>>kli.il_sprzedanych)
        {
            for(int i=1;i<=kli.il_kupionych;++i)
            {
                p>>x;
                kli.lista_kupionych.push_back(x);
            }
            for(int i=1;i<=kli.il_sprzedanych;++i)
            {
                p>>x;
                kli.lista_sprzedanych.push_back(x);
            }
            klienci[kli.nazwisko+" "+kli.imie]=kli;
        }
    }
    p.close();
    p.open("aukcje.txt",ios::in);//import aukcji z pliku
    if(p)
    {
        while(p>>auk.numer)
        {
            linr=max(linr,auk.numer);
            wyr=auk.nazwa="";
            while(wyr!="/nazwa")
            {
                if(auk.nazwa!="")auk.nazwa=auk.nazwa+" "+wyr;
                else if(wyr!="")auk.nazwa=wyr;
                p>>wyr;
            }
            p>>auk.kategoria>>auk.status>>auk.cena;
            czy=false;
            for(int i=1;i<=likat;++i)
            {
                if(kategorie[i].ff==auk.kategoria)
                {
                    ++kategorie[i].ss;
                    czy=true;
                    break;
                }
            }
            if(czy==false)
            {
                kategorie[++likat].ff=auk.kategoria;
                kategorie[likat].ss=1;
            }
            auk.opis=wyr="";
            czy=false;
            while(getline(p,wyr))
            {
                for(int i=0;i<wyr.length();++i)
                {
                    if(wyr[i]=='/'&&i+4<wyr.length()&&wyr[i+1]=='o'&&wyr[i+2]=='p'&&wyr[i+3]=='i'&&wyr[i+4]=='s')
                    {
                        czy=true;
                        break;
                    }
                    auk.opis=auk.opis+wyr[i];
                }
                if(czy==true)break;
                if(wyr!="")auk.opis=auk.opis+'\n';
            }
            p>>auk.wlasc.ff>>auk.wlasc.ss;
            p>>wyr;
            if(wyr=="/przedmiot")auk.kupujacy.ff=auk.kupujacy.ss="";
            else
            {
                auk.kupujacy.ff=wyr;
                p>>auk.kupujacy.ss;
                p>>wyr;
            }
            pocz_wg_nazwy=d_przedmiot_wg_nazwy(pocz_wg_nazwy,&auk);
            pocz_wg_ceny=d_przedmiot_wg_ceny(pocz_wg_ceny,&auk);
        }
    }
    p.close();
    linr=max(linr,0);
    while(1)
    {
        cout<<"Co chcesz zrobic?"<<endl;
        cout<<"1) Wystawic przedmiot"<<endl;
        cout<<"2) Kupic przedmiot"<<endl;
        cout<<"3) Usunac przedmiot"<<endl;
        cout<<"4) Wypisac wszystkie dostepne przedmioty"<<endl;
        cout<<"5) Wyszukac przedmiot"<<endl;
        cout<<"6) Dodac klienta"<<endl;
        cout<<"7) Usunac klienta"<<endl;
        cout<<"8) Wypisac klientow"<<endl;
        cout<<"9) Wyszukac klienta"<<endl;
        cout<<"10) Zakonczyc dzialanie programu"<<endl;
        cout<<"Wpisz cyfre: ";
        cin>>x;
        system("cls");
        if(x==1)
        {
            auk.numer=++linr;
            cout<<"Wczytaj nazwe przedmiotu (zakonczona /nazwa): ";
            wyr=auk.nazwa="";
            while(wyr!="/nazwa")
            {
                if(auk.nazwa!="")auk.nazwa=auk.nazwa+" "+wyr;
                else if(wyr!="")auk.nazwa=wyr;
                cin>>wyr;
            }
            cout<<"Wczytaj kategorie przedmiotu (1 wyraz): ";
            cin>>auk.kategoria;
            czy=false;
            for(int i=1;i<=likat;++i)
            {
                if(kategorie[i].ff==auk.kategoria)
                {
                    ++kategorie[i].ss;
                    czy=true;
                    break;
                }
            }
            if(czy==false)
            {
                kategorie[++likat].ff=auk.kategoria;
                kategorie[likat].ss=1;
            }
            auk.status=1;
            cout<<"Wczytaj cene przedmiotu: ";
            cin>>auk.cena;
            cout<<"Wczytaj opis przedmiotu (zakonczony /opis):"<<endl;
            wyr=auk.opis="";
            czy=false;
            while(1)
            {
                getline(cin,wyr);
                for(int i=0;i<wyr.length();++i)
                {
                    if(wyr[i]=='/'&&i+4<wyr.length()&&wyr[i+1]=='o'&&wyr[i+2]=='p'&&wyr[i+3]=='i'&&wyr[i+4]=='s')
                    {
                        czy=true;
                        break;
                    }
                    auk.opis=auk.opis+wyr[i];
                }
                if(czy==true)break;
                if(auk.opis!="")auk.opis=auk.opis+'\n';
            }
            cout<<"Wczytaj imie sprzedajacego: ";//jedno lub kilka polaczone znakiem podlogi
            cin>>auk.wlasc.ff;
            cout<<"Wczytaj nazwisko sprzedajacego: ";
            cin>>auk.wlasc.ss;
            auk.kupujacy.ff=auk.kupujacy.ss="";
            kli.imie=auk.wlasc.ff;
            kli.nazwisko=auk.wlasc.ss;
            kli.kw_kupna=kli.kw_sprzedazy=0;
            kli.il_kupionych=kli.il_sprzedanych=0;
            if(klienci[kli.nazwisko+" "+kli.imie].imie=="")klienci[kli.nazwisko+" "+kli.imie]=kli;
            pocz_wg_nazwy=d_przedmiot_wg_nazwy(pocz_wg_nazwy,&auk);
            pocz_wg_ceny=d_przedmiot_wg_ceny(pocz_wg_ceny,&auk);
        }
        else if(x==2)
        {
            cout<<"Wczytaj numer przedmiotu, ktory chcesz kupic: ";
            cin>>auk.numer;
            cout<<"Wczytaj imie kupujacego: ";
            cin>>auk.kupujacy.ff;
            cout<<"Wczytaj nazwisko kupujacego: ";
            cin>>auk.kupujacy.ss;
            sprzedaz=kup_produkt(pocz_wg_nazwy,pocz_wg_ceny,&auk);
            if(sprzedaz.ff==0)cout<<"Nie ma przedmiotu o podanym numerze"<<endl;
            else if(sprzedaz.ff==1)cout<<"Przedmiot o podanym numerze jest juz wyprzedany"<<endl;
            else if(sprzedaz.ff==2)
            {
                cout<<"Dokonano zakupu na kwote "<<fixed<<setprecision(2)<<sprzedaz.ss.ff<<" zl :)"<<endl;
                if(klienci[auk.kupujacy.ss+" "+auk.kupujacy.ff].imie!="")
                {
                    klienci[auk.kupujacy.ss+" "+auk.kupujacy.ff].kw_kupna+=sprzedaz.ss.ff;
                    klienci[auk.kupujacy.ss+" "+auk.kupujacy.ff].il_kupionych++;
                }
                else
                {
                    kli.imie=auk.kupujacy.ff;
                    kli.nazwisko=auk.kupujacy.ss;
                    kli.kw_kupna=sprzedaz.ss.ff;
                    kli.kw_sprzedazy=0;
                    kli.il_kupionych=1;
                    kli.il_sprzedanych=0;
                    klienci[auk.kupujacy.ss+" "+auk.kupujacy.ff]=kli;
                }
                klienci[auk.kupujacy.ss+" "+auk.kupujacy.ff].lista_kupionych.push_back(auk.numer);
                klienci[sprzedaz.ss.ss].kw_sprzedazy+=sprzedaz.ss.ff;
                klienci[sprzedaz.ss.ss].lista_sprzedanych.push_back(auk.numer);
                for(int i=1;i<=likat;++i)
                {
                    if(kategorie[i].ff==auk.kategoria)
                    {
                        --kategorie[i].ss;
                        break;
                    }
                }
            }
        }
        else if(x==3)
        {
            cout<<"Wczytaj numer przedmiotu do usuniecia: ";
            cin>>auk.numer;
            pocz_wg_nazwy=usun_produkt_wg_nazwy(pocz_wg_nazwy,auk.numer);
            pocz_wg_ceny=usun_produkt_wg_ceny(pocz_wg_ceny,auk.numer);
        }
        else if(x==4)
        {
            cout<<"Dostepne kategorie:"<<endl;
            for(int i=1;i<=likat;++i)
            {
                if(kategorie[i].ss>0)cout<<i<<") "<<kategorie[i].ff<<endl;
            }
            xy1:
            cout<<"Wpisz numer kategorii (0 oznacza wszystkie kategorie): ";
            cin>>y;
            if(y!=0&&kategorie[y].ss==0)
            {
                cout<<"Nie ma takiej kategorii lub nie zawiera ona zadnych przedmiotow"<<endl;
                goto xy1;
            }
            xy2:
            cout<<"Sortuj wg nazwy/ceny (n/c): ";
            cin>>c;
            if(c=='n')
            {
                cout<<endl;
                wypisz_wg_nazwy(pocz_wg_nazwy,y);
            }
            else if(c=='c')
            {
                cout<<endl;
                wypisz_wg_ceny(pocz_wg_ceny,y);
            }
            else
            {
                cout<<"Podano niewlasciwa opcje"<<endl;
                goto xy2;
            }
        }
        else if(x==5)
        {
            cout<<"Wpisz szukana fraze: ";
            cin>>wyr;
            wyszukaj_aukcje(pocz_wg_nazwy,wyr);
        }
        else if(x==6)
        {
            cout<<"Imie: ";
            cin>>kli.imie;
            cout<<"Nazwisko: ";
            cin>>kli.nazwisko;
            kli.kw_kupna=kli.kw_sprzedazy=0;
            kli.il_kupionych=kli.il_sprzedanych=0;
            if(klienci[kli.nazwisko+" "+kli.imie].imie=="")klienci[kli.nazwisko+" "+kli.imie]=kli;
            else cout<<"W bazie instnieje juz klient o takim samym imieniu i nazwisku!"<<endl;
        }
        else if(x==7)
        {
            cout<<"Wczytaj imie klienta do usuniecia: ";
            cin>>kli.imie;
            cout<<"Wczytaj nazwisko klienta do usuniecia: ";
            cin>>kli.nazwisko;
            if(klienci[kli.nazwisko+" "+kli.imie].imie!="")
            {
                klienci[kli.nazwisko+" "+kli.imie].imie="";
                klienci[kli.nazwisko+" "+kli.imie].nazwisko="";
                klienci[kli.nazwisko+" "+kli.imie].kw_kupna=0;
                klienci[kli.nazwisko+" "+kli.imie].kw_sprzedazy=0;
                klienci[kli.nazwisko+" "+kli.imie].il_kupionych=0;
                klienci[kli.nazwisko+" "+kli.imie].il_sprzedanych=0;
                while(!klienci[kli.nazwisko+" "+kli.imie].lista_kupionych.empty())klienci[kli.nazwisko+" "+kli.imie].lista_kupionych.pop_back();
                while(!klienci[kli.nazwisko+" "+kli.imie].lista_sprzedanych.empty())klienci[kli.nazwisko+" "+kli.imie].lista_sprzedanych.pop_back();
                cout<<"Klient pomyslnie usuniety z bazy"<<endl;
            }
            else cout<<"Nie ma takiego klienta"<<endl;
        }
        else if(x==8)
        {
            xy3:
            cout<<"Sortuj wg nazwiska/kwoty kupna/kwoty sprzedazy (n/k/s): ";
            cin>>c;
            if(c=='n')//wg_nazwiska
            {
                for(it1=klienci.begin();it1!=klienci.end();++it1)
                {
                    if(it1->ss.imie!="")
                    {
                        cout<<"Imie: "<<it1->ss.imie<<endl;
                        cout<<"Nazwisko: "<<it1->ss.nazwisko<<endl;
                        cout<<"Laczna kwota kupna: "<<it1->ss.kw_kupna<<endl;
                        cout<<"Laczna kwota sprzedazy: "<<it1->ss.kw_sprzedazy<<endl;
                        cout<<"Lista kupionych: "<<endl;
                        for(int i=0;i<it1->ss.lista_kupionych.size();++i)cout<<it1->ss.lista_kupionych[i]<<endl;
                        cout<<"Lista sprzedanych: "<<endl;
                        for(int i=0;i<it1->ss.lista_sprzedanych.size();++i)cout<<it1->ss.lista_sprzedanych[i]<<endl;
                        cout<<endl;
                    }
                }
            }

            else if(c=='k')//wg_kupna
            {
                for(it1=klienci.begin();it1!=klienci.end();++it1)
                {
                    if(it1->ss.imie!="")kupno_sprzedaz.push_back(make_pair(it1->ss.kw_kupna,it1->ss.nazwisko+" "+it1->ss.imie));
                }
                sort(kupno_sprzedaz.begin(),kupno_sprzedaz.end());
                for(int j=0;j<kupno_sprzedaz.size();++j)
                {
                    cout<<"Imie: "<<klienci[kupno_sprzedaz[j].ss].imie<<endl;
                    cout<<"Nazwisko: "<<klienci[kupno_sprzedaz[j].ss].nazwisko<<endl;
                    cout<<"Laczna kwota kupna: "<<klienci[kupno_sprzedaz[j].ss].kw_kupna<<endl;
                    cout<<"Laczna kwota sprzedazy: "<<klienci[kupno_sprzedaz[j].ss].kw_sprzedazy<<endl;
                    cout<<"ilosc kupionych: "<<klienci[kupno_sprzedaz[j].ss].il_kupionych<<endl;//tymczasowo
                    cout<<"ilosc kupionych2: "<<klienci[kupno_sprzedaz[j].ss].lista_kupionych.size() <<endl;//tymczasowo
                    cout<<"Lista kupionych: "<<endl;
                    for(int i=0;i<klienci[kupno_sprzedaz[j].ss].lista_kupionych.size();++i)cout<<klienci[kupno_sprzedaz[j].ss].lista_kupionych[i]<<endl;
                    cout<<"Lista sprzedanych: "<<endl;
                    for(int i=0;i<klienci[kupno_sprzedaz[j].ss].lista_sprzedanych.size();++i)cout<<klienci[kupno_sprzedaz[j].ss].lista_sprzedanych[i]<<endl;
                    cout<<endl;
                }
            }
            else if(c=='s')//wg sprzedazy
            {
                for(it1=klienci.begin();it1!=klienci.end();++it1)
                {
                    if(it1->ss.imie!="")kupno_sprzedaz.push_back(make_pair(it1->ss.kw_sprzedazy,it1->ss.nazwisko+" "+it1->ss.imie));
                }
                sort(kupno_sprzedaz.begin(),kupno_sprzedaz.end());
                for(int j=0;j<kupno_sprzedaz.size();++j)
                {
                    cout<<"Imie: "<<klienci[kupno_sprzedaz[j].ss].imie<<endl;
                    cout<<"Nazwisko: "<<klienci[kupno_sprzedaz[j].ss].nazwisko<<endl;
                    cout<<"Laczna kwota kupna: "<<klienci[kupno_sprzedaz[j].ss].kw_kupna<<endl;
                    cout<<"Laczna kwota sprzedazy: "<<klienci[kupno_sprzedaz[j].ss].kw_sprzedazy<<endl;
                    cout<<"Lista kupionych: "<<endl;
                    for(int i=0;i<klienci[kupno_sprzedaz[j].ss].lista_kupionych.size();++i)cout<<klienci[kupno_sprzedaz[j].ss].lista_kupionych[i]<<endl;
                    cout<<"Lista sprzedanych: "<<endl;
                    for(int i=0;i<klienci[kupno_sprzedaz[j].ss].lista_sprzedanych.size();++i)cout<<klienci[kupno_sprzedaz[j].ss].lista_sprzedanych[i]<<endl;
                    cout<<endl;
                }
            }
            else
            {
                cout<<"Podano niewlasciwa opcje"<<endl;
                goto xy3;
            }
        }
        else if(x==9)
        {
            cout<<"Wczytaj szukana fraze: ";
            cin>>wyr;
            for(it1=klienci.begin();it1!=klienci.end();++it1)
            {
                for(int i=0;i<wyr.length();++i)
                {
                    if(i>=it1->ss.nazwisko.length()||wyr[i]!=it1->ss.nazwisko[i])break;
                    if(i==wyr.length()-1)
                    {
                        cout<<it1->ss.imie<<" "<<it1->ss.nazwisko<<endl;
                    }
                }
            }
        }
        else if(x==10)
        {
            p.open("klienci.txt",ios::out);//eksport klientow do pliku
            for(it1=klienci.begin();it1!=klienci.end();++it1)
            {
                if(it1->ss.imie!="")
                {
                    p<<it1->ss.imie<<endl;
                    p<<it1->ss.nazwisko<<endl;
                    p<<it1->ss.kw_kupna<<endl;
                    p<<it1->ss.kw_sprzedazy<<endl;
                    p<<it1->ss.il_kupionych<<endl;
                    p<<it1->ss.il_sprzedanych<<endl;
                    for(int i=0;i<it1->ss.il_kupionych;++i)p<<it1->ss.lista_kupionych[i]<<endl;
                    for(int i=0;i<it1->ss.il_sprzedanych;++i)p<<it1->ss.lista_sprzedanych[i]<<endl;
                    cout<<endl;
                }

            }
            p.close();
            wypisz_do_pliku(pocz_wg_nazwy);//eksport aukcji do pliku
            return 0;
        }
        cout<<"Aby kontynuowac, nacisnij dowolny klawisz... ";
        getch();
        system("cls");
    }
}
