
// ************************* CLASSE TGiovDia **********************************************

_CLASSDEF(TGiovDia)



extern int controllabasedata();
extern int controllafiledati(char *);
extern int compareFiles(char *,char *);
extern int backuplog();


class TGiovDia:public TDialog
  {
    public:

      char string[400],string1[400],
      vis,r,fatto,errore;
      int g,m,a,i,j,k,numfonti,vero;
      struct time Time;
      struct date Date;
      struct ffblk ffblk,ffblk1;


      PTEdit MonYe,Year,field;
      PTComboGiov GGior,MMes,AAnn;
      FILE *map,*fil,*tmp;

      TGiovDia(PTWindowsObject AParent,LPSTR ResourceId);
      virtual ~TGiovDia();
      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      virtual void WMTimer()=[WM_FIRST + WM_TIMER];
      virtual void SetupWindow();
      virtual void CloseWindow();
      virtual void WMClose(RTMessage Msg)
        =[WM_FIRST+WM_CLOSE];
      char* HandInizio();
      void HandOggi()=[ID_FIRST+ID_OGGI];
      void HandMese()=[ID_FIRST+ID_MESE];
      void HandAnno()=[ID_FIRST+ID_ANNO];
      void HandCalcola()=[ID_FIRST+ID_CERCA];
      void HandPrelimin()=[ID_FIRST+ID_DATI_F];
      void HandValGior(RTMessage Msg)=[ID_FIRST+ID_GG];
      void HandValMes(RTMessage Msg)=[ID_FIRST+ID_MM];
      void HandValAnn(RTMessage Msg)=[ID_FIRST+ID_AAAA];
//      void DefCommandProc(RTMessage Msg);
  };


// ************************* FUNZIONI DI TGiovDia ***********************************

TGiovDia::TGiovDia(PTWindowsObject AParent,LPSTR ResourceId)
  :TDialog(AParent,ResourceId)
  {
    GGior=new TComboGiov(this,ID_GG,3,NULL);
    MMes=new TComboGiov(this,ID_MM,3,NULL);
    AAnn=new TComboGiov(this,ID_AAAA,5,NULL);
    MonYe=new TEdit(this,ID_M,11,NULL);
    Year=new TEdit(this,ID_Y,11,NULL);
    vis=0;
  }

TGiovDia::~TGiovDia()
  {
    KillTimer(HWindow,1);
  }




LPSTR TGiovDia::GetClassName()
  {
    return APPNAME;
  }

void TGiovDia::GetWindowClass(WNDCLASS& AWndClass)
  {
    TDialog::GetWindowClass(AWndClass);
    AWndClass.hIcon=LoadIcon(GetApplication()->hInstance,"icona");
  }

void TGiovDia::WMTimer()
  {
    gettime(&Time);
    getdate(&Date);
    sprintf(string,"%02d : %02d : %02d",Time.ti_hour,Time.ti_min,Time.ti_sec);
    SetDlgItemText(HWindow,CLOCK,string);
    sprintf(string,"%02d / %02d / %04d",Date.da_day,Date.da_mon,Date.da_year);
    SetDlgItemText(HWindow,DATA,string);
  }

void TGiovDia::HandOggi()
  {
    if(vis==1)
      return;
    if(!(map=fopen("basedata.dat","r")))
      {
	MessageBox(HWindow,"Mancano i dati preliminari",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	HandPrelimin();
        return;
      }
    vero=0;
    numfonti=0;
    fgets(string,400,map);
    sscanf(string,"%d\n",&numfonti);
    fclose(map);
    if(!numfonti)
      {
	MessageBox(HWindow,"Mancano i dati preliminari",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	HandPrelimin();
	return;
      }
    GGior->GetText(string,3);
    g=atoi(string);
    MMes->GetText(string,3);
    m=atoi(string);
    AAnn->GetText(string,5);
    a=atoi(string);
    if(a>0)
      {
//controlla se e' bisestile
        if(isbises(a))                             
//se lo e': giorni di febbraio=29
	  month[1]=29;                             
	else
//se non lo e': giorni di febbraio=28
	  month[1]=28;                             
	if((m>=1)&&(m<=12)&&(g>=1)&&(g<=month[m-1]))
	  vero=1;
      }
    if(vero)
      {                          
	sprintf(string,"%02d-%02d-%04d",g,m,a);
//Per poter usare gli acceleratori nella finestra
//del giorno non posso eseguire il dialogo in forma
//modale, percio' uso MakeWindow anziche' ExecDialog
//Di conseguenza non posso nascondere la finestra
//di partenza, ed e' il motivo per cui uso il trucco
//di cambiare vis in modo che se ho gia' aperto
//la finestra giorno, i bottoni di questa finestra
//non faranno nulla
	GetModule()->MakeWindow(new TOggiDia(this,
	"day",string,&vis));
      }
    else
      {
        GGior->Clear();
	MMes->Clear();
	AAnn->Clear();
	getdate(&Date);
	sprintf(string,"%02d",Date.da_day);
	GGior->Insert(string);
	sprintf(string,"%02d",Date.da_mon);
	MMes->Insert(string);
	sprintf(string,"%04d",Date.da_year);
	AAnn->Insert(string);
      }
  }


void TGiovDia::HandPrelimin()
  {
    if(vis==1)
      return;
    Show(SW_HIDE);
    KillTimer(HWindow,1);
    GetModule()->ExecDialog(new TDiaPreliminare(this,"diafield",Date.da_year));
//Il seguente e' un trucchetto che devo fare
//(non so perche') per rendere i tasti funzionanti
//di nuovo (tab ecc.): dopo aver reso visibile
//la finestra, la devo rinascondere e rendere visibile
	TWindowsObject::Show(SW_SHOW);
	TWindowsObject::Show(SW_HIDE);
	TWindowsObject::Show(SW_SHOW);
      if (!SetTimer(HWindow, 1, 1000U, NULL))
        MessageBox(HWindow, "Too Many Timers", "Cuckoo Clock", MB_ICONEXCLAMATION | MB_OK);
  }

void TGiovDia::HandValGior(RTMessage Msg)
  {
    if(vis==1)
      return;

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
        caricacampi(GGior,MMes,AAnn);
	GGior->SetEditSel(0,2);
	GGior->GetText(string,3);
	GGior->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      caricacampi(GGior,MMes,AAnn);
  }

void TGiovDia::HandValMes(RTMessage Msg)
  {
    if(vis==1)
      return;

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
        caricacampi(GGior,MMes,AAnn);
        MMes->SetEditSel(0,2);
	MMes->GetText(string,3);
	MMes->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      caricacampi(GGior,MMes,AAnn);
  }

void TGiovDia::HandValAnn(RTMessage Msg)
  {
    if(vis==1)
      return;

    if(Msg.LP.Hi==CBN_SETFOCUS)
      {
        caricacampi(GGior,MMes,AAnn);
	AAnn->SetEditSel(0,4);
	AAnn->GetText(string,5);
	AAnn->SetText(string);
      }
    if(Msg.LP.Hi==CBN_KILLFOCUS)
      caricacampi(GGior,MMes,AAnn);
  }

void TGiovDia::HandMese()
  {
    if(vis==1)
      return;
  }

void TGiovDia::HandAnno()
  {
    if(vis==1)
      return;
  }

void TGiovDia::HandCalcola()
  {
    if(vis==1)
      return;
//Per poter usare gli acceleratori nella finestra
//del giorno non posso eseguire il dialogo in forma
//modale, percio' uso MakeWindow anziche' ExecDialog
//Di conseguenza non posso nascondere la finestra
//di partenza, ed e' il motivo per cui uso il trucco
//di cambiare vis in modo che se ho gia' aperto
//la finestra giorno, i bottoni di questa finestra
//non faranno nulla
    scrivilog(1,"TGiovDia  HandCalcola prima");
    GetModule()->MakeWindow(new TCompuDia(this,"COMPUTE",&vis));
    scrivilog(1,"TGiovDia  HandCalcola dopo");
  }

void TGiovDia::SetupWindow()
  {

//Se non esiste la dir budback, la creo
    if(findfirst("budback",&ffblk,FA_DIREC)<0)
      if(mkdir("budback")<0)
        {
//In caso di errore...
	  MessageBox(HWindow,
	  "Non riesco a creare la dir per il backup",
	  "Backup non verra' eseguito",
	  MB_ICONEXCLAMATION | MB_OK);
	  CloseWindow();
	}
    TDialog::SetupWindow();
    if (!SetTimer(HWindow, 1, 1000U, NULL))
      MessageBox(HWindow, "Too Many Timers", "Cuckoo Clock", MB_ICONEXCLAMATION | MB_OK);
    gettime(&Time);
    getdate(&Date);
    MonYe->Clear();
    sprintf(string,"%02d-%04d",Date.da_mon,Date.da_year);
    MonYe->Insert(string);
    Year->Clear();
    sprintf(string,"%04d",Date.da_year);
    Year->Insert(string);
    caricacampi(GGior,MMes,AAnn);
    sprintf(string,"%02d : %02d : %02d",Time.ti_hour,Time.ti_min,Time.ti_sec);
    SetDlgItemText(HWindow,CLOCK,string);
    sprintf(string,"%02d / %02d / %04d",Date.da_day,Date.da_mon,Date.da_year);
    SetDlgItemText(HWindow,DATA,string);
  }

/*void TGiovDia::DefCommandProc(RTMessage Msg)
  {
    if ( Msg.WP.Hi == 0 )
      {
      }
    TDialog::DefCommandProc(Msg);
  }*/



void TGiovDia::CloseWindow()
//Prima di chiudere normalmente
//controlla l'integrita' dei dati ed esegue
//una copia di backup
//Mantengo fino a 5 copie di backup per file
//La piu' recente e' quella con l'estensione .bk5
//Ad ogni backup , la .bk1 va persa e sostituita
//da .bk2, e cosi' via.
//Pero':
//Per tutti i files da backuppare voglio
//vedere se sono diversi dall'ultima versione
//salvata. Se sono identici non faccio il backup
//evitando di perdere inutilmente le versioni
//precedenti solo perche' ho aperto e richiuso
//il programma senza cambiare niente.
//Questo mi assicura anche che tutti i backup sono
//diversi l'uno dall'altro
  {
    int a,go;

    errore=0;
    if(findfirst("budback",&ffblk,FA_DIREC)<0)
      {
//Se non esiste la dir budback, la creo
	if(mkdir("budback")<0)
	  {
//In caso di errore...
            MessageBox(HWindow,
	    "Non riesco a creare la dir per il backup",
	    "Backup non verra' eseguito",
	    MB_ICONEXCLAMATION | MB_OK);
	    errore=1;
	  }
      }
    if((!findfirst("basedata.dat",&ffblk,FA_ARCH))&&
        (!errore))
//Se esiste basedata.dat
      {
	if((r=controllabasedata())<0)
	  {
//In caso di errore...
	    sprintf(string,"Anomalia in basedata.dat\
per il seguente errore: %d",r);
	    MessageBox(HWindow,
	    string,"Backup non verra' eseguito",
	    MB_ICONEXCLAMATION | MB_OK);
	    errore=1;
	  }
//Tutto OK
//Adesso cerco di capire se c'e'
//differenza con l'ultimo backup
	chdir("budback");
	for(i=5;i>0;i--)
//Questo ciclo cerca il backup piu' recente
//appena lo trova esco dal ciclo, senno' il
//ciclo si conclude
	  {
            sprintf(string,"basedata.bk%d",i);
	    if(!findfirst(string,
	      &ffblk1,FA_ARCH))
	      {
//Se trovo il file esco dal ciclo e metto i negativa
//per far capire che il ciclo non si e' concluso
//naturalmente
		i=-1;
		break;
              }
	  }
	if(i<0)
//i<0 vuol dire che ho trovato un file di backup
//il cui nome e' in string
          {
	    go=compareFiles("tempor",string);
	  }
	else
//altrimenti i==0, non c'erano backup:
//fanne uno di certo
	  go=1;
//go<0 e' un errore, ==0 significa files uguali
//(non fare il backup), >0 significa fai il backup
	if(go<0)
	  {
//Si e' verificato un errore (improbabile arrivare qui)
	    sprintf(string,
	    "Problema nel confronto files n: %d",go);
	    MessageBox(HWindow,
	    string,
	    "Backup di questo file non verra' eseguito",
	    MB_ICONEXCLAMATION | MB_OK);
            remove("tempor");
	  }
	else if(!go)
//Non c'e' da fare backup. Rimuovo tempor
	  remove("tempor");
	else if(go>0)
	  {
//Ho il via libera al backup
//Se c'e' budback\basedata.bk2, lo faccio diventare
//budback\basedata.bk1
	    if(!findfirst("basedata.bk2",
              &ffblk1,FA_ARCH))
	      {
//Controllo se c'e' la versione piu' vecchia:
//Se c'e' devo cancellarla perche' senno' il
//rename successivo non mi funziona. Tutte le versioni
//successive andranno poi bene automaticamente
	        if(!findfirst("basedata.bk1",
	          &ffblk1,FA_ARCH))
	          remove("basedata.bk1");
	        rename("basedata.bk2",
	           "basedata.bk1");
              }
//Se c'e'  budback\basedata.bk3, lo faccio diventare
//budback\basedata.bk2
            if(!findfirst("basedata.bk3",
               &ffblk1,FA_ARCH))
	      {
	        rename("basedata.bk3",
	               "basedata.bk2");
              }
//Se c'e' budback\basedata.bk4, lo faccio diventare
//budback\basedata.bk3
            if(!findfirst("basedata.bk4",
               &ffblk1,FA_ARCH))
	      {
	        rename("basedata.bk4",
	               "basedata.bk3");
              }
//Se c'e' budback\basedata.bk5, lo faccio diventare
//budback\basedata.bk4
            if(!findfirst("basedata.bk5",
               &ffblk1,FA_ARCH))
	      {
	        rename("basedata.bk5",
	               "basedata.bk4");
	      }
//Alla fine budback\tempor diventa budback\basedata.bk5
	    rename("tempor","basedata.bk5");
          }
	chdir("..");
      }
    fatto=findfirst("*.bud",&ffblk,FA_ARCH);
//cerco il primo file di dati
    while((!fatto)&&(!errore))
      {
	if(sscanf(ffblk.ff_name,"%d.bud",&a)==1)
	  {
//Solo se e' del tipo "anno.dat"
            if((r=controllafiledati(ffblk.ff_name))<0)
	      {
//In caso di errore...
	        sprintf(string,"Anomalia in %d.bud\
per il seguente errore: %d",a,r);
                MessageBox(HWindow,
	        string,"Backup non verra' eseguito",
	        MB_ICONEXCLAMATION | MB_OK);
		errore=1;
                break;
	      }
//Tutto OK.
//Adesso cerco di capire se c'e' differenza con
//l'ultimo backup
	    chdir("budback");
	    for(i=5;i>0;i--)
//Questo ciclo cerca il backup piu' recente
//appena lo trova esco dal ciclo, senno' il
//ciclo si conclude
	      {
	        sprintf(string,"%d.bk%d",a,i);
	        if(!findfirst(string,
	          &ffblk1,FA_ARCH))
	          {
//Se trovo il file esco dal ciclo e metto i negativa
//per far capire che il ciclo non si e' concluso
//naturalmente
		    i=-1;
		    break;
                  }
	      }
	    if(i<0)
//i<0 vuol dire che ho trovato un file di backup
//il cui nome e' in string
	      go=compareFiles("tempor",string);
	    else
//altrimenti i==0, non c'erano backup:
//fanne uno di certo
	      go=1;
//go<0 e' un errore, ==0 significa files uguali
//(non fare il backup), >0 significa fai il backup
	    if(go<0)
	      {
//Si e' verificato un errore (improbabile arrivare qui)
	        sprintf(string,
	        "Problema nel confronto files n: %d",go);
	        MessageBox(HWindow,
	        string,
	        "Backup di questo file non verra' eseguito",
	        MB_ICONEXCLAMATION | MB_OK);
	        remove("tempor");
	      }
	    else if(!go)
//Non c'e' da fare backup. Rimuovo tempor
	      remove("tempor");
	    else if(go>0)
              {
//Ho il via libera al backup
//Se c'e' budback\anno.bk2, lo faccio diventare
//budback\anno.bk1
	        sprintf(string,"%d.bk2",a);
	        sprintf(string1,"%d.bk1",a);
	        if(!findfirst(string,&ffblk1,FA_ARCH))
	          {
//Controllo se c'e' la versione piu' vecchia:
//Se c'e' devo cancellarla perche' senno' il
//rename successivo non mi funziona. Tutte le versioni
//successive andranno poi bene automaticamente
	            if(!findfirst(string1,
	              &ffblk1,FA_ARCH))
	              remove(string1);
		    rename(string,string1);
                  }
//Se c'e' budback\anno.bk3, lo faccio diventare
//budback\anno.bk2
	        sprintf(string,"%d.bk3",a);
	        sprintf(string1,"%d.bk2",a);
	        if(!findfirst(string,&ffblk1,FA_ARCH))
	          {
		    rename(string,string1);
                  }
//Se c'e' budback\anno.bk4, lo faccio diventare
//budback\anno.bk3
	        sprintf(string,"%d.bk4",a);
	        sprintf(string1,"%d.bk3",a);
	        if(!findfirst(string,&ffblk1,FA_ARCH))
	          {
		    rename(string,string1);
                  }
//Se c'e' budback\anno.bk5, lo faccio diventare
//budback\anno.bk4
	        sprintf(string,"%d.bk5",a);
	        sprintf(string1,"%d.bk4",a);
	        if(!findfirst(string,&ffblk1,FA_ARCH))
	          {
		    rename(string,string1);
	          }
//Alla fine budback\tempor diventa budback\anno.bk5
		rename("tempor",string);
              }
            chdir("..");
	  }
//Cerco il prossimo "*.bud"
        fatto=findnext(&ffblk);
      }
//Faccio il backup del log
    backuplog();
    TDialog::CloseWindow();
  }

void TGiovDia::WMClose(RTMessage Msg)
  {
    CloseWindow();
  }
