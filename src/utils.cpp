#include "..\include\utils.h"

std::vector<std::string> splitByNewLine(const std::string& input)
{
	std::vector<std::string> split = { "" };
	for (auto& ch : input)
	{
		if (ch != '\n') split.back() += ch;
		else split.push_back("");
	}
	while (!split.empty() && split.back() == "") split.pop_back();
	return split;
}

std::vector<Event> parseEvents(std::span<std::string> input, int table_max_id)
{
	std::vector<Event> events;
	int prev_time = -1;
	for (auto& event_now : input)
	{
		++INPUT_LINES;
		int time = parseTime(event_now.substr(0, 5));

		if (time < prev_time) throw std::runtime_error("Time not in ascending order.");
		prev_time = time;

		if (event_now.size() < 9 || event_now[5] != ' ' || !isDigit(event_now[6]) || event_now[7] != ' ') 
			throw std::runtime_error("Incorrect input format.");

		int event_id = event_now[6] - '0';

		std::string name = parseName(event_now);

		int table_id = -1;

		if (event_id == 2) table_id = parseInt(event_now, name.size() + 9);

		if (table_id > table_max_id) throw std::runtime_error("Table id exceeds max.");

		events.push_back(Event(time, event_id, name, table_id));
	}

	return events;
}


int parseTime(const std::string& time)
{
	if (time.size() != 5 || !isDigit(time[0]) || !isDigit(time[1]) || time[2] != ':' || !isDigit(time[3]) || !isDigit(time[4]))
		throw std::runtime_error("Wrong time format, expected HH:MM.");

	int hours = (time[0] - '0') * 10 + time[1] - '0';
	int minutes = (time[3] - '0') * 10 + time[4] - '0';
	if (hours >= 24) throw std::runtime_error("Amount of hours exceeds 23");
	if (minutes >= 60) throw std::runtime_error("Amount of minutes exceeds 59");

	return hours * 60 + minutes;
}

std::string formatTime(int time)
{
	if (time >= 1440) throw std::runtime_error("Amount of minutes exceeds 1440");

	std::string formatted_time = "";

	if (time / 60 < 10) formatted_time += "0";
	formatted_time += std::to_string(time / 60) + ":";

	if (time % 60 < 10) formatted_time += "0";
	formatted_time += std::to_string(time % 60);

	return  formatted_time;
}


bool isDigit(char ch)
{
	return '0' <= ch && ch <= '9';
}

bool isAcceptedLetter(char ch)
{
	return 'a' <= ch && ch <= 'z' || isDigit(ch) || ch == '_';
}

std::string parseName(const std::string& input)
{
	if (input.size() < 9) throw std::runtime_error("No name found in input.");

	std::string name = "";
	for (int i = 8; i < input.size(); ++i)
	{
		if (isAcceptedLetter(input[i])) name += input[i];
		else
		{
			if (input[i] == ' ') break;
			throw std::runtime_error("Incorrect client name.");
		}
	}

	return name;
}

int parseInt(const std::string& input, int start)
{
	std::string out = "";
	for (int i = start; i < input.size(); ++i)
	{
		if (!isDigit(input[i])) throw std::runtime_error("Found non digit in integer.");
		out += input[i];
	}
	if (out.size() > 8) throw std::runtime_error("Too large integer.");
	return std::stoi(out);
}

std::string readFileToString(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file) throw std::runtime_error("Unable to open file.");

	return std::string((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
}