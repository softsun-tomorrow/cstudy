#include <iostream>

using namespace std;

int main()
{
    cout << "Set-Cookie:UserID=XYZ;\r\n";
    cout << "Set-Cookie:Password=XYZ123;\r\n";
    cout << "Set-Cookie:Domain=118.25.5.174:8001;\r\n";
    cout << "Set-Cookie:Path=/perl;\n";
    cout << "Content-type:text/html\r\n\r\n";

    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<meta charset=\"UTF-8\">\n";
    cout << "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n";
    cout << "<title>CGI中的Cookies</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";

    cout << "设置cookies" << endl;
    cout << "<br/>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}
