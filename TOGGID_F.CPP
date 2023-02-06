#include <print.h>
#include <string.h>
#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <time.h>
#include <static.h>
#include <combobox.h>
#include "tcomgio.h"    //contiene TComboGiov
#include "giotype.h"

int caricasaldi(long int,info_risorse *);
int aggiornasaldi(long int ,int ,
		  char *,info_risorse *);
void mostrasaldi(PTComboGiov,PTComboGiov,
		 PTStatic,info_risorse *);
int caricav(info_risorse *,int *);
int leggiprepara(long int,char *);
int scrivifile(char *,long int,char *);
int scrivinote(HWND,char *,long int,int);
int cancella(HWND,char *,long int,char *);
int controllabasedata();
int controllafiledati(char *);
int compareFiles(char *,char *);
int scrivilog(char,char *);
int backuplog();
float search_util(FILE *,char *,char,char,
		  long int *,int *,int *,int *,
		  int *,int *);
int search_year(char *,char *,info_smaller *,
		int,long int,long int);

void ordinanum(PTListGiov,PTListGiov,PTListGiov,
		      PTListGiov,PTListGiov,char);
void ordinalet(PTListGiov,PTListGiov,
		PTListGiov,PTListGiov,PTListGiov,char);



char string[401],string1[401],nomeconv[27],
     fnam1[20],fnam2[20],mot_f[100],cat_f[100];
int i,j,k,fine,num_ops,nrighe,pa,pb,pp,match,
    posiz,nanni,numfonti,g,m,a,err,plink,index;
long int numg_fa,numg_fb;
double importo_f,saldo,importo;
struct time Time;
struct date Date;
FILE *fil,*tmp,*ntt,*map,*f1,*f2,*log;


int caricav(info_risorse *tabella_risorse,int *numfonti)
  {
    if((k=controllabasedata())<0)
//In caso di errore
      return k;
//Se sono qui basedata.dat e' a posto. Non faccio
//piu' nessun controllo
    map=fopen("basedata.dat","r");
    fgets(string,400,map);
    sscanf(string,"%d\n",numfonti);
    for(i=0;i<(*numfonti);i++)
      {
	fgets(string,400,map);
	sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,
	   nomeconv);
//Di default tutte le risorse sono mostrabili
//nel campo di riepilogo
	tabella_risorse[i].mostra=1;
//Ho superato quanto c'era prima
//Copio il numero risorsa e il nome nel record
	tabella_risorse[i].posiz=posiz;
        strcpy(tabella_risorse[i].nomeconv,nomeconv);
//Leggo il giorno di apertura (se c'e')
	fgets(string,400,map);
//Cerco le informazioni sul giorno di apertura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di apertura
//Scrivo -1 nel record          
	  tabella_risorse[i].aperto=-1;
	else
	  {
//C'e' il giono di apertura e lo scrivo nel record
//(notare il cast al secondo membro)
	    sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a);
	    tabella_risorse[i].aperto=
	    (long int)a*10000+m*100+g;
	  } 
//Leggo il giorno di chiusura (se c'e')
	fgets(string,400,map);
//Cerco le informazioni sul giorno di chiusura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di chiusura
//Scrivo -1 nel record          
	  tabella_risorse[i].chiuso=-1;
	else
	  {
//C'e' il giono di chiusura e lo scrivo nel record
//(notare il cast al secondo membro)
	    sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a);
	    tabella_risorse[i].chiuso=
	    (long int)a*10000+m*100+g;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	fgets(string,400,map);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	for(j=0;j<nrighe+nanni;j++)
	  fgets(string,400,map);
      }
    fclose(map);
//Aggiungo alla fine della lista un elemento globale
    sprintf(tabella_risorse[(*numfonti)].nomeconv,"Globale");
//Questa assegnazione e' arbitraria
    tabella_risorse[(*numfonti)].posiz=0;
    return 0;
  }



int leggiprepara(long int num_g,
		 char *nome_file)
//Trova il prossimo numero di operazione disponibile
//per il giorno selezionato: se non c'era niente il
//numero e' 1, se c'era qualcosa e' 'operazione
//con numero piu' alto+1'. Se errore: -1
  {
    if(!(fil=fopen(nome_file,"r")))
//Se il file e' nuovo lo creo e restituisco 1
      {
	fil=fopen(nome_file,"w");
	fclose(fil);
        return 1;
      }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
//Si e' verificato un errore nel file dati
      return k;
//Se sono qui sono sicuro che il file dati e' a posto
//posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    do
      {
	if(!fgets(string,400,fil))
	  {
//Ho raggiunto la fine del file
//Restituisco 1
	    fclose(fil);
	    return 1;
	  }
	sscanf(string,"%ld,%d,%d\n",&numg_fa,
		&num_ops,&nrighe);
	if((numg_fa<num_g))
//Sono a un giorno precedente a quello desiderato
	  for(i=0;i<num_ops+nrighe;i++)
	    fgets(string,400,fil);
      }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Restituisco 1
        fclose(fil);
        return 1;
      }
//Se sono qui e' perche' numg_fa==num_g:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Individuo il prossimo numero disponibile
//e lo restituisco
    for(i=0;i<num_ops;i++)
      {
	fgets(string,400,fil);
	sscanf(string,
	  "%d,%lf,%[!-~ ¡-ÿ‘’]\t,%[!-~ ¡-ÿ‘’]\t,%d,%d,%ld,%d\n",
          &j,&importo_f,mot_f,cat_f,&pa,&pb,
	  &numg_fb,&k);
      }
//Se sono arrivato qui il prossimo numero
//disponibile e' j+1 e lo restituisco
    fclose(fil);
    return (j+1);
  }


int scrivifile(char *nome_file,
	       long int num_g,char *datastring)
//Struttura del file:
//giorno
//giorno
//.
//.
//dove giorno e':
//
//num_g,n_o,n_r   (num_g=aaaammgg)
//n1,+/-imp,mot	,cat ,viene_da,va_in,num_g1,m1
//.
//.
//nn_o,+/-imp,mot, cat,	viene_da,va_in,num_gn_o,mn_o
//riga note 1
//.
//.
//riga note n_r
// 
//N.B. se non ho dati in un certo giorno
//quel giorno non compare nel file
//NOTA: non creo il file e non controllo l'integrita' del file
//perche' prima di questa funzione va sempre chiamata
//leggiprepara che a sua volta crea il file e chiama controllafiledati
  {
    strcpy(string1,datastring);
    if(!(fil=fopen(nome_file,"r")))
//Il file qui dovrebbe gia' esistere!!! Errore
      return -100;
//Se sono qui sono sicuro che il file dati e' a posto
//posso procedere senza controllare l'integrita'
    tmp=fopen("temp","w");
    numg_fa=0;
    numg_fb=0;
    fine=0;
//Entro nella fase di aggiornamento dei dati
    for(;;)
      {
	if(!fgets(string,400,fil))
//Ho raggiunto la fine del file. Esco dal loop con fine=1
	  {
	    fine=1;
	    break;
          }
	sscanf(string,"%ld,%d,%d\n",&numg_fa,&num_ops,&nrighe);
	if(numg_fa>=num_g)
//Ho raggiunto o superato il giorno desiderato
//esco dal loop 
	  break;
	fprintf(tmp,"%s",string);
	for(i=0;i<num_ops+nrighe;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
    if(fine)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere e quelli successivi
//non contenevano gia' dei dati (ho raggiunto la fine)
//Lo scrivo ex novo e poi il file finira'
        fprintf(tmp,"%ld,1,0\n",num_g);
//Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",string1);
      }  
    else if(numg_fa==num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Aggiungo i nuovi dati, e poi gli appendo (fuori da questo 'do')
//i giorni successivi (se ci sono)
        fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops+1,
	nrighe);
	for(i=0;i<num_ops;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
//Raggiunta la fine dei dati del giorno
//Adesso posso scrivere la nuova stringa
	fprintf(tmp,"%s\n",string1);
	for(i=0;i<nrighe;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
    else if(numg_fa>num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Lo scrivo ex novo e poi gli devo appendere
//quello che segue
        fprintf(tmp,"%ld,1,0\n",num_g);
//Adesso posso scrivere la nuova stringa
        fprintf(tmp,"%s\n",string1);
//Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",string);
	for(i=0;i<num_ops+nrighe;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
//A questo punto i nuovi dati sono dentro
//Ricopio quello che segue (se c'e') e finisco
    if(!fine)
      while(!feof(fil))
        {
          fgets(string,400,fil);
	  if(!feof(fil))
	    fprintf(tmp,"%s",string);
	}
    fclose(fil);
    fclose(tmp);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
  }


int scrivinote(HWND HWindow, char *nome_file,
	       long int num_g,int nlnote)
//Questa funzione e' simile a scrivifile
//ma serve a scrivere le note e non i movimenti
  {
    if(!(fil=fopen(nome_file,"r")))
      {
	fil=fopen(nome_file,"w");
	fclose(fil);
      }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
//Si e' verificato un errore nel file dati
      return k;
//Se sono qui sono sicuro che il file dati e' a posto
//posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    tmp=fopen("temp","w");
    if(!(ntt=fopen("note.tmp","r")))
      {
	MessageBox(HWindow,
        "Manca il file 'note.tmp'",
        "Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(fil);
	fclose(tmp);
	return -1;
      }
    numg_fa=0;
    numg_fb=0;
    fine=0;
//Entro nella fase di aggiornamento dei dati
    for(;;)
      {
	if(!fgets(string,400,fil))
//Ho raggiunto la fine del file. Esco dal loop con fine=1
          {
	    fine=1;
	    break;
          }
         sscanf(string,"%ld,%d,%d\n",&numg_fa,
		&num_ops,&nrighe);
	if(numg_fa>=num_g)
//Ho raggiunto o superato il giorno desiderato
//esco dal loop 
	  break;
//Se sono qui sono a un giorno precedente a quello desiderato
	fprintf(tmp,"%s",string);
	for(i=0;i<num_ops+nrighe;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
    if(fine)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere e quelli successivi
//non contenevano gia' dei dati (ho raggiunto la fine)
//e ho delle note da scrivere
//Lo scrivo ex novo e poi il file finira'
	if(nlnote)
//Solo se ho note da scrivere creo il giorno
	  fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,400,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
		"Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
      }
    else if(numg_fa==num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere conteneva gia' dei dati
//Qui devo controllare se num_ops o nlnote sono non nulli
//per sapere se mettere il giorno
//Aggiungo i nuovi dati, e poi gli appendo 
//i giorni successivi (se ci sono)
	if(num_ops||nlnote)
//Solo se c'e' dato o note (ri)creo il giorno
          fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops,
	  nlnote);
	for(i=0;i<num_ops;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
//Raggiunta la fine dei dati del giorno
//Faccio scorrere in avanti il file originale
//fino alla fine del giorno
	for(i=0;i<nrighe;i++)
	  fgets(string,400,fil);
//Adesso posso scrivere le nuove note
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,400,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
	        MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
      }
    else if(numg_fa>num_g)
      {
//Questo puo' accadere se:
//Il giorno che voglio scrivere 
//non conteneva gia' dei dati ma almeno uno
//dei giorni successivi li contiene
//Lo scrivo ex novo e poi gli devo appendere
//quello che segue

//Copio string in string1 perche' mi servira'
//(e' l'intestazione del giorno che segue)
	strcpy(string1,string);
	if(nlnote)
//Solo se ho note da scrivere creo il giorno
	  fprintf(tmp,"%ld,0,%d\n",num_g,nlnote);
	for(i=0;i<nlnote;i++)
	  {
//Leggo una riga da note.tmp
	    if(!fgets(string,400,ntt))
	      {
//Mi aspettavo di leggere una stringa ma non la trovo: errore
		MessageBox(HWindow,
	        "Anomalia nel file 'note.tmp'",
	        "Problema!",MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
                fclose(ntt);
	        return -1;
	      }
//Adesso posso scrivere la nuova stringa
            fputs(string,tmp);
	  }
//Ricopio l'intestazione del giorno che segue
        fprintf(tmp,"%s",string1);
        for(i=0;i<num_ops+nrighe;i++)
	  {
	    fgets(string,400,fil);
//Ricopio la riga
	    fprintf(tmp,"%s",string);
	  }
      }
//A questo punto i nuovi dati sono dentro
//Ricopio quello che segue (se c'e') e finisco
    if(!fine)
      while(!feof(fil))
        {
          fgets(string,400,fil);
	  if(!feof(fil))
	    fprintf(tmp,"%s",string);
        }
    fclose(fil);
    fclose(tmp);
    fclose(ntt);
    remove(nome_file);
    rename("temp",nome_file);
    return 0;
  }


int cancella(HWND HWindow,
             char *nome_file,long int num_g,
	     char *datastring)
  {
    if(!(fil=fopen(nome_file,"r")))
      {
//Manca il file su cui fare la cancellazione: errore
        MessageBox(HWindow,"Manca il file dati: canc1",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return -118;
      }
    fclose(fil);
    if((k=controllafiledati(nome_file))<0)
//Si e' verificato un errore nel file dati
      return k;
//Se sono qui sono sicuro che il file dati e' a posto
//posso procedere senza controllare l'integrita'
    fil=fopen(nome_file,"r");
    tmp=fopen("temp","w");
    numg_fa=0;
    numg_fb=0;
//Entro nella fase di aggiornamento dei dati
    do
      {
	if(!fgets(string,400,fil))
//Ho raggiunto la fine del file senza trovare
//la stringa: errore
	  {
	    MessageBox(HWindow,
	    "Non trovo la stringa nel file di \
cancellazione: canc2",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(fil);
	    fclose(tmp);
            return -1;
	  }
	sscanf(string,"%ld,%d,%d\n",&numg_fa,
		&num_ops,&nrighe);
	if(numg_fa<num_g)
	  {
//Sono a un giorno precedente a quello desiderato
	    fprintf(tmp,"%s",string);
	    for(i=0;i<num_ops+nrighe;i++)
	      {
		fgets(string,400,fil);
//Ricopio la riga
	        fprintf(tmp,"%s",string);
	      }
	  }
      }
    while(numg_fa<num_g);
    if(numg_fa>num_g)
      {
//Se questo accade:
//Non ho trovato il giorno di cancellazione:
//errore 
        MessageBox(HWindow,
	"Il giorno della cancellazione \
non esiste nel file: canc7",
	"Problema!",
	MB_ICONEXCLAMATION | MB_OK);
	fclose(fil);
	fclose(tmp);
        return -1;
      }
//Se sono qua significa che numg_fa==num_g
//Ho raggiunto il giorno dell'operazione da cancellare
    if(num_ops)
      num_ops--;
    if(num_ops||nrighe)
//Solo se ci sono operazioni rimanenti o note
//scrivo il giorno di interesse
      fprintf(tmp,"%ld,%d,%d\n",numg_fa,num_ops,
      nrighe);
    match=0;
    for(i=0;i<num_ops+1;i++)
      {
	fgets(string,400,fil);
	if(strcmp(datastring,string)!=0)
          {
//Solo se non e' la stringa che voglio cancellare
//posso copiarla in temp
	    if((i==num_ops)&&(!match))
              {
//Non ho trovato la stringa da cancellare: errore
	        MessageBox(HWindow,
	        "Non trovo nel file la\
 stringa da cancellare:canc10",
	        "Problema!",
	        MB_ICONEXCLAMATION | MB_OK);
	        fclose(fil);
	        fclose(tmp);
	        return -1;
	      }
//Se sono qui posso ricopiare in temp la stringa
	    fprintf(tmp,"%s",string);
	  }
	else
	  match=1;
      }
//Scrivo le note
    for(i=0;i<nrighe;i++)
      {
	fgets(string,400,fil);
//Ricopio la riga
        fprintf(tmp,"%s",string);
      }
//A questo punto i nuovi dati sono dentro
//Ricopio quello che segue (se c'e') e finisco
    while(!feof(fil))
      {
        fgets(string,400,fil);
        if(!feof(fil))
	  fprintf(tmp,"%s",string);
      }
    fclose(fil);
    fclose(tmp);
    return 0;
  }

int controllabasedata()
//Scorre il basedata.dat per intero
//per capire se e' formattato bene
  {
    if(!(map=fopen("basedata.dat","r")))
      {
//Non trovo basedata.dat: errore
	return -1;
      }
//Adesso cambio directory e ci creo tempor
    if(chdir("budback")<0)
      {
//In caso di errore...
	fclose(map);
	return -100;
      }
    tmp=fopen("tempor","w");
    if(!fgets(string,400,map))
      {
//Non trovo niente in basedata.dat: errore
	fclose(map);
        fclose(tmp);
	chdir("..");
	return -2;
      }
    if(sscanf(string,"%d\n",&numfonti)!=1)
      {
//Formattazione non corretta: errore
	fclose(map);
	fclose(tmp);
	chdir("..");
	return -3;
      }
//Tutto OK, copio su temp
    fputs(string,tmp);
    for(i=0;i<numfonti;i++)
      {
	if(!fgets(string,400,map))
	  {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -4;
	  }
	if(sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,
	   nomeconv)!=2)
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -5;
	  }
//Tutto OK, copio su temp
        fputs(string,tmp);
//Leggo il giorno di apertura (se c'e')
	if(!fgets(string,400,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -6;
	  }
//Cerco le informazioni sul giorno di apertura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di apertura
//Tutto OK, copio su temp
            fputs(string,tmp);
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//C'e' il giono di apertura
//Tutto OK, copio su temp
          fputs(string,tmp);
	else
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -7;
	  }
//Leggo il giorno di chiusura (se c'e')
	if(!fgets(string,400,map))
          {
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -8;
	  }
//Cerco le informazioni sul giorno di chiusura
	if(!strcmp(string,"nil\n"))
//Ho trovato nil: non c'e' giorno di chiusura
//Tutto OK, copio su temp
          fputs(string,tmp);
	else if(sscanf(string,"%02d-%02d-%04d\n",
		&g,&m,&a)==3)
//C'e' il giono di chiusura
//Tutto OK, copio su temp
          fputs(string,tmp);
	else
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -9;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	if(!fgets(string,400,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -10;
	  }
	if(sscanf(string,"%d,%d\n",&nrighe,&nanni)!=2)
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    chdir("..");
	    return -11;
          }
//Tutto OK, copio su temp
        fputs(string,tmp);
	for(j=0;j<nrighe;j++)
          {
	    if(!fgets(string,400,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
	        fclose(map);
	        fclose(tmp);
		chdir("..");
		return -12;
	      }
//Tutto OK, copio su temp
            fputs(string,tmp);
	  }
	for(j=0;j<nanni;j++)
          {
	    if(!fgets(string,400,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
		fclose(map);
	        fclose(tmp);
		chdir("..");
		return -13;
	      }
	    if(sscanf(string,"%d,%lf\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
		fclose(map);
	        fclose(tmp);
		chdir("..");
		return -14;
	      }
//Tutto OK, copio su temp
            fputs(string,tmp);
	  }
      }
//Tutto OK: ritorno corretto
    fclose(map);
    fclose(tmp);
    chdir("..");
    return 1;
  } 

int controllafiledati(char *nome_file)
//Scorre il file dei dati per intero
//per capire se e' formattato bene
  {
    int g,m,a;

    if(!(fil=fopen(nome_file,"r")))
//Se non riesco ad aprirlo ci deve essere stato qualche
//problema perche' se sono qui dovrebbe esistere
      return -1;
//Adesso cambio directory e ci creo tempor
    if(chdir("budback")<0)
      {
//In caso di errore...
	fclose(map);
	return -100;
      }
    tmp=fopen("tempor","w");
    while(fgets(string,400,fil))
      {
	if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    fclose(fil);
            fclose(tmp);
	    chdir("..");
	    return -2;
	  }
//Tutto OK, copio su temp
        fputs(string,tmp);
	for(i=0;i<num_ops;i++)
          {
	    if(!fgets(string,400,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente:
//errore
		fclose(fil);
	        fclose(tmp);
	        chdir("..");
		return -3;
	      }
	    if(sscanf(string,
	      "%d,%lf,%[!-~ ¡-ÿ‘’]\t,%[!-~ ¡-ÿ‘’]\t,\
%d,%d,%ld,%d\n",
              &j,&importo_f,mot_f,cat_f,&pa,&pb,
	      &numg_fb,&k)!=8)
              {
//Trovo i dati in un formato non valido: errore
		fclose(fil);
		fclose(tmp);
		chdir("..");
		return -4;
	      }
//Tutto OK, copio su temp
	    fputs(string,tmp);
	  }
	for(i=0;i<nrighe;i++)
          {
	    if(!fgets(string,400,fil))
	      {
//Mi aspettavo una riga e invece non trovo niente:
//errore
	        fclose(fil);
	        fclose(tmp);
		chdir("..");
		return -5;
	      }
//Tutto OK, copio su temp
            fputs(string,tmp);
	  }
      }
//Se sono qui: raggiunta la fine del file
//ritorno corretto
    fclose(fil);
    fclose(tmp);
    chdir("..");
    return 1;
  }

int compareFiles(char *file1, char *file2)
//Funzione che confronta se due files di testo
//sono identici. Se lo sono ritorna 0, se non
//lo sono ritorna >0, in caso di errore
//di apertura file ritorna -1 o -2
  {
    if(!(f1=fopen(file1,"r")))
      {
        return -1;
      }
    if(!(f2=fopen(file2,"r")))
      {
        fclose(f1);
        return -2;
      }
    for(;;)
      {
	if(feof(f1))
	  {
//Ho raggiunto la fine di f1
	    if(!feof(f2))
	      {
//Quindi se non ho raggiunto la fine di f2
//allora i due files sono diversi
//(cioe' f2 e' uguale a f1 ma ha qualcosa in piu')
//Ritorno 1             
		fclose(f1);
	        fclose(f2);
                return 1;
	      }
//Se sono qui i files sono identici
//Ritorno 0
	    fclose(f1);
            fclose(f2);
	    return 0;
	  }
//Se sono qui f1 non e' finito
	if(feof(f2))
	  {
//Quindi se e' finito f2 allora i due files
//sono diversi. Chiudo e ritorno 1
	    fclose(f1);
	    fclose(f2);
	    return 2;
	  }
//Se sono qui nessuno dei due e' finito
//Prendo una riga da f1 e una da f2
        fgets(string,400,f1);
	fgets(string1,400,f2);
//Se sono qui sono riuscito a leggere una
//riga per file e le posso confrontare
	if(strcmp(string,string1)!=0)
          {
//Se trovo una differenza
//chiudo i files e ritorno 1
	    fclose(f1);
            fclose(f2);
	    return 3;
	  }
      }
  }

int scrivilog(char pre, char *linea)
//Funzione che crea un log file contenente,
//per ogni comando che viene eseguito,
//la data e l'ora, che cosa ho fatto e l'esito
//Mi serve per motivi di debugging
//Il file e' anno_mese.log e viene allungato ad ogni uso
//del programma. Ad ogni nuovo mese passo ad un file nuovo
//cosi' ho un log delle attivita' mensili
  {
    gettime(&Time);
    getdate(&Date);
    sprintf(string,"%04d_%02d.log",Date.da_year,Date.da_mon);
    if(!(log=fopen(string,"a")))
      return -1;
    if(pre)
//Solo se pre>0 inserisco data e ora
      fprintf(log,"%02d/%02d/%04d, \
%02d:%02d:%02d :\n",Date.da_day,Date.da_mon,Date.da_year,
                   Time.ti_hour,Time.ti_min,Time.ti_sec);
    fprintf(log,"%s\n",linea);
    fclose(log);
    return 0;
  }

int backuplog()
//Questa funzione si occupa dei backup del log file
//voglio tenere i log degli ultimi due mesi
  {
    getdate(&Date);
//Determino il nome del log di questo mese
    sprintf(fnam1,"%04d_%02d.log",Date.da_year,Date.da_mon);
//Determino il nome del log del mese precedente
//(potrebbe esserci uno dei due o entrambi)
    if(Date.da_mon==1)
      sprintf(fnam2,"%04d_12.log",Date.da_year-1);
    else
      sprintf(fnam2,"%04d_%02d.log",Date.da_year,Date.da_mon-1);
    if((log=fopen(fnam1,"r")))
//Se c'e' il file mese corrente
      {
	chdir("budback");
	if(!(tmp=fopen(fnam1,"w")))
          return -1;
        while(!feof(log))
	  {
	    if(fgets(string1,400,log))
	      fputs(string1,tmp);
          }
        fclose(log);
	fclose(tmp);
	chdir("..");
      }
    if((log=fopen(fnam2,"r")))
//Se c'e' il file mese precedente
      {
	chdir("budback");
	if(!(tmp=fopen(fnam2,"w")))
          return -1;
        while(!feof(log))
	  {
	    if(fgets(string1,400,log))
	      fputs(string1,tmp);
          }
        fclose(log);
	fclose(tmp);
        chdir("..");
	remove(fnam2);
	chdir("budback");
      }
    if(Date.da_mon==2)
      sprintf(fnam1,"%04d_12.log",Date.da_year-1);
    else
      sprintf(fnam1,"%04d_%02d.log",Date.da_year,Date.da_mon-2);
//Rimuovo due mesi prima
    if((log=fopen(fnam1,"r")))
      {
        fclose(log);
	remove(fnam1);
      }
    chdir("..");
    return 0;
  }

float search_util(FILE *pf,char *what,char where,char how,
		  long int *numg,int *nop,int *nrig,int *pa,
		  int *pb,int *err)
//Funzione che cerca nel file aperto (deve essere di tipo anno.bud)
//la stringa what, e la cerca in motivo (where=0)
//o in categoria (where=1). Se how=0, cerca la corrispondenza esatta
//se how=1 basta che la stringa sia contenuta.
//La prima volta che trova what, ritorna l'importo dell'entrata
//o uscita. Se non la trova o in caso di errore ritorna 0. In caso
//di errore il flag err viene settato.
//Quando trova what, numg contiene il giorno in cui l'ha trovato
//pa e pb contengono le risorse in cui l'ha trovato
  {
    (*err)=0;
    for(;;)
      {
	for(i=0;i<(*nop);i++)
//Leggo le righe rimanenti di entrate/uscite
//(se inizio un nuovo giorno saltero' questa
//parte perche' (*nop)==0
	  {
	    if(!fgets(string,400,pf))
	      {
//Mi aspettavo una riga e invece non trovo niente:
//errore
		(*err)=-1;
		return 0;
	      }
	    if(sscanf(string,
	      "%d,%lf,%[!-~ ¡-ÿ‘’]\t,%[!-~ ¡-ÿ‘’]\t,\
%d,%d,%ld,%d\n",
              &j,&importo_f,mot_f,cat_f,pa,pb,
	      &numg_fb,&k)!=8)
              {
//Trovo i dati in un formato non valido: errore
		(*err)=-2;
		return 0;
	      }
	    switch(where)
	      {
		case 0:
//Cerca su motivo
		  if(
//Se avevo messo how=1 e la stringa e' contenuta
		    (how&&strstr(mot_f,what))
		    ||
//o se avevo messo how=0 e la stringa e' identica
		    ((!how)&&(!strcmp(mot_f,what)))
		    )
		    {
//Aggiorno (*nop) tenendo conto che i non e' stato ancora
//incrementato
		      (*nop)=(*nop)-i-1;
		      return importo_f;
		    }
		  break;
		case 1:
//Cerca su categoria
		  if(
//Se avevo messo how=1 e la stringa e' contenuta
		    (how&&strstr(cat_f,what))
		    ||
//o se avevo messo how=0 e la stringa e' identica
		    ((!how)&&(!strcmp(cat_f,what)))
		    )
		    {
//Aggiorno (*nop) tenendo conto che i non e' stato ancora
//incrementato
		      (*nop)=(*nop)-i-1;
		      return importo_f;
		    }
		  break;
		default:
                  ;
              }
	  }
	for(i=0;i<(*nrig);i++)
//Leggo le righe di entrate/uscite
//Alla prima chiamata della routine (*nrig) deve essere 0.
//Dopo sara' sempre aggiornata al valore del giorno successivo
	  {
	    if(!fgets(string,400,pf))
	      {
//Mi aspettavo una riga e invece non trovo niente:
//errore
		(*err)=-3;
		return 0;
	      }
	  }
	if(!fgets(string,400,pf))
	  {
//Ho raggiunto la fine del file: non c'e' match.
//Per sicurezza (forse eccessiva) annullo (*nop) e *nrighe
	    (*nop)=0;
	    (*nrig)=0;
	    return 0;
	  }
	if(sscanf(string,"%ld,%d,%d\n",numg,
		nop,nrig)!=3)
	  {
//Trovo i dati in un formato non valido: errore
            (*err)=-4; 
	    return 0;
	  }
      }
  }

int search_year(char *nomefile,char *cat,
		info_smaller *tabellina,int numfonti,
		long int numg_ini,long int numg_fin)
//Questa funzione utilizza search_util per calcolare
//entrate, uscite, guadagno netto tra una data iniziale (numg_ini)
//e una finale (numg_fin) relativamente a una categoria
//nel file specificato. I risultati vengono messi in tabella_risorse
//e il valore restituito mi segnala gli errori
  {
    if(!(fil=fopen(nomefile,"r")))
//Se il file e' nuovo non c'e' da aggiornare niente
//restituisco 1
      {
        sprintf(string1,"search_year: file doesn't exist");
	scrivilog(1,string1);
	return 1;
      }	 
    numg_fa=0;
    num_ops=0;
    nrighe=0;
    pa=0;
    pb=0;
    err=0;
    for(;;)
      {
//Cerco l'importo relativo alla prima operazione su quella categoria
	importo_f=search_util(fil,cat,1,0,&numg_fa,&num_ops,
				  &nrighe,&pa,&pb,&err);
	if(err<0)
	  {
//Si e' verificato un errore: esco          
	    fclose(fil);
            sprintf(string1,"search_util error n. %d",err);
	    scrivilog(1,string1);
	    return err;
	  }
//Se ottengo un importo nullo vuol dire che il file e' finito.
//Esco dal loop
	if(!importo_f)
          {
	    scrivilog(1,"file finito");
	    break;
          }
	if(numg_fa>numg_fin)
//Ho superato la data che mi interessa. Esco dal loop
	  {
	    scrivilog(1,"giorno superato");
	    break;
          }
//Se sono qui ho trovato qualcosa prima della data finale
	if(numg_fa>=numg_ini)
	  {
//Il qualcosa che ho trovato e' nell'arco temporale che mi
//interessa
	    if(importo_f>0)
	      {
//Se era un'entrata devo fare riferimento a pb
	        pp=pb;
	        plink=pa;
	      }
	    else
	      {
//Se era un'uscita devo fare riferimento a pa
	        pp=pa;
	        plink=pb;
	      }
	    tabellina[numfonti].gain=
	    tabellina[numfonti].gain+
            importo_f;
	    if((importo_f>=0)&&(!plink))
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
              tabellina[numfonti].entrate=
	      tabellina[numfonti].entrate+
              importo_f;
	    if((importo_f<0)&&(!plink))
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
              tabellina[numfonti].uscite=
	      tabellina[numfonti].uscite-
              importo_f;
	    j=0;
	    do
	      {
	        if(tabellina[j].posiz==pp)
		  {
		    tabellina[j].gain=
		    tabellina[j].gain
		    +importo_f;
		    if(importo_f>=0)
		      {
		        tabellina[j].entrate=
		        tabellina[j].entrate+
                        importo_f;
		      }
		    else
		      {
			tabellina[j].uscite=
		        tabellina[j].uscite-
                        importo_f;
		      }
                    break;
		  }
                j++;
	      }
            while(j<numfonti);
	  }
      }
//Chiudo il file e ritorno correttamente
    fclose(fil);
    return 0;
  }

void ordinanum(PTListGiov Target,PTListGiov L1,
		PTListGiov L2,PTListGiov RIEPILO,
		PTListGiov RIEPRIS,char cresc)
//Questa funzione ordina le listbox contenenti numeri in
//ordine decrescente (cresc=0) o crescente (cresc=1)
//sincronizzando allo stesso tempo le altre listbox
  {
    int numitem,k;
//Prima calcolo quanti oggetti ci sono
    numitem=Target->GetCount();
    for(i=1;i<numitem;i++)
//Estraggo una stringa alla volta e la inserisco ordinata
//(La prima stringa non c'e' da estrarla)
      {
	Target->GetString(string,i);
	Target->DeleteString(i);
	sscanf(string,"%lf",&importo);
	for(j=0;j<i;j++)
	  {
	    Target->GetString(string1,j);
	    sscanf(string1,"%lf",&importo_f);
	    if(
	      (cresc && (importo<importo_f))
	      ||
	      ((!cresc) && (importo>importo_f))
	      )
	      break;
	  }
	Target->InsertString(string,j);
	RIEPILO->GetString(string,i);
	RIEPILO->DeleteString(i);
	RIEPILO->InsertString(string,j);
	RIEPRIS->GetString(string,i);
	RIEPRIS->DeleteString(i);
	RIEPRIS->InsertString(string,j);
	L1->GetString(string,i);
	L1->DeleteString(i);
	L1->InsertString(string,j);
	L2->GetString(string,i);
	L2->DeleteString(i);
	L2->InsertString(string,j);
      }
  }

void ordinalet(PTListGiov Target,PTListGiov L1,
		PTListGiov RIEP1,PTListGiov RIEP2,
		PTListGiov RIEP3,char cresc)
//Questa funzione ordina le listbox contenenti testo in
//ordine decrescente (cresc=0) o crescente (cresc=1)
//sincronizzando allo stesso tempo le altre listbox
  {
    int numitem,k;
//Prima calcolo quanti oggetti ci sono
    numitem=Target->GetCount();
    for(i=1;i<numitem;i++)
//Estraggo una stringa alla volta e la inserisco ordinata
//(La prima stringa non c'e' da estrarla)
      {
	Target->GetString(string,i);
	Target->DeleteString(i);
	for(j=0;j<i;j++)
	  {
	    Target->GetString(string1,j);
	    if(
	      (cresc && (strcmp(string,string1)<0))
	      ||
	      ((!cresc) && (strcmp(string,string1)>0))
	      )
	      break;
	  }
	Target->InsertString(string,j);
	L1->GetString(string,i);
	L1->DeleteString(i);
	L1->InsertString(string,j);
	RIEP1->GetString(string,i);
	RIEP1->DeleteString(i);
	RIEP1->InsertString(string,j);
	RIEP2->GetString(string,i);
	RIEP2->DeleteString(i);
	RIEP2->InsertString(string,j);
	RIEP3->GetString(string,i);
	RIEP3->DeleteString(i);
	RIEP3->InsertString(string,j);
      }
  }


int aggiornasaldi(long int num_g,int numfonti,
		  char *nome_file,info_risorse *tabella_risorse)
//Scorre il file dei dati per intero
//calcola i nuovi saldi
//aggiorna il file basedata.dat
  {
    int g,m,a,pp,plink,anno,deltananni,err;
    long int pippo,numg_inimese,
	     numg_finmese;
    double deltasaldo;

    pippo=num_g/100;
    anno=pippo/100;
    a=anno;
    m=pippo%100;
//Questo mi serve per capire l'inizio del mese
    numg_inimese=(long int)a*10000+m*100+1;
//Questo mi serve per capire l'inizio del mese successivo
    if(m==12)
      {
	m=1;
        a=anno+1;
      }
    else
      m++;
    numg_finmese=(long int)a*10000+m*100+1;

//inizializzo i valori dei dati globali da calcolare
//nell'elemento numfonti+1_esimo del vettore
//tabella_risorse
    tabella_risorse[numfonti].saldoinanno=0;
    tabella_risorse[numfonti].saldoinimese=0;
    tabella_risorse[numfonti].saldooggi=0;
    tabella_risorse[numfonti].saldofinmese=0;
    tabella_risorse[numfonti].saldofinanno=0;

    tabella_risorse[numfonti].entrateinianno=0;
    tabella_risorse[numfonti].entrateinimese=0;
    tabella_risorse[numfonti].entrateoggi=0;
    tabella_risorse[numfonti].entratefinmese=0;
    tabella_risorse[numfonti].entratefinanno=0;

    tabella_risorse[numfonti].usciteinianno=0;
    tabella_risorse[numfonti].usciteinimese=0;
    tabella_risorse[numfonti].usciteoggi=0;
    tabella_risorse[numfonti].uscitefinmese=0;
    tabella_risorse[numfonti].uscitefinanno=0;

    tabella_risorse[numfonti].gaininianno=0;
    tabella_risorse[numfonti].gaininimese=0;
    tabella_risorse[numfonti].gainoggi=0;
    tabella_risorse[numfonti].gainfinmese=0;
    tabella_risorse[numfonti].gainfinanno=0;

    for(i=0;i<numfonti;i++)
      {
//inizializzo i valori dei dati da calcolare
	tabella_risorse[i].saldoinimese=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldooggi=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldofinmese=
	tabella_risorse[i].saldoinanno;
	tabella_risorse[i].saldofinanno=
	tabella_risorse[i].saldoinanno;

	tabella_risorse[numfonti].saldoinanno=
	tabella_risorse[numfonti].saldoinanno+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldoinimese=
	tabella_risorse[numfonti].saldoinimese+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldooggi=
	tabella_risorse[numfonti].saldooggi+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldofinmese=
	tabella_risorse[numfonti].saldofinmese+
	tabella_risorse[i].saldoinanno;
	tabella_risorse[numfonti].saldofinanno=
	tabella_risorse[numfonti].saldofinanno+
	tabella_risorse[i].saldoinanno;


	tabella_risorse[i].entrateinianno=0;
	tabella_risorse[i].entrateinimese=0;
	tabella_risorse[i].entrateoggi=0;
	tabella_risorse[i].entratefinmese=0;
	tabella_risorse[i].entratefinanno=0;

	tabella_risorse[i].usciteinianno=0;
	tabella_risorse[i].usciteinimese=0;
	tabella_risorse[i].usciteoggi=0;
	tabella_risorse[i].uscitefinmese=0;
	tabella_risorse[i].uscitefinanno=0;

	tabella_risorse[i].gaininianno=0;
	tabella_risorse[i].gaininimese=0;
	tabella_risorse[i].gainoggi=0;
	tabella_risorse[i].gainfinmese=0;
	tabella_risorse[i].gainfinanno=0;
      }
    if(!(fil=fopen(nome_file,"r")))
//Se il file e' nuovo non c'e' da aggiornare niente
//restituisco 1
      return 1;
    numg_fa=0;
    num_ops=0;
    nrighe=0;
    pa=0;
    pb=0;
    err=0;
    for(;;)
      {
//Cerco l'importo relativo alla prima operazione disponibile
	importo_f=search_util(fil,"",1,1,&numg_fa,&num_ops,
				  &nrighe,&pa,&pb,&err);
	if(err<0)
	  {
//Si e' verificato un errore: esco          
	    fclose(fil);
	    return -1;
	  }
	if(!importo_f)
//Visto che non avevo criteri di ricerca, se ottengo un
//importo nullo vuol dire che il file e' finito.
//Lo chiudo e esco dal file
          {
            fclose(fil);
	    break;
          }  
	if(importo_f>0)
	  {
//Se era un'entrata devo fare riferimento a pb
	    pp=pb;
	    plink=pa;
	  }
	else
	  {
//Se era un'uscita devo fare riferimento a pa
	    pp=pa;
	    plink=pb;
	  }
	j=0;
	do
	  {
//Cerco il nome della risorsa associata con pp
//scorrendo tabella_risorse[].posiz
	    if(tabella_risorse[j].posiz==pp)
              {
//Trovato. Aggiorno i dati della risorsa
//Da aggiornare sempre:
//Saldo, guadagno, entrate, uscite
//da inizio anno a fine anno
	        tabella_risorse[j].saldofinanno=
	        tabella_risorse[j].saldofinanno+
	        importo_f;
	        tabella_risorse[j].gainfinanno=
	        tabella_risorse[j].gainfinanno+
	        importo_f;
//Anche se non controllo niente, solo se e' una "pura" entrata
//o uscita (niente link) avro' un effetto sul dato globale:
//infatti se c'e' un link quello che e' entrato poi
//esce da qualche altra parte (a meno che non siano anni
//diversi)
	        tabella_risorse[numfonti].saldofinanno=
	        tabella_risorse[numfonti].saldofinanno+
	        importo_f;
	        tabella_risorse[numfonti].gainfinanno=
	        tabella_risorse[numfonti].gainfinanno+
	        importo_f;
	        if(importo_f>=0)
                  {
	            tabella_risorse[j].entratefinanno=
		    tabella_risorse[j].entratefinanno+
		    importo_f;
		    if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
		      tabella_risorse[numfonti].entratefinanno=
		      tabella_risorse[numfonti].entratefinanno+
                      importo_f;
                  }
	        else
                  {
		    tabella_risorse[j].uscitefinanno=
		    tabella_risorse[j].uscitefinanno-
		    importo_f;
		    if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
		      tabella_risorse[numfonti].uscitefinanno=
		      tabella_risorse[numfonti].uscitefinanno-
                      importo_f;
		  }
		if(numg_fa<numg_inimese)
		  {
//Caso specifico per: saldo inizio mese
		    tabella_risorse[j].saldoinimese=
		    tabella_risorse[j].saldoinimese+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].saldoinimese=
		    tabella_risorse[numfonti].saldoinimese+
		    importo_f;
		  }
		if(numg_fa<numg_finmese)
		  {
//Caso specifico per: saldo fine mese
		    tabella_risorse[j].saldofinmese=
		    tabella_risorse[j].saldofinmese+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].saldofinmese=
		    tabella_risorse[numfonti].saldofinmese+
		    importo_f;
		  }
		if(numg_fa<=num_g)
		  {
//Caso specifico per: saldo odierno,
//guadagno, entrate, uscite da inizio anno a oggi
		    tabella_risorse[j].saldooggi=
		    tabella_risorse[j].saldooggi+
		    importo_f;
		    tabella_risorse[j].gaininianno=
		    tabella_risorse[j].gaininianno+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].saldooggi=
		    tabella_risorse[numfonti].saldooggi+
		    importo_f;
		    tabella_risorse[numfonti].gaininianno=
		    tabella_risorse[numfonti].gaininianno+
		    importo_f;
		    if(importo_f>=0)
                      {
		        tabella_risorse[j].entrateinianno=
		        tabella_risorse[j].entrateinianno+
			importo_f;
			if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			  tabella_risorse[numfonti].entrateinianno=
			  tabella_risorse[numfonti].entrateinianno+
                          importo_f;
		      }
		    else
                      {
		        tabella_risorse[j].usciteinianno=
			tabella_risorse[j].usciteinianno-
			importo_f;
			if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			  tabella_risorse[numfonti].usciteinianno=
			  tabella_risorse[numfonti].usciteinianno-
			  importo_f;
		      }
		  }
		if(numg_fa==num_g)
		  {
//Caso specifico per: guadagno, entrate, uscite di oggi
		    tabella_risorse[j].gainoggi=
		    tabella_risorse[j].gainoggi+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].gainoggi=
		    tabella_risorse[numfonti].gainoggi
		    +importo_f;
		    if(importo_f>=0)
		      {
		        tabella_risorse[j].entrateoggi=
		        tabella_risorse[j].entrateoggi+
		        importo_f;
		        if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
		          tabella_risorse[numfonti].entrateoggi=
		          tabella_risorse[numfonti].entrateoggi+
                          importo_f;
		      }
		    else
                      {
		        tabella_risorse[j].usciteoggi=
			tabella_risorse[j].usciteoggi-
			importo_f;
			if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
			  tabella_risorse[numfonti].usciteoggi=
			  tabella_risorse[numfonti].usciteoggi-
                          importo_f;
		      }
		  }
		if((numg_fa>=numg_inimese)&&
		   (numg_fa<=num_g))
		  {
//Caso specifico per:
//guadagno, entrate, uscite da inizio mese a oggi
		    tabella_risorse[j].gaininimese=
		    tabella_risorse[j].gaininimese+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].gaininimese=
		    tabella_risorse[numfonti].gaininimese+
                    importo_f;
		    if(importo_f>=0)
                      {
		        tabella_risorse[j].entrateinimese=
			tabella_risorse[j].entrateinimese+
			importo_f;
			if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
			  tabella_risorse[numfonti].entrateinimese=
			  tabella_risorse[numfonti].entrateinimese+
                          importo_f;
		      }
		    else
                      {
		        tabella_risorse[j].usciteinimese=
		        tabella_risorse[j].usciteinimese-
		        importo_f;
		        if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
		          tabella_risorse[numfonti].usciteinimese=
		          tabella_risorse[numfonti].usciteinimese-
                          importo_f;
		      }
		  }
		if((numg_fa>=numg_inimese)&&
		   (numg_fa<numg_finmese))
		  {
//Caso specifico per:
//entrate, uscite, guadagno da inizio mese a fine mese
		    tabella_risorse[j].gainfinmese=
		    tabella_risorse[j].gainfinmese+
		    importo_f;
//Dato globale
		    tabella_risorse[numfonti].gainfinmese=
		    tabella_risorse[numfonti].gainfinmese+
		    importo_f;
		    if(importo_f>=0)
                      {
		        tabella_risorse[j].entratefinmese=
		        tabella_risorse[j].entratefinmese+
		        importo_f;
		        if(!plink)
//Solo se e' una "pura" entrata (niente link) la conteggio
//nelle entrate globali
		          tabella_risorse[numfonti].entratefinmese=
			  tabella_risorse[numfonti].entratefinmese+
                          importo_f;
		      }
		    else
                      {
		        tabella_risorse[j].uscitefinmese=
		        tabella_risorse[j].uscitefinmese-
		        importo_f;
		        if(!plink)
//Solo se e' una "pura" uscita (niente link) la conteggio
//nelle uscite globali
		          tabella_risorse[numfonti].uscitefinmese=
		          tabella_risorse[numfonti].uscitefinmese-
		          importo_f;
		      }
		  }
		break;
              }
            j++;
	  }
	while(j<numfonti);
      }
//Se sono qui: raggiunta la fine del file
//posso passare ad aggiornare basedata.dat
    if((k=controllabasedata())<0)
//In caso di errore
      return k;
    map=fopen("basedata.dat","r");
    tmp=fopen("temp","w");
    fgets(string,400,map);
//Posso copiare in tmp
    fputs(string,tmp);
    for(i=0;i<numfonti;i++)
      {
	fgets(string,400,map);
//Posso copiare in tmp
        fputs(string,tmp);
//Leggo il giorno di apertura (se c'e')
	fgets(string,400,map);
//Cerco le informazioni sul giorno di apertura
	fputs(string,tmp);
//Leggo il giorno di chiusura (se c'e')
	fgets(string,400,map);
//Cerco le informazioni sul giorno di chiusura
	fputs(string,tmp);
//Leggo quante righe di commento e quanti anni ci sono
	fgets(string,400,map);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
//Tutto OK, posso copiare in tmp con il numero
//corretto di anni
	switch(tabella_risorse[i].casi)
          {
	    case 0:
//c'e' l'anno in corso e l'anno dopo
//nanni rimane identico
	      deltananni=0;
	      break;
	    case 1:
//c'e' l'anno in corso ma non l'anno dopo
//dovro' aggiungere un anno a nanni
	      deltananni=1;
	      break;
	    case 2:
//non c'e' l'anno in corso ma c'e' l'anno dopo
//dovro' aggiungere un anno a nanni
	      deltananni=1;
	      break;
	    case 3:
//non c'e' l'anno in corso e non c'e' l'anno dopo
//dovro' aggiungere 2 anni a nanni
	      deltananni=2;
	      break;
	    default:
//Valore sbagliato (non dovrebbe mai succedere)
	      fclose(map);
	      fclose(tmp);
	      return -13;
	  }
//Scrivo la nuova informazione su tmp
//Il valore vecchio di nanni mi servira'
//comunque nel seguito
	fprintf(tmp,"%d,%d\n",nrighe,
		nanni+deltananni);
//Copio le righe di commento
	for(j=0;j<nrighe;j++)
	  {
	    fgets(string,400,map);
//Posso copiare in tmp
            fputs(string,tmp);
	  }
//Adesso leggo tutti gli anni fino a quello di
//interesse. Al piu' mi fermero' al primo
//anno esistente dopo quello di interesse.
//Solo gli anni minori di quello di interesse
//verranno copiati inalterati in tmp
	j=0;
	while(j<nanni)
	  {
	    j++;
	    fgets(string,400,map);
	    sscanf(string,"%d,%lf\n",&a,&saldo);
	    if(a<anno)
	      fputs(string,tmp);
	    else
	      break;
//Notare che all'uscita da questo loop:
//se j<nanni  => a>=anno
	  }
//anno e anno+1 vanno sempre riscritti ex-novo
	fprintf(tmp,"%d,%.2lf\n",
	        anno,
	        tabella_risorse[i].saldoinanno);
	fprintf(tmp,"%d,%.2lf\n",
	        anno+1,
	        tabella_risorse[i].saldofinanno);
//deltasaldo serve per aggiornare il saldo
//degli anni seguenti a anno+1
//se ce ne sono
	deltasaldo=
	tabella_risorse[i].saldofinanno-
	tabella_risorse[i].saldofinannoold;
//Adesso devo decidere il valore giusto di j
//in maniera da finire a ricopiare tutti i saldi
//rimasti
	if(j<nanni)
	  {
//Solo se ci sono anni successivi
//mi pongo il problema di aggiornare j
	    if(a==anno)
              {
//Accade se non ho letto tutti
//gli anni e l'anno in questione
//era nella lista
//Vediamo se c'e' da incrementare j:
	        switch(tabella_risorse[i].casi)
                  {
	            case 0:
//c'e' l'anno in corso e l'anno dopo:
//leggo la riga per superarla
//e incremento j
		      fgets(string,400,map);
		      sscanf(string,"%d,%lf\n",&a,&saldo);
		      if(a!=anno+1)
			{
//Ci doveva essere anno+1 ma non c'e': errore
			  fclose(map);
	                  fclose(tmp);
			  return -19;
			}
		      j++;
	              break;
	            case 1:
//c'e' l'anno in corso ma non l'anno dopo
//non incremento j
		      break;
		    default:
//Tutti gli altri valori sono sbagliati
//in questo contesto (non dovrebbe mai succedere)
		      fclose(map);
	              fclose(tmp);
		      return -20;
		  }
	      }
	  }
	if(a>anno+1)
	  {
//Se accade => ho letto tutti
//gli anni oppure no, l'anno in questione
//non era nella lista e non c'e' anno+1
//in questo caso copio su temp quello
//che avevo gia' letto
	    fputs(string,tmp);
	  }
//Ora riprendo a copiare fino alla fine
	while(j<nanni)
	  {
	    j++;
	    fgets(string,400,map);
	    sscanf(string,"%d,%lf\n",&a,&saldo);
//Posso aggiornare tmp
            fprintf(tmp,"%d,%.2lf\n",a,
	    saldo+deltasaldo);
	  }
      }
//Ho finito la copia: chiudo e sostituisco
//temp a basedata.dat
    fclose(map);
    fclose(tmp);
    remove("basedata.dat");
    rename("temp","basedata.dat");
    return 1;
  }

int caricasaldi(long int num_g,info_risorse *tabella_risorse)
//Scorre il file basedata.dat per avere
//i saldi di partenza e le informazioni
//fondamentali ad essi correlate
  {
    int g,m,a,anno;
    long int pippo;

    if((k=controllabasedata())<0)
//In caso di errore
      return k;
//Se sono qui ho controllato l'integrita' del file. Adesso posso procedere
//senza fare controlli
    pippo=num_g/100;
    anno=pippo/100;
    map=fopen("basedata.dat","r");
    fgets(string,400,map);
    sscanf(string,"%d\n",&numfonti);
//Tutto OK, posso andare avanti
    for(i=0;i<numfonti;i++)
      {
	fgets(string,400,map);
	sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,
	   nomeconv);
//Leggo il giorno di apertura (se c'e')
	fgets(string,400,map);
//Leggo il giorno di chiusura (se c'e')
	fgets(string,400,map);
//Leggo quante righe di commento e quanti anni ci sono
	fgets(string,400,map);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
//Supero le righe di commento
	for(j=0;j<nrighe;j++)
	  fgets(string,400,map);
	j=0;
//Valore di default del saldo=0 se non trovo
//niente di utile in basedata.dat
	tabella_risorse[i].saldoinanno=0;
	tabella_risorse[i].saldofinannoold=0;
	tabella_risorse[i].casi=3;
	while(j<nanni)
	  {
	    j++;
	    fgets(string,400,map);
	    sscanf(string,"%d,%lf\n",&a,&saldo);
	    if(a<anno)
	      {
//Fintanto che non trovo anno:
//Valore di ....casi = 3 (per ora)
		tabella_risorse[i].casi=3;
//Valore del saldo fintanto che non trovo anno=
//saldo dell'anno letto (per ora)
		tabella_risorse[i].saldoinanno=saldo;
//Valore del saldo anno successivo =
//saldo dell'anno letto (per ora)
		tabella_risorse[i].saldofinannoold
		=saldo;
	      }
	    else if(a==anno)
	      {
//Accade se anno e' parte della lista
//Valore di ....casi = 1 (per ora)
                tabella_risorse[i].casi=1;
		tabella_risorse[i].saldoinanno=saldo;
//Valore del saldo anno successivo =
//saldo di quest'anno (per ora)
		tabella_risorse[i].saldofinannoold
		=saldo;
	      }
	    else if(a==anno+1)
	      {
//Accade se anno+1 e' parte della lista
                tabella_risorse[i].saldofinannoold
		=saldo;
		if(tabella_risorse[i].casi==1)
//Se esisteva anno => nuovo valore = 0
		  tabella_risorse[i].casi=0;
		else
//Se non esisteva anno => nuovo valore = 2
		  tabella_risorse[i].casi=2;
	      }
	  }
      }
    fclose(map);
    return 1;
  }


void mostrasaldi(PTComboGiov SELBANC,PTComboGiov SELDAT,
                 PTStatic RESULT,info_risorse *tabella_risorse)
//Mostra il dato numerico richiesto
//(saldo, entrate, ecc.)
//per la risorsa selezionata
  {
    if((index=SELBANC->GetSelIndex())<0)
      {
//Se la stringa non corrisponde a una risorsa valida
//metto la prima risorsa di default
	index=0;
	SELBANC->SetSelIndex(0);
      }
    if((k=SELDAT->GetSelIndex())<0)
      {
//Se la stringa non corrisponde a un dato valido
//metto il saldo a oggi di default
	k=2;
	SELDAT->SetSelIndex(2);
      }
    switch(k)
      {
	case 0:
          sprintf(string,"%.2lf",
		  tabella_risorse[index].saldoinanno);
	  break;
	case 1:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].saldoinimese);
	  break;
	case 2:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].saldooggi);
	  break;
	case 3:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].saldofinmese);
	  break;
	case 4:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].saldofinanno);
	  break;
	case 5:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].entrateinianno);
	  break;
	case 6:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].entrateinimese);
	  break;
	case 7:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].entrateoggi);
	  break;
	case 8:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].entratefinmese);
	  break;
	case 9:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].entratefinanno);
	  break;
	case 10:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].usciteinianno);
	  break;
	case 11:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].usciteinimese);
	  break;
	case 12:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].usciteoggi);
	  break;
	case 13:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].uscitefinmese);
	  break;
	case 14:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].uscitefinanno);
	  break;
	case 15:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].gaininianno);
	  break;
	case 16:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].gaininimese);
	  break;
	case 17:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].gainoggi);
	  break;
	case 18:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].gainfinmese);
	  break;
	case 19:
	  sprintf(string,"%.2lf",
		  tabella_risorse[index].gainfinanno);
	  break;
	default:
          ;
      }
    RESULT->SetText(string);
  }
