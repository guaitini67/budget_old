
// *********************** CLASSE TOggiDia *******************************************

_CLASSDEF(TOggiDia)




extern int leggiprepara(HWND,long int,char *);
extern int scrivifile(HWND,char *,long int,char *);
extern int scrivinote(HWND,char *,long int,int);
extern int cancella(HWND,char *,long int,char *);

class TOggiDia:public TDialog
  {
    public:

      char string[400],string1[400],
	   nomefile_in[30],nomefile_out[30],
	   entra,nomeconv[27],mot[100],cat[100],
	   mot_f[100],cat_f[100],*visino;
      int  i,j,k,g,m,a,viene_da,va_in,n_lpa,n_lpb,
	   posiz,fine,tab[5],pa,pb,index,
	   numfonti,nrighe,nlnote,nanni,num_ops;
      long int importo,numg_out,numg_in,saldo,
	       numg_fa,numg_fb,importo_f,numg_riepilogo;
      struct tm tcheck;
//La struttura che segue memorizza i dati
//di ciascuna risorsa relativamente al giorno
//selezionato
      struct info_risorse
        {
	  int posiz;
	  char nomeconv[27];
	  long int aperto,chiuso;
	  long int saldoinanno;
          long int saldoinimese;
	  long int saldooggi;
	  long int saldofinmese;
	  long int saldofinanno;
	  long int saldofinannoold;
	  long int entrateinianno;
	  long int entrateinimese;
	  long int entrateoggi;
	  long int entratefinmese;
	  long int entratefinanno;
	  long int usciteinianno;
	  long int usciteinimese;
	  long int usciteoggi;
	  long int uscitefinmese;
	  long int uscitefinanno;
	  long int gaininianno;
	  long int gaininimese;
	  long int gainoggi;
	  long int gainfinmese;
	  long int gainfinanno;
	  char casi;
	} tabella_risorse[100];
//La struttura che segue memorizza i dati
//di ciascun movimento del giorno selezionato
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
      struct date Date;
      FILE *map,*fil,*tmp;
      PTGroupBox GRUP;
      PTRadioButton PULSE,PULSU;
      PTButton BOK;
      PTComboGiov CATE;
      PTEdit IMPO,MOTI;
      PTListBox RIEPILO;
      PTStatic RESULT;
      PTGiovWindow SUM;
      PTComboGiov EGior,EMes,EAnn,UGior,UMes,UAnn,
		  RiGior,RiMes,RiAnn,DESTINA,PROVEN,
		  SELDAT,SELBANC;
      TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,
               char* giorno,char* vis);

      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      virtual void WMTimer()=[WM_FIRST + WM_TIMER];
      virtual void SetupWindow();
      virtual ~TOggiDia();
      virtual void CloseWindow();
      virtual void WMClose(RTMessage Msg)
        =[WM_FIRST+WM_CLOSE];
      int  caricaVoci();
      void caricaLista(PTComboGiov GGior,
      		       PTComboGiov MMes,
		       PTComboGiov AAnn,
		       PTComboGiov RISORSA);
      void HandAnnulla()=[ID_FIRST+ANNUL];
      void HandCancella()=[ID_FIRST+OG_CANC];
      void HandCheckEntrata(RTMessage Msg)=[ID_FIRST+PULSENTR];
      void CheckEntrata();
      void HandCheckUscita(RTMessage Msg)=[ID_FIRST+PULSUSC];
      void CheckUscita();
      void HandGDopo()=[ID_FIRST+AVANTI];
      void HandGPrima()=[ID_FIRST+DIETRO];
      void HandNote()=[ID_FIRST+OG_NOTE];
      void HandOK()=[ID_FIRST+OG_OK];
      void HandSel(RTMessage Msg)=[ID_FIRST+C_RIEPILOG];
      void SimpleSel();
      void HandSelSaldoRisorsa(RTMessage Msg)=
           [ID_FIRST+SELBANCA];
      void HandSelTipoDato(RTMessage Msg)=
	   [ID_FIRST+SELDATO];
      void HandScrivi()=[ID_FIRST+SCRIVI];
      void DefCommandProc(RTMessage Msg);
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
      int  riepiloga(long int num_g);
      int  caricasaldi(long int);
      int  aggiornasaldi(long int,char *);
      void mostrasaldi();
      void aggiornacategorie();
      void caricacategorie();
  };



// ************************** FUNZIONI DI TOggiDia **************************************

TOggiDia::TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,
                   char* giorno, char* vis)
  :TDialog(AParent,ResourceId)
  {
    BOK=new TButton(this,OG_OK,NULL);
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
    SELDAT=new TComboGiov(this,SELDATO,25,NULL);
    SELBANC=new TComboGiov(this,SELBANCA,25,NULL);
    RESULT=new TStatic(this,MOSTRADATO,25,NULL);
// punti di stop per i tabulatori nella
//finestra di riepilogo
    tab[0]=100;
    tab[1]=150;                             
    tab[2]=200;
//nlnote=numero righe di note. La metto a zero all'inizio
//(e' necessario?)
    nlnote=0;
//copio giorno in str1 per evitare strani errori
    strcpy(string,giorno);          
    sscanf(string,"%02d-%02d-%04d",&g,&m,&a);
    visino=vis;
    *visino=1;
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
    AWndClass.hIcon=LoadIcon(GetApplication()->hInstance,"icona");
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
//Imposto il timer per mostrare data e ora
    if (!SetTimer(HWindow, 1, 1000U, NULL))
      MessageBox(HWindow, "Too Many Timers", "Errore",
		 MB_ICONEXCLAMATION | MB_OK);
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
//Inserisco la voce per selezionare i dati globali
    SELBANC->AddString("Globale");
//Di default, carico il campo 'esce da' e riepilogo
//con la prima voce (che nelle mie idee e' 'contanti')
    PROVEN->GetString(string,0);
    PROVEN->Insert(string);
    SELBANC->Insert(string);
//0 = saldoinanno
    SELDAT->InsertString("Saldo inizio anno",-1);
//1 = saldoinimese
    SELDAT->InsertString("Saldo inizio mese",-1);
//2 = saldooggi
    SELDAT->InsertString("Saldo a oggi",-1);
//3 = saldofinmese
    SELDAT->InsertString("Saldo fine mese",-1);
//4 = saldofinanno
    SELDAT->InsertString("Saldo fine anno",-1);
//5 = entrateinianno
    SELDAT->InsertString("Entrate da inizio anno",-1);
//6 = entrateinimese
    SELDAT->InsertString("Entrate da inizio mese",-1);
//7 = entrateoggi
    SELDAT->InsertString("Entrate di oggi",-1);
//8 = entratefinmese
    SELDAT->InsertString("Entrate del mese",-1);
//9 = entratefinanno
    SELDAT->InsertString("Entrate fino a fine anno",-1);
//10 = usciteinianno
    SELDAT->InsertString("Uscite da inizio anno",-1);
//11 = usciteinimese
    SELDAT->InsertString("Uscite da inizio mese",-1);
//12 = usciteoggi
    SELDAT->InsertString("Uscite di oggi",-1);
//13 = uscitefinmese
    SELDAT->InsertString("Uscite del mese",-1);
//14 = uscitefinanno
    SELDAT->InsertString("Uscite fino a fine anno",-1);
//15 = gaininianno
    SELDAT->InsertString("Guadagno da inizio anno",-1);
//16 = gaininimese
    SELDAT->InsertString("Guadagno da inizio mese",-1);
//17 = gainoggi
    SELDAT->InsertString("Guadagno di oggi",-1);
//18 = gainfinmese
    SELDAT->InsertString("Guadagno fino a fine mese",-1);
//19 = gainfinanno
    SELDAT->InsertString("Guadagno fino a fine anno",-1);
//Valore di default
    SELDAT->Insert("Saldo a oggi");

    caricacategorie();
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
//Leggo il giorno di apertura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giorno di apertura
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
//Leggo il giorno di chiusura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giorno di chiusura
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
	for(j=0;j<nrighe;j++)
	  if(!fgets(string,100,map))
	    {
//Mi aspettavo di leggere una riga e non la trovo: errore
	      MessageBox(HWindow,"Anomalia nel file dati",
	      "Problema!",MB_ICONEXCLAMATION | MB_OK);
	      fclose(map);
	      return -1;
	    }
	for(j=0;j<nanni;j++)
          {
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
	        MessageBox(HWindow,"Anomalia nel file dati",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
	        return -1;
	      }
	    if(sscanf(string,"%d,%ld\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
	        MessageBox(HWindow,"Anomalia nel file dati",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		fclose(map);
	        return -1;
	      }
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
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
	RiGior->SetEditSel(0,2);
	RiGior->GetText(string,3);
	RiGior->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
      }
  }

void TOggiDia::HandValRiM(RTMessage Msg)
  {

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
	RiMes->SetEditSel(0,2);
	RiMes->GetText(string,3);
	RiMes->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
      }
  }

void TOggiDia::HandValRiA(RTMessage Msg)
  {

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
	RiAnn->SetEditSel(0,4);
	RiAnn->GetText(string,5);
	RiAnn->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(RiGior,RiMes,RiAnn);
//Aggiorna la lista
	caricaLista(RiGior,RiMes,RiAnn,SELBANC);
//Inserisco la voce per selezionare i dati globali
	SELBANC->AddString("Globale");
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
	if((n_lpa=leggiprepara(HWindow,numg_in,nomefile_in))<0)
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
	    if((n_lpb=leggiprepara(HWindow,numg_out,nomefile_out))<0)
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
	    if(scrivifile(HWindow,nomefile_out,numg_out,string)
	       <0)
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
	if(scrivifile(HWindow,nomefile_in,numg_in,string)<0)
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
	aggiornacategorie();
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
	if((n_lpa=leggiprepara(HWindow,numg_out,nomefile_out))<0)
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
	      ((n_lpb=leggiprepara(HWindow,numg_in,nomefile_in))<0)
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
	    if(scrivifile(HWindow,nomefile_in,numg_in,string)<0)
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
	if(scrivifile(HWindow,nomefile_out,numg_out,string)<0)
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
        aggiornacategorie();
	CATE->Clear();
//Quindi aggiorno il riepilogo sul giorno dell'uscita
	riepiloga(numg_out);
      }
  }


int  TOggiDia::riepiloga(long int num_g)
//Scorre il file selezionato alla ricerca di num_g
//e quando lo trova (se lo trova) riempie il campo di
//riepilogo
//Inoltre scrive le note nel file "note.tmp"  
  {
    int g,m,a;
    long int pippo;
    char nome_file[30];

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

//con tcheck e la funzione mktime determino il
//giorno della settimana
    tcheck.tm_year=a-1900;
    tcheck.tm_mon=m-1;
    tcheck.tm_mday=g;
    tcheck.tm_hour=0;
    tcheck.tm_min=0;
    tcheck.tm_sec=1;
    tcheck.tm_isdst=-1;
    if(mktime(&tcheck)==-1)
      tcheck.tm_wday=7;
    sprintf(nome_file,"%d.bud",a);
//Scrivo la data nei campi data riepilogo
    sprintf(string,"%02d",g);
    RiGior->Insert(string);
    sprintf(string,"%02d",m);
    RiMes->Insert(string);
    sprintf(string,"%02d",a);
    RiAnn->Insert(string);
    sprintf(string,"%s",gset[tcheck.tm_wday]);
    SetDlgItemText(HWindow,GSETTIMANA,string);

    if(caricasaldi(num_g)<0)
      {
//C'e' stato un problema nella lettura di basedata.dat
	MessageBox(HWindow,"Anomalia nel file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    if(aggiornasaldi(num_g,
       nome_file)<0)
      {
//C'e' stato un problema nella lettura del file di dati
//o nella riscrittura di basedata.dat
	MessageBox(HWindow,
	"Anomalia nell'aggiornamento dei saldi",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    mostrasaldi();

//Individuo nome file da aprire
    sprintf(nome_file,"%d.bud",a);
    if(!(tmp=fopen("note.tmp","w")))
      {
//Non riesco a creare il file: errore (precauzione eccessiva?)
        MessageBox(HWindow,
        "Non riesco a creare il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    if(!(fil=fopen(nome_file,"r")))
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
            index=0;
	    while(tabella_risorse[index].posiz<pb)
	      {
                index++;
              }
            sprintf(string,
"%s\t| +%ld\t| %s\t| %s",tabella_risorse[index].nomeconv,
              importo_f,mot_f,cat_f);
	  }
	else
	  {
//Se e' un'uscita
	    index=0;
	    while(tabella_risorse[index].posiz<pa)
	      {
                index++;
              }
	    sprintf(string,
"%s\t| %ld\t| %s\t| %s",tabella_risorse[index].nomeconv,
                importo_f,mot_f,cat_f);
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
//Mi manda alla pagina delle note. Quando
//esco scrivo il file
  {
    int a;
    long int pippo;

    GetModule()->ExecDialog(new TNoteDia(this,"NOTE",
                            &nlnote,numg_riepilogo));
//Individuo il file su cui scrivere l'uscita
    pippo= numg_riepilogo/100;
    a=pippo/100;
    sprintf(nomefile_in,"%d.bud",a);
    scrivinote(HWindow,nomefile_in,numg_riepilogo,nlnote);
    riepiloga(numg_riepilogo);
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

/*void TOggiDia::DefChildProc(RTMessage Msg)
{
    if ( (Msg.WP.Hi == 0) && (Msg.LP.Hi == WM_CHAR))
    {
      MessageBox(HWindow,
      "Hai premuto ALT+f",
      "Evviva!",MB_ICONEXCLAMATION | MB_OK);
    }
  TDialog::DefChildProc(Msg);
} */

    
void TOggiDia::DefCommandProc(RTMessage Msg)
  {
    if ( Msg.WP.Hi == 0 )
      {
	switch(Msg.WP.Lo)
	  {
	    case ACC_CTA:
//Se digito CTRL+A, mi equivale a premere ANNULLA
	      HandAnnulla();
	      break;
	    case ACC_CTD:
//Se digito CTRL+D, mi posiziona su IMPORTO
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTE:
//Se digito CTRL+E, mi seleziona Entrata
              CheckEntrata();
	      break;
	    case ACC_CTF:
//Se digito CTRL+F, mi posiziona sulla lista movimenti
	      SetFocus(RIEPILO->HWindow);
	      break;
	    case ACC_CTG:
//Se digito CTRL+G, mi posiziona su data di riepilogo
	      SetFocus(RiGior->HWindow);
	      break;
	    case ACC_CTN:
//Se digito CTRL+N, mi equivale a premere NOTE
	      HandNote();
	      break;
	    case ACC_CTR:
//Se digito CTRL+R, mi seleziona Uscita
	      CheckUscita();
	      break;
	    case ACC_CTQ:
//Se digito CTRL+Q, mi posiziona su ENTRA IN
	      SetFocus(DESTINA->HWindow);
	      break;
	    case ACC_CTS:
//Se digito CTRL+S, mi equivale a premere SCRIVI
//Metto il focus su importo per toglierlo
//ai campi date e impedire date errate
	      SetFocus(IMPO->HWindow);
	      HandScrivi();
	      break;
	    case ACC_CTW:
//Se digito CTRL+W, mi posiziona su ESCE DA
	      SetFocus(PROVEN->HWindow);
	      break;
	    case ACC_CTEND:
//Se digito CTRL+fine, mi posiziona su OK
	      SetFocus(BOK->HWindow);
	      break;
	    case ACC_CTDELETE:
//Se digito CTRL+canc, mi equivale a premere CANC
	      HandCancella();
	      break;
	    case ACC_CTRETURN:
//Se digito CTRL+enter, mi equivale a cliccare due volte
//in una voce di riepilogo
	      if(RIEPILO->GetSelString(string1,400)>1)
	        SimpleSel();
	      break;
	    case ACC_CTLEFT:
//Se digito CTRL+freccia sx, mi visualizza il giorno prima
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
	      SetFocus(SELDAT->HWindow);
	      HandGPrima();
	      break;
	    case ACC_CTRIGHT:
//Se digito CTRL+freccia dx, mi visualizza il giorno dopo
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
	      SetFocus(SELDAT->HWindow);
	      HandGDopo();
	      break;
	    case ACC_CTNEXT:
//Se digito CTRL+page down, mi equivale a premere VAI
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
	      SetFocus(SELDAT->HWindow);
	      HandVai();
	      break;
	    default:
              ;
	  }
      }
    TDialog::DefCommandProc(Msg);
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

void TOggiDia::CloseWindow()
  {
    *visino=0;
    TDialog::CloseWindow();
  }

void TOggiDia::WMClose(RTMessage Msg)
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
	CheckEntrata();
      }
  }

void TOggiDia::CheckEntrata()
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


void TOggiDia::HandCheckUscita(RTMessage Msg)
  {
//Deseleziono entrata e seleziono uscita
//La provenienza di default e' la prima
//della lista
//Il giorno dell'uscita e' quello corrente
//dell'entrata

    if(Msg.LP.Hi==BN_CLICKED)
      {
	CheckUscita();
      }
  }

void TOggiDia::CheckUscita()
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

void TOggiDia::HandSel(RTMessage Msg)
  {
    if(Msg.LP.Hi==LBN_DBLCLK)
      {
//Se clicco due volte su un elemento del riepilogo:
//carica i campi con i dati relativi a quel movimento
	SimpleSel();
      }
  }


void TOggiDia::SimpleSel()
  {
    int g,m,a,i,j,k;
    long int pippo;

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
    if(sscanf(string,"%[!-z ]\t| %ld\t| %[!-z ]\t| %[!-z ]",
      nomeconv,&importo,mot,cat)!=4)
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
        while(tabella_risorse[i].posiz<j)
          {
	    i++;
          }
	DESTINA->Insert(tabella_risorse[i].nomeconv);
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
	while(tabella_risorse[i].posiz<j)
	  {
	    i++;
          }
	PROVEN->Insert(tabella_risorse[i].nomeconv);
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
        while(tabella_risorse[i].posiz<j)
          {
	    i++;
          }
        PROVEN->Insert(tabella_risorse[i].nomeconv);
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
        while(tabella_risorse[i].posiz<j)
          {
            i++;
          }
        DESTINA->Insert(tabella_risorse[i].nomeconv);
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


void TOggiDia::HandSelSaldoRisorsa(RTMessage Msg)
//Mi fa vedere il dato della risorsa che seleziono
  {
    if(Msg.LP.Hi==CBN_SELENDOK)
      {
        mostrasaldi();
      }
  }


void TOggiDia::HandCancella()
//Cancella la voce selezionata nel box di riepilogo
  {
    int a,index;
    long int pippo,numg_in,numg_out;

//Trovo l'anno relativo alla data di riepilogo
    pippo= numg_riepilogo/100;
    a=pippo/100;
    index=RIEPILO->GetSelIndex();
    if(index<0)
//Non era selezionato nulla: ritorno
      return;
    RIEPILO->GetSelString(string,300);
    k=RIEPILO->GetSelIndex();
    if(sscanf(string,"%[!-z ]\t| %ld\t| %[!-z ]\t| %[!-z ]",
       nomeconv,&importo,mot,cat)!=4)
      {
//Per qualche motivo la stringa e' corrotta: Errore
//(Puo' succedere?)
        MessageBox(HWindow,"Anomalia stringa:p1",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
//Ricavo le informazioni aggiuntive
//della voce selezionata
    viene_da=tabella_movimenti[k].posiz_da;
    va_in=tabella_movimenti[k].posiz_in;
    n_lpa=tabella_movimenti[k].numop_a;
    n_lpb=tabella_movimenti[k].numop_b;
//Passo preliminare per calcolare l'anno
//dell'operazione collegata (se c'e')
    pippo= tabella_movimenti[k].numg_b/100;
    if (importo>=0)
      {
//Ho selezionato un'entrata: nomefile_in
//e' quello ricavato da numg_riepilogo
	numg_in=numg_riepilogo;
	numg_out=tabella_movimenti[k].numg_b;
	sprintf(nomefile_in,"%d.bud",a);
//Adesso ricavo l'anno dell'operazione di uscita
//e ci formo nomefile_out
	a=pippo/100;
	sprintf(nomefile_out,"%d.bud",a);
//stringa da cercare ed eliminare nel file di entrata
	sprintf(string,"%d,+%ld,%s\t,%s\t,%d,%d,%ld,%d\n",
	        n_lpa,importo,mot,cat,viene_da,va_in,
		numg_out,n_lpb);
	if((cancella(HWindow,nomefile_in,numg_in,
	  string))<0)
	  {
//Doveva esserci un'entrata, ma nel file non c'e':
//errore
	    MessageBox(HWindow,"Anomalia: il movimento in\
 entrata da cancellare non esiste nel file",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
//Tutto e' andato bene,
//nomefile_in diventa backin e temp
//diventa nomefile_in. Lo devo fare qui
//nel caso che nomefile_out=nomefile_in.
//In caso di errore devo poter ripristinare il vecchio file.
	rename(nomefile_in,"backin");
	rename("temp",nomefile_in);
	if(viene_da)
          {
//Solo se c'e' un'uscita formo la seconda stringa
//per il file di uscita
	    sprintf(string1,"%d,-%ld,%s\t,%s\t,%d,%d,%ld,%d\n",
	            n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_in,n_lpa);
	    if((cancella(HWindow,nomefile_out,numg_out,
	      string1))<0)
	      {
//Doveva esserci un'uscita, ma nel file non c'e':
//errore
	        MessageBox(HWindow,"Anomalia: il movimento in\
 uscita da cancellare non esiste nel file",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
//Prima di uscire ripristino il file gia' modificato
//nella sua versione originale
                remove(nomefile_in);
                rename("backin",nomefile_in);
		return;
	      }
//Tutto e' andato bene, temp diventa nomefile_out
            remove(nomefile_out);
	    rename("temp",nomefile_out);
	  }
	remove("backin");
      }
    else
      {
//Ho selezionato un'uscita: nomefile_out
//e' quello ricavato da numg_riepilogo
	numg_out=numg_riepilogo;
	numg_in=tabella_movimenti[k].numg_b;
	sprintf(nomefile_out,"%d.bud",a);
//Adesso ricavo l'anno dell'operazione di entrata
//e ci formo nomefile_in
	a=pippo/100;
	sprintf(nomefile_in,"%d.bud",a);
	importo=-importo;
//stringa da cercare ed eliminare nel file di uscita
	sprintf(string,"%d,-%ld,%s\t,%s\t,%d,%d,%ld,%d\n",
	        n_lpa,importo,mot,cat,viene_da,va_in,
		numg_in,n_lpb);
	if((cancella(HWindow,nomefile_out,numg_out,
	  string))<0)
	  {
//Doveva esserci un'uscita, ma nel file non c'e':
//errore
	    MessageBox(HWindow,"Anomalia: il movimento in\
uscita da cancellare non esiste nel file",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    return;
	  }
//Tutto e' andato bene,
//nomefile_out diventa backout e temp
//diventa nomefile_out. Lo devo fare qui
//nel caso che nomefile_in=nomefile_out.
//In caso di errore devo poter ripristinare il vecchio file.
	rename(nomefile_out,"backout");
	rename("temp",nomefile_out);
	if(va_in)
          {
//Solo se c'e' un'entrata formo la seconda stringa
//per il file di entrata
	    sprintf(string1,"%d,+%ld,%s\t,%s\t,%d,%d,%ld,%d\n",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_out,n_lpa);
	    if((cancella(HWindow,nomefile_in,numg_in,
	      string1))<0)
	      {
//Doveva esserci un'entrata, ma nel file non c'e':
//errore
	        MessageBox(HWindow,"Anomalia: il movimento in\
uscita da cancellare non esiste nel file",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
//Prima di uscire ripristino il file gia' modificato
//nella sua versione originale
                remove(nomefile_out);
                rename("backout",nomefile_out);
		return;
	      }
//Tutto e' andato bene, temp diventa nomefile_in
            remove(nomefile_in);
	    rename("temp",nomefile_in);
	  }
        remove("backout");
      }
//Aggiorno la finestra di riepilogo
    riepiloga(numg_riepilogo);
  }

int TOggiDia::caricasaldi(long int num_g)
//Scorre il file basedata.dat per avere
//i saldi di partenza e le informazioni
//fondamentali ad essi correlate
  {
    int g,m,a,anno;
    long int pippo;

    pippo=num_g/100;
    anno=pippo/100;
    if(!(map=fopen("basedata.dat","r")))
      {
//Non trovo basedata.dat: errore
	MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
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
        MessageBox(HWindow,"Anomalia nel file dati:cs1",
	"Problema !",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	return -1;
      }
//Tutto OK, posso andare avanti
    for(i=0;i<numfonti;i++)
      {
	if(!fgets(string,100,map))
	  {
//Mi aspettavo di leggere una riga e non la trovo: errore
            MessageBox(HWindow,"Anomalia nel file dati:cs2",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
          }
	if(sscanf(string,"%d,%[!-z ]\n",&posiz,
	   nomeconv)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs3",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Tutto OK. Leggo il giorno di apertura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs4",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giorno di apertura
	if(!strcmp(string,"nil\n"))
	  {
//Ho trovato nil: non c'e' giorno di apertura
	  }
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
	  {
//Formattazione corretta
	  }
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs4",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Leggo il giorno di chiusura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs5",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Cerco le informazioni sul giorno di chiusura
	if(!strcmp(string,"nil\n"))
	  {
//Ho trovato nil: non c'e' giorno di chiusura
	  }
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
	  {
//Formattazione corretta
	  }
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs6",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs7",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
	if(sscanf(string,"%d,%d\n",&nrighe,&nanni)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:cs8",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    return -1;
	  }
//Supero le righe di commento
	for(j=0;j<nrighe;j++)
          {
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga
//e non la trovo: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:cs9",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
		return -1;
	      }
	  }
	j=0;
//Valore di default del saldo=0 se non trovo
//niente di utile in basedata.dat
	tabella_risorse[i].saldoinanno=0;
	tabella_risorse[i].saldofinannoold=0;
	tabella_risorse[i].casi=3;
	while(j<nanni)
	  {
	    j++;
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo:
//errore
		MessageBox(HWindow,
		"Anomalia nel file dati:cs10",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
		return -1;
	      }
	    if(sscanf(string,"%d,%ld\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:cs11",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
		return -1;
	      }
	    if(a<anno)
	      {
//Fintanto che non trovo anno:
//Valore di ....casi = 3 (per ora)
		tabella_risorse[i].casi=3;
//Valore del saldo fintanto che non trovo anno=
//saldo dell'anno letto (per ora)
		tabella_risorse[i].saldoinanno=saldo;
//Valore del saldo anno successivo =
//saldo dell'anno letto (per ora)
		tabella_risorse[i].saldofinannoold
		=saldo;
	      }
	    else if(a==anno)
	      {
//Accade se anno e' parte della lista
//Valore di ....casi = 1 (per ora)
                tabella_risorse[i].casi=1;
		tabella_risorse[i].saldoinanno=saldo;
//Valore del saldo anno successivo =
//saldo di quest'anno (per ora)
		tabella_risorse[i].saldofinannoold
		=saldo;
	      }
	    else if(a==anno+1)
	      {
//Accade se anno+1 e' parte della lista
                tabella_risorse[i].saldofinannoold
		=saldo;
		if(tabella_risorse[i].casi==1)
//Se esisteva anno => nuovo valore = 0
		  tabella_risorse[i].casi=0;
		else
//Se non esisteva anno => nuovo valore = 2
		  tabella_risorse[i].casi=2;
	      }
	  }
      }
    fclose(map);
    return 1;
  }


int TOggiDia::aggiornasaldi(long int num_g,
		  char *nome_file)
//Scorre il file dei dati per intero
//calcola i nuovi saldi
//aggiorna il file basedata.dat
  {
    int g,m,a,pp,plink,anno,deltananni;
    long int pippo,deltasaldo,numg_inimese,
	     numg_finmese;

    pippo=num_g/100;
    anno=pippo/100;
    a=anno;
    m=pippo%100;
//Questo mi serve per capire l'inizio del mese
    numg_inimese=(long int)a*10000+m*100+1;
//Questo mi serve per capire l'inizio del mese successivo
    if(m==12)
      {
	m=1;
        a=anno+1;
      }
    else
      m++;
    numg_finmese=(long int)a*10000+m*100+1;

//inizializzo i valori dei dati globali da calcolare
//nell'elemento numfonti+1_esimo del vettore
//tabella_risorse
    tabella_risorse[numfonti].saldoinanno=0;
    tabella_risorse[numfonti].saldoinimese=0;
    tabella_risorse[numfonti].saldooggi=0;
    tabella_risorse[numfonti].saldofinmese=0;
    tabella_risorse[numfonti].saldofinanno=0;

    tabella_risorse[numfonti].entrateinianno=0;
    tabella_risorse[numfonti].entrateinimese=0;
    tabella_risorse[numfonti].entrateoggi=0;
    tabella_risorse[numfonti].entratefinmese=0;
    tabella_risorse[numfonti].entratefinanno=0;

    tabella_risorse[numfonti].usciteinianno=0;
    tabella_risorse[numfonti].usciteinimese=0;
    tabella_risorse[numfonti].usciteoggi=0;
    tabella_risorse[numfonti].uscitefinmese=0;
    tabella_risorse[numfonti].uscitefinanno=0;

    tabella_risorse[numfonti].gaininianno=0;
    tabella_risorse[numfonti].gaininimese=0;
    tabella_risorse[numfonti].gainoggi=0;
    tabella_risorse[numfonti].gainfinmese=0;
    tabella_risorse[numfonti].gainfinanno=0;

    for(i=0;i<numfonti;i++)
      {
//inizializzo i valori dei dati da calcolare
	tabella_risorse[i].saldoinimese=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldooggi=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldofinmese=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldofinanno=
	tabella_risorse[i].saldoinanno;

	tabella_risorse[numfonti].saldoinanno=
	tabella_risorse[numfonti].saldoinanno+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldoinimese=
	tabella_risorse[numfonti].saldoinimese+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldooggi=
	tabella_risorse[numfonti].saldooggi+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldofinmese=
	tabella_risorse[numfonti].saldofinmese+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldofinanno=
	tabella_risorse[numfonti].saldofinanno+
	tabella_risorse[i].saldoinanno;


	tabella_risorse[i].entrateinianno=0;
	tabella_risorse[i].entrateinimese=0;
	tabella_risorse[i].entrateoggi=0;
	tabella_risorse[i].entratefinmese=0;
	tabella_risorse[i].entratefinanno=0;

	tabella_risorse[i].usciteinianno=0;
	tabella_risorse[i].usciteinimese=0;
	tabella_risorse[i].usciteoggi=0;
	tabella_risorse[i].uscitefinmese=0;
	tabella_risorse[i].uscitefinanno=0;

	tabella_risorse[i].gaininianno=0;
	tabella_risorse[i].gaininimese=0;
	tabella_risorse[i].gainoggi=0;
	tabella_risorse[i].gainfinmese=0;
	tabella_risorse[i].gainfinanno=0;
      }
    if(!(fil=fopen(nome_file,"r")))
//Se il file e' nuovo non c'e' da aggiornare niente
//restituisco 1
      return 1;
    do
      {
	if(!fgets(string,400,fil))
	  {
//Ho raggiunto la fine del file
//esco dal loop
	    fclose(fil);
	    break;
          }
	if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    MessageBox(HWindow,"Anomalia nel file dati:ag1",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    return -1;
	  }
	for(i=0;i<num_ops;i++)
          {
	    if(!fgets(string,400,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente: errore
	        MessageBox(HWindow,
	        "Anomalia nel file dati:ag2",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        return -1;
	      }
	    if(sscanf(string,
	      "%d,%ld,%[!-z ]\t,%[!-z ]\t,\
%d,%d,%ld,%d\n",
              &j,&importo_f,mot_f,cat_f,&pa,&pb,
	      &numg_fb,&k)!=8)
              {
//Trovo i dati in un formato non valido: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:ag3",
                "Problema!",MB_ICONEXCLAMATION | MB_OK);
		fclose(fil);
                return -1;
	      }
	    if(importo_f>=0)
	      {
//Se era un'entrata devo fare riferimento a pb
		pp=pb;
		plink=pa;
	      }
	    else
	      {
//Se era un'uscita devo fare riferimento a pa
		pp=pa;
		plink=pb;
	      }
	    j=0;
	    do
	      {
//Cerco il nome della risorsa associata con pp
//scorrendo tabella_risorse[].posiz
		if(tabella_risorse[j].posiz==pp)
                  {
//Trovato. Aggiorno i dati della risorsa
//Da aggiornare sempre:
//Saldo, guadagno, entrate, uscite
//da inizio anno a fine anno
	            tabella_risorse[j].saldofinanno=
		    tabella_risorse[j].saldofinanno+
		    importo_f;
		    tabella_risorse[j].gainfinanno=
		    tabella_risorse[j].gainfinanno+
		    importo_f;
//Anche se non controllo niente, solo se e' una "pura" entrata
//o uscita (niente link) avro' un effetto sul dato globale:
//infatti se c'e' un link quello che e' entrato poi
//esce da qualche altra parte (a meno che non siano anni
//diversi)
		    tabella_risorse[numfonti].saldofinanno=
		    tabella_risorse[numfonti].saldofinanno+
		    importo_f;
		    tabella_risorse[numfonti].gainfinanno=
		    tabella_risorse[numfonti].gainfinanno+
		    importo_f;
		    if(importo_f>=0)
                      {
		        tabella_risorse[j].entratefinanno=
		        tabella_risorse[j].entratefinanno+
			importo_f;
			if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			  tabella_risorse[numfonti].entratefinanno=
			  tabella_risorse[numfonti].entratefinanno+
                          importo_f;
                      }
		    else
                      {
			tabella_risorse[j].uscitefinanno=
		        tabella_risorse[j].uscitefinanno-
		        importo_f;
			if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			  tabella_risorse[numfonti].uscitefinanno=
			  tabella_risorse[numfonti].uscitefinanno-
                          importo_f;
		      }
		    if(numg_fa<numg_inimese)
		      {
//Caso specifico per: saldo inizio mese
			tabella_risorse[j].saldoinimese=
		        tabella_risorse[j].saldoinimese+
		        importo_f;
//Dato globale
			tabella_risorse[numfonti].saldoinimese=
			tabella_risorse[numfonti].saldoinimese+
			importo_f;
		      }
		    if(numg_fa<numg_finmese)
		      {
//Caso specifico per: saldo fine mese
			tabella_risorse[j].saldofinmese=
		        tabella_risorse[j].saldofinmese+
		        importo_f;
//Dato globale
			tabella_risorse[numfonti].saldofinmese=
			tabella_risorse[numfonti].saldofinmese+
			importo_f;
		      }
		    if(numg_fa<=num_g)
		      {
//Caso specifico per: saldo odierno,
//guadagno, entrate, uscite da inizio anno a oggi
			tabella_risorse[j].saldooggi=
		        tabella_risorse[j].saldooggi+
			importo_f;
			tabella_risorse[j].gaininianno=
		        tabella_risorse[j].gaininianno+
			importo_f;
//Dato globale
			tabella_risorse[numfonti].saldooggi=
			tabella_risorse[numfonti].saldooggi+
			importo_f;
			tabella_risorse[numfonti].gaininianno=
			tabella_risorse[numfonti].gaininianno+
			importo_f;
			if(importo_f>=0)
                          {
			    tabella_risorse[j].entrateinianno=
		            tabella_risorse[j].entrateinianno+
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			      tabella_risorse[numfonti].entrateinianno=
			      tabella_risorse[numfonti].entrateinianno+
                              importo_f;
			  }
			else
                          {
			    tabella_risorse[j].usciteinianno=
			    tabella_risorse[j].usciteinianno-
			    importo_f;
                          }
			    if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			      tabella_risorse[numfonti].usciteinianno=
			      tabella_risorse[numfonti].usciteinianno-
                              importo_f;
		      }
		    if(numg_fa==num_g)
		      {
//Caso specifico per: guadagno, entrate, uscite di oggi
			tabella_risorse[j].gainoggi=
			tabella_risorse[j].gainoggi+
			importo_f;
//Dato globale
			tabella_risorse[numfonti].gainoggi=
			tabella_risorse[numfonti].gainoggi
			+importo_f;
			if(importo_f>=0)
			  {
			    tabella_risorse[j].entrateoggi=
		            tabella_risorse[j].entrateoggi+
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			      tabella_risorse[numfonti].entrateoggi=
			      tabella_risorse[numfonti].entrateoggi+
                              importo_f;
			  }
			else
                          {
			    tabella_risorse[j].usciteoggi=
			    tabella_risorse[j].usciteoggi-
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			      tabella_risorse[numfonti].usciteoggi=
			      tabella_risorse[numfonti].usciteoggi-
                              importo_f;
			  }
		      }
		    if((numg_fa>=numg_inimese)&&
		       (numg_fa<=num_g))
		      {
//Caso specifico per:
//guadagno, entrate, uscite da inizio mese a oggi
			tabella_risorse[j].gaininimese=
		        tabella_risorse[j].gaininimese+
			importo_f;
//Dato globale
			tabella_risorse[numfonti].gaininimese=
			tabella_risorse[numfonti].gaininimese+
                        importo_f;
			if(importo_f>=0)
                          {
			    tabella_risorse[j].entrateinimese=
			    tabella_risorse[j].entrateinimese+
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			      tabella_risorse[numfonti].entrateinimese=
			      tabella_risorse[numfonti].entrateinimese+
                              importo_f;
			  }
			else
                          {
			    tabella_risorse[j].usciteinimese=
			    tabella_risorse[j].usciteinimese-
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			      tabella_risorse[numfonti].usciteinimese=
			      tabella_risorse[numfonti].usciteinimese-
                              importo_f;
			  }
		      }
		    if((numg_fa>=numg_inimese)&&
		       (numg_fa<numg_finmese))
		      {
//Caso specifico per:
//entrate, uscite, guadagno da inizio mese a fine mese
			tabella_risorse[j].gainfinmese=
		        tabella_risorse[j].gainfinmese+
			importo_f;
//Dato globale
			tabella_risorse[numfonti].gainfinmese=
			tabella_risorse[numfonti].gainfinmese+
			importo_f;
			if(importo_f>=0)
                          {
		            tabella_risorse[j].entratefinmese=
			    tabella_risorse[j].entratefinmese+
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			      tabella_risorse[numfonti].entratefinmese=
			      tabella_risorse[numfonti].entratefinmese+
                              importo_f;
			  }
			else
                          {
		            tabella_risorse[j].uscitefinmese=
			    tabella_risorse[j].uscitefinmese-
			    importo_f;
			    if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			      tabella_risorse[numfonti].uscitefinmese=
			      tabella_risorse[numfonti].uscitefinmese-
                              importo_f;
			  }
		      }
		    break;
                  }
	        j++;
	      }
	    while(j<numfonti);
	  }
	for(i=0;i<nrighe;i++)
	  if(!fgets(string,400,fil))
	    {
//Mi aspettavo una riga e invece non trovo niente: errore
	      MessageBox(HWindow,
	      "Anomalia nel file dati:ag4",
	      "Problema!",MB_ICONEXCLAMATION | MB_OK);
	      fclose(fil);
	      return -1;
	    }
      }
    while(1);
//Se sono qui: raggiunta la fine del file
//posso passare ad aggiornare basedata.dat
    fclose(fil);
    if(!(map=fopen("basedata.dat","r")))
      {
//Non trovo basedata.dat: errore
	MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -1;
      }
    tmp=fopen("temp","w");
    if(!fgets(string,100,map))
      {
//Non trovo niente in basedata.dat: errore
	MessageBox(HWindow,
	"Non ci sono risorse in 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	fclose(tmp);
	return -1;
      }
    if(sscanf(string,"%d\n",&numfonti)!=1)
      {
//Formattazione non corretta: errore
        MessageBox(HWindow,"Anomalia nel file dati:ag5",
	"Problema !",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	fclose(tmp);
	return -1;
      }
//Tutto OK, posso copiare in tmp
    fputs(string,tmp);
    for(i=0;i<numfonti;i++)
      {
	if(!fgets(string,100,map))
	  {
//Mi aspettavo di leggere una riga e non la trovo: errore
            MessageBox(HWindow,"Anomalia nel file dati:ag6",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
          }
	if(sscanf(string,"%d,%[!-z ]\n",&posiz,
	   nomeconv)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag7",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
//Tutto OK, posso copiare in tmp
        fputs(string,tmp);
//Leggo il giorno di apertura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag8",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
//Cerco le informazioni sul giorno di apertura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di apertura
//Tutto OK, posso copiare in tmp
          fputs(string,tmp);
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//Tutto OK, posso copiare in tmp
          fputs(string,tmp);
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag9",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
//Leggo il giorno di chiusura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag10",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
//Cerco le informazioni sul giorno di chiusura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di chiusura
//Tutto OK, posso copiare in tmp
          fputs(string,tmp);
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//Tutto OK, posso copiare in tmp
          fputs(string,tmp);
	else
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag11",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag12",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
	  }
	if(sscanf(string,"%d,%d\n",&nrighe,&nanni)!=2)
	  {
//Formattazione non corretta: errore
	    MessageBox(HWindow,"Anomalia nel file dati:ag13",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(tmp);
	    return -1;
          }
//Tutto OK, posso copiare in tmp con il numero
//corretto di anni
	switch(tabella_risorse[i].casi)
          {
	    case 0:
//c'e' l'anno in corso e l'anno dopo
//nanni rimane identico
	      deltananni=0;
	      break;
	    case 1:
//c'e' l'anno in corso ma non l'anno dopo
//dovro' aggiungere un anno a nanni
	      deltananni=1;
	      break;
	    case 2:
//non c'e' l'anno in corso ma c'e' l'anno dopo
//dovro' aggiungere un anno a nanni
	      deltananni=1;
	      break;
	    case 3:
//non c'e' l'anno in corso e non c'e' l'anno dopo
//dovro' aggiungere 2 anni a nanni
	      deltananni=2;
	      break;
	    default:
//Valore sbagliato (non dovrebbe mai succedere)
	      MessageBox(HWindow,
	      "Anomalia nel file dati:ag14",
	      "Problema!",MB_ICONEXCLAMATION | MB_OK);
	      fclose(map);
	      fclose(tmp);
	      return -1;
	  }
//Scrivo la nuova informazione su tmp
//Il valore vecchio di nanni mi servira'
//comunque nel seguito
	fprintf(tmp,"%d,%d\n",nrighe,
		nanni+deltananni);
//Copio le righe di commento
	for(j=0;j<nrighe;j++)
          {
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga
//e non la trovo: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:ag15",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
		fclose(tmp);
	        return -1;
	      }
//Tutto OK, posso copiare in tmp
            fputs(string,tmp);
	  }
//Adesso leggo tutti gli anni fino a quello di
//interesse. Al piu' mi fermero' al primo
//anno esistente dopo quello di interesse.
//Solo gli anni minori di quello di interesse
//verranno copiati inalterati in tmp
	j=0;
	while(j<nanni)
	  {
	    j++;
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo:
//errore
		MessageBox(HWindow,
		"Anomalia nel file dati:ag16",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
	        fclose(tmp);
		return -1;
	      }
	    if(sscanf(string,"%d,%ld\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:ag17",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
	        fclose(tmp);
		return -1;
	      }
	    if(a<anno)
	      fputs(string,tmp);
	    else
	      break;
//Notare che all'uscita da questo loop:
//se j<nanni  => a>=anno
	  }
//anno e anno+1 vanno sempre riscritti ex-novo
	fprintf(tmp,"%d,%ld\n",
	        anno,
	        tabella_risorse[i].saldoinanno);
	fprintf(tmp,"%d,%ld\n",
	        anno+1,
	        tabella_risorse[i].saldofinanno);
//deltasaldo serve per aggiornare il saldo
//degli anni seguenti a anno+1
//se ce ne sono
	deltasaldo=
	tabella_risorse[i].saldofinanno-
	tabella_risorse[i].saldofinannoold;
//Adesso devo decidere il valore giusto di j
//in maniera da finire a ricopiare tutti i saldi
//rimasti
	if(j<nanni)
	  {
//Solo se ci sono anni successivi
//mi pongo il problema di aggiornare j
	    if(a==anno)
              {
//Accade se non ho letto tutti
//gli anni e l'anno in questione
//era nella lista
//Vediamo se c'e' da incrementare j:
	        switch(tabella_risorse[i].casi)
                  {
	            case 0:
//c'e' l'anno in corso e l'anno dopo:
//leggo la riga per superarla
//e incremento j
		      if(!fgets(string,100,map))
	                {
//Mi aspettavo di leggere una riga e non la trovo: errore
			  MessageBox(HWindow,
			  "Anomalia nel file dati:ag17_1",
			  "Problema!",
			  MB_ICONEXCLAMATION | MB_OK);
	                  fclose(map);
	                  fclose(tmp);
			  return -1;
	                }
	              if(sscanf(string,"%d,%ld\n",&a,&saldo)!=2)
	                {
//Formattazione non corretta: errore
		          MessageBox(HWindow,
		          "Anomalia nel file dati:ag17_2",
			  "Problema!",
			  MB_ICONEXCLAMATION | MB_OK);
	                  fclose(map);
	                  fclose(tmp);
		          return -1;
			}
		      if(a!=anno+1)
			{
//Ci doveva essere anno+1 ma non c'e': errore
		          MessageBox(HWindow,
		          "Anomalia nel file dati:ag17_3",
			  "Problema!",
			  MB_ICONEXCLAMATION | MB_OK);
	                  fclose(map);
	                  fclose(tmp);
		          return -1;
			}
		      j++;
	              break;
	            case 1:
//c'e' l'anno in corso ma non l'anno dopo
//non incremento j
		      break;
		    default:
//Tutti gli altri valori sono sbagliati
//in questo contesto (non dovrebbe mai succedere)
		      MessageBox(HWindow,
		      "Anomalia nel file dati:ag18",
		      "Problema!",
		      MB_ICONEXCLAMATION | MB_OK);
	              fclose(map);
	              fclose(tmp);
	              return -1;
		  }
	      }
	  }
	if(a>anno+1)
	  {
//Se accade => ho letto tutti
//gli anni oppure no, l'anno in questione
//non era nella lista e non c'e' anno+1
//in questo caso copio su temp quello
//che avevo gia' letto
	    fputs(string,tmp);
	  }
//Ora riprendo a copiare fino alla fine
	while(j<nanni)
	  {
	    j++;
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
	        MessageBox(HWindow,"Anomalia nel file dati:ag19",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
	        fclose(tmp);
		return -1;
	      }
	    if(sscanf(string,"%d,%ld\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
		MessageBox(HWindow,
		"Anomalia nel file dati:ag20",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(map);
	        fclose(tmp);
		return -1;
	      }
//Tutto OK, posso aggiornare tmp
            fprintf(tmp,"%d,%ld\n",a,
	    saldo+deltasaldo);
	  }
      }
//Ho finito la copia: chiudo e sostituisco
//temp a basedata.dat
    fclose(map);
    fclose(tmp);
    remove("basedata.dat");
    rename("temp","basedata.dat");
    return 1;
  }

void TOggiDia::mostrasaldi()
//Mostra il dato numerico richiesto
//(saldo, entrate, ecc.)
//per la risorsa selezionata
  {
    if((index=SELBANC->GetSelIndex())<0)
      {
//Se la stringa non corrisponde a una risorsa valida
//metto la prima risorsa di default
	index=0;
	SELBANC->SetSelIndex(0);
      }
    if((k=SELDAT->GetSelIndex())<0)
      {
//Se la stringa non corrisponde a un dato valido
//metto il saldo a oggi di default
	k=2;
	SELDAT->SetSelIndex(2);
      }
    switch(k)
      {
	case 0:
          sprintf(string,"%ld",
		  tabella_risorse[index].saldoinanno);
	  break;
	case 1:
	  sprintf(string,"%ld",
		  tabella_risorse[index].saldoinimese);
	  break;
	case 2:
	  sprintf(string,"%ld",
		  tabella_risorse[index].saldooggi);
	  break;
	case 3:
	  sprintf(string,"%ld",
		  tabella_risorse[index].saldofinmese);
	  break;
	case 4:
	  sprintf(string,"%ld",
		  tabella_risorse[index].saldofinanno);
	  break;
	case 5:
	  sprintf(string,"%ld",
		  tabella_risorse[index].entrateinianno);
	  break;
	case 6:
	  sprintf(string,"%ld",
		  tabella_risorse[index].entrateinimese);
	  break;
	case 7:
	  sprintf(string,"%ld",
		  tabella_risorse[index].entrateoggi);
	  break;
	case 8:
	  sprintf(string,"%ld",
		  tabella_risorse[index].entratefinmese);
	  break;
	case 9:
	  sprintf(string,"%ld",
		  tabella_risorse[index].entratefinanno);
	  break;
	case 10:
	  sprintf(string,"%ld",
		  tabella_risorse[index].usciteinianno);
	  break;
	case 11:
	  sprintf(string,"%ld",
		  tabella_risorse[index].usciteinimese);
	  break;
	case 12:
	  sprintf(string,"%ld",
		  tabella_risorse[index].usciteoggi);
	  break;
	case 13:
	  sprintf(string,"%ld",
		  tabella_risorse[index].uscitefinmese);
	  break;
	case 14:
	  sprintf(string,"%ld",
		  tabella_risorse[index].uscitefinanno);
	  break;
	case 15:
	  sprintf(string,"%ld",
		  tabella_risorse[index].gaininianno);
	  break;
	case 16:
	  sprintf(string,"%ld",
		  tabella_risorse[index].gaininimese);
	  break;
	case 17:
	  sprintf(string,"%ld",
		  tabella_risorse[index].gainoggi);
	  break;
	case 18:
	  sprintf(string,"%ld",
		  tabella_risorse[index].gainfinmese);
	  break;
	case 19:
	  sprintf(string,"%ld",
		  tabella_risorse[index].gainfinanno);
	  break;
	default:
          ;
      }
    RESULT->SetText(string);
  }

void TOggiDia::HandSelTipoDato(RTMessage Msg)
//Mi fa vedere il tipo di dato che seleziono
  {
    if(Msg.LP.Hi==CBN_SELENDOK)
      {
        mostrasaldi();
      }
  }

void TOggiDia::aggiornacategorie()
//Aggiorna il file di categorie con quelle
//che trova nella lista
  {
    int e;

    if((CATE->GetText(string1,42)>0))
//Se c'e' una stringa
      if((CATE->FindExactString(string1,-1))<0)
//Se la stringa non esisteva nella lista
//la aggiungo
	CATE->AddString(string1);
//Adesso ricopio nel file tutti i membri della lista
    if((e=CATE->GetCount())<0)
      return;
    tmp=fopen("categ.dat","w");
    fprintf(tmp,"%d\n",e);
    for(i=0;i<e;i++)
      {
	CATE->GetString(cat,i);
	fprintf(tmp,"%s\n",cat);
      }
    fclose(tmp);
  }

void TOggiDia::caricacategorie()
//Carica la lista delle categorie con
//la lista memorizzata in categ.dat
  {
    int e;

    if(!(tmp=fopen("categ.dat","r")))
      {
//Non trovo il file, ne inizializzo
//uno vuoto e ritorno
	tmp=fopen("categ.dat","w");
	fprintf(tmp,"0\n");
	fclose(tmp);
	return;
      }
    CATE->ClearList();
    fgets(string,50,tmp);
    sscanf(string,"%d\n",&e);
    for(i=0;i<e;i++)
      {
        fgets(string,50,tmp);
	sscanf(string,"%[!-z ]\n",cat);
	CATE->AddString(cat);
      }
    fclose(tmp);
  }