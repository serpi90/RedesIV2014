#ifndef Helper_H
#define	Helper_H
#include <string>
#include <cstdlib>
#include <ctime>

class Helper {
	public:
		enum class Colours
			: char {
				NORMAL, WHITE, D_RED, RED, BG_RED, D_GREEN, GREEN, BG_GREEN, BROWN, YELLOW, BG_YELLOW, D_BLUE, BLUE, BG_BLUE, PURPLE, PINK, BG_PURPLE, D_CYAN, CYAN, BG_CYAN,

		};
		Helper() {
			srand(time(NULL));
		}
		;
		static void output(FILE*, std::stringstream&, Colours = Colours::NORMAL);
		static void output(FILE*, std::string, Colours = Colours::NORMAL);
		static unsigned int doSleep(int min, int max);
	private:
		static std::string col2string(Colours);

};

#endif	/* Helper_H */

