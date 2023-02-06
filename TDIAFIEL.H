
// *********************** CLASSE TDiaPreliminare *******************************************

_CLASSDEF(TDiaPreliminare)

class TDiaPreliminare:public TDialog
  {
    public:
      
      double saldo;
      char *endp;
      int i,j,k,numfonti,anno,gg,mm,aa,posiz,
      newposiz,nrighe,nanni;
      char string[401],filemap[10],
      nomeconv[27],aperto[12],chiuso[12];
      FILE *map,*temp;
      PTEdit ESTESE;
      PTComboGiov GAPERTO,MAPERTO,AAPERTO,
      GCHIUSO,MCHIUSO,ACHIUSO,IDENTI,POSTODI,
      INIZIALE,INIANNO;

      TDiaPreliminare(PTWindowsObject AParent,LPSTR ResourceId,int an);
      virtual void SetupWindow();
      virtual LPSTR GetClassName();
      virtual void GetWindowClass(WNDCLASS& AWndClass);
      void aggiornaFile();
      void HandOK()=[ID_FIRST+ID_FOK];
      void HandInsert()=[ID_FIRST+ID_FINS];
      void HandMsgIniAnno(RTMessage Msg)=[ID_FIRST+ID_INIANNO];
      void HandMsgIniziale(RTMessage Msg)=[ID_FIRST+ID_INIZIALE];
      void HandSelident(RTMessage Msg)=[ID_FIRST+ID_IDENT];
      void HandCambia()=[ID_FIRST+ID_FCAMB];
      void HandCanc()=[ID_FIRST+ID_FCANC];
      void HandAnnul()=[ID_FIRST+ID_FANNUL];
      void HandScriviSaldo()=[ID_FIRST+ID_SCRISA];
  };

// ************************** FUNZIONI DI TDiaPreliminare **************************************

TDiaPreliminare::TDiaPreliminare(PTWindowsObject AParent,LPSTR ResourceId,int an)
  :TDialog(AParent,ResourceId)
  {
    INIZIALE=new TComboGiov(this,ID_INIZIALE,15,NULL);
    INIANNO=new TComboGiov(this,ID_INIANNO,5,NULL);
    GAPERTO=new TComboGiov(this,ID_GAPERTO,3,NULL);
    MAPERTO=new TComboGiov(this,ID_MAPERTO,3,NULL);
    AAPERTO=new TComboGiov(this,ID_AAPERTO,5,NULL);
    GCHIUSO=new TComboGiov(this,ID_GCHIUSO,3,NULL);
    MCHIUSO=new TComboGiov(this,ID_MCHIUSO,3,NULL);
    ACHIUSO=new TComboGiov(this,ID_ACHIUSO,5,NULL);
    ESTESE=new TEdit(this,ID_ESTESO,0,NULL);
    IDENTI=new TComboGiov(this,ID_IDENT,25,NULL);
    POSTODI=new TComboGiov(this,ID_POSTODI,25,NULL);
    anno=an;
  }

LPSTR TDiaPreliminare::GetClassName()
  {
    return "diafield";
  }

void TDiaPreliminare::GetWindowClass(WNDCLASS& AWndClass)
  {
    TDialog::GetWindowClass(AWndClass);
    AWndClass.hIcon=LoadIcon(GetApplication()->hInstance,"icona");
  }

void TDiaPreliminare::HandOK()
  {
    CloseWindow();
  }

void TDiaPreliminare::HandInsert()
  {
    if(!(map=fopen("basedata.dat","r")))
      {
        MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if(!(IDENTI->GetText(nomeconv,27)))
      {
        MessageBox(HWindow,"Manca il nome",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
        return;
      }
    if((IDENTI->FindExactString(nomeconv,-1))>=0)
      {
        MessageBox(HWindow,"Non si puo' dare un nome gia' esistente! Cambialo!",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        fclose(map);
        return;
      }
    temp=fopen("base.tmp","w");
    fgets(string,400,map);
    sscanf(string,"%d",&numfonti);
    fprintf(temp,"%d\n",numfonti+1);
    while(!feof(map))
      {
	fgets(string,400,map);
	if(!feof(map))
	  fprintf(temp,"%s",string);
      }
    fprintf(temp,"%d,%s\n",newposiz,nomeconv);
    i=0;
    if(GAPERTO->GetText(aperto,5))
      if((gg=atoi(aperto))>0)
        i++;
    if(MAPERTO->GetText(aperto,5))
      if((mm=atoi(aperto))>0)
        i++;
    if(AAPERTO->GetText(aperto,7))
      if((aa=atoi(aperto))>0)
	i++;
    INIANNO->GetText(string,7);
    if((j=atoi(string))<=0)
      {
        MessageBox(HWindow,"Inserire anno saldo!",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	fclose(temp);
	remove("base.dat");
        return;
      }
    if(i==3)
      {
        if(j<aa)
	  {
	    MessageBox(HWindow,"Anno saldo minore anno apertura!",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(temp);
	    remove("base.dat");
            return;
	  }
	fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
      }
    else
      fprintf(temp,"nil\n");
    i=0;
    if(GCHIUSO->GetText(chiuso,5))
      if((gg=atoi(chiuso))>0)
        i++;
    if(MCHIUSO->GetText(chiuso,5))
      if((mm=atoi(chiuso))>0)
        i++;
    if(ACHIUSO->GetText(chiuso,7))
      if((aa=atoi(chiuso))>0)
        i++;
    if(i==3)
      {
        if(j>aa)
	  {
	    MessageBox(HWindow,"Anno saldo maggiore anno chiusura!",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(temp);
	    remove("base.dat");
            return;
	  }
	fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
      }
    else
      fprintf(temp,"nil\n");
    INIZIALE->GetText(string,14);
    k=sscanf(string,"%lf%s",&saldo,nomeconv);
    if(k!=1)
      {
        MessageBox(HWindow,"Saldo inserito non e' un numero valido",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(temp);
        fclose(map);
        remove("base.tmp");
	return;
      }
    nanni=1;
    k=ESTESE->GetNumLines();
    if(ESTESE->GetLine(string,100,k-1))
      fprintf(temp,"%d,%d\n",k,nanni);          
    else
      fprintf(temp,"%d,%d\n",--k,nanni);
    for(i=0;i<k;i++)
      {
        if(ESTESE->GetLine(string,100,i))
	  fprintf(temp,"%s\n",string);
        else
	  fprintf(temp,"\n");
      }
    for(i=0;i<nanni;i++)
      {
        INIANNO->GetText(string,7);
	fprintf(temp,"%s,",string);
	INIZIALE->GetText(string,14);
	sscanf(string,"%lf",&saldo);
	fprintf(temp,"%.2lf\n",saldo);
      }
    fclose(temp);
    fclose(map);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    aggiornaFile();
    GAPERTO->Clear();
    MAPERTO->Clear();
    AAPERTO->Clear();
    GCHIUSO->Clear();
    MCHIUSO->Clear();
    ACHIUSO->Clear();
    ESTESE->Clear();
    INIZIALE->ClearList();
    INIANNO->ClearList();
  }

void TDiaPreliminare::HandCambia()
  {
    if(!(IDENTI->GetText(nomeconv,27)))
      {
        MessageBox(HWindow,"Manca il nome della risorsa!",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if(!(POSTODI->GetText(nomeconv,27)))
      {
	MessageBox(HWindow,"Specificare 'Questa risorsa'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if((k=POSTODI->FindExactString(nomeconv,-1))<0)
      {
	MessageBox(HWindow,"Il nome specificato in 'Questa risorsa' non corrisponde",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    IDENTI->GetText(string,27);
    if((i=IDENTI->FindExactString(string,-1))>=0)
      if(i!=k)
        {
	  MessageBox(HWindow,"Stai provando a dare nomi uguali a risorse diverse!",
	  "Problema!",MB_ICONEXCLAMATION | MB_OK);
	  return;
        }
    if(!(map=fopen("basedata.dat","r")))
      {
        MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    temp=fopen("base.tmp","w");
    fgets(string,400,map);
    fprintf(temp,"%s",string);
    for(i=0;i<k;i++)
      {
        fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	for(j=0;j<nrighe+nanni;j++)
	  {
	    fgets(string,400,map);
	    fprintf(temp,"%s",string);
	  }
      }
    fgets(string,400,map);
    sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,nomeconv);
    fgets(string,400,map);
    fgets(string,400,map);
    fgets(string,400,map);
    sscanf(string,"%d,%d\n",&nrighe,&nanni);
    for(j=0;j<nrighe+nanni;j++)
      fgets(string,400,map);
    IDENTI->GetText(nomeconv,27);
    fprintf(temp,"%d,%s\n",posiz,nomeconv);
    i=0;
    if(GAPERTO->GetText(aperto,5))
      if((gg=atoi(aperto))>0)
        i++;
    if(MAPERTO->GetText(aperto,5))
      if((mm=atoi(aperto))>0)
        i++;
    if(AAPERTO->GetText(aperto,7))
      if((aa=atoi(aperto))>0)
	i++;
    k=INIANNO->GetCount();
    INIANNO->GetText(string,7);
    j=atoi(string);
    if((j<=0)&&(k<0))
      {
        MessageBox(HWindow,"Inserire anno saldo!",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	fclose(map);
	fclose(temp);
	remove("base.dat");
	return;
      }
    if((i==3)&&(j>0))
      {
        if(j<aa)
	  {
	    MessageBox(HWindow,"Anno saldo minore anno apertura!",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(temp);
	    remove("base.dat");
            return;
	  }
	fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
      }
    else if(i==3)
      fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
    else
      fprintf(temp,"nil\n");
    i=0;
    if(GCHIUSO->GetText(chiuso,5))
      if((gg=atoi(chiuso))>0)
        i++;
    if(MCHIUSO->GetText(chiuso,5))
      if((mm=atoi(chiuso))>0)
        i++;
    if(ACHIUSO->GetText(chiuso,7))
      if((aa=atoi(chiuso))>0)
	i++;
    if((i==3)&&(j>0))
      {
        if(j>aa)
	  {
	    MessageBox(HWindow,"Anno saldo maggiore anno chiusura!",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(temp);
	    remove("base.dat");
            return;
	  }
	fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
      }
    else if(i==3)
      fprintf(temp,"%02d-%02d-%04d\n",gg,mm,aa);
    else
      fprintf(temp,"nil\n");
    if(j>0)
      {
        INIZIALE->GetText(string,14);
	k=sscanf(string,"%lf%s",&saldo,nomeconv);
        if(k!=1)
          {
            MessageBox(HWindow,"Saldo inserito non e' un numero valido",
	    "Problema!",MB_ICONEXCLAMATION | MB_OK);
	    fclose(map);
	    fclose(temp);
	    remove("base.dat");
	    return;
	  }
        INIANNO->GetText(string,7);
        if((k=INIANNO->FindExactString(string,-1))>=0)
          {
      	    INIANNO->DeleteString(k);
            INIANNO->AddString(string);
	    INIZIALE->DeleteString(k);
	    INIZIALE->GetText(string,14);
            INIZIALE->InsertString(string,k);
          }
        else
          {
	    k=INIANNO->AddString(string);
	    INIZIALE->GetText(string,14);
            INIZIALE->InsertString(string,k);
	  }
      }
    nanni=INIANNO->GetCount();
    k=ESTESE->GetNumLines();
    if(ESTESE->GetLine(string,100,k-1))
      fprintf(temp,"%d,%d\n",k,nanni);          
    else
      fprintf(temp,"%d,%d\n",--k,nanni);
    for(i=0;i<k;i++)
      {
        if(ESTESE->GetLine(string,100,i))
	  fprintf(temp,"%s\n",string);
	else
	  fprintf(temp,"\n");
      }
    for(i=0;i<nanni;i++)
      {
        INIANNO->GetString(string,i);
	fprintf(temp,"%s,",string);
	INIZIALE->GetString(string,i);
	fprintf(temp,"%s\n",string);
      }
    while(!feof(map))
      {
        fgets(string,400,map);
	if(!feof(map))
	  fprintf(temp,"%s",string);
      }
    fclose(map);
    fclose(temp);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    IDENTI->GetText(nomeconv,27);
    k=IDENTI->FindExactString(nomeconv,-1);
    aggiornaFile();
    GAPERTO->Clear();
    MAPERTO->Clear();
    AAPERTO->Clear();
    GCHIUSO->Clear();
    MCHIUSO->Clear();
    ACHIUSO->Clear();
    ESTESE->Clear();
    INIZIALE->Clear();
    INIANNO->Clear();
    INIZIALE->ClearList();
    INIANNO->ClearList(); 
  }

void TDiaPreliminare::HandCanc()
  {
    if(!(IDENTI->GetText(nomeconv,27)))
      {
	MessageBox(HWindow,"Inserire il nome",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if((k=IDENTI->FindExactString(nomeconv,-1))<0)
      {
        MessageBox(HWindow,"Il nome non corrisponde",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if(!(map=fopen("basedata.dat","r")))
      {
        MessageBox(HWindow,"Manca il file 'basedata.dat'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
        return;
      }
    if(MessageBox(HWindow,
    "Se hai gia' usato questa risorsa potresti avere problemi. Sei sicuro?",
    "Attenzione!",MB_ICONEXCLAMATION | MB_YESNO)==IDNO)
      {
        fclose(map);
        return;
      }
    temp=fopen("base.tmp","w");
    fgets(string,400,map);
    sscanf(string,"%d",&numfonti);
    fprintf(temp,"%d\n",--numfonti);
    for(i=0;i<k;i++)
      {
        fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	for(j=0;j<nrighe+nanni;j++)
	  {
	    fgets(string,400,map);
	    fprintf(temp,"%s",string);
	  }
      }
    fgets(string,400,map);
    fgets(string,400,map);
    fgets(string,400,map);
    fgets(string,400,map);
    sscanf(string,"%d,%d\n",&nrighe,&nanni);
    for(j=0;j<nrighe+nanni;j++)
      fgets(string,400,map);
    for(i=k;i<numfonti;i++)
      {
        fgets(string,400,map);
	fprintf(temp,"%s",string);
//	sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,nomeconv);
//	fprintf(temp,"%d,%s\n",--posiz,nomeconv);
	fgets(string,400,map);
        fprintf(temp,"%s",string);
	fgets(string,400,map);
        fprintf(temp,"%s",string);
	fgets(string,400,map);
	fprintf(temp,"%s",string);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	for(j=0;j<nrighe+nanni;j++)
	  {
	    fgets(string,400,map);
	    fprintf(temp,"%s",string);
	  }
      }
    fclose(map);
    fclose(temp);
    remove("basedata.dat");
    rename("base.tmp","basedata.dat");
    aggiornaFile();
    GAPERTO->Clear();
    MAPERTO->Clear();
    AAPERTO->Clear();
    GCHIUSO->Clear();
    MCHIUSO->Clear();
    ACHIUSO->Clear();
    ESTESE->Clear();
    INIZIALE->Clear();
    INIANNO->Clear();
    INIZIALE->ClearList();
    INIANNO->ClearList();
  }

void TDiaPreliminare::HandAnnul()
  {
    IDENTI->Clear();
    POSTODI->Clear();
    GAPERTO->Clear();
    MAPERTO->Clear();
    AAPERTO->Clear();
    GCHIUSO->Clear();
    MCHIUSO->Clear();
    ACHIUSO->Clear();
    ESTESE->Clear();
    INIZIALE->Clear();
    INIZIALE->ClearList();
    INIANNO->ClearList();
  }

void TDiaPreliminare::HandSelident(RTMessage Msg)
  {
    if(Msg.LP.Hi==CBN_SELCHANGE)
      {                                          
	k=IDENTI->GetSelIndex();
	if(!(map=fopen("basedata.dat","r")))
	  {
            return;
          }
	fgets(string,400,map);
	for(i=0;i<k;i++)
	  {
	    fgets(string,400,map);
	    fgets(string,400,map);
	    fgets(string,400,map);
	    fgets(string,400,map);
	    sscanf(string,"%d,%d\n",&nrighe,&nanni);
	    for(j=0;j<nrighe+nanni;j++)
	      fgets(string,400,map);
	  }
	GAPERTO->Clear();
	MAPERTO->Clear();
	AAPERTO->Clear();
	GCHIUSO->Clear();
	MCHIUSO->Clear();
	ACHIUSO->Clear();
	ESTESE->Clear();
	INIANNO->Clear();
	INIANNO->ClearList();
	INIZIALE->Clear();
	INIZIALE->ClearList();
	fgets(string,400,map);
	fgets(string,400,map);
	sscanf(string,"%s\n",aperto);
	if(!strcmp(aperto,"nil"))
	  {
	  }
        else
	  {
	    sscanf(aperto,"%02d-%02d-%04d\n",&gg,&mm,&aa);
	    sprintf(string,"%02d",gg);
	    GAPERTO->Insert(string);
	    sprintf(string,"%02d",mm);
	    MAPERTO->Insert(string);
	    sprintf(string,"%04d",aa);
	    AAPERTO->Insert(string);
	  }
	fgets(string,400,map);
	sscanf(string,"%s\n",chiuso);
	if(!strcmp(chiuso,"nil"))
          {
	  }
        else
	  {
	    sscanf(chiuso,"%02d-%02d-%04d\n",&gg,&mm,&aa);
            sprintf(string,"%02d",gg);
	    GCHIUSO->Insert(string);
	    sprintf(string,"%02d",mm);
	    MCHIUSO->Insert(string);
	    sprintf(string,"%04d",aa);
	    ACHIUSO->Insert(string);
	  }
	fgets(string,400,map);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	for(i=0;i<nrighe;i++)
	  {
	    fgets(string,400,map);
	    sscanf(string,"%[!-~ ¡-ÿ‘’]\n",string);//sintassi per non fermarsi di fronte agli spazi
	    strcat(string,"\r\n");       // '\r'=carriage return + '\n'=line feed
				      //insieme mandano a capo nel controllo di edit
	    ESTESE->Insert(string);           //insieme mandano a capo il controllo di edit
	  }
	for(j=0;j<nanni;j++)
	  {
	    fgets(string,400,map);
	    sscanf(string,"%d,%lf",&aa,&saldo);
	    sprintf(string,"%d",aa);
	    i=INIANNO->AddString(string);
	    sprintf(string,"%.2lf",saldo);
	    INIZIALE->InsertString(string,i);
	  }
	sprintf(string,"%d",anno);   
	INIANNO->Insert(string);
	if((i=INIANNO->FindExactString(string,-1))>=0)
	  {
	    INIZIALE->GetString(string,i);
            INIZIALE->Insert(string);
	  }
	else
	  {
	  }
	fclose(map);
      }
  }

void TDiaPreliminare::HandMsgIniAnno(RTMessage Msg)
  {
    switch(Msg.LP.Hi)
      {
	case CBN_SELCHANGE:
	  i=INIANNO->GetSelIndex();
	  INIANNO->GetString(string,i);
	  INIANNO->Insert(string);
	  INIZIALE->GetString(string,i);
          INIZIALE->Insert(string);
        default: ;
      }                                          
  }

void TDiaPreliminare::HandMsgIniziale(RTMessage Msg)
  {
    switch(Msg.LP.Hi)
      {
	case CBN_SELCHANGE:
	  i=INIZIALE->GetSelIndex();
	  INIANNO->GetString(string,i);
	  INIANNO->Insert(string);
	  INIZIALE->GetString(string,i);
          INIZIALE->Insert(string);
        default: ;
      }                                          
  }

void TDiaPreliminare::HandScriviSaldo()
  {
    TMessage Message;

    if(!(IDENTI->GetText(nomeconv,27)))
      {
        MessageBox(HWindow,"Manca il nome della risorsa",
	"Operazione Proibita!",MB_ICONEXCLAMATION | MB_OK);
	return;
      }
    if((IDENTI->FindExactString(nomeconv,-1))<0)
      {
	MessageBox(HWindow,"La voce di risorsa e' nuova. \
Usa prima 'Scrivi voce'",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return;
      }
    i=0;
    if(GAPERTO->GetText(aperto,5))
      if((gg=atoi(aperto))>0)
        i++;
    if(MAPERTO->GetText(aperto,5))
      if((mm=atoi(aperto))>0)
        i++;
    if(AAPERTO->GetText(aperto,7))
      if((aa=atoi(aperto))>0)
	i++;
    INIANNO->GetText(string,7);
    if((j=atoi(string))<=0)
      {
        MessageBox(HWindow,"Inserire anno saldo!",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return;
      }
    if(i==3)
      if(j<aa)
        {
	  MessageBox(HWindow,"Anno saldo minore anno apertura!",
	  "Problema!",MB_ICONEXCLAMATION | MB_OK);
	  return;
	}
    i=0;
    if(GCHIUSO->GetText(chiuso,5))
      if((gg=atoi(chiuso))>0)
        i++;
    if(MCHIUSO->GetText(chiuso,5))
      if((mm=atoi(chiuso))>0)
        i++;
    if(ACHIUSO->GetText(chiuso,7))
      if((aa=atoi(chiuso))>0)
        i++;
    if(i==3)
      if(j>aa)
        {
	  MessageBox(HWindow,"Anno saldo maggiore anno chiusura!",
	  "Problema!",MB_ICONEXCLAMATION | MB_OK);
	  return;
	}
    INIZIALE->GetText(string,14);
    k=sscanf(string,"%lf%s",&saldo,nomeconv);
    if(k!=1)
      {
	MessageBox(HWindow,"Saldo inserito non\
e' un numero valido",
	"Problema!",MB_ICONEXCLAMATION | MB_OK);
	return;
      }
    INIANNO->GetText(string,7);
    if((k=INIANNO->FindExactString(string,-1))>=0)
      {
//Se l'anno esisteva gia'
	INIANNO->DeleteString(k);
        INIANNO->AddString(string);
	INIZIALE->DeleteString(k);
	INIZIALE->GetText(string,14);
        INIZIALE->InsertString(string,k);
      }
    else
      {
	k=INIANNO->AddString(string);
	INIZIALE->GetText(string,14);
        INIZIALE->InsertString(string,k);
      }
    IDENTI->GetText(nomeconv,27);
    POSTODI->Insert(nomeconv);
    INIANNO->Clear();
    INIZIALE->Clear();
    HandCambia();
    IDENTI->SetSelIndex(k);
    Message.LP.Hi=CBN_SELCHANGE;
    HandSelident(Message);
  }

void TDiaPreliminare::SetupWindow()
  {
    TDialog::SetupWindow();
    aggiornaFile();
  }

void TDiaPreliminare::aggiornaFile()
  {
    newposiz=1;
    if(!(map=fopen("basedata.dat","r")))
      {
	map=fopen("basedata.dat","w");
	fprintf(map,"0\n");
	fclose(map);
        return;
      }
    IDENTI->ClearList();
    POSTODI->ClearList();
    INIANNO->ClearList();
    INIZIALE->ClearList();
    fgets(string,400,map);
    sscanf(string,"%d\n",&numfonti);
    for(i=0;i<numfonti;i++)
      {
	fgets(string,400,map);
	sscanf(string,"%d,%[!-~ ¡-ÿ‘’]\n",&posiz,nomeconv);
	fgets(string,400,map);
	fgets(string,400,map);
	fgets(string,400,map);
	sscanf(string,"%d,%d\n",&nrighe,&nanni);
	IDENTI->AddString(nomeconv);
	POSTODI->AddString(nomeconv);
	for(j=0;j<nrighe+nanni;j++)
	  fgets(string,400,map);
        newposiz=posiz+1;
      }
    fclose(map);
  }