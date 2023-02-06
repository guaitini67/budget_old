const unsigned int LBN_VSCROLL = LBN_SETFOCUS + 10;

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
};


_CLASSDEF(TListGiov)

class TListGiov:public TListBox
{
  public:
    TListGiov(PTWindowsObject AParent, int ResourceId,
	      PTModule AModule = NULL)
    :TListBox(AParent,ResourceId,AModule)
      {
      };

 virtual void WMVScroll(RTMessage Msg)
      =[WM_FIRST+WM_VSCROLL]
      {
	TWindow::WMVScroll(Msg);
	SendMessage(Parent->HWindow,WM_COMMAND,Attr.Id,
	MAKELONG(HWindow,LBN_VSCROLL));
      };

    int VerCur()
      {
	return GetScrollPos(HWindow,SB_VERT);
      };

    void SetVCur(int posy)
      {
	SetScrollPos(HWindow,SB_VERT,posy,TRUE);
      };

};