WM_COMMAND        TMessage   SendMessage

    sprintf(string,"%d.map",a);
    if(!(pf=fopen(string,"r")))
      {
        numfonti=PROVEN->GetCount();
	pf=fopen(string,"w");
	fprintf(pf,"%d\n",numfonti);
	for(i=0;i<numfonti;i++)
	  {
	    PROVEN->GetString(string,i);
            fprintf(pf,"%d,%s,0\n",i+1,string);
          }
	fprintf(pf,"0\n");
        fclose(pf);
      }


		switch(tabella_risorse[i].casi)
                  {
	            case 0:
//c'e' l'anno in corso e l'anno dopo:
//incremento j
		      j++;
	              break;
	            case 1:
//c'e' l'anno in corso ma non l'anno dopo
//non incremento j
		      break;
	            case 2:
//non c'e' l'anno in corso ma c'e' l'anno dopo
	              deltananni=1;
	              break;
	            case 3:
//non c'e' l'anno in corso e non c'e' l'anno dopo
	              deltananni=2;
	              break;
	            default:
//Valore sbagliato (non dovrebbe mai succedere)
	              MessageBox(HWindow,"Anomalia nel file dati",
	              "Problema!",MB_ICONEXCLAMATION | MB_OK);
	              fclose(map);
	              fclose(tmp);
	              return -1;
		  }
