

// ********************************* TMesWin ********************************************

_CLASSDEF(TMesWin)

class TMesWin:public TGiovWindow
  {
    public:
     TMesWin(PTWindowsObject,LPSTR,LPSTR,LPSTR,LPSTR);
  };

/*class TMesWin:public TGiovWindow
  {
    public:
     TMesWin(PTWindowsObject,LPSTR,LPSTR,LPSTR,LPSTR);
     virtual ~TMesWin();
     virtual void SetupWindow();
     virtual void Prima()=[CM_FIRST+SUM_GPRIMA];
     virtual void Dopo()=[CM_FIRST+SUM_GDOPO];
     virtual void calcola();
     virtual void riempi(HDC DC,int);
  }; */

// ********************************* TMeseDia *******************************************

_CLASSDEF(TMeseDia)

class TMeseDia:public TDialog
  {
    public:
     TMeseDia(PTWindowsObject,LPSTR,LPSTR);
  };

/*
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
  }; */

// ************************** FUNZIONI DI TMeseDia **************************************

TMeseDia::TMeseDia(PTWindowsObject AParent,LPSTR ResourceId,LPSTR str):
      TDialog(AParent,ResourceId)
  {
  }

/*
TMeseDia::TMeseDia(PTWindowsObject AParent,LPSTR ResourceId,LPSTR str):
      TDialog(AParent,ResourceId)
  {
    CATIN=new TListBox(this,CAT_IN,NULL);
    CATOUT=new TListBox(this,CAT_OUT,NULL);
    strcpy(mese,str);
  }

void TMeseDia::SetupWindow()
  {
    TDialog::SetupWindow();
    calcola();
    Summary();
  }

void TMeseDia::calcola()
  {
    sscanf(mese,"%02d-%04d",&m,&a);
    sprintf(all,"%d.map",a);
    pf=fopen(all,"r");
    if(!(gf=fopen("categ.dat","r")))
      {
	gf=fopen("categ.dat","w");
	fprintf(gf,"0,0\n");
	fclose(gf);
      }
    else
      {
	fscanf(gf,"%d,%d\n",&e,&u);
	for(i=0;i<e;i++)
	  {
	    fscanf(gf,"%[!-z ]\n",cat);
	    CATIN->AddString(cat);
	  }
	for(i=0;i<u;i++)
	  {
	    fscanf(gf,"%[!-z ]\n",cat);
	    CATOUT->AddString(cat);
	  }
	fclose(gf);
      }
    while(!feof(pf))
      {
	fscanf(pf,"%5s)%ld,%d,%d,%d,%ld\n",ord,&rimpre,&e,&u,&n,&rimpreb);
	for(i=0;i<e;i++)
	  {
	    fgets(all,110,pf);
	    sscanf(all,"%s\t | %[!-z ]\t | %[!-z ]\n",lire,mot,cat);   // lettura formattata della stringa
	    if(CATIN->FindExactString(cat,-1)<0)
              CATIN->AddString(cat);
	  }
	for(i=0;i<u;i++)
	  {
	    fgets(all,110,pf);
	    sscanf(all,"%s\t | %[!-z ]\t | %[!-z ]\n",lire,mot,cat);   // lettura formattata della stringa
	    if(CATOUT->FindExactString(cat,-1)<0)
	      CATOUT->AddString(cat);
	  }
	for(i=0;i<=n;i++)
	  {
	    fgets(all,110,pf);
	  }
      }
    fclose(pf);
    categorie(CATIN,CATOUT);
  }

void TMeseDia::HandOK()
  {
    CloseWindow();
  }

void TMeseDia::HandSummary()
  {
    Summary();
  }

void TMeseDia::Summary()
  {
    SUM=new TMesWin(this,"Riepilogo conti",mese,"","");
    SUM->Attr.X=100;
    SUM->Attr.Y=100;
    SUM->Attr.W=800;
    SUM->Attr.H=600;
    GetApplication()->MakeWindow(SUM);
  } */

// ************************** FUNZIONI DI TMesWin **************************************


TMesWin::TMesWin(PTWindowsObject AParent,LPSTR ATitle
             ,LPSTR nom,LPSTR or="",LPSTR giorno=""):
    TGiovWindow(AParent,ATitle,nom,or,giorno,NULL)
  {
  }

/*
TMesWin::TMesWin(PTWindowsObject AParent,LPSTR ATitle
             ,LPSTR nom,LPSTR or="",LPSTR giorno=""):
    TGiovWindow(AParent,ATitle,nom,or,giorno,NULL)
  {
    AssignMenu("BARRA1");
    Attr.Style|=WS_POPUPWINDOW|WS_CAPTION|WS_VSCROLL;
    Scroller=new TScroller(this,8,15,80,1000);
    strcpy(nome,nom);
  } 

TMesWin::~TMesWin()
  {
    remove("mese.dat");
  }

void TMesWin::SetupWindow()
  {
    TWindow::SetupWindow();
    sscanf(nome,"%02d-%04d",&m,&a);
    calcola();
  }

void TMesWin::calcola()
  {
    char nome1[10];

    sprintf(str1,"%02d-%02d-%04d",month[m-1],m,a);
    sprintf(ord,"%02d.%02d",m,month[m-1]);
    strcpy(nome1,nome);
    sprintf(nome,"%d.map",a);
    TGiovWindow::calcola();
    strcpy(nome,nome1);
    totc=0;
    sprintf(all,"%d.map",a);
    pf=fopen(all,"r");
    sprintf(ord,"%02d.01",m);
    gf=fopen("mese.dat","w");
    tf=fopen("categ.dat","r");
    fscanf(tf,"%d,%d\n",&ee,&uu);
    if(cercainfile(pf,&pos,ord))
      {
	for(ii=0;ii<ee;ii++)
	  {
            totc=0;
	    fscanf(tf,"%[!-z ]\n",cat1);
	    do
              {
	        fscanf(pf,"%5s)%ld,%d,%d,%d,%ld\n",ord1,&rimpre,&e,&u,&n,&rimpreb);
	        for(i=0;i<e;i++)
	          {
	            fscanf(pf,"%s\t | %[!-z ]\t | %[!-z ]\n",lire,mot,cat);   // lettura formattata della stringa
		    if(!strcmp(cat1,cat))
		      {
		        totc=totc+atol(lire);
		        if(!isdigit(lire[0]))
		          totc=totc+atol(lire+1);
		        if(!isdigit(lire[1]))
		          totc=totc+atol(lire+2);
		      }
                  }
 	        for(i=0;i<=(u+n);i++)
		  fgets(all,110,pf);
		sscanf(ord1,"%02d.%02d",&m1,&g);
	      }
	    while(g!=month[m-1]);
	    if(totc)
	      fprintf(gf,"e%ld,%s\n",totc,cat1);
	    fsetpos(pf,&pos);
          }
	for(ii=0;ii<uu;ii++)
	  {
            totc=0;
	    fscanf(tf,"%[!-z ]\n",cat1);
	    do
              {
	        fscanf(pf,"%5s)%ld,%d,%d,%d,%ld\n",ord1,&rimpre,&e,&u,&n,&rimpreb);
	        for(i=0;i<e;i++)
		  fgets(all,110,pf);
		for(i=0;i<u;i++)
		  {
	            fscanf(pf,"%s\t | %[!-z ]\t | %[!-z ]\n",lire,mot,cat);   // lettura formattata della stringa
		    if(!strcmp(cat1,cat))
		      {
		        totc=totc+atol(lire);
		        if(!isdigit(lire[0]))
		          totc=totc+atol(lire+1);
		        if(!isdigit(lire[1]))
		          totc=totc+atol(lire+2);
		      }
                  }
		for(i=0;i<=n;i++)
		  fgets(all,110,pf);
		sscanf(ord1,"%02d.%02d",&m1,&g);
	      }
	    while(g!=month[m-1]);
	    if(totc)
	      fprintf(gf,"u%ld,%s\n",totc,cat1);
	    fsetpos(pf,&pos);
          }
      }
    fclose(gf);
    fclose(pf);
    fclose(tf);
  }

void TMesWin::riempi(HDC DC,int prn)
  {
    char *pippo;

    k=a;
    SetMapMode(DC,MM_LOENGLISH);
    Font=CreateFont(-12,0,0,0,0,0,0,0,0,0,0,0,0,"Times New Roman");
    SelectObject(DC,Font);
    pf=fopen("mese.dat","r");
    sprintf(all,"RIEPILOGO CONTI DEL MESE DI %s %d",mes[m-1],k);
    j=-60;
    TextOut(DC,280,j,all,strlen(all));
    sprintf(all,"RIEPILOGO ENTRATE PER CATEGORIE");
    TextOut(DC,300,j-40,all,strlen(all));
    sprintf(all,"CATEGORIA");
    TextOut(DC,100,j-60,all,strlen(all));
    sprintf(all,"IMPORTO TOTALE");
    TextOut(DC,540,j-60,all,strlen(all));
    j=j-100;
    while((pippo=fgets(all,110,pf))&&(all[0]=='e'))
      {
	sscanf(all,"e%ld,%[!-z ]\n",&tote,cat);
        sprintf(all,"%s",cat);
        TextOut(DC,100,j,all,strlen(all));
	sprintf(all,"%ld",tote);
        TextOut(DC,540,j,all,strlen(all));
	j=j-20;
        pagina
      }
    if(pippo)                                  //tiene conto della prima stringa di uscite
      sscanf(all,"u%ld,%[!-z ]\n",&totu,cat);  //letta nel ciclo precedente
    sprintf(all,"RIEPILOGO USCITE PER CATEGORIE");
    TextOut(DC,300,j-20,all,strlen(all));
    sprintf(all,"CATEGORIA");
    TextOut(DC,100,j-40,all,strlen(all));
    sprintf(all,"IMPORTO TOTALE");
    TextOut(DC,540,j-40,all,strlen(all));
    j=j-80;
    if(pippo)                                  //ora stampa la prima stringa di uscita
      {                                        //
	sprintf(all,"%s",cat);
	TextOut(DC,100,j,all,strlen(all));
	sprintf(all,"%ld",totu);
        TextOut(DC,540,j,all,strlen(all));
	j=j-20;
	pagina
      }
    while(fgets(all,110,pf)&&(all[0]=='u'))
      {
	sscanf(all,"u%ld,%[!-z ]\n",&totu,cat);
        sprintf(all,"%s",cat);
        TextOut(DC,100,j,all,strlen(all));
	sprintf(all,"%ld",totu);
        TextOut(DC,540,j,all,strlen(all));
	j=j-20;
	pagina
      }
    fclose(pf);
        j=j-20;
 	sprintf(all,"TOTALE EFFETTIVO ENTRATE BANCA+CONTANTI DA INIZIO MESE:");
	TextOut(DC,40,j-20,all,strlen(all));
	sprintf(all,"%ld",totem+totebm-ovem);
	TextOut(DC,540,j-20,all,strlen(all));
	pagina
	sprintf(all,"TOTALE EFFETTIVO ENTRATE BANCA+CONTANTI DA INIZIO ANNO:");
	TextOut(DC,40,j-40,all,strlen(all));
	sprintf(all,"%ld",totean+toteban-ovean);
	TextOut(DC,540,j-40,all,strlen(all));
	pagina
	sprintf(all,"TOTALE EFFETTIVO USCITE BANCA+CONTANTI DA INIZIO MESE:");
	TextOut(DC,40,j-80,all,strlen(all));
	sprintf(all,"%ld",totum+totubm-ovum);
	TextOut(DC,540,j-80,all,strlen(all));
	pagina
	sprintf(all,"TOTALE EFFETTIVO USCITE BANCA+CONTANTI DA INIZIO ANNO:");
	TextOut(DC,40,j-100,all,strlen(all));
	sprintf(all,"%ld",totuan+totuban-ovuan);
	TextOut(DC,540,j-100,all,strlen(all));
   DeleteObject(Font);
  }

void TMesWin::Prima()
  {
    HDC DC;

    if(m==1)
      {
	sprintf(all,"%d.map",a-1);
        if(!(pf=fopen(all,"r")))
	  MessageBox(HWindow,"Non ci sono dati sull'anno selezionato",
"File non trovato", MB_OK);
	else
	  {
            m=12;
	    a--;
            fclose(pf);
          }
      }
    else
      m--;
    calcola();
    InvalidateRect(HWindow,NULL,TRUE);
    DC=GetDC(HWindow);
    riempi(DC,0);
    ReleaseDC(HWindow,DC);
  }

void TMesWin::Dopo()
  {
    HDC DC;

    if(m==12)
      {
	sprintf(all,"%d.map",a+1);
        if(!(pf=fopen(all,"r")))
	  MessageBox(HWindow,"Non ci sono dati sull'anno selezionato",
"File non trovato", MB_OK);
	else
	  {
            m=1;
	    a++;
            fclose(pf);
          }
      }
    else
      m++;
    calcola();
    InvalidateRect(HWindow,NULL,TRUE);
    DC=GetDC(HWindow);
    riempi(DC,0);
    ReleaseDC(HWindow,DC);
  } */
