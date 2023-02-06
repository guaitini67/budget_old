
_CLASSDEF(TComboGiov)

class TComboGiov:public TComboBox
{
  public:
    TComboGiov(PTWindowsObject AParent, int ResourceId, WORD ATextLen,
	      PTModule AModule = NULL)
    :TComboBox(AParent,ResourceId,ATextLen,AModule)
      {};

    void Insert(LPSTR AString)
      {
	SetWindowText(HWindow, AString);
      };
    void HandEnter(RTMessage Msg)
      {
	if ( (Msg.WP.Hi == 0) && (Msg.LP.Lo == 0x0D) )
		    MessageBox(HWindow,
	    "Hai premuto ENTER",
	    "Evviva!",MB_ICONEXCLAMATION | MB_OK);
      };
};

typedef    struct
        {
	  int posiz;
          char nomeconv[27];
	  long int aperto,chiuso;
	  long int saldoin;
	  long int saldooggi;
	  long int saldofin;
	  long int saldofinold;
	  char casi;
	} info_risorse;