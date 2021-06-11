
#include "logging.h"
#include <stdlib.h>

void func1()
{
     char *s = "hello world";
     *s = 'H';

}


int main(int argc, char* argv[])
{
    LogInit("./logs", "true", "TRACE");

    //打印文本
    TRACELOG("TRACELOG");
    DEBUGLOG("DEBUGLOG");
    INFOLOG("INFOLOG") ;
    WARNLOG("WARNLOG") ;
    ERRORLOG("ERRORLOG");
    CRITICALLOG("CRITICALLOG");

    //打印二进制 
    std::array<char, 80> buf1;

    std::vector<char> buf2;
    buf2.push_back('a');
    buf2.push_back('b');
    buf2.push_back('c');

    int len = 10;
    char * p = (char *) malloc(sizeof(char) * len);
    memset(p, 1, len);
    std::vector<char> buf3(p, p + len);

    TRACERAWDATALOG("hex print", buf3); 

    func1();


    return 0;
}
