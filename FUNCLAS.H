
// ************************** FUNZIONI DI TGiovApp **************************************

TGiovApp::TGiovApp(LPSTR AName,HINSTANCE hInstance,HINSTANCE hPrevInstance,
		   LPSTR lpCmdLine,int nCmdShow):
		   TApplication(AName,hInstance,hPrevInstance,lpCmdLine,nCmdShow)
  {
  }


void TGiovApp::InitMainWindow()
  {
    MainWindow=new TGiovDia(NULL,APPNAME);
  }

