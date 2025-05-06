#include "..\include\Server.h"

Server::Server() {}

void Server::startUp(const std::string& input)
{
    inputLog = input;
    auto inputSplit = splitByNewLine(inputLog);

    tables = parseInt(inputSplit[0]);
    ++INPUT_LINES;

    if (inputSplit.size() < 2 || inputSplit[1].size() != 11 || inputSplit[1][5] != ' ')
        throw std::runtime_error("Incorrect time format.");

    startTime = parseTime(inputSplit[1].substr(0, 5));
    endTime = parseTime(inputSplit[1].substr(6, 5));
    ++INPUT_LINES;

    price = parseInt(inputSplit[2]);
    ++INPUT_LINES;

    events = parseEvents(std::span(inputSplit).subspan(3), tables);

    tablesOccupied = 0;
    revenue = 0;
    outputLog = "";
    tablesStatsLog = "";
    tableOccupationTime.assign(tables + 1, 0);
    clientsToTable.clear();
    clientsQueue = std::queue<std::string>();
    
    tablesStats.assign(tables + 1, Table());
    for (int i = 1; i <= tables; ++i) tablesStats[i].tableNumber = i;

    readyToProcessOutputLog = 1;
    readyToProcessTablesStats = 0;
}

void Server::processOutputLog()
{
    if (!readyToProcessOutputLog) throw std::runtime_error("Server processing before startup.");
    readyToProcessOutputLog = 0;

    outputLog += formatTime(startTime) + '\n';

    for (Event& event : events)
    {
        outputLog += formatTime(event.time) + " " +
            std::to_string(event.eventId) + " " +
            event.clientName;
        if (event.eventId == 2) outputLog += " " + std::to_string(event.tableId);
        outputLog += '\n';

        switch (event.eventId)
        {
        case 1:
            if (event.time < startTime || endTime < event.time)
            {
                outputLog += formatTime(event.time) + " 13 NotOpenYet\n";
                continue;
            }

            if (clientsToTable.find(event.clientName) != clientsToTable.end())
            {
                outputLog += formatTime(event.time) + " 13 YouShallNotPass\n";
                continue;
            }

            clientsToTable.insert({ event.clientName, 0 });
            break;
        case 2:
            if (clientsToTable.find(event.clientName) == clientsToTable.end())
            {
                outputLog += formatTime(event.time) + " 13 ClientUnknown\n";
                continue;
            }

            if (tableOccupationTime[event.tableId] != 0)
            {
                outputLog += formatTime(event.time) + " 13 PlaceIsBusy\n";
                continue;
            }

            ++tablesOccupied;
            tableOccupationTime[event.tableId] = event.time;
            clientsToTable[event.clientName] = event.tableId;
            break;
        case 3:
            if (tablesOccupied < tables)
            {
                outputLog += formatTime(event.time) + " 13 ICanWaitNoLonger!\n";
                continue;
            }
            
            if (clientsQueue.size() > tables)
            {
                outputLog += formatTime(event.time) + " 11 " + event.clientName + '\n';
                continue;
            }

            clientsQueue.push(event.clientName);
            break;
        case 4:
            if (clientsToTable.find(event.clientName) == clientsToTable.end())
            {
                outputLog += formatTime(event.time) + " 13 ClientUnknown\n";
                continue;
            }

            if (clientsToTable[event.clientName] != 0)
            {
                int tableId = clientsToTable[event.clientName];
                int timeOccupied = event.time - tableOccupationTime[tableId];
                int hours = timeOccupied / 60;
                hours += timeOccupied % 60 > 0;
                tablesStats[tableId].revenue += price * hours;
                tablesStats[tableId].time += timeOccupied;

                if (clientsQueue.empty())
                {
                    tableOccupationTime[tableId] = 0;
                    --tablesOccupied;
                }
                else
                {
                    outputLog += formatTime(event.time) + " 12 " + clientsQueue.front() +
                        " " + std::to_string(tableId) + '\n';
                    tableOccupationTime[tableId] = event.time;
                    clientsToTable[clientsQueue.front()] = tableId;
                    clientsQueue.pop();
                }
            }

            clientsToTable.erase(event.clientName);
            break;
        default:
            throw std::runtime_error("Unknown event id.");
        }
    }

    for (auto& client : clientsToTable)
    {
        outputLog += formatTime(endTime) + " 11 " + client.first + '\n';

        if (client.second != 0)
        {
            int tableId = client.second;
            int timeOccupied = endTime - tableOccupationTime[tableId];
            int hours = timeOccupied / 60;
            hours += timeOccupied % 60 > 0;
            tablesStats[tableId].revenue += price * hours;
            tablesStats[tableId].time += timeOccupied;
        }
    }

    outputLog += formatTime(endTime) + '\n';

    readyToProcessTablesStats = 1;
}

std::string Server::getOutputLog()
{
    return outputLog;
}

void Server::processTablesStats()
{
    if (!readyToProcessTablesStats) 
        throw std::runtime_error("Processing tables stats out of order.");
    readyToProcessTablesStats = 0;

    for (int i = 1; i <= tables; ++i)
    {
        tablesStatsLog += 
            "Table number: " + std::to_string(tablesStats[i].tableNumber) + ". " +
            "Revenue: " + std::to_string(tablesStats[i].revenue) + ". " +
            "Time occupied: " + formatTime(tablesStats[i].time) + ".\n";
    }
}

std::string Server::getTablesStats()
{
    return tablesStatsLog;
}