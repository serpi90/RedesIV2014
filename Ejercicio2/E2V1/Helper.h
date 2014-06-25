#ifndef Helper_H
#define	Helper_H

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

class Helper {
	public:
		enum class Colours {
			BG_BLACK,
			WHITE,
			D_RED,
			RED,
			BG_RED,
			D_GREEN,
			GREEN,
			BG_GREEN,
			BROWN,
			YELLOW,
			BG_YELLOW,
			D_BLUE,
			BLUE,
			BG_BLUE,
			PURPLE,
			PINK,
			BG_PURPLE,
			D_CYAN,
			CYAN,
			BG_CYAN,

		};
		Helper() {
			srand(time(NULL));
		}
		;
		static void output(FILE*, std::stringstream&, Colours = Colours::WHITE);
		static void output(FILE*, std::string, Colours = Colours::WHITE);
		static unsigned int doSleep(int min, int max);
	private:
		static std::string col2string(Colours);

};

#endif	/* Helper_H */

