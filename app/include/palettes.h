#include <random>

const int NUM_COLORS = 8;
struct palette {
	TGAColor cols[NUM_COLORS];
	TGAColor getrandom() { return cols[rand() % NUM_COLORS]; };
};
std::vector<palette> palettes;

static TGAColor parsehex(std::string hex)
{
	int cols[3];
	if (hex[0] == '#') hex.erase(0, 1);
	for (int i = 0; i < 3; i++) {
		std::stringstream ss;
		char temp[2];
		hex.copy(temp, 2, i*2);
		ss << std::hex << temp;
		ss >> cols[i];
	}
	return TGAColor(cols[0], cols[1], cols[2], 255);
}

static void loadpalettes(std::string fname)
{
	std::ifstream in;
	in.open(fname, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	std::string col;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line);
		palette p;
		int i = 0;
		while (getline(iss, col, ',') && i < NUM_COLORS) {
			p.cols[i] = parsehex(col);
			i++;
		}
		palettes.push_back(p);
	}
}