
// ************************* CLASSE TNoteDia *******************************************

_CLASSDEF(TNoteDia)

extern int scrivilog(char,char *);

class TNoteDia:public TDialog
  {
    public:

      int i,*nn,g,m,a;
      long int numg,pippo;
      long int importo,numg_out,numg_in,saldo,
	       numg_fa,numg_fb,importo_f,numg_riepilogo;
      char all[401],str1[25];
      struct tm tcheck;
      FILE *nf;
      PTEdit PAD;

      TNoteDia(PTWindowsObject,LPSTR,int *,long int);
      ~TNoteDia();
      LPSTR GetClassName();
      void GetWindowClass(WNDCLASS&);
      virtual void CloseWindow();
      virtual void WMClose(RTMessage Msg);
      virtual void SetupWindow();
      void HandOK()=[ID_FIRST+NOTE_OK];
      void HandAnnulla()=[ID_FIRST+NOTE_ANN];
      int  scrivinote(char *nome_file,long int num_g);
  };



// ************************** FUNZIONI DI TNoteDia **************************************

TNoteDia::TNoteDia(PTWindowsObject AParent,
		   LPSTR ResourceId,int *n,long int ng)
  :TDialog(AParent,ResourceId)
  {
    PAD=new TEdit(this,NOTES,0,NULL);
    nn=n;
    numg=ng;
  }
                                                                             
TNoteDia::~TNoteDia()
  {
  }

LPSTR TNoteDia::GetClassName()
  {
    return "note";
  }

void TNoteDia::GetWindowClass(WNDCLASS& AWndClass)
  {
    TDialog::GetWindowClass(AWndClass);
    AWndClass.hIcon=LoadIcon(GetApplication()->hInstance,"icona");
  }

void TNoteDia::SetupWindow()
  {
    TDialog::SetupWindow();
    PAD->Clear();
    g= numg%100;
    pippo= numg/100;
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
    if(!(nf=fopen("note.tmp","r")))
      {
//Non trovo note.tmp: errore
        MessageBox(HWindow,
	"Manca il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	scrivilog(1,"Setup note: Manca il file 'note.tmp'");
	CloseWindow();
      }
    sprintf(all,"NOTE PER IL GIORNO: %02d-%02d-%04d, %s",
	    g,m,a,gset[tcheck.tm_wday]);      
    SetDlgItemText(HWindow,NOTE_DATA,all);
    scrivilog(1,all);
    if(*nn)
      {
	for(i=0;i<*nn;i++)
	  {
	    if(!fgets(all,400,nf))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
		"Anomalia nel file 'note.tmp'",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
		fclose(nf);
		scrivilog(1,"Setup note: Anomalia nel file 'note.tmp'");
		CloseWindow();
	      }
	    sscanf(all,"%[!-~ ¡-ÿ‘’]\n",all);
	    scrivilog(0,all);
	    strcat(all,"\r\n");
	    PAD->Insert(all);
	  }
//	SendMessage(HWindow,WM_LBUTTONDOWN,0,0);
      }
    fclose(nf);
//    sprintf(all,"NOTE PER IL GIORNO: %02d-%02d-%04d, %s",
//	    g,m,a,gset[tcheck.tm_wday]);      
//    SetDlgItemText(HWindow,NOTE_DATA,all);
    scrivilog(1,"Setup note eseguito");
  }

void TNoteDia::HandOK()
//Scrivo il testo corrente in note.tmp ed esco
//Alla fine avro' aggiornato note.tmp e nlnote
  {
    int k;

    scrivilog(1,"Note: OK spinto. Nuove note:");
    *nn=PAD->GetNumLines();
    k=*nn;
    if(k)
      if(!(PAD->GetLine(all,190,k-1)))
	(*nn)--;
    sprintf(all,"%d",*nn);
    nf=fopen("note.tmp","w");
    for(i=0;i<k;i++)
      {
	if(PAD->GetLine(all,190,i))
          {
	    fprintf(nf,"%s\n",all);
	    scrivilog(0,all);
	  }
	else
          {
	    fprintf(nf,"\n");
	    scrivilog(0,"");
	  }
      }
    fclose(nf);
    CloseWindow();
  }

void TNoteDia::HandAnnulla()
//Non modifico note.tmp ed esco dalle note.
//Prima chiedo conferma (e' una cosa sensata?)
  {
    if
    (
      MessageBox(HWindow,
      "Attenzione: le eventuali modifiche \
fatte andranno perse. Confermi annulla?",
      "Annulla e esci",MB_ICONEXCLAMATION | MB_YESNO)
	        ==IDNO)
       {
//Ci ho ripensato, non vado avanti 
	 scrivilog(1,"Note: Attenzione: le eventuali modifiche \
fatte andranno perse. Confermi annulla? NO");
	 return;
       }
//Si', sono sicuro, perdi le modifiche
    CloseWindow();
  }

void TNoteDia::CloseWindow()
  {
    TDialog::CloseWindow();
  }

void TNoteDia::WMClose(RTMessage Msg)
  {
    if
    (
      MessageBox(HWindow,
      "Attenzione: le eventuali modifiche \
fatte andranno perse. Confermi annulla?",
      "Annulla e esci",MB_ICONEXCLAMATION | MB_YESNO)
	        ==IDNO)
       {
//Ci ho ripensato, non vado avanti 
	 scrivilog(1,"Note: Attenzione: le eventuali modifiche \
fatte andranno perse. Confermi annulla? NO");
	 return;
       }
//Si', sono sicuro, perdi le modifiche
    CloseWindow();
  }
