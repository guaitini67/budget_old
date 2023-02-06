
int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
char *mes[]={"GENNAIO","FEBBRAIO","MARZO","APRILE","MAGGIO","GIUGNO","LUGLIO",
                     "AGOSTO","SETTEMBRE","OTTOBRE","NOVEMBRE","DICEMBRE"};
char *gset[]={"Domenica","Lunedi","Martedi","Mercoledi","Giovedi","Venerdi","Sabato",""};

#define pagina \
if(prn&&(j<-800)) \
  {EndPage(DC);\
   StartPage(DC);\
   SetMapMode(DC,MM_LOENGLISH);\
   SelectObject(DC,Font);\
   j=-60;}


