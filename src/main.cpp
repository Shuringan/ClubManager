#include<iostream>

#include "..\include\Event.h"
#include "..\include\globals.h"
#include "..\include\utils.h"
#include "..\include\Server.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Expected input file" << std::endl;
        return 1;
    }

    try
    {
        std::string input = readFileToString(argv[1]);

        Server server;

        server.startUp(input);

        server.processOutputLog();

        std::cout << server.getOutputLog();

        server.processTablesStats();
        std::cout << server.getTablesStats();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error in line: " << INPUT_LINES << " " << e.what() << std::endl;
        return 1;
    }

    return 0;
}