#include "parserCmd.h"
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(getuid() == 0)
        std::cout << "Logged in under root\n";
    else
    {
        std::cout << "Access Denied\n";
        return(1);
    }
    
    boost::asio::io_context ioc;
    Server s(ioc, ParserCmd::parse(argc, argv));
    ParserCmd p(&s);
    ioc.run();
    return 0;
}