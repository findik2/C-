#include "stdafx.h"
#include "file.h"
#include "userio.h"

userio io;
bool file::met(string in) {
	char stat = in[0];
	char mode = in[1];
	if (mode == '=' || mode == '<' || mode == '>')in.erase(0, 2);
	if (mode == '=') {
		if (stat == 'H' && player.getHealth() == stoi(in)) return true;
		else if (stat == 'E' && player.getEnergy() == stoi(in)) return true;
	}else if (mode == '<') {
		if (stat == 'H' && player.getHealth() < stoi(in)) return true;
		else if (stat == 'E' && player.getEnergy() < stoi(in)) return true;
	}else if (mode == '>') {
		if (stat == 'H' && player.getHealth() > stoi(in)) return true;
		else if (stat == 'E' && player.getEnergy() > stoi(in)) return true;
	}else {
		if (player.has(in) == true)return true;
	}
	return false;
}
void file::set(string in) {
	char stat = in[0];
	char mode = in[1];
	in.erase(0, 2);
	if (mode == '+') {
		if (stat == 'H') player.heal(1);
		else if (stat == 'E') player.rest();
		else if (stat == 'I') player.add(in);
	}else if (mode == '=') {
		if (stat == 'H') player.heal(stoi(in));
		else if (stat == 'E') player.sleep();
	}else if (mode == '-') {
		if (stat == 'H') player.hit(stoi(in));
		else if (stat == 'E') player.move();
		else if (stat == 'I') player.drop(in);
	}
}
void file::update(string in) {
	stringstream ss;
	ss << "Chapter " << size(in) << " | Room " << in << endl;
	string title = ss.str();
	SetConsoleTitle(title.c_str());
	cout << "H: " << player.getHealth() << "/10    E:" << player.getEnergy() << "/10    STR:" << player.getStrength() << "    DEF:" << player.getArmour() << "    INV:" << player.inventory() << endl;
}
void file::chapter(int current) {
	system("cls");
	player.move();
	string room = to_string(current);
	string pretext, condition, parameter;
	vector<string> options, posttext, parameters;
	if (story[current].size() == 1) {
		if (story[current][0][0] == '/')
			room = story[current][0].substr(1, story[current][0].size());
	}else {
		for (auto &i : story[current]) {
			if (i.substr(0, 2) == "IF") {
				condition = i.substr(3, i.size());
				continue;
			}else if (i.substr(0, 4) == "ELSE") {
				tuple<string, string> splitIn = io.split(condition);
				if (met(get<0>(splitIn)) == true)i = get<1>(splitIn);
				else i = i.substr(4, i.size());
			}
			if (i.substr(0, 5) == "FIGHT") {
				i.erase(0, 6);
				for (unsigned int j = 0; j < i.size(); j++) {
					if (i[j] != ',')parameter += i[j];
					else {
						parameters.push_back(parameter);
						parameter = "";
					}
				}
			}else if (i[0] == '~') set(i.erase(0, 1));
			else if (i[0] == '#') player.die(i.erase(0, 1));
			else if (i[0] == '!') pretext = i.erase(0, 1);
			else if (i[0] == '-') options.push_back(i.erase(0, 1));
			else if (i[0] == '/') room = i.erase(0, 1);
			else posttext.push_back(i);
		}
		if (player.getHealth() > 0) {
			update(room);
			for (auto &i : posttext)io.print(i);
			if (options.size() > 0) {
				if (pretext.empty())player.add(io.itemChoice(options));
				else room += to_string(io.choice(pretext, options));
			}
			else if (parameters.size() > 0) {
				player.fight(human(parameters), false);
				if (player.getHealth() > 0) {
					for (string &i : parameters)
						if((i.find_first_not_of("0123456789") == string::npos) == false)
							player.add(i);
					room += "1";
					io.prompt("You won the fight!\n[Press enter to continue]", true);
				}
			}
			else io.prompt("[Press enter to continue]", true);
			chapter(stoi(room));
		}
	}
}
void file::play(string filename) {
	story.clear();
	system("cls");
	io.print("Opening file");
	ifstream book(filename);
	int count = 0;
	if (book.is_open()) {
		SetConsoleTitle(filename.c_str());
		system("cls");
		string line;
		int chap = 0;
		while (getline(book, line)) {
			int result = 0;
			stringstream myStream(line);
			if (myStream >> result) chap = result;
			else story[chap].push_back(line);
		}book.close();
		string name = io.input("You wake up in a forest with a concussion to the head. You can't remember your name. What would you like to call yourself?");
		player = human(name, 6, 9, 10, 10);
		chapter(0);
	}else io.prompt("Unable to open the book.");
}
