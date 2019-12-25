#include "StdAfx.h"
#include "LoginWnd.h"
#include <process.h>
#include "json.h"
#include "TIMCloud.h"
#include "GenerateTestUserSig.h"
#include "../common/imutil.h"
#include "../common/Base.h"
#include "MsgBox.h"

CLoginWnd::CLoginWnd()
{
  
}

 CDuiString CLoginWnd::GetSkinFile()
{
    return _T("login.xml");
};

void CLoginWnd::Notify(TNotifyUI& msg)
{
  
    if (msg.sType == DUI_MSGTYPE_CLICK)
    {
        if (msg.pSender->GetName() == _T("login_btn"))
        { //登入
            OnLoginBtn();
        }
        if (msg.pSender->GetName() == _T("logout_btn"))
        {
            OnLogoutBtn();
        }
       
    }
    
    __super::Notify(msg);
}
DuiLib::CControlUI* CLoginWnd::CreateControl(LPCTSTR pstrClass)
{
    return nullptr;
}

LRESULT CLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}
LRESULT CLoginWnd::OnClose(UINT uMsg,
    WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
   
    return WindowImplBase::OnClose(uMsg, wParam, lParam, bHandled);
}


void CLoginWnd::InitSDK()
{ //初始化ImSDK
    //获取配置
    Json::Value json_user_config;
    json_user_config[kTIMUserConfigIsReadReceipt] = true;  // 开启已读回执
    Json::Value json_http_proxy;
    // json_http_proxy[kTIMHttpProxyInfoIp] = "http://127.0.0.1";
     //json_http_proxy[kTIMHttpProxyInfoPort] = 12639;


    Json::Value json_socks5_value;
    //json_socks5_value[kTIMSocks5ProxyInfoIp] = "111.222.333.444";
    //json_socks5_value[kTIMSocks5ProxyInfoPort] = 8888;
   // json_socks5_value[kTIMSocks5ProxyInfoUserName] = "";
   // json_socks5_value[kTIMSocks5ProxyInfoPassword] = "";
    Json::Value json_config;
    json_config[kTIMSetConfigUserConfig] = json_user_config;
    //json_config[kTIMSetConfigSocks5ProxyInfo] = json_socks5_value;
  //  json_config[kTIMSetConfigHttpProxyInfo] = json_http_proxy;
    json_config[kTIMSetConfigUserDefineData] = "1.3.4.5.6.7";

    TIMSetConfig(json_config.toStyledString().c_str(), NULL, this);


    CHAR szTPath[MAX_PATH] = { 0 };
    ::GetModuleFileNameA(NULL, szTPath, MAX_PATH); //获取工具目录
    int len = strlen(szTPath);
    for (int i = len; i >= 0; i--)
    {
        if ((szTPath[i] == '/') || (szTPath[i] == '\\'))
        {
            szTPath[i] = 0;
            break;
        }
    }

    // 初始化
    GenerateTestUserSig* instance = GenerateTestUserSig::GetInstance();
    instance->SetLogPath(szTPath);
    std::string path = szTPath;// Wide2UTF8(m_LogPath->GetText().GetData());
   // uint64_t sdk_app_id = atoi(sdkappid.c_str());
    uint64_t sdk_app_id = GenerateTestUserSig::GetInstance()->getSDKAppID();
    std::string json_init_cfg;

    Json::Value json_value_init;
    json_value_init[kTIMSdkConfigLogFilePath] = path;
    json_value_init[kTIMSdkConfigConfigFilePath] = path;

    TIMInit(sdk_app_id, json_value_init.toStyledString().c_str());
    // ChangeMainView(INITSDK_VIEW, LOGIN_VIEW);
    //Logf("InitSdk", kTIMLog_Info, "sdkappid:%lld Log&Cfg path:%s", sdk_app_id, path.c_str());
}

void CLoginWnd::InitWindow()
{
   // SetIcon(IDR_MAINFRAME);
    m_UserIdEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("userid_edit")));
    m_UserPassEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("userpass_edit")));
    InitSDK();
}


void CLoginWnd::MsgBox(std::string title, const char* fmt, ...)
{
    std::string tmp;
    va_list ap;
    va_start(ap, fmt);
    FmtV(tmp, fmt, ap);
    va_end(ap);

    CMsgBox msgbox;
    msgbox.DuiMessageBox(this, UTF82Wide(tmp).c_str(), UTF82Wide(title).c_str());
}


void CLoginWnd::OnLoginBtn()
{ //登入
    std::string userid = m_UserIdEdit->GetText().GetStringA();
    std::string userpass = m_UserPassEdit->GetText().GetStringA();
    std::string usersig = GenerateTestUserSig::GetInstance()->genTestUserSig(userid, userpass);
    GenerateTestUserSig* instance = GenerateTestUserSig::GetInstance();
    instance->SetLoginId(userid.c_str());
    //login_id = userid;
    TIMLogin(userid.c_str(), usersig.c_str(), [](int32_t code, const char* desc, const char* json_param, const void* user_data) {
        CLoginWnd* ths = (CLoginWnd*)user_data;
        GenerateTestUserSig* instance = GenerateTestUserSig::GetInstance();
        if (code != ERR_SUCC)
        { // 登入失败
            instance->SetLoginStatus(FALSE);
            ths->MsgBox("Login", "Failure!code:%d desc", code, desc);
          //  ths->Logf("Login", kTIMLog_Error, "Failure!code:%d desc", code, desc);
            return;
        }
        //登入成功
       //设置任何人加我需要确认
        Json::Value modify_item;
        modify_item[kTIMUserProfileItemNickName] = "change my nick name"; // 修改昵称
        modify_item[kTIMUserProfileItemGender] = kTIMGenderType_Female;  // 修改性别
        modify_item[kTIMUserProfileItemAddPermission] = kTIMProfileAddPermission_NeedConfirm;  // 修改添加好友权限

        //Json::Value json_user_profile_item_custom;
        //json_user_profile_item_custom[kTIMUserProfileCustemStringInfoKey] = "Str";  // 修改个人资料自定义字段 " Str " 的值
        //json_user_profile_item_custom[kTIMUserProfileCustemStringInfoValue] = "my define data";
        //modify_item[kTIMUserProfileItemCustomStringArray].append(json_user_profile_item_custom);
        int ret = TIMProfileModifySelfUserProfile(modify_item.toStyledString().c_str(), [](int32_t code, const char* desc, const char* json_params, const void* user_data)
        {
            CLoginWnd* ths = (CLoginWnd*)user_data;
            if (ERR_SUCC != code)
            {
                ths->MsgBox("change profile", "Failure!code:%d desc", code, desc);
                // 修改自己的个人资料失败
                return;
            }
        }, ths);
        instance->SetLoginStatus(TRUE);
        ::PostMessage(ths->m_hWnd, WM_CLOSE, 0, 0);
     
    }, this);

  //  Logf("Login", kTIMLog_Info, "User Id:%s Sig:%s", userid.c_str(), usersig.c_str());
  //  SetControlText(_T("cur_loginid_lbl"), UTF82Wide(userid).c_str());
    
}

void CLoginWnd::OnLogoutBtn()
{ //登出
   
}


