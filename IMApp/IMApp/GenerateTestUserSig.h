#pragma once

/*
 * Module:   GenerateTestUserSig
 *
 * Function: 用于生成测试用的 UserSig，UserSig 是腾讯云为其云服务设计的一种安全保护签名。
 *           其计算方法是对 SDKAppID、UserID 和 EXPIRETIME 进行加密，加密算法为 HMAC-SHA256。
 *
 * Attention: 请不要将如下代码发布到您的线上正式版本的 App 中，原因如下：
 *            
 *            本文件中的代码虽然能够正确计算出 UserSig，但仅适合快速调通 SDK 的基本功能，不适合线上产品，
 *            这是因为客户端代码中的 SECRETKEY 很容易被反编译逆向破解，尤其是 Web 端的代码被破解的难度几乎为零。
 *            一旦您的密钥泄露，攻击者就可以计算出正确的 UserSig 来盗用您的腾讯云流量。
 *            
 *            正确的做法是将 UserSig 的计算代码和加密密钥放在您的业务服务器上，然后由 App 按需向您的服务器获取实时算出的 UserSig。
 *            由于破解服务器的成本要高于破解客户端 App，所以服务器计算的方案能够更好地保护您的加密密钥。
 *
 * Reference：https://cloud.tencent.com/document/product/269/32688#Server
 */

#include <string>
#include <stdint.h>
using namespace std;
class GenerateTestUserSig
{
public:
  static  GenerateTestUserSig * _Instance;
 static GenerateTestUserSig* GetInstance();
private:
    GenerateTestUserSig();
    ~GenerateTestUserSig();

    const uint32_t SDKAPPID = 1400296827;
    string m_path;
    string m_login_id;
    int m_Login_status;
public:
    bool SetLoginId(const char* loginId);
    bool GetLoginId(char* strLoginId);
    bool SetLogPath(const char* lpPath);
    bool GetLogPath(char* lpPath);
    int SetLoginStatus(int status);
    int GetLoginStatus();
public:
   // static GenerateTestUserSig& instance();
    uint32_t getSDKAppID() const;
  
    std::string genTestUserSig(const std::string& userId,const std::string& userPass);
};
