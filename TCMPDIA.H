
// ************************* CLASSE TCompuDia *******************************************

_CLASSDEF(TCompuDia)


extern int caricasaldi(long int,info_risorse *);
extern int aggiornasaldi(long int ,int ,char *,info_risorse *);
extern void mostrasaldi(PTComboGiov,PTComboGiov,
		 PTStatic,info_risorse *);
extern int scrivilog(char,char *);
extern int caricav(info_risorse *,int *);
extern float search_util(FILE *,char *,char,char,
		  long int *,int *,int *,int *,
		  int *,int *);
extern int search_year(char *,char *,info_smaller *,
		       int,long int,long int);
extern void ordinanum(PTListGiov,PTListGiov,PTListGiov,
		      PTListGiov,PTListGiov,char);
extern void ordinalet(PTListGiov,PTListGiov,
		PTListGiov,PTListGiov,PTListGiov,char);

extern info_smaller t_risorse2[100];
extern info_risorse t_risorse3[100];


class TCompuDia:public TDialog
  {
    public:

      int i,j,k,index,numfonti,numcat,a,m,g,tab[5],stato,erno;

      PTComboGiov RiGior,RiMes,RiAnn,DaGior,DaMes,
      DaAnn,AGior,AMes,AAnn,
      SELBANC,SELBANC1,SELDAT,CATE;

      PTListGiov RIEPILO,RIEP1,RIEP2,RIEP3,RIEPRIS;
      PTButton BFATTO;

      PTRadioButton ELIGN,ELIEN,ELIUN,ELITN;
      PTGroupBox GRUP;

      PTStatic RESULT,WAIT;

      long int numg,numg_ini,numg_fin;


      char string[401],string1[400],cat[100],nomefile[30],
           creris,crerie,crerie1,crerie2,crerie3,*visino;

      struct date Date;

      struct tm tcheck;

      FILE *nf,*tmp;

      TCompuDia(PTWindowsObject,LPSTR,char *);
      ~TCompuDia();
      virtual void CloseWindow();
      virtual void WMClose(RTMessage Msg)
        =[WM_FIRST+WM_CLOSE];
      LPSTR GetClassName();
      void GetWindowClass(WNDCLASS&);
      virtual void SetupWindow();
      void DefCommandProc(RTMessage Msg);
      void caricacategorie();
      void caricaRisorse();
      void EseguiCalcolo(int,int);
      void mostradato();
      int riepiloga();
      void HandVai()=[ID_FIRST+OG_VAI];
      void HandGDopo()=[ID_FIRST+AVANTI];
      void HandGPrima()=[ID_FIRST+DIETRO];
      void HandSelSaldoRisorsa(RTMessage Msg)=
	   [ID_FIRST+SELBANCA1];
      void HandSelTipoDato(RTMessage Msg)=
	   [ID_FIRST+SELDATO];
      void HandCheckEliGN(RTMessage Msg)=[ID_FIRST+ELIGNUL];
      void EliGN();
      void HandCheckEliEN(RTMessage Msg)=[ID_FIRST+ELIENUL];
      void EliEN();
      void HandCheckEliUN(RTMessage Msg)=[ID_FIRST+ELIUNUL];
      void EliUN();
      void HandCheckEliTN(RTMessage Msg)=[ID_FIRST+ELITNUL];
      void EliTN();
      void HandEsegui()=[ID_FIRST+DOIT];
      void HandValData(PTComboGiov, PTComboGiov,
			    PTComboGiov,PTComboGiov,
			    RTMessage);
      void HandCancella()=[ID_FIRST+OG_CANC];
      void HandValRiG(RTMessage Msg)=[ID_FIRST+ID_GG];
      void HandValRiM(RTMessage Msg)=[ID_FIRST+ID_MM];
      void HandValRiA(RTMessage Msg)=[ID_FIRST+ID_AA];
      void HandValGDa(RTMessage Msg)=[ID_FIRST+ID_GDA];
      void HandValMDa(RTMessage Msg)=[ID_FIRST+ID_MDA];
      void HandValADa(RTMessage Msg)=[ID_FIRST+ID_ADA];
      void HandValGA(RTMessage Msg)=[ID_FIRST+ID_GA];
      void HandValMA(RTMessage Msg)=[ID_FIRST+ID_MA];
      void HandValAA(RTMessage Msg)=[ID_FIRST+ID_AAAA];
      void HandCalcola()=[ID_FIRST+C_CALCO];
      void lanciacalc();
      void HandPulisci()=[ID_FIRST+ANNUL];
      void HandFatto()=[ID_FIRST+C_DONE];
      void HandRiepilo(RTMessage Msg)=
	   [ID_FIRST+C_RIEPILOG];
      void HandRiep1(RTMessage Msg)=
	   [ID_FIRST+C_RIEPILOG1];
      void HandRiep2(RTMessage Msg)=
	   [ID_FIRST+C_RIEPILOG2];
      void HandRiep3(RTMessage Msg)=
	   [ID_FIRST+C_RIEPILOG3];
      void HandRiepRis(RTMessage Msg)=
	   [ID_FIRST+C_RIEPRIS];
      void HandOrdRis()=
	   [ID_FIRST+C_ORDRIS];
      void HandOrdRiep()=
	   [ID_FIRST+C_ORDRIEP];
      void HandOrdRiep1()=
	   [ID_FIRST+C_ORDRIEP1];
      void HandOrdRiep2()=
	   [ID_FIRST+C_ORDRIEP2];
      void HandOrdRiep3()=
	   [ID_FIRST+C_ORDRIEP3];
  };



// ************************** FUNZIONI DI TCompuDia **************************************

TCompuDia::TCompuDia(PTWindowsObject AParent,
		   LPSTR ResourceId,char *vis)
  :TDialog(AParent,ResourceId)
  {
    BFATTO=new TButton(this,C_DONE,NULL);
    RiGior=new TComboGiov(this,ID_GG,3,NULL);
    RiMes=new TComboGiov(this,ID_MM,3,NULL);
    RiAnn=new TComboGiov(this,ID_AAAA,5,NULL);
    DaGior=new TComboGiov(this,ID_GDA,3,NULL);
    DaMes=new TComboGiov(this,ID_MDA,3,NULL);
    DaAnn=new TComboGiov(this,ID_ADA,5,NULL);
    AGior=new TComboGiov(this,ID_GA,3,NULL);
    AMes=new TComboGiov(this,ID_MA,3,NULL);
    AAnn=new TComboGiov(this,ID_AA,5,NULL);
    SELBANC=new TComboGiov(this,SELBANCA,25,NULL);
    SELBANC1=new TComboGiov(this,SELBANCA1,25,NULL);
    SELDAT=new TComboGiov(this,SELDATO,25,NULL);
    CATE=new TComboGiov(this,CATEG,40,NULL);
    RIEPILO=new TListGiov(this,C_RIEPILOG,NULL);
    RIEP1=new TListGiov(this,C_RIEPILOG1,NULL);
    RIEP2=new TListGiov(this,C_RIEPILOG2,NULL);
    RIEP3=new TListGiov(this,C_RIEPILOG3,NULL);
    RIEPRIS=new TListGiov(this,C_RIEPRIS,NULL);
    WAIT=new TStatic(this,C_ASPETTA,50,NULL);
    GRUP=new TGroupBox(this,GRUPPO,NULL);
    ELIGN=new TRadioButton(this,ELIGNUL,GRUP,NULL);
    ELIEN=new TRadioButton(this,ELIENUL,GRUP,NULL);
    ELIUN=new TRadioButton(this,ELIUNUL,GRUP,NULL);
    ELITN=new TRadioButton(this,ELITNUL,GRUP,NULL);
    RESULT=new TStatic(this,MOSTRADATO,25,NULL);
// punti di stop per i tabulatori nella
//finestra di riepilogo
    tab[0]=100;
    tab[1]=150;                             
    tab[2]=200;

    visino=vis;
    *visino=1;
  }
                                                                             
TCompuDia::~TCompuDia()
  {
  }


LPSTR TCompuDia::GetClassName()
  {
    return "compute";
  }

void TCompuDia::GetWindowClass(WNDCLASS& AWndClass)
  {
    TDialog::GetWindowClass(AWndClass);
    AWndClass.hIcon=LoadIcon(GetApplication()->hInstance,"icona");
  }

void TCompuDia::SetupWindow()
  {
    scrivilog(1,"TCompuDia SetuWindow: inizio setup");
    TDialog::SetupWindow();
// messaggi che impostano i tabulatori
    SendMessage(RIEPILO->HWindow,
    LB_SETTABSTOPS,3,(DWORD)tab);
//Default: Elimina voci tutto nullo
    ELITN->Check();
    stato=4;
//Carico le risorse
    caricaRisorse();
//Carico le categorie
    caricacategorie();
//Il default per "da" e' 01-01-anno corrente
//e per "a" e' il giorno corrente
//(Calcoli da inizio anno a oggi)
    getdate(&Date);
    sprintf(string,"%02d",Date.da_day);
    DaGior->Insert("01");
    AGior->Insert(string);
    RiGior->Insert(string);
    sprintf(string,"%02d",Date.da_mon);
    DaMes->Insert("01");
    AMes->Insert(string);
    RiMes->Insert(string);
    sprintf(string,"%04d",Date.da_year);
    DaAnn->Insert(string);
    AAnn->Insert(string);
    RiAnn->Insert(string);

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

//Default di ordinamento: decrescenti per i numeri
//e crescenti per le stringhe di testo
    creris=1;
    crerie=1;
    crerie1=0;
    crerie2=0;
    crerie3=0;

    riepiloga();
    scrivilog(1,"TCompuDia: Eseguito setup");

  }


void TCompuDia::caricacategorie()
//Carica la lista delle categorie con
//la lista memorizzata in categ.dat
  {
    if(!(tmp=fopen("categ.dat","r")))
      {
//Non trovo il file, ne inizializzo
//uno vuoto e ritorno
	tmp=fopen("categ.dat","w");
	fprintf(tmp,"0\n");
	fclose(tmp);
	CATE->InsertString("Tutte le categorie",0);
        CATE->Insert("Tutte le categorie");
	return;
      }
    CATE->ClearList();
    fgets(string,400,tmp);
    sscanf(string,"%d\n",&numcat);
    for(i=0;i<numcat;i++)
      {
	fgets(string,400,tmp);
	sscanf(string,"%[!-~ ¡-ÿ‘’]\n",cat);
	CATE->AddString(cat);
      }
    fclose(tmp);
    CATE->InsertString("Tutte le categorie",0);
//Default e' tutte le categorie
    CATE->Insert("Tutte le categorie");
  }

void TCompuDia::caricaRisorse()
//Legge basedata.dat ed estrae le informazioni sulle
//risorse. Molte qui non servono
  {
    erno=caricav(t_risorse3,&numfonti);
    if(erno<0)
//Se c'e' qualche problema da caricav
      {
	sprintf(string,"caricaRisorse Problema caricav j= %d",j);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return;
      }
    SELBANC->ClearList();
    SELBANC1->ClearList();
    for(i=0;i<=numfonti;i++)
      {
	t_risorse2[i].posiz=t_risorse3[i].posiz;
	SELBANC->AddString(t_risorse3[i].nomeconv);
	SELBANC1->AddString(t_risorse3[i].nomeconv);
      }
//Default SELBANC e' globale
    SELBANC->GetString(string,numfonti);
    SELBANC->Insert(string);
//Default SELBANC1 e' prima voce
    SELBANC1->GetString(string,0);
    SELBANC1->Insert(string);
  }

void TCompuDia::HandValData(PTComboGiov Fst, PTComboGiov F1,
			    PTComboGiov F2,PTComboGiov F3,
			    RTMessage Msg)
  {
    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
	caricacampi(F1,F2,F3);
	Fst->SetEditSel(0,2);
	Fst->GetText(string,3);
	Fst->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      {
	caricacampi(F1,F2,F3);
      }
  }


void TCompuDia::HandValRiG(RTMessage Msg)
  {
    HandValData(RiGior,RiGior,RiMes,RiAnn,Msg);
  }

void TCompuDia::HandValRiM(RTMessage Msg)
  {
    HandValData(RiMes,RiGior,RiMes,RiAnn,Msg);
  }

void TCompuDia::HandValRiA(RTMessage Msg)
  {
    HandValData(RiAnn,RiGior,RiMes,RiAnn,Msg);
  }

void TCompuDia::HandValGDa(RTMessage Msg)
  {
    HandValData(DaGior,DaGior,DaMes,DaAnn,Msg);
  }

void TCompuDia::HandValMDa(RTMessage Msg)
  {
    HandValData(DaMes,DaGior,DaMes,DaAnn,Msg);
  }

void TCompuDia::HandValADa(RTMessage Msg)
  {
    HandValData(DaAnn,DaGior,DaMes,DaAnn,Msg);
  }

void TCompuDia::HandValGA(RTMessage Msg)
  {
    HandValData(AGior,AGior,AMes,AAnn,Msg);
  }

void TCompuDia::HandValMA(RTMessage Msg)
  {
    HandValData(AMes,AGior,AMes,AAnn,Msg);
  }

void TCompuDia::HandValAA(RTMessage Msg)
  {
    HandValData(AAnn,AGior,AMes,AAnn,Msg);
  }

void TCompuDia::HandCalcola()
  {
    scrivilog(1,"Mouse: Calcola spinto");
    lanciacalc();
  }

void TCompuDia::lanciacalc()
  {
    int numfile;

    scrivilog(1,"Compute: Calcola spinto");
//Per iniziare il giorno di inizio e quello di fine
//Mi assicuro che la data di partenza sia corretta
    caricacampi(DaGior,DaMes,DaAnn);
    DaAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    DaMes->GetText(string,3);
    sscanf(string,"%d",&m);
    DaGior->GetText(string,3);
    sscanf(string,"%d",&g);
    numg_ini=(long int)a*10000+m*100+g;
//Mi assicuro che la data di arrivo sia corretta
    caricacampi(AGior,AMes,AAnn);
    AAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    AMes->GetText(string,3);
    sscanf(string,"%d",&m);
    AGior->GetText(string,3);
    sscanf(string,"%d",&g);
    numg_fin=(long int)a*10000+m*100+g;
    if(numg_ini>numg_fin)
//Se le due date non sono successive esco
      {
	MessageBox(HWindow,
	"Data di partenza deve essere anteriore a data finale",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,
	"Compute: Data di partenza deve essere anteriore a data finale");
	return;
      }
//Determino su quanti file devo lavorare.
//Infatti le due date potrebbero essere su anni diversi
    DaAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    AAnn->GetText(string,5);
    sscanf(string,"%d",&j);
    numfile=j-a;
    CATE->GetText(cat,42);
    if((k=CATE->FindExactString(cat,-1))<0)
//Se la cosa scritta non corrisonde a una categoria non fare nulla
//(ripulisco la casella per evidenziare il "misfatto")
      {
        CATE->Clear();
	return;
      }
    WAIT->SetText("Attendere prego...");
    if(!k)
//Se ho chiesto tutte le categorie
      {
	for(j=1;j<=numcat;j++)
	  {
	    sprintf(string,"Attendere prego... %d",j);
	    WAIT->SetText(string);
	    CATE->GetString(cat,j);
	    EseguiCalcolo(a,numfile);
            mostradato();
	  }
      }
    else
      {
        EseguiCalcolo(a,numfile);
	mostradato();
      }
//Ordino di default per guadagno netto decrescente
    crerie1=0;
    ordinanum(RIEP1,RIEP2,RIEP3,RIEPILO,RIEPRIS,crerie1);
    WAIT->SetText("Ordinato decrescente per guadagno netto");
    crerie1=1;
    scrivilog(1,"Compute: Calcola eseguito");
  }

void TCompuDia::EseguiCalcolo(int a,int numfile)
  {
    int k;

//Uso l'ultimo elemento della tabella per
//contenere i dati di tutte le risorse
    for(i=0;i<=numfonti;i++)
      {
	t_risorse2[i].entrate=0;
	t_risorse2[i].uscite=0;
	t_risorse2[i].gain=0;
      }
    for(i=0;i<=numfile;i++)
//La mia ricerca sara' su tutti i file che mi interessano
      {
	sprintf(nomefile,"%d.bud",a+i);
	k=search_year(nomefile,cat,t_risorse2,
		      numfonti,numg_ini,numg_fin);
	if(k<0)
	  {
	    sprintf(string,"Compute:search_year err. n. %d",k);
	    MessageBox(HWindow,string,
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    scrivilog(1,string);
	    return;
	  }
      }
  }

void TCompuDia::HandFatto()
  {
    scrivilog(1,"Compute: Fatto spinto");
    CloseWindow();
  }


void TCompuDia::CloseWindow()
  {
    scrivilog(1,"Compute: Chiusura");
    *visino=0;
    TDialog::CloseWindow();
  }

void TCompuDia::WMClose(RTMessage Msg)
  {
    scrivilog(1,"Compute: wmclose");
    CloseWindow();
  }

void TCompuDia::mostradato()
//Mostra il dato numerico
//(guadagno, entrate, uscite)
//per la risorsa selezionata
  {
    if((index=SELBANC->GetSelIndex())<0)
      {
//Se la stringa non corrisponde a una risorsa valida
//metto il globale di default
	index=numfonti;
	SELBANC->SetSelIndex(numfonti);
      }
    sprintf(string,"%s",t_risorse3[index].nomeconv);
    RIEPRIS->AddString(string);
    RIEPILO->AddString(cat);
    sprintf(string,"%.2lf\t",
	    t_risorse2[index].gain);
    RIEP1->AddString(string);
    sprintf(string,"%.2lf\t",
	    t_risorse2[index].entrate);
    RIEP2->AddString(string);
    sprintf(string,"%.2lf\t",
	    t_risorse2[index].uscite);
    RIEP3->AddString(string);
  }

void TCompuDia::HandRiepilo(RTMessage Msg)
  {
    if(Msg.LP.Hi==LBN_SELCHANGE)
      {
        k=RIEPILO->GetSelIndex();
        RIEP1->SetSelIndex(k);
        RIEP2->SetSelIndex(k);
	RIEP3->SetSelIndex(k);
	RIEPRIS->SetSelIndex(k);
      }
  }

void TCompuDia::HandRiep1(RTMessage Msg)
  {
    if(Msg.LP.Hi==LBN_SELCHANGE)
      {
        k=RIEP1->GetSelIndex();
        RIEPILO->SetSelIndex(k);
        RIEP2->SetSelIndex(k);
	RIEP3->SetSelIndex(k);
	RIEPRIS->SetSelIndex(k);
      }
  }

void TCompuDia::HandRiep2(RTMessage Msg)
  {
    if(Msg.LP.Hi==LBN_SELCHANGE)
      {
        k=RIEP2->GetSelIndex();
        RIEP1->SetSelIndex(k);
        RIEPILO->SetSelIndex(k);
	RIEP3->SetSelIndex(k);
	RIEPRIS->SetSelIndex(k);
      }
  }

void TCompuDia::HandRiep3(RTMessage Msg)
  {
    int posy;

    posy=RIEP3->VerCur();
    RIEP1->SetVCur(posy);
    RIEP2->SetVCur(posy);
    RIEPILO->SetVCur(posy);
    RIEPRIS->SetVCur(posy);
    SendMessage(RIEP1->HWindow,
    WM_VSCROLL,SB_THUMBPOSITION,posy);
    SendMessage(RIEP2->HWindow,
    WM_VSCROLL,SB_THUMBPOSITION,posy);
    SendMessage(RIEPILO->HWindow,
    WM_VSCROLL,SB_THUMBPOSITION,posy);
    SendMessage(RIEPRIS->HWindow,
    WM_VSCROLL,SB_THUMBPOSITION,posy);
    if(Msg.LP.Hi==LBN_SELCHANGE)
      {
        k=RIEP3->GetSelIndex();
        RIEP1->SetSelIndex(k);
        RIEP2->SetSelIndex(k);
	RIEPILO->SetSelIndex(k);
	RIEPRIS->SetSelIndex(k);
      }
  }

void TCompuDia::HandRiepRis(RTMessage Msg)
  {
    if(Msg.LP.Hi==LBN_SELCHANGE)
      {
        k=RIEPRIS->GetSelIndex();
        RIEP1->SetSelIndex(k);
        RIEP2->SetSelIndex(k);
	RIEP3->SetSelIndex(k);
	RIEPILO->SetSelIndex(k);
      }
  }

void TCompuDia::HandPulisci()
  {
    scrivilog(1,"TCompuDia  HandPulisci");
    RIEPILO->ClearList();
    RIEP1->ClearList();
    RIEP2->ClearList();
    RIEP3->ClearList();
    RIEPRIS->ClearList();
    WAIT->SetText("");
  }


void TCompuDia::HandOrdRiep1()
  {
    scrivilog(1,"TCompuDia  HandOrdRiep1");
    ordinanum(RIEP1,RIEP2,RIEP3,RIEPILO,RIEPRIS,crerie1);
    sprintf(string,"HandOrdRiep1 crerie1: %d",crerie1);
    scrivilog(1,string);
    if(!crerie1)
      {
	WAIT->SetText("Ordinato decrescente per guadagno netto");
	crerie1=1;
      }
    else
      {
	WAIT->SetText("Ordinato crescente per guadagno netto");
	crerie1=0;
      }
  }

void TCompuDia::HandOrdRiep2()
  {
    scrivilog(1,"TCompuDia  HandOrdRiep2");
    ordinanum(RIEP2,RIEP1,RIEP3,RIEPILO,RIEPRIS,crerie2);
    sprintf(string,"HandOrdRiep2 crerie2: %d",crerie2);
    scrivilog(1,string);
    if(!crerie2)
      {
	WAIT->SetText("Ordinato decrescente per entrate");
	crerie2=1;
      }
    else
      {
	WAIT->SetText("Ordinato crescente per entrate");
	crerie2=0;
      }
  }

void TCompuDia::HandOrdRiep3()
  {
    scrivilog(1,"TCompuDia  HandOrdRiep3");
    ordinanum(RIEP3,RIEP1,RIEP2,RIEPILO,RIEPRIS,crerie3);
    sprintf(string,"HandOrdRiep3 crerie3: %d",crerie3);
    scrivilog(1,string);
    if(!crerie3)
      {
	WAIT->SetText("Ordinato decrescente per uscite");
	crerie3=1;
      }
    else
      {
	WAIT->SetText("Ordinato crescente per uscite");
	crerie3=0;
      }
  }

void TCompuDia::HandOrdRiep()
  {
    scrivilog(1,"TCompuDia  HandOrdRiep");
    ordinalet(RIEPILO,RIEPRIS,RIEP1,RIEP2,RIEP3,crerie);
    sprintf(string,"HandOrdRiep crerie: %d",crerie);
    scrivilog(1,string);
    if(!crerie)
      {
	WAIT->SetText("Ordinato decrescente per categorie");
	crerie=1;
      }
    else
      {
	WAIT->SetText("Ordinato crescente per categorie");
	crerie=0;
      }
  }

void TCompuDia::HandOrdRis()
  {
    scrivilog(1,"TCompuDia  HandOrdRis");
    ordinalet(RIEPRIS,RIEPILO,RIEP1,RIEP2,RIEP3,creris);
    sprintf(string,"HandOrdRis crerie2: %d",creris);
    scrivilog(1,string);
    if(!creris)
      {
	WAIT->SetText("Ordinato decrescente per risorse");
	creris=1;
      }
    else
      {
	WAIT->SetText("Ordinato crescente per risorse");
	creris=0;
      }
  }

void TCompuDia::DefCommandProc(RTMessage Msg)
//Qui scelgo una serie di tasti che mi permettono
//di evitare di lavorare col mouse.
//NOTA BENISSIMO: ho scoperto che se provo a usare
//CTRL+H, dopo non mi capisce piu' il backspace
//nei controlli di edit
  {
    if ( Msg.WP.Hi == 0 )
      {
	switch(Msg.WP.Lo)
	  {
	    case ACC_CTA:
//CTRL+A Mi posiziona sulla data A
	      scrivilog(1,"TCompuDia CTRL-A");
	      SetFocus(AGior->HWindow);
	      break;
	    case ACC_CTD:
//CTRL+D Mi posiziona sulla data DA
	      scrivilog(1,"TCompuDia CTRL-D");
	      SetFocus(DaGior->HWindow);
	      break;
	    case ACC_CTE:
//CTRL+E Mi equivale a premere PULISCI
	      scrivilog(1,"TCompuDia CTRL-E");
	      SendDlgItemMessage(HWindow,
	      ANNUL, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      ANNUL, BM_SETSTATE, FALSE, 0);
	      HandPulisci();
	      break;
	    case ACC_CTG:
//Se digito CTRL+G, mi posiziona su data di riepilogo
	      scrivilog(1,"TCompuDia CTRL-G");
	      SetFocus(RiGior->HWindow);
	      break;
	    case ACC_CTQ:
//CTRL+Q Mi posiziona sulla selezione categorie
	      scrivilog(1,"TCompuDia CTRL-Q");
	      SetFocus(CATE->HWindow);
	      break;
	    case ACC_CTR:
//Se digito CTRL+R, mi mette il fuoco sul check selezionato
	      scrivilog(1,"TCompuDia CTRL-R");
	      switch(stato)
		{
		  case 1:
	            SetFocus(ELIGN->HWindow);
		    break;
		  case 2:
		    SetFocus(ELIEN->HWindow);
		    break;
		  case 3:
		    SetFocus(ELIUN->HWindow);
		    break;
		  case 4:
		    SetFocus(ELITN->HWindow);
		    break;
		  default:
                    ;
                }
	      break;
	    case ACC_CTS:
//CTRL+S Mi posiziona sul riepilogo box risorse
	      scrivilog(1,"TCompuDia CTRL-S");
	      SetFocus(RIEPRIS->HWindow);
	      break;
	    case ACC_CTW:
//CTRL+W Mi equivale a premere CALCOLA
	      scrivilog(1,"TCompuDia CTRL-W");
	      SendDlgItemMessage(HWindow,
	      C_CALCO, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_CALCO, BM_SETSTATE, FALSE, 0);
	      lanciacalc();
	      break;
	    case ACC_ESCAPE:
//Se spingo ESC, mi posiziona su FATTO
	      scrivilog(1,"TCompuDia CTRL-ESC");
	      SetFocus(BFATTO->HWindow);
	      break;
	    case ACC_F1:
//Se spingo F1, mi equivale a ordinare secondo le risorse
	      scrivilog(1,"TCompuDia F1");
	      SendDlgItemMessage(HWindow,
	      C_ORDRIS, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_ORDRIS, BM_SETSTATE, FALSE, 0);
	      HandOrdRis();
	      break;
	    case ACC_F2:
//Se spingo F2, mi equivale a ordinare secondo le categorie
	      scrivilog(1,"TCompuDia F2");
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP, BM_SETSTATE, FALSE, 0);
	      HandOrdRiep();
	      break;
	    case ACC_F3:
//Se spingo F3, mi equivale a ordinare secondo il guadagno
	      scrivilog(1,"TCompuDia F3");
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP1, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP1, BM_SETSTATE, FALSE, 0);
	      HandOrdRiep1();
	      break;
	    case ACC_F4:
//Se spingo F4, mi equivale a ordinare secondo le entrate
	      scrivilog(1,"TCompuDia F4");
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP2, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP2, BM_SETSTATE, FALSE, 0);
	      HandOrdRiep2();
	      break;
	    case ACC_F5:
//Se spingo F5, mi equivale a ordinare secondo le uscite
	      scrivilog(1,"TCompuDia F5");
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP3, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      C_ORDRIEP3, BM_SETSTATE, FALSE, 0);
	      HandOrdRiep3();
	      break;
	    case ACC_CTDELETE:
//Se digito CTRL+canc, mi equivale a premere CANCELLA
//C'e' anche l'effetto speciale del bottone premuto
	      scrivilog(1,"TCompuDia CTRL-DEL");
	      SendDlgItemMessage(HWindow,
	      OG_CANC, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
	      SendDlgItemMessage(HWindow,
	      OG_CANC, BM_SETSTATE, FALSE, 0);
	      HandCancella();
	      break;
	    case ACC_CTUP:
//Se digito CTRL+freccia su, mi visualizza il giorno prima
//Metto il focus su SELDAT per toglierlo
//ai campi date e impedire date errate
//(inoltre e' anche una cosa desiderabile)
//C'e' anche l'effetto speciale del bottone premuto
	      scrivilog(1,"TCompuDia CTRL-UP");
	      SendDlgItemMessage(HWindow,
	      DIETRO, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
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
	      scrivilog(1,"CTRL-DWN");
	      SendDlgItemMessage(HWindow,
	      AVANTI, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
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
	      scrivilog(1,"TCompuDia CTRL-PGDWN");
	      SendDlgItemMessage(HWindow,
	      OG_VAI, BM_SETSTATE, TRUE, 0);
//Loop di ritardo
	      for(i=0;i<10000;i++)
		j=2*i;
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


int TCompuDia::riepiloga()
  {
    int a,m,g;

    RiAnn->GetText(string,5);
    sscanf(string,"%d",&a);
    RiMes->GetText(string,3);
    sscanf(string,"%d",&m);
    RiGior->GetText(string,3);
    sscanf(string,"%d",&g);
    numg=(long int)a*10000+m*100+g;
    sprintf(nomefile,"%d.bud",a);
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
    sprintf(string,"%s",gset[tcheck.tm_wday]);
    SetDlgItemText(HWindow,GSETTIMANA,string);

    if((erno=caricasaldi(numg,t_risorse3))<0)
      {        
//C'e' stato un problema nella lettura di basedata.dat
        sprintf(string,
	"TCompuDia, riepiloga, caricasaldi,\
controllabasedata: erno n. %d",erno);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return -1;
      }
    if((erno=aggiornasaldi(numg,numfonti,
       nomefile,t_risorse3))<0)
      {
//C'e' stato un problema nella lettura del file di dati
//o nella riscrittura di basedata.dat
	sprintf(string,"TCompuDia, riepiloga, \
aggiornasaldi : erno n. %d",erno);
	MessageBox(HWindow,string,
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,string);
	return -1;
      }
    mostrasaldi(SELBANC1,SELDAT,RESULT,t_risorse3);
    return 0;
  }

void TCompuDia::HandVai()
  {
//Mostra i movimenti alla data indicata nei campi
//di riepilogo 
    scrivilog(1,"TCompuDia: Forse Mouse Vai");
    riepiloga();
  }

void TCompuDia::HandGDopo()
  {
//Mostra i movimenti del giorno successivo
    int g,m,a;

    scrivilog(1,"TCoumpuDia: Forse Mouse Gdopo");
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
    RiGior->Clear();
    RiMes->Clear();
    RiAnn->Clear();
    sprintf(string,"%02d",g);
    RiGior->Insert(string);
    sprintf(string,"%02d",m);
    RiMes->Insert(string);
    sprintf(string,"%04d",a);
    RiAnn->Insert(string);
    riepiloga();
  }

void TCompuDia::HandGPrima()
  {
//Mostra i movimenti del giorno precedente 
    int g,m,a;

    scrivilog(1,"TCompuDia: Forse Mouse Gprima");
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
    RiGior->Clear();
    RiMes->Clear();
    RiAnn->Clear();
    sprintf(string,"%02d",g);
    RiGior->Insert(string);
    sprintf(string,"%02d",m);
    RiMes->Insert(string);
    sprintf(string,"%04d",a);
    RiAnn->Insert(string);
    riepiloga();
  }


void TCompuDia::HandSelSaldoRisorsa(RTMessage Msg)
//Mi fa vedere il dato della risorsa che seleziono
  {
    if(Msg.LP.Hi==CBN_SELENDOK)
      {
	mostrasaldi(SELBANC1,SELDAT,RESULT,t_risorse3);
      }
  }

void TCompuDia::HandSelTipoDato(RTMessage Msg)
//Mi fa vedere il tipo di dato che seleziono
  {
    if(Msg.LP.Hi==CBN_SELENDOK)
      {
	mostrasaldi(SELBANC1,SELDAT,RESULT,t_risorse3);
      }
  }

void TCompuDia::HandCancella()
//Cancella la voce selezionata nel box di riepilogo
  {
    scrivilog(1,"TCompuDia  HandCancella");
    index=RIEPILO->GetSelIndex();
    if(index<0)
      {
//Non era selezionato nulla: ritorno
	scrivilog(1,"TCompuDia HandCancella: nulla ritorno");
	return;
      }
    RIEPILO->DeleteString(index);
    RIEP1->DeleteString(index);
    RIEP2->DeleteString(index);
    RIEP3->DeleteString(index);
    RIEPRIS->DeleteString(index);
  }

void TCompuDia::HandCheckEliGN(RTMessage Msg)
  {
//Seleziono elimina guadagno nullo e deseleziono gli altri
    if(Msg.LP.Hi==BN_CLICKED)
      {
        scrivilog(1,"TCompuDia Mouse HEliGN");
	EliGN();
      }
  }

void TCompuDia::EliGN()
  {
//Se era gia' selezionato non faccio nulla
    if(stato==1)
      return;
    ELIGN->Check();
    ELIEN->Uncheck();
    ELIUN->Uncheck();
    ELITN->Uncheck();
    stato=1;
  }

void TCompuDia::HandCheckEliEN(RTMessage Msg)
  {
//Seleziono elimina entrate nulle e deseleziono gli altri
    if(Msg.LP.Hi==BN_CLICKED)
      {
	scrivilog(1,"TCompuDia Mouse HEliEN");
	EliEN();
      }
  }

void TCompuDia::EliEN()
  {
//Se era gia' selezionato non faccio nulla
    if(stato==2)
      return;
    ELIGN->Uncheck();
    ELIEN->Check();
    ELIUN->Uncheck();
    ELITN->Uncheck();
    stato=2;
  }

void TCompuDia::HandCheckEliUN(RTMessage Msg)
  {
//Seleziono elimina uscite nulle e deseleziono gli altri
    if(Msg.LP.Hi==BN_CLICKED)
      {
	scrivilog(1,"TCompuDia Mouse HEliUN");
	EliUN();
      }
  }

void TCompuDia::EliUN()
  {
//Se era gia' selezionato non faccio nulla
    if(stato==3)
      return;
    ELIGN->Uncheck();
    ELIEN->Uncheck();
    ELIUN->Check();
    ELITN->Uncheck();
    stato=3;
  }

void TCompuDia::HandCheckEliTN(RTMessage Msg)
  {
//Seleziono elimina tutto nullo e deseleziono gli altri
    if(Msg.LP.Hi==BN_CLICKED)
      {
	scrivilog(1,"TCompuDia Mouse HEliTN");
	EliTN();
      }
  }

void TCompuDia::EliTN()
  {
//Se era gia' selezionato non faccio nulla
    if(stato==4)
      return;
    ELIGN->Uncheck();
    ELIEN->Uncheck();
    ELIUN->Uncheck();
    ELITN->Check();
    stato=4;
  }

void TCompuDia::HandEsegui()
  {
    double gain,in,out;
    char cancella;

    scrivilog(1,"TCompuDia  HandEsegui");
    k=RIEPILO->GetCount();
    for(i=0;i<k;i++)
      {

	RIEP1->GetString(string,i);
	sscanf(string,"%lf",&gain);
	RIEP2->GetString(string,i);
	sscanf(string,"%lf",&in);
	RIEP3->GetString(string,i);
	sscanf(string,"%lf",&out);
	cancella=0;
	switch(stato)
	  {
	    case 1:
	      if(!gain)
                cancella=1;
	      break;
	    case 2:
	      if(!in)
		cancella=1;
	      break;
	    case 3:
	      if(!out)
		cancella=1;
	      break;
	    case 4:
	      if((!gain)&&(!in)&&(!out))
		cancella=1;
	      break;
	    default:
	      ;
	  }
	if(cancella)
          {
            RIEPILO->DeleteString(i);
            RIEP1->DeleteString(i);
	    RIEP2->DeleteString(i);
            RIEP3->DeleteString(i);
	    RIEPRIS->DeleteString(i);
	    i--;
	    k--;
          }
      }
  }

