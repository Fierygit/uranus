#include <sstream>
#include "string.h"
#define BUFFER_SIZE  256
const char g_szHeader[] = "HTTP/1.1 200 OK\r\n"
     "Connection: Keey-Live\r\n"
     "Content-Type: text/xml\r\n\r\n";// 响应报文头解析
bool HttpRespHeaderPrase(const std::string& strHeader)
{
    int count=0;
if ( strHeader.empty() )
{
return false;
}
// 行缓冲区
char szLineBuf[BUFFER_SIZE] = {0};

// 通过流来提取报文头中内容
std::stringstream ssHeader(strHeader.c_str());

// 提取第一行内容
ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');

// 获取HTTP版本
char szHttpVersion[BUFFER_SIZE] = {0};
sscanf(szLineBuf, "HTTP/%[^ ]", szHttpVersion);

// 获取状态码
strtok(szLineBuf," ");
int iStatusCode = atoi(strtok(NULL, " "));

// 逐行提取余下的报文参数名值对
std::string strParamName;
std::string strParamValue;
for (;;)
{
// 提取一行内容
ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
// 跳过参数名之前的无效字符
// 有效起始地址
char* start = szLineBuf + count;

// 解析完毕
if ( 0 == strlen(start) )
{
break;
}
// 查找冒号出现的位置
char* p = strchr(start, ':');
if ( p - start > 0 )
{
strParamName.assign(start, p - start);
}
else
{
// 解析出错
return false;
}

// 跳过无效字符
count = strspn(p, ": ");
strParamValue.assign(p + count);
}

return true;
}
int main(int argc, char* argv[])
{
HttpRespHeaderPrase(std::string(g_szHeader));

return 0;
}