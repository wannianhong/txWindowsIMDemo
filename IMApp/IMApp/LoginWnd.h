#pragma once
#include "UIlib.h"
//////////////////////////////////////////////////////////////////////////
///
using namespace DuiLib;

class CLoginWnd : public WindowImplBase
{
public:
	static int LoginDlg(HWND hParent)
	{
		CLoginWnd* pWnd = new CLoginWnd();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		return pWnd->ShowModal();
	}
	//overwrite
	virtual LPCTSTR GetWindowClassName() const { return _T("IM_Login"); };
	virtual UINT GetClassStyle() const { return /*UI_CLASSSTYLE_FRAME |*/ CS_DBLCLKS; };
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
public: //cb
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	
public:
	CLoginWnd(void);
	void InitSDK();
	void MsgBox(std::string title, const char* fmt, ...);
protected:
	virtual CDuiString GetSkinFile();
public:
	CEditUI* m_UserIdEdit = nullptr;
	CEditUI* m_UserPassEdit = nullptr;
public:
	void OnLoginBtn();
	void OnLogoutBtn();
	
	virtual void InitWindow();

	
};
