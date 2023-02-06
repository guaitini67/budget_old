#include <commdlg.h>
#include <print.h>
#include <owl.h>
#include <stdlib.h>
#include <string.h>
#include <dialog.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <combobox.h>
#include <edit.h>
#include <button.h>
#include <ctype.h>
#include <mmsystem.h>
#include <time.h>
#include <groupbox.h>
#include <radiobut.h>

#include "totali.h"


#include "budget.h"     //contiene le definizioni degli identificatori
#include "tcomgio.h"    //contiene TComboGiov
#include "globfun.h"    //contiene le funzioni globali
#include "tdiafield.h"   //contiene una o piu' classi
#include "tnotedia.h"   //contiene una o piu' classi
#include "tgiovwin.h"   //contiene una o piu' classi


// *********************** CLASSE TOggiDia *******************************************

_CLASSDEF(TOggiDia)

class TOggiDia:public TDialog
  {
    public:

      char string[400],string1[400],
	   nomefile_in[30],nomefile_out[30],
	   entra,nomeconv[27],mot[100],cat[100],
           mot_f[100],cat_f[100];
      int  i,j,k,g,m,a,viene_da,va_in,n_lpa,n_lpb,
	   posiz,fine,tab[5],pa,pb,
	   numfonti,nrighe,nlnote,nanni,num_ops;
      long int importo,numg_out,numg_in,saldo,
	       numg_fa,numg_fb,importo_f,numg_riepilogo;
      struct info_risorse
        {
	  int posiz;
          char nomeconv[27];
	  long int aperto,chiuso;
	} tabella_risorse[100];
      struct info_movimenti
	{
	  int posiz_da;
          int posiz_in;
	  int numop_a;
          int numop_b;
	  long int numg_a;
	  long int numg_b;
	} tabella_movimenti[100];


      struct time Time;
      struct tm tcheck;
      struct date Date;
      FILE *map,*fileout,*filein,*fil,*tmp,*ntt;
      PTGroupBox GRUP;
      PTRadioButton PULSE,PULSU;
      PTComboGiov CATE;
      PTEdit IMPO,MOTI;
      PTListBox RIEPILO;
      PTStatic whatday;
      PTGiovWindow SUM;
      PTComboGiov EGior,EMes,EAnn,UGior,UMes,UAnn,
		  RiGior,RiMes,RiAnn,DESTINA,PROVEN,
		  SELDAT,SELBANC;
      TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,char* giorno);

      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      virtual void WMTimer()=[WM_FIRST + WM_TIMER];
      virtual void SetupWindow();
      virtual ~TOggiDia();
      int  caricaVoci();
      void caricaLista(PTComboGiov GGior,
      		       PTComboGiov MMes,
		       PTComboGiov AAnn,
		       PTComboGiov RISORSA);
      void HandAnnulla()=[ID_FIRST+ANNUL];
      void HandCambia()=[ID_FIRST+CAMBIA];
      void HandCheckEntrata(RTMessage Msg)=[ID_FIRST+PULSENTR];
      void HandCheckUscita(RTMessage Msg)=[ID_FIRST+PULSUSC];
      void HandGDopo()=[ID_FIRST+AVANTI];
      void HandGPrima()=[ID_FIRST+DIETRO];
      void HandNote()=[ID_FIRST+OG_NOTE];
      void HandOK()=[ID_FIRST+OG_OK];
      void HandSel(RTMessage Msg)=[ID_FIRST+C_RIEPILOG];
      void HandScrivi()=[ID_FIRST+SCRIVI];
      void HandVai()=[ID_FIRST+OG_VAI];
      void HandValGen(RTMessage Msg)=[ID_FIRST+ID_GEN];
      void HandValMen(RTMessage Msg)=[ID_FIRST+ID_MEN];
      void HandValAen(RTMessage Msg)=[ID_FIRST+ID_AEN];
      void HandValGusc(RTMessage Msg)=[ID_FIRST+ID_GUSC];
      void HandValMusc(RTMessage Msg)=[ID_FIRST+ID_MUSC];
      void HandValAusc(RTMessage Msg)=[ID_FIRST+ID_AUSC];
      void HandValRiG(RTMessage Msg)=[ID_FIRST+ID_GG];
      void HandValRiM(RTMessage Msg)=[ID_FIRST+ID_MM];
      void HandValRiA(RTMessage Msg)=[ID_FIRST+ID_AAAA];
      int  leggiprepara(long int num_g,char *nome_file);
      int  scrivifile(char *nome_file,long int num_g,
		      char *datastring);
      int  scrivinote(char *nome_file,long int num_g);
      int  riepiloga(long int num_g);

//      void cancella(PTListBox,long int *,long int *);
  };


// ************************** FUNZIONI DI TOggiDia **************************************

TOggiDia::TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,
                   char* giorno)
  :TDialog(AParent,ResourceId)
  {
    IMPO=new TEdit(this,IMPORTO,25,NULL);
    MOTI=new TEdit(this,MOTIVO,40,NULL);
    CATE=new TComboGiov(this,CATEG,40,NULL);
    EGior=new TComboGiov(this,ID_GEN,3,NULL);
    EMes=new TComboGiov(this,ID_MEN,3,NULL);
    EAnn=new TComboGiov(this,ID_AEN,5,NULL);
    UGior=new TComboGiov(this,ID_GUSC,3,NULL);
    UMes=new TComboGiov(this,ID_MUSC,3,NULL);
    UAnn=new TComboGiov(this,ID_AUSC,5,NULL);
    RiGior=new TComboGiov(this,ID_GG,3,NULL);
    RiMes=new TComboGiov(this,ID_MM,3,NULL);
    RiAnn=new TComboGiov(this,ID_AAAA,5,NULL);
    GRUP=new TGroupBox(this,GRUPPO,NULL);
    PULSE=new TRadioButton(this,PULSENTR,GRUP,NULL);
    PULSU=new TRadioButton(this,PULSUSC,GRUP,NULL);
    RIEPILO=new TListBox(this,C_RIEPILOG,NULL);
    DESTINA=new TComboGiov(this,C_BANCA_IN,25,NULL);
    PROVEN=new TComboGiov(this,C_BANCA_OUT,25,NULL);
    SELDAT=new TComboGiov(this,C_BANCA_OUT,25,NULL);
    SELBANC=new TComboGiov(this,SELBANCA,25,NULL);
// punti di stop per i tabulatori nella
//finestra di riepilogo
    tab[0]=50;                              
    tab[1]=100;                             
    tab[2]=150;
//nlnote=numero righe di note. La metto a zero all'inizio
//(e' necessario?)
    nlnote=0;
    strcpy(string,giorno);          //copio giorno in str1 per evitare strani errori
    sscanf(string,"%02d-%02d-%04d",&g,&m,&a);
  }

TOggiDia::~TOggiDia()
  {
    KillTimer(HWindow,1);
  }


LPSTR TOggiDia::GetClassName()
  {
    return "day";
  }

void TOggiDia::GetWindowClass(WNDCLASS& AWndClass)
  {
    TDialog::GetWindowClass(AWndClass);
  }

void TOggiDia::WMTimer()
  {
    gettime(&Time);
    getdate(&Date);
    sprintf(string,"%02d : %02d : %02d",Time.ti_hour,Time.ti_min,Time.ti_sec);
    SetDlgItemText(HWindow,CLOCK,string);
    sprintf(string,"%02d / %02d / %02d",Date.da_day,Date.da_mon,Date.da_year);
    SetDlgItemText(HWindow,DATA,string);
  }

void TOggiDia::SetupWindow()
  {
    TDialog::SetupWindow();
// messaggi che impostano i tabulatori
    SendMessage(RIEPILO->HWindow,
    LB_SETTABSTOPS,3,(DWORD)tab);
//
//Default: Uscita (Triste verita'!)
    PULSU->Check();
//Inizializzo la data di uscita e quella di riepilogo
//al valore arrivato dal costruttore della classe
    sprintf(string,"%02d",g);
    UGior->Insert(string);
    RiGior->Insert(string);
    sprintf(string,"%02d",m);
    UMes->Insert(string);
    RiMes->Insert(string);
    sprintf(string,"%04d",a);
    UAnn->Insert(string);
    RiAnn->Insert(string);
    numg_out=(long int)a*10000+m*100+g;
    entra=0;
//caricavoci mette in una struttura alcune info
//importanti di basedata.dat
    if(caricaVoci()<0)
      {
//C'e' stato un problema nella lettura di basedata.dat
	MessageBox(HWindow,"Anomalia nel file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return;
      }
//Carico una lista per 'esce da'
//compatibile col giorno corrente
    caricaLista(UGior,UMes,UAnn,PROVEN);
//Carico una lista per 'entra in'
//compatibile col giorno corrente
//Notare che uso comunque la data di uscita
//perche' non ho ancora quella di entrata
    caricaLista(UGior,UMes,UAnn,DESTINA);
//Carico una lista per box riepilogo
//compatibile col giorno corrente
    caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Di default, carico il campo 'esce da' e riepilogo
//con la prima voce (che nelle mie idee e' 'contanti')
    PROVEN->GetString(string,0);
    PROVEN->Insert(string);
    SELBANC->Insert(string);
    riepiloga(numg_out);
  }

int TOggiDia::caricaVoci()
  {
    if(!(map=fopen("basedata.dat","r")))
      {
//Non trovo basedata.dat: errore
	MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    DESTINA->ClearList();
    PROVEN->ClearList();
    SELBANC->ClearList();
    if(!fgets(string,100,map))
      {
//Non trovo niente in basedata.dat: errore
	MessageBox(HWindow,
	"Non ci sono risorse in 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	return -1;
      }
    if(sscanf(string,"%d\n",&numfonti)!=1)
      {
//Formattazione non corretta: errore
        MessageBox(HWindow,"Anomalia nel file dati",
	"Problema !",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	return -1;
      }
    for(i=0;i<numfonti;i++)
      {
	if(!fgets(string,100,map))
	  {
//Mi aspettavo di leggere una riga e non la trovo: errore
            MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
          }
	if(sscanf(string,"%d,%[!-z ]\n",&posiz,
	   nomeconv)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Ho superato quanto c'era prima
//Copio il numero risorsa e il nome nel record
	tabella_risorse[i].posiz=posiz;
        strcpy(tabella_risorse[i].nomeconv,nomeconv);
//Leggo il giono di apertura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giono di apertura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di apertura
//Scrivo -1 nel record          
	  tabella_risorse[i].aperto=-1;
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//C'e' il giono di apertura e lo scrivo nel record
//(notare il cast al secondo membro)
	  tabella_risorse[i].aperto=
	  (long int)a*10000+m*100+g;
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Leggo il giono di chiusura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giono di chiusura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di chiusura
//Scrivo -1 nel record          
          tabella_risorse[i].chiuso=-1;
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//C'e' il giono di chiusura e lo scrivo nel record
//(notare il cast al secondo membro)
          tabella_risorse[i].chiuso=
	  (long int)a*10000+m*100+g;
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
	if(sscanf(string,"%d,%d\n",&nrighe,&nanni)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
          }
	for(j=0;j<nrighe+nanni;j++)
	  if(!fgets(string,100,map))
	    {
//Mi aspettavo di leggere una riga e non la trovo: errore
	      MessageBox(HWindow,"Anomalia nel file dati",
	      "Problema!",MB_ICONEXCLAMATION | MB_OK);
	      fclose(map);
	      return -1;
	    }
      }
    fclose(map);
    return 0;
  }

void TOggiDia::caricaLista(PTComboGiov GGior,
      		           PTComboGiov MMes,
		           PTComboGiov AAnn,
		           PTComboGiov RISORSA)
  {

    int a,m,g,j;
    long int num_g;
    char nomeconv[27];

    j=0;
//Controllo se avevo scritto qualcosa
//Se e' cosi', lo memorizzo
    if(RISORSA->GetText(nomeconv,27))
      j=1;
    RISORSA->ClearList();
    GGior->GetText(string,3);
    g=atoi(string);
    MMes->GetText(string,3);
    m=atoi(string);
    AAnn->GetText(string,5);
    a=atoi(string);
    num_g=(long int)a*10000+m*100+g;
    for(i=0;i<numfonti;i++)
      {
//Casi in cui aggiungero' la risorsa alla lista:
	if(
//1: non ho specificato data di apertura e chiusura
	  ((tabella_risorse[i].aperto==-1)&&
	  (tabella_risorse[i].chiuso==-1))
        ||
//2: ho specificato solo la data di chiusura, ma
//questa e' posteriore al giorno che mi interessa
	  ((tabella_risorse[i].aperto==-1)&&
	  (tabella_risorse[i].chiuso>num_g))
        ||
//3: ho specificato solo la data di apertura, ma
//questa e' anteriore al giorno che mi interessa
	  ((tabella_risorse[i].aperto<num_g)&&
	  (tabella_risorse[i].chiuso==-1))
        ||
//4: ho specificato entrambe le date, e
//il giorno che mi interessa e'in mezzo
	  ((tabella_risorse[i].aperto<num_g)&&
	  (tabella_risorse[i].chiuso>num_g))
	  )
	  {
	    RISORSA->AddString(tabella_risorse[i].nomeconv);
	  }
      }
//Se c'era qualcosa scritto, lo rimetto dov'era
//(ci pensera' HandScrivi a controllare
//se e' una stringa corretta)
    if(j)
      RISORSA->Insert(nomeconv);
  }

void TOggiDia::HandValGen(RTMessage Msg)
  {
    if((!(DESTINA->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'entra in' 
	if(!controllacampi(EGior,EMes,EAnn))
//Solo se la data e' valida,
//Aggiorna la lista di entrata
	  caricaLista(EGior,EMes,EAnn,DESTINA);
	return;
      }
//Se ho scritto 'entra in'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
	EGior->SetEditSel(0,2);
	EGior->GetText(string,3);
	EGior->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
      }
  }

void TOggiDia::HandValMen(RTMessage Msg)
  {
    if((!(DESTINA->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'entra in' 
	if(!controllacampi(EGior,EMes,EAnn))
//Solo se la data e' valida,
//Aggiorna la lista di entrata
	  caricaLista(EGior,EMes,EAnn,DESTINA);
	return;
      }
//Se ho scritto 'entra in'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
	EMes->SetEditSel(0,2);
	EMes->GetText(string,3);
	EMes->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
      }
  }

void TOggiDia::HandValAen(RTMessage Msg)
  {
    if((!(DESTINA->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'entra in' 
	if(!controllacampi(EGior,EMes,EAnn))
//Solo se la data e' valida,
//Aggiorna la lista di entrata
	  caricaLista(EGior,EMes,EAnn,DESTINA);
	return;
      }
//Se ho scritto 'entra in'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
	EAnn->SetEditSel(0,4);
	EAnn->GetText(string,5);
	EAnn->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(EGior,EMes,EAnn);
//Aggiorna la lista
	caricaLista(EGior,EMes,EAnn,DESTINA);
      }
  }

void TOggiDia::HandValGusc(RTMessage Msg)
  {
    if((!(PROVEN->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'esce da' 
	if(!controllacampi(UGior,UMes,UAnn))
//Solo se la data e' valida,
//Aggiorna la lista di uscita
	  caricaLista(UGior,UMes,UAnn,PROVEN);
	return;
      }
//Se ho scritto 'esce da'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
        caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
	UGior->SetEditSel(0,2);
	UGior->GetText(string,3);
	UGior->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
        caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
      }
  }

void TOggiDia::HandValMusc(RTMessage Msg)
  {
    if((!(PROVEN->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'esce da' 
	if(!controllacampi(UGior,UMes,UAnn))
//Solo se la data e' valida,
//Aggiorna la lista di uscita
	  caricaLista(UGior,UMes,UAnn,PROVEN);
	return;
      }
//Se ho scritto 'esce da'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
	UMes->SetEditSel(0,2);
	UMes->GetText(string,3);
	UMes->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
        caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
      }
  }

void TOggiDia::HandValAusc(RTMessage Msg)
  {
    if((!(PROVEN->GetText(string,27)))
       &&
       ((Msg.LP.Hi==CBN_SETFOCUS)
       ||(Msg.LP.Hi==CBN_KILLFOCUS))
      )
      {
//Non ho scritto 'esce da' 
	if(!controllacampi(UGior,UMes,UAnn))
//Solo se la data e' valida,
//Aggiorna la lista di uscita
	  caricaLista(UGior,UMes,UAnn,PROVEN);
	return;
      }
//Se ho scritto 'esce da'
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
	UAnn->SetEditSel(0,4);
	UAnn->GetText(string,5);
	UAnn->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
        caricacampi(UGior,UMes,UAnn);
//Aggiorna la lista
	caricaLista(UGior,UMes,UAnn,PROVEN);
      }
  }

void TOggiDia::HandValRiG(RTMessage Msg)
  {

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
        caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
	RiGior->SetEditSel(0,2);
	RiGior->GetText(string,3);
	RiGior->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
      }
  }

void TOggiDia::HandValRiM(RTMessage Msg)
  {

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
	RiMes->SetEditSel(0,2);
	RiMes->GetText(string,3);
	RiMes->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
      }
  }

void TOggiDia::HandValRiA(RTMessage Msg)
  {

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
	RiAnn->SetEditSel(0,4);
	RiAnn->GetText(string,5);
	RiAnn->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
      }
  }

void TOggiDia::HandScrivi()
  {
    if(entra)
      {
//Scrivo un'entrata:
//Controllo che i dati in entrata siano a posto
//(obbligatorio)
//Quindi vedo se la somma in entrata viene
//da un'altra mia risorsa
	if(!(DESTINA->GetText(string,27)))
	  {
	    MessageBox(HWindow,"Inserire 'Entra in'",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
	if((k=DESTINA->FindExactString(string,-1))<0)
	  {
	    MessageBox(HWindow,
	    "'Entra in' inserito non corrisponde a \
una voce valida",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Individuo il numero della risorsa selezionata
	i=0;
	while(strcmp(string,tabella_risorse[i].nomeconv))
	  i++;
	va_in=tabella_risorse[i].posiz;
	IMPO->GetText(string,27);
	k=sscanf(string,"%ld%s",&importo,nomeconv);
	if(k!=1)
          {
	    MessageBox(HWindow,
	    "Importo inserito non e' un numero valido",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
          }
	if(!(MOTI->GetText(mot,42)))
	  {
	    MessageBox(HWindow,"Inserire Motivo",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
	if(!(CATE->GetText(cat,42)))
	  {
	    MessageBox(HWindow,"Inserire Categoria",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
	EAnn->GetText(string,5);
	sscanf(string,"%d",&a);
	EMes->GetText(string,3);
	sscanf(string,"%d",&m);
	EGior->GetText(string,3);
	sscanf(string,"%d",&g);
	numg_in=(long int)a*10000+m*100+g;
//Individuo il file su cui scrivere l'entrata
	sprintf(nomefile_in,"%d.bud",a);
//Individuo il numero d'ordine dell'entrata
//nel giorno scelto (serve per il link)
	if((n_lpa=leggiprepara(numg_in,nomefile_in))<0)
	  {
	    MessageBox(HWindow,
	    "Anomalia nel file dati entrata",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Controllo preliminare sulla data di uscita
//(in questa fase mi e' concesso
//lasciarla in bianco o scriverla male)
	if((j=controllacampi(UGior,UMes,UAnn))<0)
          {
//Se non e' un giorno valido:
//Aggiorno la lista di uscita con il giorno
//di entrata (mi puo' servire dopo)
	    caricaLista(EGior,EMes,EAnn,PROVEN);
	  }
//Verifico se l'entrata esce da un'altra mia risorsa
	if(!(PROVEN->GetText(string,27)))
	  {
//No, non esce da un'altra mia risorsa
	    n_lpb=0;
	    viene_da=0;
	    numg_out=0;
	  }
	else if((k=PROVEN->FindExactString(string,-1))<0)
          {
//Ho  scritto qualcosa, ma non corrisponde a niente:
//chiedo lumi sul da farsi:
//o faccio come se non avessi scritto niente
//o annullo
	    if
            (
	    MessageBox(HWindow,
	    "'Esce da' inserita non e'una voce valida.\
La ignoro e vado avanti (e' come se non l'avessi scritta)?",
	    "Problema!",MB_ICONEXCLAMATION | MB_YESNO)
	    ==IDNO
	    )
//Ho deciso che voglio correggere prima di scrivere
	      return;
//Se sono qui vuol dire che voglio ignorarla
	    n_lpb=0;
	    viene_da=0;
            numg_out=0;
	    PROVEN->Clear();
	    UAnn->Clear();
	    UMes->Clear();
            UGior->Clear();
          }
	else
	  {
//Si, esce da un'altra risorsa
//Devo scrivere anche questa informazione
//
//Prima mi accerto della data inserita
	    if(j<0)
	      {
//Se non e' un giorno valido chiedo lumi:
//o tengo il giorno dell'entrata
//o annullo
	        if
                (
	        MessageBox(HWindow,
		"La data di uscita non e' corretta \
o manca. Uso lo stesso giorno dell'entrata?",
	        "Problema!",MB_ICONEXCLAMATION | MB_YESNO)
	        ==IDNO
	        )
//Ho deciso che voglio correggere prima di scrivere
		  return;
//Se sono qui allora ricopio i campi di entrata
//nell'uscita
	        EAnn->GetText(string1,5);
		UAnn->Insert(string1);
		EMes->GetText(string1,3);
		UMes->Insert(string1);
		EGior->GetText(string1,3);
		UGior->Insert(string1);
	      }
//Individuo il numero della risorsa selezionata
	    i=0;
	    while(strcmp(string,tabella_risorse[i].nomeconv))
              i++;          
	    viene_da=tabella_risorse[i].posiz;
	    UAnn->GetText(string,5);
	    sscanf(string,"%d",&a);
	    UMes->GetText(string,3);
	    sscanf(string,"%d",&m);
	    UGior->GetText(string,3);
	    sscanf(string,"%d",&g);
	    numg_out=(long int)a*10000+m*100+g;
//Individuo il file su cui scrivere l'uscita
//Potrebbe essere diverso da quello di entrata
//(se gli anni sono diversi)
	    sprintf(nomefile_out,"%d.bud",a);
//Individuo il numero d'ordine dell'uscita
//nel giorno scelto (serve per il link)
	    if((n_lpb=leggiprepara(numg_out,nomefile_out))<0)
	      {
		MessageBox(HWindow,
		"Anomalia nel file dati uscita",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
                return;
	      }
	    if(n_lpb==n_lpa)
//Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
//Questo e' un errore, questa e' la soluzione
		n_lpa=n_lpb+1;
//Ho tutte le informazioni per scrivere nel file di uscita
//Preparo la stringa da scrivere nel file
	    sprintf(string,"%d,-%ld,%s\t,%s\t,%d,%d,%ld,%d",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_in,n_lpa);
//Scrivo la stringa nel file di uscita
	    if(scrivifile(nomefile_out,numg_out,string)<0)
	      {
		MessageBox(HWindow,
		"Anomalia nel file dati uscita",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		return;
	      }
	  }
//Ho tutte le informazioni per scrivere nel file di entrata
//Preparo la stringa da scrivere nel file
        sprintf(string,"%d,+%ld,%s\t,%s\t,%d,%d,%ld,%d",
		n_lpa,importo,mot,cat,viene_da,va_in,
		numg_out,n_lpb);
//Scrivo la stringa nel file di entrata
	if(scrivifile(nomefile_in,numg_in,string)<0)
	  {
	    MessageBox(HWindow,
	    "Anomalia nel file dati entrata",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Ora i file sono stati aggiornati con successo
//Ripulisco i campi importo, motivo, categoria
//ma non gli altri.
	IMPO->Clear();
	MOTI->Clear();
	CATE->Clear();
//Quindi aggiorno il riepilogo sul giorno dell'entrata
	riepiloga(numg_in);
      }
    else
      {
//Scrivo un'uscita:
//Controllo che i dati in uscita siano a posto
//(obbligatorio)
//Quindi vedo se la somma in uscita va
//in un'altra mia risorsa
	if(!(PROVEN->GetText(string,27)))
	  {
	    MessageBox(HWindow,"Inserire 'Esce da'",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
	if((k=PROVEN->FindExactString(string,-1))<0)
	  {
	    MessageBox(HWindow,
	    "'Esce da' inserito non corrisponde \
a una voce valida",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Individuo il numero della risorsa selezionata
        i=0;
	while(strcmp(string,tabella_risorse[i].nomeconv))
          i++;          
	viene_da=tabella_risorse[i].posiz;
	IMPO->GetText(string,27);
	k=sscanf(string,"%ld%s",&importo,nomeconv);
	if(k!=1)
          {
	    MessageBox(HWindow,
	    "Importo inserito non e' un numero valido",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
          }
	if(!(MOTI->GetText(mot,42)))
	  {
	    MessageBox(HWindow,"Inserire Motivo",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
	if(!(CATE->GetText(cat,42)))
	  {
	    MessageBox(HWindow,"Inserire Categoria",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
	UAnn->GetText(string,5);
	sscanf(string,"%d",&a);
	UMes->GetText(string,3);
	sscanf(string,"%d",&m);
	UGior->GetText(string,3);
	sscanf(string,"%d",&g);
	numg_out=(long int)a*10000+m*100+g;
//Individuo il file su cui scrivere l'uscita
	sprintf(nomefile_out,"%d.bud",a);
//Individuo il numero d'ordine dell'uscita
//nel giorno scelto (serve per il link)
	if((n_lpa=leggiprepara(numg_out,nomefile_out))<0)
	  {
	    MessageBox(HWindow,
	    "Anomalia nel file dati uscita",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Controllo preliminare sulla data di entrata
//(in questa fase mi e' concesso
//lasciarla in bianco o scriverla male)
	if((j=controllacampi(EGior,EMes,EAnn))<0)
          {
//Se non e' un giorno valido:
//Aggiorno la lista di entrata con il giorno
//di uscita (mi puo' servire dopo)
	    caricaLista(UGior,UMes,UAnn,DESTINA);
	  }
//Verifico se l'uscita entra in un'altra mia risorsa
	if(!(DESTINA->GetText(string,27)))
	  {
//No, non entra in un'altra mia risorsa
	    n_lpb=0;
	    va_in=0;
	    numg_in=0;
	  }
	else if((va_in=DESTINA->FindExactString(string,-1))<0)
          {
//Ho  scritto qualcosa, ma non corrisponde a niente:
//chiedo lumi sul da farsi:
//o faccio come se non avessi scritto niente
//o annullo
	    if
            (
	    MessageBox(HWindow,
	    "'Entra in' inserita non e'una voce valida.\
La ignoro e vado avanti (e' come se non l'avessi scritta)?",
	    "Problema!",MB_ICONEXCLAMATION | MB_YESNO)
	    ==IDNO
	    )
//Ho deciso che voglio correggere prima di scrivere
	      return;
//Se sono qui vuol dire che voglio ignorarla
	    n_lpb=0;
	    va_in=0;
	    numg_in=0;
	    DESTINA->Clear();
	    EAnn->Clear();
	    EMes->Clear();
            EGior->Clear();
          }
	else
	  {
//Si, entra in un'altra risorsa
//Devo scrivere anche questa informazione
//
//Prima mi accerto della data inserita
	    if(j<0)
	      {
//Se non e' un giorno valido chiedo lumi:
//o tengo il giorno dell'uscita
//o annullo
	        if
                (
	        MessageBox(HWindow,
		"La data di entrata non e' corretta \
o manca. Uso lo stesso giorno dell'uscita?",
	        "Problema!",MB_ICONEXCLAMATION | MB_YESNO)
	        ==IDNO
	        )
//Ho deciso che voglio correggere prima di scrivere
		  return;
//Se sono qui allora ricopio i campi di uscita
//nell'entrata
	        UAnn->GetText(string1,5);
		EAnn->Insert(string1);
		UMes->GetText(string1,3);
		EMes->Insert(string1);
		UGior->GetText(string1,3);
		EGior->Insert(string1);
	      }
//Individuo il numero della risorsa selezionata
	    i=0;
	    while(strcmp(string,tabella_risorse[i].nomeconv))
              i++;          
	    va_in=tabella_risorse[i].posiz;
	    EAnn->GetText(string,5);
	    sscanf(string,"%d",&a);
	    EMes->GetText(string,3);
	    sscanf(string,"%d",&m);
	    EGior->GetText(string,3);
	    sscanf(string,"%d",&g);
	    numg_in=(long int)a*10000+m*100+g;
//Individuo il file su cui scrivere l'entrata
//Potrebbe essere diverso da quello di uscita
//(se gli anni sono diversi)
	    sprintf(nomefile_in,"%d.bud",a);
//Individuo il numero d'ordine dell'entrata
//nel giorno scelto (serve per il link)
	    if
	      ((n_lpb=leggiprepara(numg_in,nomefile_in))<0)
	      {
		MessageBox(HWindow,
		"Anomalia nel file dati entrata",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		return;
	      }
	    if(n_lpb==n_lpa)
//Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
//Questo e' un errore, questa e' la soluzione
		n_lpa=n_lpb+1;
//Ho tutte le informazioni per scrivere nel file di entrata
//Preparo la stringa da scrivere nel file
	    sprintf(string,"%d,+%ld,%s\t,%s\t,%d,%d,%ld,%d",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_out,n_lpa);
//Scrivo la stringa nel file di entrata
	    if(scrivifile(nomefile_in,numg_in,string)<0)
	      {
		MessageBox(HWindow,
		"Anomalia nel file dati entrata",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		return;
	      }
	  }
//Ho tutte le informazioni per scrivere nel file di uscita
//Preparo la stringa da scrivere nel file
	sprintf(string,"%d,-%ld,%s\t,%s\t,%d,%d,%ld,%d",
		n_lpa,importo,mot,cat,viene_da,va_in,
		numg_in,n_lpb);
//Scrivo la stringa nel file di uscita
        if(scrivifile(nomefile_out,numg_out,string)<0)
	  {
	    MessageBox(HWindow,
	    "Anomalia nel file dati uscita",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
//Ora i file sono stati aggiornati con successo
//Ripulisco i campi importo, motivo, categoria
//ma non gli altri.
	IMPO->Clear();
	MOTI->Clear();
	CATE->Clear();
//Quindi aggiorno il riepilogo sul giorno dell'uscita
	riepiloga(numg_out);
      }
  }

int  TOggiDia::leggiprepara(long int num_g,char *nome_file)
//Trova il prossimo numero di operazione disponibile
//per il giorno selezionato: se non c'era niente il
//numero e' 1, se c'era qualcosa e' 'operazione
//con numero piu' alto+1'. Se errore: -1
  {
    if(!(fil=fopen(nome_file,"r")))
//Se il file e' nuovo restituisco 1
      return 1;
    do
      {
	if(!fgets(string,400,fil))
	  {
//Ho raggiunto la fine del file
//Restituisco 1
	    fclose(fil);
	    return 1;
          }
	if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    MessageBox(HWindow,"Anomalia nel file dati:1",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    return -1;
	  }
	if((numg_fa<num_g))
//Sono a un giorno precedente a quello desiderato
	  for(i=0;i<num_ops+nrighe;i++)
	    if(!fgets(string,400,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
	        MessageBox(HWindow,
		"Anomalia nel file dati:2",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	fclose(fil);
		return -1;
	      }
      }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Restituisco 1
        fclose(fil);
        return 1;
      }
//Se sono qui e' perche' numg_fa==num_g:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Individuo il prossimo numero disponibile
//e lo restituisco
    for(i=0;i<num_ops;i++)
      {
        if(!fgets(string,300,fil))
          {
//Mi aspettavo una riga e invece non trovo niente: errore
            MessageBox(HWindow,"Anomalia nel file dati:3",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(fil);
            return -1;
          }
        if(sscanf(string,
          "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
          &j,&importo_f,mot_f,cat_f,&pa,&pb,
          &numg_fb,&k)!=8)
          {
//Trovo i dati in un formato non valido: errore
            MessageBox(HWindow,"Anomalia nel file dati:4",
            "Problema!",MB_ICONEXCLAMATION | MB_OK);
            fclose(fil);
            return -1;
          }
      }
//Se sono arrivato qui il prossimo numero
//disponibile e' j+1 e lo restituisco
    fclose(fil);
    return (j+1);
  }


int TOggiDia::scrivifile(char *nome_file,long int num_g,
		char *datastring)
//Struttura del file:
//giorno
//giorno
//.
//.
//dove giorno e':
//
//num_g,n_o,n_r   (num_g=aaaammgg)
//n1,+/-imp,mot	,cat ,viene_da,va_in,num_g1,m1
//.
//.
//nn_o,+/-imp,mot, cat,	viene_da,va_in,num_gn_o,mn_o
//riga note 1
//.
//.
//riga note n_r
// 
//N.B. se non ho dati in un certo giorno
//quel giorno non compare nel file
  {
    strcpy(string1,datastring);
    if(!(fil=fopen(nome_file,"r")))
      {
	fil=fopen(nome_file,"w");
	fclose(fil);
	fil=fopen(nome_file,"r");
      }
    tmp=fopen("temp","w");
    numg_fa=0;
    numg_fb=0;
    fine=0;
//Entro nella fase di aggiornamento dei dati
    do
      {
	if(!fgets(string,400,fil))
//Ho raggiunto la fine del file
	  fine=1;
	else if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    MessageBox(HWindow,"Anomalia nel file dati:5",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    fclose(tmp);
            return -1;
	  }
	if((numg_fa<num_g)&&(!fine))
	  {
//Sono a un giorno precedente a quello desiderato
	    fprintf(tmp,"%s",string);
	    for(i=0;i<num_ops;i++)
	      {
		if(!fgets(string,400,fil))
		  {
//Mi aspettavo una riga e invece non trovo niente: errore
	            MessageBox(HWindow,"Anomalia nel file dati:6",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
	    	    fclose(tmp);
            	    return -1;
		  }
		if(sscanf(string,
		"%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
		&j,&importo_f,mot_f,cat_f,&pa,&pb,
		&numg_fb,&k)!=8)
		  {
//Trovo i dati in un formato non valido: errore
		    MessageBox(HWindow,"Anomalia nel file dati:7",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
	    	    fclose(tmp);
            	    return -1;
		  }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	        fprintf(tmp,"%s",string);
	      }
	    for(i=0;i<nrighe;i++)
	      {
		if(!fgets(string,300,fil))
		  {
//Mi aspettavo una riga (di note)
//e invece non trovo niente: errore
		    MessageBox(HWindow,"Anomalia nel file dati:8",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
	    	    fclose(tmp);
            	    return -1;
		  }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
		fprintf(tmp,"%s",string);
	      }
	  }
      }
    while((numg_fa<num_g)&&(!fine));
    if(fine)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere e quelli successivi
//non contenevano gia' dei dati (ho raggiunto la fine)
//Lo scrivo ex novo e poi il file finira'
        fprintf(tmp,"%ld,1,0\n",num_g);
//Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",string1);
      }  
    else if(numg_fa==num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Aggiungo i nuovi dati, e poi gli appendo (fuori da questo 'do')
//i giorni successivi (se ci sono)
        fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops+1,
	nrighe);
	for(i=0;i<num_ops;i++)
	  {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
		MessageBox(HWindow,"Anomalia nel file dati:9",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	    	fclose(tmp);
            	return -1;
	      }
	    if(sscanf(string,
	    "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
	    &j,&importo_f,mot_f,cat_f,&pa,&pb,
	    &numg_fb,&k)!=8)
	      {
//Trovo i dati in un formato non valido: errore
	        MessageBox(HWindow,"Anomalia nel file dati:10",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
//Raggiunta la fine dei dati del giorno
//Adesso posso scrivere la nuova stringa
	fprintf(tmp,"%s\n",string1);
	for(i=0;i<nrighe;i++)
	  {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga (di note)
//e invece non trovo niente: errore
	        MessageBox(HWindow,"Anomalia nel file dati:11",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
    else if(numg_fa>num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Lo scrivo ex novo e poi gli devo appendere
//quello che segue
        fprintf(tmp,"%ld,1,0\n",num_g);
//Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",string1);
//Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",string);
        for(i=0;i<num_ops;i++)
          {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
	        MessageBox(HWindow,"Anomalia nel file dati:12",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                return -1;
	      }
	    if(sscanf(string,
	      "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
	      &j,&importo_f,mot_f,cat_f,&pa,&pb,
	      &numg_fb,&k)!=8)
	      {
//Trovo i dati in un formato non valido: errore
	        MessageBox(HWindow,"Anomalia nel file dati",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
	for(i=0;i<nrighe;i++)
	  {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga (di note)
//e invece non trovo niente: errore
	        MessageBox(HWindow,"Anomalia nel file dati:13",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
//A questo punto i nuovi dati sono dentro
//Ricopio quello che segue (se c'e') e finisco
//NOTA: non faccio nessun controllo sui dati dei giorni
//successivi. Se c'e' qualche problema me ne accorgero'
//accedendo a quei giorni in seguito.
    if(!fine)
      while(!feof(fil))
        {
          fgets(string,300,fil);
	  if(!feof(fil))
	    fprintf(tmp,"%s",string);
        }
    fclose(fil);
    fclose(tmp);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
  }

int  TOggiDia::riepiloga(long int num_g)
//Scorre il file selezionato alla ricerca di num_g
//e quando lo trova (se lo trova) riempie il campo di
//riepilogo
//Inoltre scrive le note nel file "note.tmp"  
  {
    int g,m,a;
    long int pippo;

    RIEPILO->ClearList();
//numg_riepilogo viene aggiornato solo
//in questa funzione.
//Mi viene comodo per la gestione delle note
    numg_riepilogo=num_g;
//Individuo da num_g anno, mese, giorno
    g= num_g%100;
    pippo= num_g/100;
    m= pippo%100;
    a=pippo/100;
//Scrivo la data nei campi data riepilogo
    sprintf(string,"%02d",g);
    RiGior->Insert(string);
    sprintf(string,"%02d",m);
    RiMes->Insert(string);
    sprintf(string,"%02d",a);
    RiAnn->Insert(string);
//Individuo nome file da aprire
    sprintf(string,"%d.bud",a);
    if(!(tmp=fopen("note.tmp","w")))
      {
//Non riesco a creare il file: errore (precauzione eccessiva?)
        MessageBox(HWindow,
        "Non riesco a creare il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    if(!(fil=fopen(string,"r")))
//Se il file non esiste ritorno normalmente
//con un note.tmp vuoto e nlnote=0
      {
        nlnote=0;
	fclose(tmp);
	return 0;
      }
    do
      {
	if(!fgets(string,400,fil))
	  {
//Ho raggiunto la fine del file senza trovare
//il giorno. Ritorno normalmente con un note.tmp vuoto
//e nlnote=0
            nlnote=0;
//Non ci sono righe di commento, non scrive nulla
            SetDlgItemText(HWindow,ANNOTAZ,"");
	    fclose(tmp);
	    fclose(fil);
	    return 0;
          }
	if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    MessageBox(HWindow,
	    "Anomalia nel file dati:r1",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(tmp);
	    fclose(fil);
	    return -1;
	  }
	if((numg_fa<num_g))
//Sono a un giorno precedente a quello desiderato
	  for(i=0;i<num_ops+nrighe;i++)
	    if(!fgets(string,400,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:r2",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(tmp);
		fclose(fil);
		return -1;
	      }
      }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
      {
//Se sono qui:
//Il giorno che mi interessa 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Ritorno normalmente con un note.tmp vuoto
//e nlnote=0
        nlnote=0;
//Non ci sono righe di commento, non scrive nulla
        SetDlgItemText(HWindow,ANNOTAZ,"");
	fclose(tmp);
	fclose(fil);
	return 0;
      }
//Se sono qui: numg_fa==num_g
//Il giorno che mi interessa contiene gia' dei dati
//Comincia la scrittura della casella
    for(i=0;i<num_ops;i++)
      {
	if(!fgets(string,300,fil))
	  {
//Mi aspettavo una riga e invece non trovo niente: errore
	    MessageBox(HWindow,
	    "Anomalia nel file dati:r3",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(tmp);
	    fclose(fil);
	    return -1;
	  }
	if(sscanf(string,
	  "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
	  &j,&importo_f,mot_f,cat_f,&pa,&pb,
	  &numg_fb,&k)!=8)
	  {
//Trovo i dati in un formato non valido: errore
            MessageBox(HWindow,
	    "Anomalia nel file dati:r4",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(tmp);
	    fclose(fil);
	    return -1;
	  }
//Se sono arrivato qui tutto e' a posto
//Posso scrivere nella casella di riepilogo
	if(importo_f>0)
	  {
//Se e' un'entrata
            sprintf(string,
"+%ld\t| %s\t| %s",importo_f,mot_f,cat_f);
	  }
	else
	  {
//Se e' un'uscita
	    sprintf(string,
"%ld\t| %s\t| %s",importo_f,mot_f,cat_f);
          }
        RIEPILO->AddString(string);
//Ora scrivo in un record le informazioni relative
//a questo movimento. Mi serviranno per cancellare
//e per selezionare e cose simili.
        tabella_movimenti[i].posiz_da=pa;
	tabella_movimenti[i].posiz_in=pb;
        tabella_movimenti[i].numop_a=j;
        tabella_movimenti[i].numop_b=k;
        tabella_movimenti[i].numg_a=numg_fa;
        tabella_movimenti[i].numg_b=numg_fb;
      }  
//Se sono arrivato qui ho finito il riempimento
//Ora controllo le righe di note: se ci sono
//le appoggio sul file "note.tmp"
    for(i=0;i<nrighe;i++)
      {
        if(!fgets(string,300,fil))
          {
//Mi aspettavo una riga e invece non trovo niente: errore
            MessageBox(HWindow,
            "Anomalia nel file dati:r5",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    fclose(tmp);
	    return -1;
	  }
//Se sono qui posso ricopiare su note.tmp la riga
	fputs(string,tmp);
      }
    if(nrighe)
//Se ci sono righe di commento, lo scrive
      SetDlgItemText(HWindow,ANNOTAZ,
      "Ci sono annotazioni"); 
    else
//Se non ci sono righe di commento, non scrive nulla
      SetDlgItemText(HWindow,ANNOTAZ,"");
//nlnote indica il numero di righe di note
//viene assegnato all'inizio,
//in questa funzione o dopo aver scritto le note
    nlnote=nrighe;
    fclose(tmp);
    fclose(fil);
    return 0;
  }


void TOggiDia::HandCambia()
  {
  }

void TOggiDia::HandAnnulla()
  {
//Cancella i campi e ripristina una configurazione di default
    int g,m,a;

    IMPO->Clear();
    MOTI->Clear();
    CATE->Clear();
    DESTINA->Clear();
    PROVEN->Clear();
    if(entra)
      {
//Se e' selezionato il pulsante entrata
//cancello i campi relativi a uscita,
//mantengo la data dell'entrata, e metto
//su "va in" il primo elemento della lista
	UGior->Clear();
	UMes->Clear();
	UAnn->Clear();
	caricacampi(EGior,EMes,EAnn);
	caricaLista(EGior,EMes,EAnn,DESTINA);
	DESTINA->GetString(string,0);
        DESTINA->Insert(string);
      }
    else
      {
//Se e' selezionato il pulsante uscita
//cancello i campi relativi a data entrata,
//mantengo la data dell'uscita, e metto
//su "viene da" il primo elemento della lista
	EGior->Clear();
	EMes->Clear();
	EAnn->Clear();
	caricacampi(UGior,UMes,UAnn);
	caricaLista(UGior,UMes,UAnn,PROVEN);
	PROVEN->GetString(string,0);
        PROVEN->Insert(string);
      }
  }

void TOggiDia::HandNote()
  {
    int a;
    long int pippo;

    GetModule()->ExecDialog(new TNoteDia(this,"NOTE",
                            &nlnote,numg_riepilogo));
//Individuo il file su cui scrivere l'uscita
    pippo= numg_riepilogo/100;
    a=pippo/100;
    sprintf(nomefile_in,"%d.bud",a);
    scrivinote(nomefile_in,numg_riepilogo);
    riepiloga(numg_riepilogo);
  }

int TOggiDia::scrivinote(char *nome_file,long int num_g)
//Questa funzione e' simile a scrivifile
//ma serve a scrivere le note e non i movimenti
  {
    if(!(fil=fopen(nome_file,"r")))
      {
	fil=fopen(nome_file,"w");
	fclose(fil);
	fil=fopen(nome_file,"r");
      }
    tmp=fopen("temp","w");
    if(!(ntt=fopen("note.tmp","r")))
      {
	MessageBox(HWindow,
        "Manca il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(fil);
	fclose(tmp);
	return -1;
      }
    numg_fa=0;
    numg_fb=0;
    fine=0;
//Entro nella fase di aggiornamento dei dati
    do
      {
	if(!fgets(string,400,fil))
//Ho raggiunto la fine del file
	  fine=1;
	else if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    MessageBox(HWindow,"Anomalia nel file dati:5",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    fclose(tmp);
            fclose(ntt);
            return -1;
	  }
	if((numg_fa<num_g)&&(!fine))
	  {
//Sono a un giorno precedente a quello desiderato
	    fprintf(tmp,"%s",string);
	    for(i=0;i<num_ops;i++)
	      {
		if(!fgets(string,400,fil))
		  {
//Mi aspettavo una riga e invece non trovo niente: errore
	            MessageBox(HWindow,"Anomalia nel file dati:6",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
		    fclose(tmp);
                    fclose(ntt);
            	    return -1;
		  }
		if(sscanf(string,
		"%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
		&j,&importo_f,mot_f,cat_f,&pa,&pb,
		&numg_fb,&k)!=8)
		  {
//Trovo i dati in un formato non valido: errore
		    MessageBox(HWindow,"Anomalia nel file dati:7",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
		    fclose(tmp);
                    fclose(ntt);
            	    return -1;
		  }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	        fprintf(tmp,"%s",string);
	      }
	    for(i=0;i<nrighe;i++)
	      {
		if(!fgets(string,300,fil))
		  {
//Mi aspettavo una riga (di note)
//e invece non trovo niente: errore
		    MessageBox(HWindow,"Anomalia nel file dati:8",
	            "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    	    fclose(fil);
		    fclose(tmp);
                    fclose(ntt);
		    return -1;
		  }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
		fprintf(tmp,"%s",string);
	      }
	  }
      }
    while((numg_fa<num_g)&&(!fine));
    if(fine)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere e quelli successivi
//non contenevano gia' dei dati (ho raggiunto la fine)
//e ho delle note da scrivere
//Lo scrivo ex novo e poi il file finira'
	if(nlnote)
//Solo se ho note da scrivere creo il giorno
	  fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,300,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
      }
    else if(numg_fa==num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Qui non mi importa se non ho note da scrivere
//perche' qualcosa devo mettere comunque
//Aggiungo i nuovi dati, e poi gli appendo 
//i giorni successivi (se ci sono)
        fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops,
	nlnote);
	for(i=0;i<num_ops;i++)
	  {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
		MessageBox(HWindow,"Anomalia nel file dati:9",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
		fclose(tmp);
                fclose(ntt);
            	return -1;
	      }
	    if(sscanf(string,
	    "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
	    &j,&importo_f,mot_f,cat_f,&pa,&pb,
	    &numg_fb,&k)!=8)
	      {
//Trovo i dati in un formato non valido: errore
	        MessageBox(HWindow,"Anomalia nel file dati:10",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
		fclose(tmp);
                fclose(ntt);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
//Raggiunta la fine dei dati del giorno
//Faccio scorrere in avanti il file originale
//fino alla fine del giorno
	for(i=0;i<nrighe;i++)
	  if(!fgets(string,300,fil))
	    {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	      MessageBox(HWindow,
	      "Anomalia nel file dati: punto scorrimento",
	      "Problema!",MB_ICONEXCLAMATION | MB_OK);
	      fclose(fil);
	      fclose(tmp);
              fclose(ntt);
	      return -1;
	    }
//Adesso posso scrivere le nuove note
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,300,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
      }
    else if((numg_fa>num_g)&&nlnote)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Lo scrivo ex novo e poi gli devo appendere
//quello che segue

//Copio string in string1 perche' mi servira'
//(e' l'intestazione del giorno che segue)
	strcpy(string1,string);
	if(nlnote)
//Solo se ho note da scrivere creo il giorno
	  fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,300,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
//Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",string1);
        for(i=0;i<num_ops;i++)
          {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
	        MessageBox(HWindow,"Anomalia nel file dati:12",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
		fclose(tmp);
                fclose(ntt);
                return -1;
	      }
	    if(sscanf(string,
	      "%d,%ld,%[!-z ]\t,%[!-z ]\t,%d,%d,%ld,%d\n",
	      &j,&importo_f,mot_f,cat_f,&pa,&pb,
	      &numg_fb,&k)!=8)
	      {
//Trovo i dati in un formato non valido: errore
	        MessageBox(HWindow,"Anomalia nel file dati",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
		fclose(tmp);
                fclose(ntt);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
	for(i=0;i<nrighe;i++)
	  {
	    if(!fgets(string,300,fil))
	      {
//Mi aspettavo una riga (di note)
//e invece non trovo niente: errore
	        MessageBox(HWindow,"Anomalia nel file dati:13",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
		fclose(tmp);
                fclose(ntt);
                return -1;
	      }
//Ho superato tutto quello che c'era prima
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
//A questo punto i nuovi dati sono dentro
//Ricopio quello che segue (se c'e') e finisco
//NOTA: non faccio nessun controllo sui dati dei giorni
//successivi. Se c'e' qualche problema me ne accorgero'
//accedendo a quei giorni in seguito.
    if(!fine)
      while(!feof(fil))
        {
          fgets(string,300,fil);
	  if(!feof(fil))
	    fprintf(tmp,"%s",string);
        }
    fclose(fil);
    fclose(tmp);
    fclose(ntt);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
  }


void TOggiDia::HandVai()
  {
//Mostra i movimenti alla data indicata nei campi
//di riepilogo 
    long int numg;
    int g,m,a;

    caricacampi(RiGior,RiMes,RiAnn);
    RiAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    RiMes->GetText(string,3);
    sscanf(string,"%d",&m);
    RiGior->GetText(string,3);
    sscanf(string,"%d",&g);
    numg=(long int)a*10000+m*100+g;
    riepiloga(numg);
  }

void TOggiDia::HandGPrima()
  {
//Mostra i movimenti del giorno precedente 
    int g,m,a;
    long int numg;

//Assicuro la validita' della data
//sul riepilogo e la ricavo 
    caricacampi(RiGior,RiMes,RiAnn);
    RiAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    RiMes->GetText(string,3);
    sscanf(string,"%d",&m);
    RiGior->GetText(string,3);
    sscanf(string,"%d",&g);
//Determino qual e' il giorno prima
    if(g==1)
      {
//Se e' il primo del mese
	if(m>1)
	  {
//Se il mese non e' gennaio posso decrementare
	    m--;
//g diventa l'ultimo del mese precedente
	    g=month[m-1]; 
          }
	else            
	  {
//Se il mese e' gennaio:
//decrementa anno, mese=dicembre
//giorno=31
	     a--;          
	     m=12;         
             g=31; 
	  }
      }
    else
//Se non e' il primo del mese decrementa il giorno
      g--;
//Ricavo il numero data e aggiorno il riepilogo
    numg=(long int)a*10000+m*100+g;
    riepiloga(numg);
  }

void TOggiDia::HandGDopo()
  {
//Mostra i movimenti del giorno successivo
    int g,m,a;
    long int numg;

//Assicuro la validita' della data
//sul riepilogo e la ricavo 
    caricacampi(RiGior,RiMes,RiAnn);
    RiAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    RiMes->GetText(string,3);
    sscanf(string,"%d",&m);
    RiGior->GetText(string,3);
    sscanf(string,"%d",&g);
//Determino qual e' il giorno dopo
    if(g==month[m-1])
      {
//se e' l'ultimo del mese
	if(m<12)          
	  {
//se non e' dicembre incrementa mese
//e g diventa 1
	    m++;          
	    g=1;          
          }
	else              
	  {
//Se e' dicembre:
//incrementa anno, mese=gennaio
//giorno=1
	    a++;     
	    m=1;     
	    g=1;
	    if(isbises(a+1))
//Se l'anno successivo e' bisestile: giorni di febbraio=29;		                             
	      month[1]=29;
	    else
//Se non e' bisestile: giorni di febbraio=28;		                             
	      month[1]=28;
          }
      }
    else
//Se non e' l'ultimo del mese incrementa g
      g++;            
//Ricavo il numero data e aggiorno il riepilogo
    numg=(long int)a*10000+m*100+g;
    riepiloga(numg);
  }

void TOggiDia::HandOK()
  {
    CloseWindow();
  }

void TOggiDia::HandCheckEntrata(RTMessage Msg)
  {
//Deseleziono uscita e seleziono entrata
//La destinazione di default e' la prima
//della lista
//Il giorno dell'entrata e' quello corrente
//dell'uscita

    if(Msg.LP.Hi==BN_CLICKED)
      {
//Se era gia' selezionato non faccio nulla
	if(entra)
          return;
	PULSE->Check();
	PULSU->Uncheck();
	DESTINA->GetString(string,0);
        DESTINA->Insert(string);
	PROVEN->Clear();
        UGior->GetText(string,3);
	UGior->ClearList();
	EGior->Insert(string);
	UMes->GetText(string,3);
	UMes->ClearList();
	EMes->Insert(string);
	UAnn->GetText(string,5);
	UAnn->ClearList();
	EAnn->Insert(string);
	entra=1;
      }
  }

void TOggiDia::HandCheckUscita(RTMessage Msg)
  {
//Deseleziono entrata e seleziono uscita
//La provenienza di default e' la prima
//della lista
//Il giorno dell'uscita e' quello corrente
//dell'entrata

    if(Msg.LP.Hi==BN_CLICKED)
      {
//Se era gia' selezionato non faccio nulla
	if(!entra)
          return;
	PULSE->Uncheck();
	PULSU->Check();
	PROVEN->GetString(string,0);
	PROVEN->Insert(string);
	DESTINA->Clear();
        EGior->GetText(string,3);
	EGior->ClearList();
	UGior->Insert(string);
	EMes->GetText(string,3);
	EMes->ClearList();
	UMes->Insert(string);
	EAnn->GetText(string,5);
	EAnn->ClearList();
	UAnn->Insert(string);
	entra=0;
      }
  }

void TOggiDia::HandSel(RTMessage Msg)
  {
    int g,m,a,i,j,k,stop;
    long int pippo;

    if(Msg.LP.Hi==LBN_DBLCLK)
      {
//Se clicco due volte su un elemento del riepilogo:
//carica i campi con i dati relativi a quel movimento
//Dapprima pulisco tutto
        IMPO->Clear();                                         
	MOTI->Clear();
	CATE->Clear();
	EGior->Clear();                                         
	EMes->Clear();
	EAnn->Clear();
	UGior->Clear();
	UMes->Clear();
	UAnn->Clear();
	DESTINA->Clear();
	PROVEN->Clear();
//Catturo la stringa selezionata e tolgo la selezione
	RIEPILO->GetSelString(string,400);
	k=RIEPILO->GetSelIndex();
	RIEPILO->SetSelIndex(-1);
        if(sscanf(string,"%ld\t| %[!-z ]\t| %[!-z ]",
	&importo,mot,cat)!=3)
	  {
//Per qualche motivo la stringa e' corrotta: Errore
//(Puo' succedere?)
	    MessageBox(HWindow,"Anomalia stringa:p1",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
            return;
	  }
	if(importo>=0)
	  {
//E' un'entrata: attivo il pulsante di entrata
//e disattivo quello di uscita
	    PULSE->Check();
	    PULSU->Uncheck();
	    entra=1;
	    sprintf(string,"%ld",importo);
//Inserisco importo, motivo e categoria
//nei campi opportuni 
	    IMPO->Insert(string);
	    MOTI->Insert(mot);
	    CATE->Insert(cat);
//Recupero il numero risorsa dell'entrata
	    j=tabella_movimenti[k].posiz_in;
	    i=0;
            stop=0;
	    do
	      {
//Cerco il nome della risorsa associata con il numero
//scorrendo tabella_risorse[].posiz
		if(tabella_risorse[i].posiz==j)
                  {
//Trovato. Lo inserisco in "entra in"
		    DESTINA->Insert(tabella_risorse[i].nomeconv);
		    stop=1;
                  }
                i++;
              }
	    while((i<numfonti)&&(!stop));
//Determino la data di entrata e la inserisco
//nei campi appositi
	    g= tabella_movimenti[k].numg_a%100;
	    pippo= tabella_movimenti[k].numg_a/100;
	    m= pippo%100;
	    a=pippo/100;
	    sprintf(string,"%02d",g);
            EGior->Insert(string);
	    sprintf(string,"%02d",m);
	    EMes->Insert(string);
	    sprintf(string,"%04d",a);
	    EAnn->Insert(string);
//Aggiorno la lista delle risorse di entrata
//per il giorno in questione
	    caricaLista(EGior,EMes,EAnn,DESTINA);
//Decido se c'e' un'uscita: se il numero risorsa
//e' 0 non c'e'
	    if(!(j=tabella_movimenti[k].posiz_da))
	      return;
//C'e' un'uscita: trovo il nome della risorsa
	    i=0;
            stop=0;
	    do
	      {
//Cerco il nome della risorsa associata con il numero
//scorrendo tabella_risorse[].posiz
		if(tabella_risorse[i].posiz==j)
                  {
//Trovato. Lo inserisco in "viene da"
		    PROVEN->Insert(tabella_risorse[i].nomeconv);
                    stop=1;
                  }
                i++;
              }
	    while((i<numfonti)&&(!stop));
//Determino la data di uscita e la inserisco
//nei campi appositi
	    g= tabella_movimenti[k].numg_b%100;
	    pippo= tabella_movimenti[k].numg_b/100;
	    m= pippo%100;
	    a=pippo/100;
	    sprintf(string,"%02d",g);
            UGior->Insert(string);
	    sprintf(string,"%02d",m);
	    UMes->Insert(string);
	    sprintf(string,"%04d",a);
	    UAnn->Insert(string);
//Aggiorno la lista delle risorse di uscita
//per il giorno in questione
	    caricaLista(UGior,UMes,UAnn,PROVEN);
	  }
	else
	  {
//E' un'uscita: attivo il pulsante di uscita
//e disattivo quello di entrata
	    PULSU->Check();
	    PULSE->Uncheck();
	    entra=0;
//Cambio segno a importo per farlo diventare positivo
//(ora e' selezionato il pulsante di uscita)
	    importo=-importo;
	    sprintf(string,"%ld",importo);
//Inserisco importo, motivo e categoria
//nei campi opportuni 
	    IMPO->Insert(string);
	    MOTI->Insert(mot);
            CATE->Insert(cat);
//Recupero il numero risorsa dell'uscita
	    j=tabella_movimenti[k].posiz_da;
	    i=0;
            stop=0;
	    do
	      {
//Cerco il nome della risorsa associata con il numero
//scorrendo tabella_risorse[].posiz
		if(tabella_risorse[i].posiz==j)
		  {
//Trovato. Lo inserisco in "viene da"
		    PROVEN->Insert(tabella_risorse[i].nomeconv);
                    stop=1;
                  }
		i++;
              }
	    while((i<numfonti)&&(!stop));
//Determino la data di uscita e la inserisco
//nei campi appositi
	    g= tabella_movimenti[k].numg_a%100;
	    pippo= tabella_movimenti[k].numg_a/100;
	    m= pippo%100;
	    a=pippo/100;
	    sprintf(string,"%02d",g);
            UGior->Insert(string);
	    sprintf(string,"%02d",m);
	    UMes->Insert(string);
	    sprintf(string,"%04d",a);
	    UAnn->Insert(string);
//Aggiorno la lista delle risorse di uscita
//per il giorno in questione
	    caricaLista(UGior,UMes,UAnn,PROVEN);
//Decido se c'e' un'entrata: se il numero risorsa
//e' 0 non c'e'
	    if(!(j=tabella_movimenti[k].posiz_in))
	      return;
//C'e' un'entrata: trovo il nome della risorsa
	    i=0;
            stop=0;
	    do
	      {
//Cerco il nome della risorsa associata con il numero
//scorrendo tabella_risorse[].posiz
		if(tabella_risorse[i].posiz==j)
                  {
//Trovato. Lo inserisco in "entra in"
		    DESTINA->Insert(tabella_risorse[i].nomeconv);
		    stop=1;
                  }
                i++;
              }
	    while((i<numfonti)&&(!stop));
//Determino la data di entrata e la inserisco
//nei campi appositi
	    g= tabella_movimenti[k].numg_b%100;
	    pippo= tabella_movimenti[k].numg_b/100;
	    m= pippo%100;
	    a=pippo/100;
	    sprintf(string,"%02d",g);
            EGior->Insert(string);
	    sprintf(string,"%02d",m);
	    EMes->Insert(string);
	    sprintf(string,"%04d",a);
	    EAnn->Insert(string);
//Aggiorno la lista delle risorse di entrata
//per il giorno in questione
	    caricaLista(EGior,EMes,EAnn,DESTINA);
	  }
      }
  }

/*

void TOggiDia::cancella(PTListBox R,long int *tot,long int *totb)
  {
    int index;

    index=R->GetSelIndex();
    if(index>=0)
    {
      R->GetSelString(all,110);
      if(all[0]!='O')
        if(all[0]!='B')
          {        
            sscanf(all,"%s\t | %s\t | %s",lire,mot,cat);   // lettura formattata della stringa
	    *tot=*tot-atol(lire);
	  }
        else
	  {
	    sscanf(all,"B%s\t | %s\t | %s",lire,mot,cat);   // lettura formattata della stringa
	    *totb=*totb-atol(lire);
	  }
      else
        if(all[1]!='B')
          {        
            sscanf(all,"O%s\t | %s\t | %s",lire,mot,cat);   // lettura formattata della stringa
	    *tot=*tot-atol(lire);
	  }
        else
	  {
	    sscanf(all,"OB%s\t | %s\t | %s",lire,mot,cat);   // lettura formattata della stringa
	    *totb=*totb-atol(lire);
          }
      R->DeleteString(index);
      sprintf(all,"%ld",rimpre+tote-totu);
      SetDlgItemText(HWindow,RIMTOM,all);
    }
  }


  */
