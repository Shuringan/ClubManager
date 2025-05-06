#pragma once
#include<string>
#include<vector>
#include <stdexcept>
#include<fstream>
#include<span>


#include "Event.h"
#include "globals.h"

std::vector<Event> parseEvents(std::span<std::string> input, int table_max_id);
std::vector<std::string> splitByNewLine(const std::string& input);

int parseTime(const std::string& time);
std::string formatTime(int time);

bool isDigit(char ch);
bool isAcceptedLetter(char ch);
std::string parseName(const std::string& input);
int parseInt(const std::string& input, int start = 0);

std::string readFileToString(const std::string& filePath);