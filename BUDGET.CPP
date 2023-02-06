#include <commdlg.h>
#include <print.h>
#include <owl.h>
#include <stdlib.h>
#include <string.h>
#include <dialog.h>
#include <bios.h>
#include <dos.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <combobox.h>
#include <edit.h>
#include <button.h>
#include <ctype.h>
#include <mmsystem.h>
#include <time.h>
#include <groupbox.h>
#include <radiobut.h>


extern int month[12];
extern char *gset[];




#define APPNAME "budget"

#include "giotype.h"
#include "budget.h"     //contiene le definizioni degli identificatori
#include "tcomgio.h"    //contiene TComboGiov
#include "globfun.h"    //contiene le funzioni globali
#include "tdiafield.h"   //contiene una o piu' classi
#include "tnotedia.h"   //contiene una o piu' classi
#include "tcmpdia.h"   //contiene una o piu' classi
#include "tgiovwin.h"   //contiene una o piu' classi
#include "toggidia.h"   //contiene una o piu' classi
#include "tgiovdia.h"   //contiene una o piu' classi
#include "tgiovapp.h"   //contiene una o piu' classi



// *************************************************************************************
// ***************************** MODULO PRINCIPALE *************************************
// *************************************************************************************


int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,
		   int nCmdShow)
    {
      TGiovApp GiovApp(APPNAME,
			hInstance,hPrevInstance,lpCmdLine,nCmdShow);
      if(hPrevInstance==NULL)
        {
          GiovApp.Run();
          return GiovApp.Status;
	}
      else
	return 0;
    }