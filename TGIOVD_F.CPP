#include <print.h>
#include <string.h>
#include <stdio.h>

int controllabasedata();
int controllafiledati(char *);

char string[400],mot_f[100],cat_f[100],
     nomeconv[27];
int i,j,k,num_ops,nrighe,nanni,pa,pb,
    numfonti,posiz,g,m,a;
long int numg_fa,numg_fb;
double importo_f,saldo;
FILE *map,*fil,*tmp;


int controllabasedata()
//Scorre il basedata.dat per intero
//per capire se e' formattato bene
  {
    if(!(map=fopen("basedata.dat","r")))
      {
//Non trovo basedata.dat: errore
	return -1;
      }
    tmp=fopen("temp","w");
    if(!fgets(string,100,map))
      {
//Non trovo niente in basedata.dat: errore
	fclose(map);
        fclose(tmp);
	return -2;
      }
    if(sscanf(string,"%d\n",&numfonti)!=1)
      {
//Formattazione non corretta: errore
	fclose(map);
	fclose(tmp);
	return -3;
      }
//Tutto OK, copio su temp
    fputs(string,tmp);
    for(i=0;i<numfonti;i++)
      {
	if(!fgets(string,100,map))
	  {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
	    return -4;
	  }
	if(sscanf(string,"%d,%[!-z ]\n",&posiz,
	   nomeconv)!=2)
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    return -5;
	  }
//Tutto OK, copio su temp
        fputs(string,tmp);
//Leggo il giorno di apertura (se c'e')
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
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
	    return -7;
	  }
//Leggo il giorno di chiusura (se c'e')
	if(!fgets(string,100,map))
          {
	    fclose(map);
	    fclose(tmp);
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
	    return -9;
	  }
//Leggo quante righe di commento e quanti anni ci sono
	if(!fgets(string,100,map))
          {
//Mi aspettavo di leggere una riga e non la trovo: errore
	    fclose(map);
	    fclose(tmp);
	    return -10;
	  }
	if(sscanf(string,"%d,%d\n",&nrighe,&nanni)!=2)
	  {
//Formattazione non corretta: errore
	    fclose(map);
	    fclose(tmp);
	    return -11;
          }
//Tutto OK, copio su temp
        fputs(string,tmp);
	for(j=0;j<nrighe;j++)
          {
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
	        fclose(map);
	        fclose(tmp);
	        return -12;
	      }
//Tutto OK, copio su temp
            fputs(string,tmp);
	  }
	for(j=0;j<nanni;j++)
          {
	    if(!fgets(string,100,map))
	      {
//Mi aspettavo di leggere una riga e non la trovo: errore
		fclose(map);
	        fclose(tmp);
		return -13;
	      }
	    if(sscanf(string,"%d,%lf\n",&a,&saldo)!=2)
	      {
//Formattazione non corretta: errore
		fclose(map);
	        fclose(tmp);
		return -14;
	      }
//Tutto OK, copio su temp
            fputs(string,tmp);
	  }
      }
//Tutto OK: ritorno corretto
    fclose(map);
    fclose(tmp);
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
    tmp=fopen("temp","w");
    while(fgets(string,400,fil))
      {
	if(sscanf(string,"%ld,%d,%d\n",&numg_fa,
	        &num_ops,&nrighe)!=3)
	  {
//Trovo i dati in un formato non valido: errore 
	    fclose(fil);
            fclose(tmp);
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
		return -3;
	      }
	    if(sscanf(string,
	      "%d,%lf,%[!-z ]\t,%[!-z ]\t,\
%d,%d,%ld,%d\n",
              &j,&importo_f,mot_f,cat_f,&pa,&pb,
	      &numg_fb,&k)!=8)
              {
//Trovo i dati in un formato non valido: errore
		fclose(fil);
		fclose(tmp);
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
    return 1;
  }
