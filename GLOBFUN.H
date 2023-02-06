// ***************************************************************************************
// ************************ SEZIONE DICHIARAZIONE FUNZIONI *******************************
// ***************************************************************************************

int isbises(int);
void categorie(PTListBox,PTListBox);
void caricacampi(PTComboGiov,PTComboGiov,PTComboGiov);
int controllacampi(PTComboGiov,PTComboGiov,PTComboGiov);

// ***************************************************************************************
// ************************ SEZIONE DEFINIZIONE FUNZIONI *********************************
// ***************************************************************************************

int isbises(int anno)               // controlla se un anno e' bisestile
  {
    if(anno%4)                      // Se non e' divisibile per 4
      return FALSE;                 // non e' bisestile.
    else if(anno%100)               // Se lo e' e non e' la fine del secolo
      return TRUE;                  // e' bisestile.
    else if(anno%400)               // Se e' fine secolo ma non e' divisibile
      return FALSE;                 // per 400 non e' bisestile.
    else                            // Se e' fine secolo divisibile per 400
      return TRUE;                  // e' bisestile.
  }

void categorie(PTListBox Cin,PTListBox Cout)
  {
    int e,u,i;
    char cat[41];
    FILE *gf;

    e=Cin->GetCount();
    u=Cout->GetCount();
    gf=fopen("categ.dat","w");
    fprintf(gf,"%d,%d\n",e,u);
    for(i=0;i<e;i++)
      {
	Cin->GetString(cat,i);
	fprintf(gf,"%s\n",cat);
      }
    for(i=0;i<u;i++)
      {
	Cout->GetString(cat,i);
	fprintf(gf,"%s\n",cat);
      }
    fclose(gf);
  }


void caricacampi(PTComboGiov GGior,
PTComboGiov MMes,PTComboGiov AAnn)
//Controlla la validita' della data
//e nel caso di errore mette la data
//di oggi. Alla fine mette nelle liste
//i valori ammissibili: numero di giorni
//mesi da 1 a 12, e anno in corso +/-2
  {
    struct date tmpdate;
    char string[6];
    int i,a,m,g;

//Prendo la data di oggi
//nel caso mi servisse
    getdate(&tmpdate);
//Leggo cosa ho scritto nel campo anno
//e verifico che sia valido
    AAnn->GetText(string,5);
    if((a=atoi(string))<=0)
//Non e' valido sostituisco con l'anno di oggi
      {
	a=tmpdate.da_year;
      }
//Vedo se l'anno e' bisestile per determinare
//i giorni di febbraio
    if(isbises(a))
      month[1]=29;
    else
      month[1]=28;
//Leggo cosa ho scritto nel campo mese
//e verifico che sia valido
    MMes->GetText(string,3);
    if(((m=atoi(string))<=0)||(m>12))
//Non e' valido sostituisco col mese di oggi
      {
	m=tmpdate.da_mon;
      }
//Leggo cosa ho scritto nel campo giorno
//e verifico che sia valido
    GGior->GetText(string,3);
    if(((g=atoi(string))<=0)||(g>month[m-1]))
//Non e' valido
      if(tmpdate.da_day<=month[m-1])
//Se il giorno di oggi e' ammesso per il mese
//in questione metto il giorno di oggi
        {
	  g=tmpdate.da_day;
	}
      else
//Se non e' un numero ammesso metto
//l'ultimo del mese in questione
        {
          g=month[m-1];
	}
//Adesso pulisco le liste, inserisco
//la data e creo liste aggiornate
//per giorno mese e anno
    GGior->ClearList();
    MMes->ClearList();
    AAnn->ClearList();
    sprintf(string,"%02d",g);
    GGior->Insert(string);
    sprintf(string,"%02d",m);
    MMes->Insert(string);
    sprintf(string,"%04d",a);
    AAnn->Insert(string);
    for(i=1;i<=12;i++)
      {
        sprintf(string,"%02d",i);
	MMes->AddString(string);
      }
    for(i=1;i<=month[m-1];i++)
      {
        sprintf(string,"%02d",i);
	GGior->AddString(string);
      }
//Inserisco se possibile anno-2
    if(a>2)
      {
	sprintf(string,"%04d",a-2);
	AAnn->AddString(string);
      }
//Inserisco se possibile anno-1
    if(a>1)
      {
	sprintf(string,"%04d",a-1);
	AAnn->AddString(string);
      }
//Inserisco anno
    sprintf(string,"%04d",a);
    AAnn->AddString(string);
//Inserisco anno+1
    sprintf(string,"%04d",a+1);
    AAnn->AddString(string);
//Inserisco anno+2
    sprintf(string,"%04d",a+2);
    AAnn->AddString(string);
  }

int controllacampi(PTComboGiov GGior,
PTComboGiov MMes,PTComboGiov AAnn)
//Differenza rispetto a caricacampi:
//Questa fa solo il controllo sul
//formato della data. Se va bene
//carica le liste corrispondenti
//altrimenti esce con errore
  {
    char string[6];
    int i,a,m,g;

//Controllo che l'anno sia valido
    AAnn->GetText(string,5);
    if((a=atoi(string))<=0)
      return -1;
//Controllo se l'anno e' bisestile
    if(isbises(a))
      month[1]=29;
    else
      month[1]=28;
//Controllo che il mese sia valido
    MMes->GetText(string,3);
    if(((m=atoi(string))<=0)||(m>12))
      return -1;
//Controllo che il giorno sia valido
    GGior->GetText(string,3);
    if(((g=atoi(string))<=0)||(g>month[m-1]))
      return -1;
//Se tutto andava bene:
//Aggiorno le liste dei tre campi
    GGior->ClearList();
    MMes->ClearList();
    AAnn->ClearList();
    sprintf(string,"%02d",g);
    GGior->Insert(string);
    sprintf(string,"%02d",m);
    MMes->Insert(string);
    sprintf(string,"%04d",a);
    AAnn->Insert(string);
    for(i=1;i<=12;i++)
      {
        sprintf(string,"%02d",i);
	MMes->AddString(string);
      }
    for(i=1;i<=month[m-1];i++)
      {
        sprintf(string,"%02d",i);
	GGior->AddString(string);
      }
//Inserisco se possibile anno-2
    if(a>2)
      {
	sprintf(string,"%04d",a-2);
	AAnn->AddString(string);
      }
//Inserisco se possibile anno-1
    if(a>1)
      {
	sprintf(string,"%04d",a-1);
	AAnn->AddString(string);
      }
//Inserisco anno
    sprintf(string,"%04d",a);
    AAnn->AddString(string);
//Inserisco anno+1
    sprintf(string,"%04d",a+1);
    AAnn->AddString(string);
//Inserisco anno+2
    sprintf(string,"%04d",a+2);
    AAnn->AddString(string);
    return 0;
  }
