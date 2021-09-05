//不同系统配置文件（比如说windows 文件在Linux 环境编译之类的）
#include "atlconv.h"
void f(wchar_t *a)
{
    BSTR b=(BSTR)a;
    BSTR c = (BSTR)a;
#ifdef WIN32
    BSTR d;
    BSTR e = (BSTR)d;
}
#endif