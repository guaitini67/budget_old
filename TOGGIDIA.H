
// *********************** CLASSE TOggiDia *******************************************

_CLASSDEF(TOggiDia)


extern int caricasaldi(long int,info_risorse *);
extern int aggiornasaldi(long int ,int ,char *,info_risorse *);
extern void mostrasaldi(PTComboGiov,PTComboGiov,
                 PTStatic,info_risorse *);
extern int leggiprepara(long int,char *);
extern int scrivifile(char *,long int,char *);
extern int scrivinote(HWND,char *,long int,int);
extern int cancella(HWND,char *,long int,char *);
extern int scrivilog(char,char *);
extern int caricav(info_risorse *,int *);
extern float search_util(FILE *,char *,char,char,
		  long int *,int *,int *,int *,
		  int *,int *);

extern info_risorse t_risorse1[100];
extern info_movimenti tabella_movimenti[100];

class TOggiDia:public TDialog
  {
    public:

      char string[401],string1[401],slog[20],
	   nomefile_in[30],nomefile_out[30],
	   entra,nomeconv[27],mot[100],cat[100],
	   mot_f[100],cat_f[100],*visino,visetto;
      int  i,j,k,g,m,a,viene_da,va_in,n_lpa,n_lpb,
	   posiz,fine,tab[5],pa,pb,index,
	   numfonti,nrighe,nlnote,nanni,num_ops;
      int ind[100],erno;
      long int numg_out,numg_in,numg_fa,numg_fb,
	       numg_riepilogo;
      double importo,saldo,importo_f;
      struct tm tcheck;
      struct time Time;
      struct date Date;
      FILE *map,*fil,*tmp;
      PTGroupBox GRUP;
      PTRadioButton PULSE,PULSU;
      PTButton BOK;
      PTComboGiov CATE;
      PTEdit IMPO,MOTI;
      PTListBox RIEPILO,FAIVED;
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
      void ComplexSel();
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
      void aggiornacategorie();
      void caricacategorie();
      void HandNonVed()=[ID_FIRST+ID_NONFARVBUT];
      void nonved();
      void HandFarVed()=[ID_FIRST+ID_FAIVBUT];
      void ved();
      void HandSoloNonVed()=[ID_FIRST+ID_SOLNFARVBUT];
      void solononved();
      void HandSoloVed()=[ID_FIRST+ID_SOLFAIVBUT];
      void soloved();
      void HandTutteVis()=[ID_FIRST+ID_TUTTEVIS];
      void tuttevis();
      void HandCalcolaBut()=[ID_FIRST+ID_CERCA];
      void eseccalc();
  };



// ************************** FUNZIONI DI TOggiDia **************************************

TOggiDia::TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,
                   char* giorno, char* vis)
  :TDialog(AParent,ResourceId)
  {
    BOK=new TButton(this,OG_OK,NULL);
    IMPO=new TEdit(this,IMPORTO,25,NULL);
    MOTI=new TEdit(this,MOTIVO,80,NULL);
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
    FAIVED=new TListBox(this,ID_FAIVED,NULL);
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
    visetto=0;
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
    scrivilog(0,"");
    scrivilog(1,"*****");
    scrivilog(0,"setup oggi eseguito");
    scrivilog(0,"*****");
  }

int TOggiDia::caricaVoci()
  {
    DESTINA->ClearList();
    PROVEN->ClearList();
    SELBANC->ClearList();
    i=caricav(t_risorse1,&numfonti);
    if(i<0)
//Se c'e' qualche problema da caricav
      {
        sprintf(string,"caricaRisorse Problema caricav %d",i);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return -1;
      }
//Faccio vedere tutte le risorse all'inizio
    for(i=0;i<numfonti;i++)
      {
	sprintf(string,"VIS:  %s",t_risorse1[i].nomeconv);
	FAIVED->AddString(string);
      }
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
	  ((t_risorse1[i].aperto==-1)&&
	  (t_risorse1[i].chiuso==-1))
        ||
//2: ho specificato solo la data di chiusura, ma
//questa e' posteriore al giorno che mi interessa
	  ((t_risorse1[i].aperto==-1)&&
	  (t_risorse1[i].chiuso>num_g))
        ||
//3: ho specificato solo la data di apertura, ma
//questa e' anteriore al giorno che mi interessa
	  ((t_risorse1[i].aperto<num_g)&&
	  (t_risorse1[i].chiuso==-1))
        ||
//4: ho specificato entrambe le date, e
//il giorno che mi interessa e'in mezzo
	  ((t_risorse1[i].aperto<num_g)&&
	  (t_risorse1[i].chiuso>num_g))
	  )
	  {
	    RISORSA->AddString(t_risorse1[i].nomeconv);
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
    scrivilog(1,"Forse Mouse Scrivi");
    IMPO->GetText(string,27);
    k=sscanf(string,"%lf%s",&importo,nomeconv);
    if(k!=1)
      {
        MessageBox(HWindow,
	"Importo inserito non e' un numero valido",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"HandScrivi: Importo inserito non e' \
un numero valido");
	return;
      }
//Il trucco seguente e' per troncare via eventuali
//cifre decimali oltre la seconda, e scoprire importi di fatto
//nulli (es. 0.001). Notare che >=0.005 diventa 0.01
    sprintf(string,"%.2lf",importo);
    sscanf(string,"%lf",&importo);
    if(importo<=0)
      {
        MessageBox(HWindow,
	"I valori negativi o nulli non sono ammessi.",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"HandScrivi: I valori negativi o nulli\
non sono ammessi.");
        return;
      }
    if(!(MOTI->GetText(mot,82)))
      {
        MessageBox(HWindow,"Inserire Motivo",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"HandScrivi: Inserire Motivo");
	return;
      }
    if(!(CATE->GetText(cat,42)))
      {
        MessageBox(HWindow,"Inserire Categoria",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"HandScrivi: Inserire Categoria");
	return;
      }
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
	    scrivilog(1,"HandScrivi: Inserire 'Entra in'");
	    return;
	  }
	if((k=DESTINA->FindExactString(string,-1))<0)
	  {
	    MessageBox(HWindow,
	    "'Entra in' inserito non corrisponde a \
una voce valida",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,"HandScrivi: 'Entra in' inserito non \
corrisponde a una voce valida");
	    return;
	  }
//Individuo il numero della risorsa selezionata
	i=0;
	while(strcmp(string,t_risorse1[i].nomeconv))
	  i++;
	va_in=t_risorse1[i].posiz;
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
	    scrivilog(1,"HandScrivi: Anomalia nel file dati entrata 1");
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
              {
		scrivilog(1,"HandScrivi: 'Esce da' inserita non e'una voce valida.\
La ignoro e vado avanti (e' come se non l'avessi scritta)? NO");
		return;
              }
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
                  {
//Ho deciso che voglio correggere prima di scrivere
		    scrivilog(1,"HandScrivi: La data di uscita non e' corretta \
o manca. Uso lo stesso giorno dell'entrata? NO");
		    return;
                  }
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
	    while(strcmp(string,t_risorse1[i].nomeconv))
              i++;          
	    viene_da=t_risorse1[i].posiz;
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
		scrivilog(1,"HandScrivi: Anomalia nel file dati uscita 2");
		return;
	      }
                   if(n_lpb==n_lpa)
//Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
//Questo e' un errore, questa e' la soluzione
		n_lpa=n_lpb+1;
//Ho tutte le informazioni per scrivere nel file di uscita
//Preparo la stringa da scrivere nel file
	    sprintf(string,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_in,n_lpa);
	    scrivilog(1,"HandScrivi: scrivifile 1 - \
nomefile_out,numg_out,string:");
	    scrivilog(0,nomefile_out);
	    scrivilog(0,ltoa(numg_out,slog,10));
	    scrivilog(0,string);
//Scrivo la stringa nel file di uscita
	    if((erno=scrivifile(nomefile_out,numg_out,string))
	       <0)
	      {
		sprintf(string,"HandScrivi uscita: scrivifile erno %d",
		        erno);
		MessageBox(HWindow,string,
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		scrivilog(1,string);
		return;
	      }
	  }
//Ho tutte le informazioni per scrivere nel file di entrata
//Preparo la stringa da scrivere nel file
        sprintf(string,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d",
		n_lpa,importo,mot,cat,viene_da,va_in,
		numg_out,n_lpb);
	scrivilog(1,"HandScrivi: scrivifile 2 - \
nomefile_in,numg_in,string:");
	scrivilog(0,nomefile_in);
	scrivilog(0,ltoa(numg_in,slog,10));
	scrivilog(0,string);
//Scrivo la stringa nel file di entrata
	if((erno=scrivifile(nomefile_in,numg_in,string))<0)
	  {
	    sprintf(string,"HandScrivi entrata: scrivifile erno %d",
	            erno);
	    MessageBox(HWindow,string,
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,string);
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
	    scrivilog(1,"HandScrivi: Inserire 'Esce da'");
	    return;
	  }
	if((k=PROVEN->FindExactString(string,-1))<0)
	  {
	    MessageBox(HWindow,
	    "'Esce da' inserito non corrisponde \
a una voce valida",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,"HandScrivi: 'Esce da' inserito non corrisponde \
a una voce valida");
	    return;
	  }
//Individuo il numero della risorsa selezionata
        i=0;
	while(strcmp(string,t_risorse1[i].nomeconv))
          i++;          
	viene_da=t_risorse1[i].posiz;
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
	    scrivilog(1,"HandScrivi: Anomalia nel file dati uscita 8");
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
              {
//Ho deciso che voglio correggere prima di scrivere
		scrivilog(1,"HandScrivi: 'Entra in' inserita non e'una voce valida.\
La ignoro e vado avanti (e' come se non l'avessi scritta)? NO");
	        return;
              }
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
                  {
//Ho deciso che voglio correggere prima di scrivere
		    scrivilog(1,"HandScrivi: La data di entrata non e' corretta \
o manca. Uso lo stesso giorno dell'uscita? NO");
		    return;
                  }
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
	    while(strcmp(string,t_risorse1[i].nomeconv))
              i++;          
	    va_in=t_risorse1[i].posiz;
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
		scrivilog(1,"HandScrivi: Anomalia nel file dati entrata 9");
		return;
	      }
	    if(n_lpb==n_lpa)
//Se i due giorni coincidono, ho ottenuto n_lpa==n_lpb!!
//Questo e' un errore, questa e' la soluzione
		n_lpa=n_lpb+1;
//Ho tutte le informazioni per scrivere nel file di entrata
//Preparo la stringa da scrivere nel file
	    sprintf(string,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_out,n_lpa);
	    scrivilog(1,"HandScrivi: scrivifile 3 - \
nomefile_in,numg_in,string:");
	    scrivilog(0,nomefile_in);
	    scrivilog(0,ltoa(numg_in,slog,10));
	    scrivilog(0,string);
//Scrivo la stringa nel file di entrata
	    if((erno=scrivifile(nomefile_in,numg_in,string))<0)
	      {
		sprintf(string,"HandScrivi entrata: scrivifile erno %d",
		        erno);
		MessageBox(HWindow,string,
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		scrivilog(1,string);
		return;
	      }
	  }
//Ho tutte le informazioni per scrivere nel file di uscita
//Preparo la stringa da scrivere nel file
	sprintf(string,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d",
		n_lpa,importo,mot,cat,viene_da,va_in,
		numg_in,n_lpb);
	scrivilog(1,"HandScrivi: scrivifile 4 - \
nomefile_out,numg_out,string:");
	scrivilog(0,nomefile_out);
	scrivilog(0,ltoa(numg_out,slog,10));
	scrivilog(0,string);
//Scrivo la stringa nel file di uscita
	if((erno=scrivifile(nomefile_out,numg_out,string))<0)
	  {
	    sprintf(string,"HandScrivi uscita: scrivifile erno %d",
	            erno);
	    MessageBox(HWindow,string,
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,string);
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
    int g,m,a,ii;
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

    if((erno=caricasaldi(num_g,t_risorse1))<0)
      {
//C'e' stato un problema nella lettura di basedata.dat
	sprintf(string,
	"TOggiDia, riepiloga, caricasaldi,\
controllabasedata: erno n. %d",erno);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return -1;
      }
    if((erno=aggiornasaldi(num_g,numfonti,
       nome_file,t_risorse1))<0)
      {
//C'e' stato un problema nella lettura del file di dati
//o nella riscrittura di basedata.dat
	sprintf(string,"TOggiDia, riepiloga, \
aggiornasaldi : erno n. %d",erno);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return -1;
      }
    mostrasaldi(SELBANC,SELDAT,RESULT,t_risorse1);

//Individuo nome file da aprire
    sprintf(nome_file,"%d.bud",a);
    if(!(tmp=fopen("note.tmp","w")))
      {
//Non riesco a creare il file: errore (precauzione eccessiva?)
        MessageBox(HWindow,
	"Non riesco a creare il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"riepiloga: Non riesco a creare il file 'note.tmp'");
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
	    scrivilog(1,"riepiloga: Anomalia nel file dati:r1");
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
		scrivilog(1,"riepiloga: Anomalia nel file dati:r2");
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
    ii=0;
    for(i=0;i<num_ops;i++)
      {
	if(!fgets(string,400,fil))
	  {
//Mi aspettavo una riga e invece non trovo niente: errore
	    MessageBox(HWindow,
	    "Anomalia nel file dati:r3",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(tmp);
	    fclose(fil);
	    scrivilog(1,"riepiloga: Anomalia nel file dati:r3");
	    return -1;
	  }
	if(sscanf(string,
	  "%d,%lf,%[!-~ ¡-ÿ‘’]\t,%[!-~ ¡-ÿ‘’]\t,%d,%d,%ld,%d\n",
	  &j,&importo_f,mot_f,cat_f,&pa,&pb,
	  &numg_fb,&k)!=8)
	  {
//Trovo i dati in un formato non valido: errore
            MessageBox(HWindow,
	    "Anomalia nel file dati:r4",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(tmp);
	    fclose(fil);
	    scrivilog(1,"riepiloga: Anomalia nel file dati:r4");
	    return -1;
	  }
//Se sono arrivato qui tutto e' a posto
//Posso scrivere nella casella di riepilogo
	if(importo_f>0)
	  {
//Se e' un'entrata
            index=0;
	    while(t_risorse1[index].posiz<pb)
	      {
                index++;
              }
            sprintf(string,
"%s\t| +%.2lf\t| %s\t| %s",t_risorse1[index].nomeconv,
              importo_f,mot_f,cat_f);
	  }
	else
	  {
//Se e' un'uscita
	    index=0;
	    while(t_risorse1[index].posiz<pa)
	      {
                index++;
              }
	    sprintf(string,
"%s\t| %.2lf\t| %s\t| %s",t_risorse1[index].nomeconv,
                importo_f,mot_f,cat_f);
	  }
	if(t_risorse1[index].mostra)
	  {
//Solo se il flag .mostra della risorsa
//non e' 0 il movimento viene mostrato
	    RIEPILO->AddString(string);
//Ora scrivo in un record le informazioni relative
//a questo movimento. Mi serviranno per cancellare
//e per selezionare e cose simili.
            tabella_movimenti[ii].posiz_da=pa;
	    tabella_movimenti[ii].posiz_in=pb;
	    tabella_movimenti[ii].numop_a=j;
	    tabella_movimenti[ii].numop_b=k;
	    tabella_movimenti[ii].numg_a=numg_fa;
	    tabella_movimenti[ii].numg_b=numg_fb;
	    ii++;
	  }
      }
//Se sono arrivato qui ho finito il riempimento
//Ora controllo le righe di note: se ci sono
//le appoggio sul file "note.tmp"
    for(i=0;i<nrighe;i++)
      {
        if(!fgets(string,400,fil))
          {
//Mi aspettavo una riga e invece non trovo niente: errore
            MessageBox(HWindow,
            "Anomalia nel file dati:r5",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    fclose(tmp);
	    scrivilog(1,"riepiloga: Anomalia nel file dati:r5");
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
    scrivilog(1,"annulla end");
  }

void TOggiDia::HandNote()
//Mi manda alla pagina delle note. Quando
//esco scrivo il file
  {
    int a;
    long int pippo;

    scrivilog(1,"Forse Mouse note");
    GetModule()->ExecDialog(new TNoteDia(this,"NOTE",
                            &nlnote,numg_riepilogo));
//Individuo il file su cui scrivere l'uscita
    pippo= numg_riepilogo/100;
    a=pippo/100;
    sprintf(nomefile_in,"%d.bud",a);
    scrivinote(HWindow,nomefile_in,numg_riepilogo,nlnote);
    riepiloga(numg_riepilogo);
    scrivilog(1,"HandNote: note end");
  }


void TOggiDia::HandVai()
  {
//Mostra i movimenti alla data indicata nei campi
//di riepilogo 
    long int numg;
    int g,m,a;

    scrivilog(1,"Forse Mouse Vai");
    caricacampi(RiGior,RiMes,RiAnn);
    RiAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    RiMes->GetText(string,3);
    sscanf(string,"%d",&m);
    RiGior->GetText(string,3);
    sscanf(string,"%d",&g);
    numg=(long int)a*10000+m*100+g;
    riepiloga(numg);
    scrivilog(0,"Giorno corrente:");
    scrivilog(0,ltoa(numg,string,10));
  }

void TOggiDia::DefCommandProc(RTMessage Msg)
//Qui scelgo una serie di tasti che mi permettono
//di evitare di lavorare col mouse.
//NOTA BENISSIMO: ho scoperto che se provo a usare
//CTRL+H, dopo non mi capisce piu' il backspace
//nei controlli di edit
  {
//    int sele;

    if(visetto)
      return;

    if ( Msg.WP.Hi == 0 )
      {
	switch(Msg.WP.Lo)
	  {
	    case ACC_CTA:
//Se digito CTRL+A, mi equivale a premere ANNULLA
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un stfocus in piu' per creare ritardo)
              scrivilog(1,"CTRL-A");
	      SendDlgItemMessage(HWindow,
	      ANNUL, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ANNUL, BM_SETSTATE, FALSE, 0);
	      HandAnnulla();
	      break;
	    case ACC_CTD:
//Se digito CTRL+D, mi posiziona su IMPORTO
	      scrivilog(1,"CTRL-D");
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTE:
//Se digito CTRL+E, mi seleziona Entrata
//e mi posiziona su IMPORTO
	      scrivilog(1,"CTRL-E");
	      CheckEntrata();
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTF:
//Se digito CTRL+F, mi posiziona sulla lista movimenti
//e seleziona la prima voce se c'e'
	      scrivilog(1,"CTRL-F");
	      SetFocus(RIEPILO->HWindow);
	      RIEPILO->SetSelIndex(0);
	      break;
	    case ACC_CTG:
//Se digito CTRL+G, mi posiziona su data di riepilogo
	      scrivilog(1,"CTRL-G");
	      SetFocus(RiGior->HWindow);
	      break;
	    case ACC_CTK:
//Se digito CTRL+K, mi equivale a premere CALCOLI
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un setfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-K");
	      SendDlgItemMessage(HWindow,
	      ID_CERCA, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_CERCA, BM_SETSTATE, FALSE, 0);
              eseccalc();
	      break;
	    case ACC_CTN:
//Se digito CTRL+N, mi equivale a premere NOTE
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un setfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-N");
	      SendDlgItemMessage(HWindow,
	      OG_NOTE, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      OG_NOTE, BM_SETSTATE, FALSE, 0);
	      HandNote();
	      break;
	    case ACC_CTR:
//Se digito CTRL+R, mi seleziona Uscita
//e mi posiziona su IMPORTO
	      scrivilog(1,"CTRL-R");
	      CheckUscita();
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTQ:
//Se digito CTRL+Q, mi posiziona su ENTRA IN
	      scrivilog(1,"CTRL-Q");
	      SetFocus(DESTINA->HWindow);
	      break;
	    case ACC_CTS:
//Se digito CTRL+S, mi equivale a premere SCRIVI
//Metto il focus su importo per toglierlo
//ai campi date e impedire date errate
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un stfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-S");
	      SendDlgItemMessage(HWindow,
	      SCRIVI, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      SCRIVI, BM_SETSTATE, FALSE, 0);
	      HandScrivi();
	      break;
	    case ACC_CTT:
//Se digito CTRL+T, mi posiziona sulla lista visibilita'
	      scrivilog(1,"CTRL-T");
	      SetFocus(FAIVED->HWindow);
	      break;
	    case ACC_CTW:
//Se digito CTRL+W, mi posiziona su ESCE DA
	      scrivilog(1,"CTRL-W");
	      SetFocus(PROVEN->HWindow);
	      break;
	    case ACC_ESCAPE:
//Se spingo ESC, mi posiziona su OK
	      SetFocus(BOK->HWindow);
	      break;
	    case ACC_F1:
//Se spingo F1, mi calcola la conversione
//lira -> euro nel campo importo
	      scrivilog(1,"F1");
	      IMPO->GetText(string,27);
	      k=sscanf(string,"%lf%s",
	      &importo,nomeconv);
	      if(k==1)
		{
		  importo=importo/1936.27;
		  sprintf(string,"%.2f",importo);
                  IMPO->Clear();
                  IMPO->Insert(string);
	        }
	      break;
	    case ACC_F2:
//Se spingo F2, mi calcola la conversione
//euro -> lira nel campo importo
	      scrivilog(1,"F2");
	      IMPO->GetText(string,27);
	      k=sscanf(string,"%lf%s",
	      &importo,nomeconv);
	      if(k==1)
		{
		  importo=importo*1936.27;
		  sprintf(string,"%.0f",importo);
                  IMPO->Clear();
                  IMPO->Insert(string);
	        }
	      break;
	    case ACC_F3:
//Se spingo F3, mi calcola la conversione
//lira -> euro nel campo dati
	      scrivilog(1,"F3");
	      RESULT->GetText(string,27);
	      k=sscanf(string,"%lf%s",
	      &importo,nomeconv);
	      if(k==1)
		{
		  importo=importo/1936.27;
		  sprintf(string,"%.2f",importo);
		  RESULT->SetText(string);
	        }
	      break;
	    case ACC_F4:
//Se spingo F4, mi calcola la conversione
//euro -> lira nel campo dati
	      scrivilog(1,"F4");
	      RESULT->GetText(string,27);
	      k=sscanf(string,"%lf%s",
	      &importo,nomeconv);
	      if(k==1)
		{
		  importo=importo*1936.27;
		  sprintf(string,"%.0f",importo);
		  RESULT->SetText(string);
	        }
	      break;
	    case ACC_F5:
//Se spingo F5, equivale a spingere
//"selezionate non visibili" (con effetto bottone)
	      scrivilog(1,"F5");
	      SendDlgItemMessage(HWindow,
	      ID_NONFARVBUT, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_NONFARVBUT, BM_SETSTATE, FALSE, 0);
              nonved();
	      break;
	    case ACC_F6:
//Se spingo F6, equivale a spingere
//"solo selezionate non visibili" (con effetto bottone)
	      scrivilog(1,"F6");
	      SendDlgItemMessage(HWindow,
	      ID_SOLNFARVBUT, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_SOLNFARVBUT, BM_SETSTATE, FALSE, 0);
              solononved();
	      break;
	    case ACC_F7:
//Se spingo F7, equivale a spingere
//"selezionate visibili" (con effetto bottone)
	      scrivilog(1,"F7");
	      SendDlgItemMessage(HWindow,
	      ID_FAIVBUT, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_FAIVBUT, BM_SETSTATE, FALSE, 0);
              ved();
	      break;
	    case ACC_F8:
//Se spingo F8, equivale a spingere
//"solo selezionate visibili" (con effetto bottone)
	      scrivilog(1,"F8");
	      SendDlgItemMessage(HWindow,
	      ID_SOLFAIVBUT, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_SOLFAIVBUT, BM_SETSTATE, FALSE, 0);
              soloved();
	      break;
	    case ACC_F9:
//Se spingo F9, equivale a spingere
//"tutte visibili" (con effetto bottone)
	      scrivilog(1,"F9");
	      SendDlgItemMessage(HWindow,
	      ID_TUTTEVIS, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      ID_TUTTEVIS, BM_SETSTATE, FALSE, 0);
              tuttevis();
	      break;
	    case ACC_CTDELETE:
//Se digito CTRL+canc, mi equivale a premere CANC
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un stfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-DEL");
	      SendDlgItemMessage(HWindow,
	      OG_CANC, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(IMPO->HWindow);
	      SendDlgItemMessage(HWindow,
	      OG_CANC, BM_SETSTATE, FALSE, 0);
	      HandCancella();
	      break;
	    case ACC_CTRETURN:
//Se digito CTRL+enter, mi equivale a cliccare due volte
//in una voce di riepilogo
//Alla fine mi posiziona su IMPORTO
	      scrivilog(1,"CTRL-RET");
	      if(RIEPILO->GetSelString(string1,400)>1)
	        SimpleSel();
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTALTRET:
//Se digito CTRL+alt+enter, mi equivale a cliccare due volte
//in una voce di riepilogo, selezionare di nuovo e cancellare
//E' utile per sveltire le sostituzioni
//Alla fine mi posiziona su IMPORTO
	      scrivilog(1,"CTRL-ALT-RET");
	      if(RIEPILO->GetSelString(string1,400)>1)
		ComplexSel();
	      SetFocus(IMPO->HWindow);
	      break;
	    case ACC_CTUP:
//Se digito CTRL+freccia su, mi visualizza il giorno prima
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un stfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-UP");
	      SendDlgItemMessage(HWindow,
	      DIETRO, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(SELDAT->HWindow);
	      SendDlgItemMessage(HWindow,
	      DIETRO, BM_SETSTATE, FALSE, 0);
	      HandGPrima();
	      break;
	    case ACC_CTDOWN:
//Se digito CTRL+freccia giu', mi visualizza il giorno dopo
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
//C'e' anche l'effetto speciale del bottone premuto
//(faccio un stfocus in piu' per creare ritardo)
	      scrivilog(1,"CTRL-DWN");
	      SendDlgItemMessage(HWindow,
	      AVANTI, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(SELDAT->HWindow);
	      SendDlgItemMessage(HWindow,
	      AVANTI, BM_SETSTATE, FALSE, 0);
	      HandGDopo();
	      break;
	    case ACC_CTNEXT:
//Se digito CTRL+page down, mi equivale a premere VAI
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
	      scrivilog(1,"CTRL-PGDWN");
	      SendDlgItemMessage(HWindow,
	      OG_VAI, BM_SETSTATE, TRUE, 0);
	      SetFocus(SELBANC->HWindow);
	      SetFocus(SELDAT->HWindow);
	      SendDlgItemMessage(HWindow,
	      OG_VAI, BM_SETSTATE, FALSE, 0);
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

    scrivilog(1,"Forse Mouse Gprima");
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
    scrivilog(0,"Giorno corrente:");
    scrivilog(0,ltoa(numg,string,10));
  }

void TOggiDia::HandGDopo()
  {
//Mostra i movimenti del giorno successivo
    int g,m,a;
    long int numg;

    scrivilog(1,"Forse Mouse Gdopo");
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
    scrivilog(0,"Giorno corrente:");
    scrivilog(0,ltoa(numg,string,10));
  }

void TOggiDia::HandOK()
  {
    scrivilog(1,"OK spinto");
    CloseWindow();
  }

void TOggiDia::CloseWindow()
  {
    *visino=0;
    remove("note.tmp");
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
        scrivilog(1,"Mouse chkentrata");
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
	scrivilog(1,"Mouse chkuscita");
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
	scrivilog(1,"Mouse seleziona 2 click");
	SimpleSel();
      }
  }

void TOggiDia::ComplexSel()
//Seleziona una stringa e la cancella dal box
//di riepilogo. Utile per cambiare
  {
    int sele;

//Memorizzo la selezione
    sele=RIEPILO->GetSelIndex();
//Tiro su la stringa selezionata (che si deseleziona)
    SimpleSel();
//Riseleziono la stringa e la cancello
    RIEPILO->SetSelIndex(sele);
    HandCancella();
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
    if(sscanf(string,"%[!-~ ¡-ÿ‘’]\t| %lf\t| %[!-~ ¡-ÿ‘’]\t| %[!-~ ¡-ÿ‘’]",
      nomeconv,&importo,mot,cat)!=4)
      {
//Per qualche motivo la stringa e' corrotta: Errore
//(Puo' succedere?)
	MessageBox(HWindow,"Anomalia stringa:p1",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"SimpleSel: Anomalia stringa:p1");
	return;
      }
    if(importo>=0)
      {
//E' un'entrata: attivo il pulsante di entrata
//e disattivo quello di uscita
        PULSE->Check();
        PULSU->Uncheck();
        entra=1;
	sprintf(string,"%.2lf",importo);
//Inserisco importo, motivo e categoria
//nei campi opportuni 
        IMPO->Insert(string);
        MOTI->Insert(mot);
        CATE->Insert(cat);
//Recupero il numero risorsa dell'entrata
        j=tabella_movimenti[k].posiz_in;
        i=0;
	while(t_risorse1[i].posiz<j)
          {
	    i++;
          }
	DESTINA->Insert(t_risorse1[i].nomeconv);
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
	while(t_risorse1[i].posiz<j)
	  {
	    i++;
          }
	PROVEN->Insert(t_risorse1[i].nomeconv);
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
	sprintf(string,"%.2lf",importo);
//Inserisco importo, motivo e categoria
//nei campi opportuni 
        IMPO->Insert(string);
	MOTI->Insert(mot);
        CATE->Insert(cat);
//Recupero il numero risorsa dell'uscita
        j=tabella_movimenti[k].posiz_da;
        i=0;
	while(t_risorse1[i].posiz<j)
          {
	    i++;
          }
	PROVEN->Insert(t_risorse1[i].nomeconv);
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
	while(t_risorse1[i].posiz<j)
          {
            i++;
          }
	DESTINA->Insert(t_risorse1[i].nomeconv);
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
	mostrasaldi(SELBANC,SELDAT,RESULT,t_risorse1);
      }
  }


void TOggiDia::HandCancella()
//Cancella la voce selezionata nel box di riepilogo
  {
    int a,index;
    long int pippo,numg_in,numg_out;

    scrivilog(1,"Forse Mouse Cancella");
//Trovo l'anno relativo alla data di riepilogo
    pippo= numg_riepilogo/100;
    a=pippo/100;
    index=RIEPILO->GetSelIndex();
    if(index<0)
      {
//Non era selezionato nulla: ritorno
	scrivilog(1,"HandCancella: nulla ritorno");
	return;
      }
    RIEPILO->GetSelString(string,300);
    k=RIEPILO->GetSelIndex();
    if(sscanf(string,"%[!-~ ¡-ÿ‘’]\t| %lf\t| %[!-~ ¡-ÿ‘’]\t| %[!-~ ¡-ÿ‘’]",
       nomeconv,&importo,mot,cat)!=4)
      {
//Per qualche motivo la stringa e' corrotta: Errore
//(Puo' succedere?)
	MessageBox(HWindow,"Anomalia stringa:p1",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"HandCancella: Anomalia stringa:p1");
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
	sprintf(string,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",
	        n_lpa,importo,mot,cat,viene_da,va_in,
		numg_out,n_lpb);
	scrivilog(1,"HandCancella: cancella 1 - nomefile_in, \
numg_in, string:");
	scrivilog(0,nomefile_in);
	scrivilog(0,ltoa(numg_in,slog,10));
	scrivilog(0,string);
	if((cancella(HWindow,nomefile_in,numg_in,
	  string))<0)
	  {
//Doveva esserci un'entrata, ma nel file non c'e':
//errore
	    MessageBox(HWindow,"Anomalia: il movimento in\
 entrata da cancellare non esiste nel file",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,"HandCancella: Anomalia: il movimento in\
 entrata da cancellare non esiste nel file");
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
	    sprintf(string1,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",
	            n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_in,n_lpa);
	    scrivilog(1,"HandCancella: cancella 2 - nomefile_out, \
numg_out, string1:");
	    scrivilog(0,nomefile_out);
	    scrivilog(0,ltoa(numg_out,slog,10));
	    scrivilog(0,string1);
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
		scrivilog(1,"HandCancella: Anomalia: il movimento in\
 uscita da cancellare non esiste nel file");
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
	sprintf(string,"%d,-%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",
	        n_lpa,importo,mot,cat,viene_da,va_in,
		numg_in,n_lpb);
	scrivilog(1,"HandCancella: cancella 3 - nomefile_out, \
numg_out, string:");
	scrivilog(0,nomefile_out);
	scrivilog(0,ltoa(numg_out,slog,10));
	scrivilog(0,string);
	if((cancella(HWindow,nomefile_out,numg_out,
	  string))<0)
	  {
//Doveva esserci un'uscita, ma nel file non c'e':
//errore
	    MessageBox(HWindow,"Anomalia: il movimento in\
uscita da cancellare non esiste nel file",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,"HandCancella: Anomalia: il movimento in\
uscita da cancellare non esiste nel file");
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
	    sprintf(string1,"%d,+%.2lf,%s\t,%s\t,%d,%d,%ld,%d\n",
		    n_lpb,importo,mot,cat,viene_da,va_in,
		    numg_out,n_lpa);
	    scrivilog(1,"HandCancella: cancella 4 - nomefile_in, \
numg_in, string1:");
	    scrivilog(0,nomefile_in);
	    scrivilog(0,ltoa(numg_in,slog,10));
	    scrivilog(0,string1);
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
		scrivilog(1,"HandCancella: Anomalia: il movimento in\
uscita da cancellare non esiste nel file");
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

void TOggiDia::HandSelTipoDato(RTMessage Msg)
//Mi fa vedere il tipo di dato che seleziono
  {
    if(Msg.LP.Hi==CBN_SELENDOK)
      {
	scrivilog(1,"HandSelTipoDato");
	mostrasaldi(SELBANC,SELDAT,RESULT,t_risorse1);
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
    fgets(string,400,tmp);
    sscanf(string,"%d\n",&e);
    for(i=0;i<e;i++)
      {
	fgets(string,400,tmp);
	sscanf(string,"%[!-~ ¡-ÿ‘’]\n",cat);
	CATE->AddString(cat);
      }
    fclose(tmp);
  }

void TOggiDia::HandNonVed()
  {
    scrivilog(1,"Mouse HandNonVed");
    nonved();
  }

void TOggiDia::nonved()
//Non fa vedere nel riepilogo le risorse
//selezionate
  {
//Prima prendo gli indici delle risorse
//selezionate nella casella
    k=FAIVED->GetSelIndexes(ind,100);
    for(i=0;i<k;i++)
      {
//Gli indici selezionati li metto non visibili
        j=ind[i];
	t_risorse1[j].mostra=0;
	sprintf(string,"NONVIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
        FAIVED->InsertString(string,j);
      }
    HandVai();
  }

void TOggiDia::HandFarVed()
  {
    scrivilog(1,"Mouse HandFarVed");
    ved();
  }

void TOggiDia::ved()
//Fa vedere nel riepilogo le risorse
//selezionate
  {
//Prima prendo gli indici delle risorse
//selezionate nella casella
    k=FAIVED->GetSelIndexes(ind,100);
    for(i=0;i<k;i++)
      {
//Gli indici selezionati li metto visibili
	j=ind[i];
	t_risorse1[j].mostra=1;
	sprintf(string,"VIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
	FAIVED->InsertString(string,j);
      }
    HandVai();
  }

void TOggiDia::HandSoloVed()
  {
    scrivilog(1,"Mouse HandSoloVed");
    soloved();
  }

void TOggiDia::HandSoloNonVed()
  {
    scrivilog(1,"Mouse HandSoloNonVed");
    solononved();
  }

void TOggiDia::solononved()
//Non fa vedere nel riepilogo SOLO le risorse
//selezionate
  {
//Prima prendo gli indici delle risorse
//selezionate nella casella
    k=FAIVED->GetSelIndexes(ind,100);
    i=FAIVED->GetCount();
    for(j=0;j<i;j++)
      {
//Metto prima tutti gli indici visibili
	t_risorse1[j].mostra=1;
	sprintf(string,"VIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
        FAIVED->InsertString(string,j);
      }
    for(i=0;i<k;i++)
      {
//Gli indici selezionati li metto non visibili
	j=ind[i];
	t_risorse1[j].mostra=0;
	sprintf(string,"NONVIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
        FAIVED->InsertString(string,j);
      }
    HandVai();
  }

void TOggiDia::soloved()
//Fa vedere nel riepilogo SOLO le risorse
//selezionate
  {
//Prima prendo gli indici delle risorse
//selezionate nella casella
    k=FAIVED->GetSelIndexes(ind,100);
    i=FAIVED->GetCount();
    for(j=0;j<i;j++)
      {
//Metto prima tutti gli indici non visibili
	t_risorse1[j].mostra=0;
	sprintf(string,"NONVIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
        FAIVED->InsertString(string,j);
      }
    for(i=0;i<k;i++)
      {
//Gli indici selezionati li metto visibili
	j=ind[i];
	t_risorse1[j].mostra=1;
	sprintf(string,"VIS:  %s",
	t_risorse1[j].nomeconv);
	FAIVED->DeleteString(j);
        FAIVED->InsertString(string,j);
      }
    HandVai();
  }

void TOggiDia::HandTutteVis()
  {
    scrivilog(1,"Mouse HandTutteVis");
    tuttevis();
  }

void TOggiDia::tuttevis()
//Fa vedere nel riepilogo TUTTE le risorse
  {
    k=FAIVED->GetCount();
    for(i=0;i<k;i++)
      {
	t_risorse1[i].mostra=1;
	sprintf(string,"VIS:  %s",
	t_risorse1[i].nomeconv);
	FAIVED->DeleteString(i);
        FAIVED->InsertString(string,i);
      }
    HandVai();
  }

void TOggiDia::HandCalcolaBut()
  {
    if(visetto)
      return;

    eseccalc();
    scrivilog(1,"spinto calcola");
  }

void TOggiDia::eseccalc()
  {
    GetModule()->MakeWindow(new TCompuDia(this,"COMPUTE",&visetto));
  }


