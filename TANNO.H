
// ********************************* TAnWin ********************************************

_CLASSDEF(TAnWin)

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

_CLASSDEF(TAnWin)

class TAnnoDia:public TMeseDia
  {
    public:
     PTAnWin SUMA;

     TAnnoDia(PTWindowsObject,LPSTR,LPSTR);
//     virtual void SetupWindow();
//     virtual void calcola();
     virtual void Summary();
  };

// ************************** FUNZIONI DI TAnnoDia **************************************

TAnnoDia::TAnnoDia(PTWindowsObject AParent,LPSTR ResourceId,LPSTR str):
      TMeseDia(AParent,ResourceId,str)
  {
    CATIN=new TListBox(this,CAT_IN,NULL);
    CATOUT=new TListBox(this,CAT_OUT,NULL);
    strcpy(mese,str);
  }

void TAnnoDia::Summary()
  {
    SUMA=new TAnWin(this,"Riepilogo conti",mese,"","");
    SUMA->Attr.X=100;
    SUMA->Attr.Y=100;
    SUMA->Attr.W=800;
    SUMA->Attr.H=600;
    GetApplication()->MakeWindow(SUMA);
  }

// ************************** FUNZIONI DI TMesWin **************************************

TAnWin::TAnWin(PTWindowsObject AParent,LPSTR ATitle
             ,LPSTR nom,LPSTR or="",LPSTR giorno=""):
    TGiovWindow(AParent,ATitle,nom,or,giorno,NULL)
  {
    AssignMenu("BARRA2");
    Attr.Style|=WS_POPUPWINDOW|WS_CAPTION|WS_VSCROLL;
    Scroller=new TScroller(this,8,15,80,1000);
    strcpy(nome,nom);
  }

TAnWin::~TAnWin()
  {
    remove("anno.dat");
  }

void TAnWin::SetupWindow()
  {
    TWindow::SetupWindow();
    sscanf(nome,"%02d-%04d",&m,&a);
    calcola();
  }

void TAnWin::calcola()
  {
    char nome1[10];

    sprintf(str1,"31-12-%04d",a);
    sprintf(ord,"12.31");
    strcpy(nome1,nome);
    sprintf(nome,"%d.map",a);
    TGiovWindow::calcola();
    strcpy(nome,nome1);
    totc=0;
    sprintf(all,"%d.map",a);
    pf=fopen(all,"r");
    sprintf(ord,"01.01");
    gf=fopen("anno.dat","w");
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
	      }
	    while(strcmp(ord1,"12.31"));
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
	      }
	    while(strcmp(ord1,"12.31"));
	    if(totc)
	      fprintf(gf,"u%ld,%s\n",totc,cat1);
	    fsetpos(pf,&pos);
	  }
      }
    fclose(gf);
    fclose(pf);
    fclose(tf);
  }

void TAnWin::riempi(HDC DC,int prn)
  {
    char *pippo;

    k=a;
    SetMapMode(DC,MM_LOENGLISH);
    Font=CreateFont(-12,0,0,0,0,0,0,0,0,0,0,0,0,"Times New Roman");
    SelectObject(DC,Font);
    pf=fopen("anno.dat","r");
    sprintf(all,"RIEPILOGO CONTI DELL'ANNO %d",k);
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
	sprintf(all,"TOTALE EFFETTIVO ENTRATE BANCA+CONTANTI DA INIZIO ANNO:");
	TextOut(DC,40,j-40,all,strlen(all));
	sprintf(all,"%ld",totean+toteban-ovean);
	TextOut(DC,540,j-40,all,strlen(all));
	pagina
	sprintf(all,"TOTALE EFFETTIVO USCITE BANCA+CONTANTI DA INIZIO ANNO:");
	TextOut(DC,40,j-100,all,strlen(all));
	sprintf(all,"%ld",totuan+totuban-ovuan);
	TextOut(DC,540,j-100,all,strlen(all));
   DeleteObject(Font);
  }

void TAnWin::Prima()
  {
    HDC DC;

    sprintf(all,"%d.map",a-1);
    if(!(pf=fopen(all,"r")))
      MessageBox(HWindow,"Non ci sono dati sull'anno selezionato",
"File non trovato", MB_OK);
    else
      {
	a--;
        fclose(pf);
      }
    calcola();
    InvalidateRect(HWindow,NULL,TRUE);
    DC=GetDC(HWindow);
    riempi(DC,0);
    ReleaseDC(HWindow,DC);
  }

void TAnWin::Dopo()
  {
    HDC DC;

    sprintf(all,"%d.map",a+1);
    if(!(pf=fopen(all,"r")))
      MessageBox(HWindow,"Non ci sono dati sull'anno selezionato",
"File non trovato", MB_OK);
    else
      {
        a++;
        fclose(pf);
      }
    calcola();
    InvalidateRect(HWindow,NULL,TRUE);
    DC=GetDC(HWindow);
    riempi(DC,0);
    ReleaseDC(HWindow,DC);
  }
