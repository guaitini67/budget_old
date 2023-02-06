
// *********************** CLASSE TGiovWindow ****************************************

_CLASSDEF(TGiovWindow)

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
  };

// ************************** FUNZIONI DI TGiovWindow ***********************************

TGiovWindow::TGiovWindow(PTWindowsObject AParent,LPSTR ATitle
             ,LPSTR nom,LPSTR or,LPSTR giorno, int wee):
    TWindow(AParent,ATitle)
  {
    AssignMenu("BARRA");
    Attr.Style|=WS_POPUPWINDOW|WS_CAPTION|WS_VSCROLL;
    Scroller=new TScroller(this,8,15,80,1000);
    strcpy(nome,nom);
    strcpy(ord,or);
    strcpy(str1,giorno);
    week=wee;
  }

void TGiovWindow::SetupWindow()
  {
    TWindow::SetupWindow();
  }

