#pragma once

#include<string>

struct Event
{
	int time, eventId;
	std::string clientName;
	int tableId = -1;

	Event(int time, int id, const std::string& name, int table = -1) :
		time(time), eventId(id), clientName(name), tableId(table) {}
};