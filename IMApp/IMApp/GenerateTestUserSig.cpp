/*
* Module:   GenerateTestUserSig
*
* Function: 用于获取 TIMLogin 接口所必须的 UserSig，腾讯云使用 UserSig 进行安全校验，保护您的 IM 功能不被盗用
*/
#include "GenerateTestUserSig.h"
#include "./zlib/zlib.h"
#include<vector>
#include <time.h>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include <Windows.h>
#include <bcrypt.h>
#include "../common/Base.h"
#include "../common/httpclient.h"
using namespace std;
#pragma comment(lib, "bcrypt.lib")

GenerateTestUserSig* GenerateTestUserSig::_Instance=nullptr;

GenerateTestUserSig* GenerateTestUserSig::GetInstance()
{ 
    if (!_Instance)
    {
        _Instance=new GenerateTestUserSig();

    }
   
    return _Instance;
}

GenerateTestUserSig::GenerateTestUserSig()
{
    m_Login_status = 0;
    m_login_id = "";
}

GenerateTestUserSig::~GenerateTestUserSig()
{

}

//GenerateTestUserSig& GenerateTestUserSig::instance()
//{
//    static GenerateTestUserSig aInstance;
//    return aInstance;
//}

bool GenerateTestUserSig::SetLoginId(const char* loginId)
{
    m_login_id = loginId;
    return false;
}

bool GenerateTestUserSig::GetLoginId(char* strLoginId)
{
    strcpy(strLoginId, m_login_id.c_str());
	return false;
}

bool GenerateTestUserSig::SetLogPath(const char* lpPath)
{
  m_path = lpPath;
    return false;
}

bool GenerateTestUserSig::GetLogPath(char* lpPath)
{
  
    strcpy(lpPath, m_path.c_str());
    return false;
}

int GenerateTestUserSig::SetLoginStatus(int status)
{
    m_Login_status = status;
    return 0;
}

int GenerateTestUserSig::GetLoginStatus()
{
    return m_Login_status;
}



uint32_t GenerateTestUserSig::getSDKAppID() const
{
    return SDKAPPID;
}

std::string GenerateTestUserSig::genTestUserSig(const std::string& userId,
    const std::string& userPass)
{
    string reportJson="userId="+userId+"&pass="+ userPass;
    HttpClient httpclient(L"");
    std::wstring contentLength = format(L"Content-Length: %lu", reportJson.size());

    std::wstring url = L"http://www.xxxxxx.com/im/test.php";

    std::vector<std::wstring> headers;
    headers.push_back(L"Content-Type: application/x-www-form-urlencoded; charset=utf-8");
    headers.push_back(contentLength);

   
    std::string respData;
    DWORD ret = httpclient.http_post(url, headers, reportJson, respData);
   // LOGNF("jsonStr: %s, ret: %lu, respData: %s", Ansi2Wide(reportJson).c_str(), ret, UTF82Wide(respData).c_str());
    return respData;
}
