#include "Helper.h"

#include <unistd.h>
#include <sstream>

void Helper::output(FILE* file, std::stringstream &ss, Colours colour) {
	output(file, ss.str(), colour);
	ss.str("");
}

void Helper::output(FILE* file, std::string s, Colours colour) {
	std::string out = col2string(colour) + s + col2string(Colours::DEFAULT);
	if (write(fileno(file), out.c_str(), out.size())) {
	}
}

unsigned int Helper::doSleep(int min, int max) {
	unsigned int time = min + rand() % (max - min + 1);
	sleep(min + rand() % (max - min + 1));
	return time;
}

const std::string Helper::col2string(Colours c) {
	switch (c) {
		case Colours::DEFAULT:
			return "\033[0m";
			break;
		case Colours::WHITE:
			return "\033[30m";
			break;
		case Colours::D_RED:
			return "\033[31m";
			break;
		case Colours::RED:
			return "\033[1;31m";
			break;
		case Colours::BG_BLACK:
			return "\033[40m";
		case Colours::BG_RED:
			return "\033[41m";
			break;
		case Colours::D_GREEN:
			return "\033[32m";
			break;
		case Colours::GREEN:
			return "\033[1;32m";
			break;
		case Colours::BG_GREEN:
			return "\033[42m";
			break;
		case Colours::BROWN:
			return "\033[33m";
			break;
		case Colours::YELLOW:
			return "\033[1;33m";
			break;
		case Colours::BG_YELLOW:
			return "\033[43m";
			break;
		case Colours::D_BLUE:
			return "\033[34m";
			break;
		case Colours::BLUE:
			return "\033[1;34m";
			break;
		case Colours::BG_BLUE:
			return "\033[44m";
			break;
		case Colours::PURPLE:
			return "\033[35m";
			break;
		case Colours::PINK:
			return "\033[1;35m";
			break;
		case Colours::BG_PURPLE:
			return "\033[45m";
			break;
		case Colours::D_CYAN:
			return "\033[36m";
			break;
		case Colours::CYAN:
			return "\033[1;36m";
			break;
		case Colours::BG_CYAN:
			return "\033[46m";
			break;
		default:
			return "\033[0m";
	}
}
