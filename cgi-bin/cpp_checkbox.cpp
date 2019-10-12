#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

int main()
{
    Cgicc formData;
    bool maths_flag, physics_flag;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<head>\n";
    cout << "<meta charset=\"UTF-8\">\n";
    cout << "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n";
    cout << "<title>向 CGI 程序传递复选框数据</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    
    maths_flag = formData.queryCheckbox("maths");
    if(maths_flag){
        cout << "Math Flag: ON" << endl;
    } else {
        cout << "Math Flag: OFF" << endl;
    }
    cout << "<br/>\n";

    
    physics_flag = formData.queryCheckbox("physics");
    if(physics_flag){
        cout << "Physics Flag: ON" << endl;
    } else {
        cout << "Physics Flag: OFF" << endl;
    }
    cout << "<br/>\n";

    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}
