#pragma once
#include<string>
#include<map>
#include<set>
#include<queue>
#include<span>

#include "Event.h"
#include "globals.h"
#include "utils.h"
#include "Table.h"

class Server
{
public:
	Server();

	void startUp(const std::string& input);

	void processOutputLog();

	void processTablesStats();

	std::string getOutputLog();

	std::string getTablesStats();

private:
	int tables = -1, startTime = -1, endTime = -1, price = -1;
	int tablesOccupied = 0, revenue = 0;
	bool readyToProcessOutputLog = 0, readyToProcessTablesStats = 0;
	std::string inputLog, outputLog, tablesStatsLog;
	std::vector<Event> events;
	std::vector<int> tableOccupationTime;
	std::map<std::string, int> clientsToTable;
	std::queue<std::string> clientsQueue;
	std::vector<Table> tablesStats;
};