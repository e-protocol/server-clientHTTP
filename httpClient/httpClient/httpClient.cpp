#include "parserCmd.h"



int main()
{
	setlocale(LC_ALL, "Ru");
	asio::io_context ioc;
	std::shared_ptr<Client> cl = std::make_shared<Client>(ioc);
	ParserCmd parserCmd(cl.get());
	ioc.run();
	return 0;
}
