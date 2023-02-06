
// ********************************* TGiovApp *******************************************

_CLASSDEF(TGiovApp)

class TGiovApp:public TApplication
  {
    public:
      TGiovApp(LPSTR AName,HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	       int nCmdShow);
      virtual void InitInstance();
      virtual void InitMainWindow();
      virtual BOOL ProcessAppMsg(LPMSG AMessage);
//      virtual BOOL ProcessAccels(LPMSG PMessage);
  };

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

void TGiovApp::InitInstance()
  {
    TApplication::InitInstance();
    HAccTable=LoadAccelerators(hInstance,"scorciatoie");
  }

BOOL TGiovApp::ProcessAppMsg(LPMSG Message)
  {
    return ProcessMDIAccels(Message) ||
    ProcessAccels(Message) ||
    ProcessDlgMsg(Message);
  }

