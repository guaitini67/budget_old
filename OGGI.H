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
