#include "stdafx.h"
#include "human.h"
#include "userio.h"
userio humanio;

human::human() {
	maxEnergy = 10;
	maxHealth = 10;
	name = "Enemy";
	energy = 10;
	health = 10;
	strength = 1;
	armour = 0;
	accuracy = 0.9;
	dodge = 0.5;
}
human::human(string n, int e, int h, int em, int hm) {
	maxEnergy = em;
	maxHealth = hm;
	name = n;
	energy = e;
	health = h;
	strength = 1;
	armour = 0;
	accuracy = 0.9;
	dodge = 0.3;
}
human::human(vector<string> parameters) {
	maxEnergy = stoi(parameters.at(1));
	maxHealth = stoi(parameters.at(2));
	name = parameters.at(0);
	energy = stoi(parameters.at(1));
	health = stoi(parameters.at(2));
	strength = stoi(parameters.at(3));
	armour = stoi(parameters.at(4));
	accuracy = stod(parameters.at(5));
	dodge = stod(parameters.at(6));
	vector<string>(parameters.begin() + 7, parameters.end()).swap(parameters);
	backpack = parameters;
}

void human::move() { 
	energy--; 
	if (getEnergy() <= 0) {
		humanio.print("You collapsed from exhaustion.");
		if (inventory() != "Nothing")
			humanio.print("You were robbed while you were out.");
		exhausted();
		humanio.prompt("[Press enter to continue]", true);
		system("cls");
	}
}
void human::rest() {
	if (energy <= maxEnergy-3)energy += 3;
	else energy = maxEnergy;
}
void human::exhausted() {
	if (energy == 0) {
		backpack.clear();
		sleep();
	}
}
void human::sleep() { energy = maxHealth; }

void human::hit(int damage) { 
	health -= damage; 
	if (health <= 0) die();
}
void human::heal(int heal) { 
	if(getHealth()+heal<maxHealth)health += heal; 
	else health = maxHealth;
}
void human::die(string in) { 
	health = 0; 
	if (name!="Enemy") {
		if (in != "")humanio.print(in);
		humanio.prompt("\nGAME OVER\n[Press enter to return to the title screen]\n", true);
	}
}
void human::revive() { health = maxHealth; }

string human::inventory() {
	if (backpack.empty()) return "Nothing";
	else {
		string out;
		for (auto &i : backpack) out += ", "+i;
		return out.erase(0,1);
	}
}
bool human::has(string item) {
	for (auto &i : backpack)
		if (i.find(item) == 0)
			return true;
	return false;
}
void human::add(string item) {
	if (has(item) == false)backpack.push_back(item);
}
void human::drop(string item) {
	if(has(item)==true)
		backpack.erase(remove(backpack.begin(), backpack.end(), item), backpack.end());
}

void human::setStrength(int str) { strength = str; }
void human::setArmour(int def) { armour = def; }
void human::setAccuracy(double acc) { accuracy = acc; }
void human::setDodge(double ag) { dodge = ag; }

void human::fight(human person, bool turn) {
	bool playersTurn = turn;
	string append;
	int delay = 100;
	while (person.getHealth() > 0 && getHealth() > 0) {
		if (_kbhit()) {
			delay = 0;
			_getch();
		}
		double hitChance;
		if (playersTurn == true) hitChance = getAccuracy() * (1 - person.getDodge()) * 100;
		else hitChance = person.getAccuracy() * (1 - getDodge()) * 100;
		for (int i = 0; i < 6; i++) {
			string playerHealth = humanio.quantity(getHealth(), getMaxHealth());
			string enemyHealth = humanio.quantity(person.getHealth(), person.getMaxHealth());
			if (playersTurn == true) humanio.overwrite(playerHealth + humanio.pad(i, getName(), 5 - i) + person.getName() + enemyHealth);
			else humanio.overwrite(playerHealth + getName() + humanio.pad(5 - i, person.getName(), i) + enemyHealth);
			Sleep(delay);
		}
		if (rand() % 100 > hitChance) {
			stringstream ss;
			if (playersTurn == true)ss << "       "<< getName() << " missed " << person.getName() << "!";
			else ss << "       " << person.getName() << " missed " << getName() << "!";
			append = ss.str();
		}
		else {
			stringstream ss;
			if (playersTurn == true)ss << "       " << getName() << " hit "<< person.getName() << " dealing " << getStrength() << " damage!   ";
			else ss << "       " << person.getName() << " hit " << getName() << " dealing " << person.getStrength() << " damage!   ";
			append = ss.str();

			if (playersTurn == true)humanio.overwrite(humanio.quantity(getHealth(), getMaxHealth()) + humanio.pad(5, getName(), 0) + person.getName() + humanio.quantity(person.getHealth()- getStrength(), person.getMaxHealth()) + append);
			else humanio.overwrite(humanio.quantity(getHealth() - person.getStrength(), getMaxHealth()) + getName() + humanio.pad(0, person.getName(), 5) + humanio.quantity(person.getHealth(), person.getMaxHealth()) + append);

			if (playersTurn == true)person.hit(getStrength());
			else hit(person.getStrength());
		}
		if(person.getHealth() > 0 && getHealth() > 0)
			for (int i = 0; i < 6; i++) {
				string playerHealth = humanio.quantity(getHealth(), getMaxHealth());
				string enemyHealth = humanio.quantity(person.getHealth(), person.getMaxHealth());
				if (playersTurn == true) humanio.overwrite(playerHealth + humanio.pad(5-i, getName(),  i) + person.getName() + enemyHealth + append);
				else humanio.overwrite(playerHealth + getName() + humanio.pad(i, person.getName(), 5-i) + enemyHealth + append);
				Sleep(delay);
			}
		playersTurn = !playersTurn;
	}
	cout << endl;
}
