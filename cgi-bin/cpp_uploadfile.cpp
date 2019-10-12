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
    Cgicc cgi;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<head>\n";
    cout << "<meta charset=\"UTF-8\">\n";
    cout << "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n";
    cout << "<title>CGI 中的文件上传</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    
    //获取要上传的文件列表
    const_file_iterator file = cgi.getFile("userfile");
    if(file!=cgi.getFiles().end()){
        //在cout中发送数据类型
        cout << HTTPContentHeader(file->getDataType());
        //在cout中写入内容
        file->writeToStream(cout);
    }
    cout << "<文件上传成功>\n"; 
    cout << "<br/>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}
