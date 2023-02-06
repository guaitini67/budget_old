// ***************************************************************************************
// ************************ SEZIONE DEFINIZIONE CLASSI ***********************************
// ***************************************************************************************

// ******************************* TGiovDia **********************************************


class TGiovDia:public TDialog
  {
    public:

      char string[25];
      int g,m,a,i;
      struct time Time;
      struct date Date;
      PTEdit MonYe,Year;
      PTComboGiov GGior,MMes,AAnn;

      TGiovDia(PTWindowsObject AParent,LPSTR ResourceId);
      virtual ~TGiovDia();
      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      virtual void WMTimer()=[WM_FIRST + WM_TIMER];
      virtual void SetupWindow();
      char* HandInizio();
      void HandOggi()=[ID_FIRST+ID_OGGI];
      void HandMese()=[ID_FIRST+ID_MESE];
      void HandAnno()=[ID_FIRST+ID_ANNO];
      void HandCerca()=[ID_FIRST+ID_CERCA];
      void HandValGior(RTMessage Msg)=[ID_FIRST+ID_GG];
      void HandValMes(RTMessage Msg)=[ID_FIRST+ID_MM];
      void HandValAnn(RTMessage Msg)=[ID_FIRST+ID_AAAA];
  };



// ********************************* TOggiDia *******************************************

class TOggiDia:public TDialog
  {
    public:

      char string[25],str1[25],nome[10],filenext[10],ord[6],ord1[6];
      char lire[21],mot[41],cat[41],all[151],stringi[5];
      int i,j,k,tab[2],g,m,a,e,u,n,n1,bank;
      long int rimpre,rimpreb,rimpost,rimpostb,tote,toteb,totu,totub,val,valb;
      struct time Time;
      struct tm tcheck;
      struct date Date;
      FILE *pf,*tf,*nf,*next,*cf,*zf;
      fpos_t pos;
      PTComboGiov CATI,CATO;
      PTEdit IN,OUT,MOTI,MOTO;
      PTListBox RIN,ROUT;
      PTStatic whatday;
      PTGiovWindow SUM;
      PTComboGiov GGior,MMes,AAnn;
      TOggiDia(PTWindowsObject AParent,LPSTR ResourceId,char* giorno);

      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      virtual void WMTimer()=[WM_FIRST + WM_TIMER];
      virtual void SetupWindow();
      virtual ~TOggiDia();
      void aggiornaFile();
      void cancella(PTListBox,long int *,long int *);
      void caricaGiorno();
      void scrivi(PTEdit,PTEdit,PTComboGiov,PTListBox,long int *);
      void seleziona(PTEdit,PTEdit,PTComboGiov,PTListBox);
      void HandScrii()=[ID_FIRST+OG_SCRII];
      void HandCani()=[ID_FIRST+OG_CANI];
      void HandOvi()=[ID_FIRST+OG_OVI];
      void HandBanki()=[ID_FIRST+OG_BANKI];
      void HandAni()=[ID_FIRST+OG_ANI];
      void HandSeli(RTMessage Msg)=[ID_FIRST+C_RIEPI];
      void HandSelo(RTMessage Msg)=[ID_FIRST+C_RIEPO];
      void HandScrio()=[ID_FIRST+OG_SCRIO];
      void HandCano()=[ID_FIRST+OG_CANO];
      void HandOvo()=[ID_FIRST+OG_OVO];
      void HandBanko()=[ID_FIRST+OG_BANKO];
      void HandAno()=[ID_FIRST+OG_ANO];
      void HandVai()=[ID_FIRST+OG_VAI];
      void HandGPrima()=[ID_FIRST+DIETRO];
      void HandGDopo()=[ID_FIRST+AVANTI];
      void HandNote()=[ID_FIRST+OG_NOTE];
      void HandSummary()=[ID_FIRST+SUMMARY];
      void HandOK()=[ID_FIRST+OG_OK];
      void HandValGior(RTMessage Msg)=[ID_FIRST+ID_GG];
      void HandValMes(RTMessage Msg)=[ID_FIRST+ID_MM];
      void HandValAnn(RTMessage Msg)=[ID_FIRST+ID_AAAA];
  };


// ********************************* TNoteDia *******************************************

class TNoteDia:public TDialog
  {
    public:

      int i,*nn,week;
      char all[151],str1[25];
      FILE *nf;
      PTEdit PAD;

      TNoteDia(PTWindowsObject,LPSTR,LPSTR,int *,int);
      virtual void SetupWindow();
      void HandOK()=[ID_FIRST+NOTE_OK];
  };


// ********************************* TGiovWindow ****************************************

class TGiovWindow:public TWindow
  {
    public:

     FILE *pf,*gf,*tf;
     int i,ii,j,k,e,ee,uu,u,n,g,m,g1,m1,r1,r2,a,week;
     long int rimpre,rimpreb,rimpost,rimpostb,totean,totuan,totem,totum,tote,totu,val,totc;
     long int toteb,totub,totebm,totubm,toteban,totuban,ove,ovu,ovean,ovuan,ovem,ovum;
     char lire[21],mot[41],cat[41],cat1[41],all[151],nome[10],ord[6],ord1[6],str1[25];
     fpos_t pos;
     HDC DC;
     HFONT Font,HOldFont;

     TGiovWindow(PTWindowsObject,LPSTR,LPSTR,LPSTR,LPSTR,int);
     virtual void SetupWindow();
     virtual void Paint(HDC DC,PAINTSTRUCT& PS);
     virtual void riempi(HDC DC,int);
     virtual void HandStampa()=[CM_FIRST+SUM_STAMPA];
     virtual void Prima()=[CM_FIRST+SUM_GPRIMA];
     virtual void Dopo()=[CM_FIRST+SUM_GDOPO];
     virtual void stampa();
     virtual void calcola();
  };

// ********************************* TMesWin ********************************************

class TMesWin:public TGiovWindow
  {
    public:
     TMesWin(PTWindowsObject,LPSTR,LPSTR,LPSTR,LPSTR);
     virtual ~TMesWin();
     virtual void SetupWindow();
     virtual void Prima()=[CM_FIRST+SUM_GPRIMA];
     virtual void Dopo()=[CM_FIRST+SUM_GDOPO];
     virtual void calcola();
     virtual void riempi(HDC DC,int);
  };

// ********************************* TMeseDia *******************************************

class TMeseDia:public TDialog
  {
    public:

     int i,ii,e,u,ee,uu,n,m,m1,a,g;
     long int rimpre,rimpreb,totc;
     char lire[21],mot[41],cat[41],all[151],ord[6],mese[25];
     FILE *pf,*gf;
     PTListBox CATIN,CATOUT;
     fpos_t pos;
     PTMesWin SUM;

     TMeseDia(PTWindowsObject,LPSTR,LPSTR);
     virtual void SetupWindow();
     virtual void calcola();
     void HandOK()=[ID_FIRST+ME_OK];
     void HandSummary()=[ID_FIRST+ME_SUM];
     virtual void Summary();
  };

// ********************************* TAnWin ********************************************

class TAnWin:public TGiovWindow
  {
    public:

     TAnWin(PTWindowsObject,LPSTR,LPSTR,LPSTR,LPSTR);
     virtual ~TAnWin();
     virtual void SetupWindow();
     virtual void Prima()=[CM_FIRST+SUM_GPRIMA];
     virtual void Dopo()=[CM_FIRST+SUM_GDOPO];
     virtual void calcola();
     virtual void riempi(HDC DC,int);
  };
// ********************************* TAnnoDia *******************************************

class TAnnoDia:public TMeseDia
  {
    public:
     PTAnWin SUMA;

     TAnnoDia(PTWindowsObject,LPSTR,LPSTR);
//     virtual void SetupWindow();
//     virtual void calcola();
     virtual void Summary();
  };

// ********************************* TGiovApp *******************************************

class TGiovApp:public TApplication
  {

    public:
      TGiovApp(LPSTR AName,HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	       int nCmdShow);
    protected:
      virtual void InitMainWindow();
  };


