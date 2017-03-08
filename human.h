#ifndef HUMAN_H
#define HUMAN_H
class human {
	int maxEnergy;
	int maxHealth;
	string name;
	int energy;
	int health;
	int strength;
	int armour;
	double accuracy;
	double dodge;
	vector<string> backpack;
public:
	human();
	human(string,int,int,int,int);
	human(vector<string>);

	string getName() { return name; }
	int getEnergy() { return energy; }
	int getHealth() { return health; }
	int getMaxEnergy() { return maxEnergy; }
	int getMaxHealth() { return maxHealth; }
	int getStrength() { return strength; }
	int getArmour() { return armour; }
	double getAccuracy() { return accuracy; }
	double getDodge() { return dodge; }

	void move();
	void rest();
	void sleep();
	void exhausted();

	void hit(int);
	void heal(int);
	void die(string = "");
	void revive();

	string inventory();
	bool has(string);
	void add(string);
	void drop(string);

	void fight(human,bool);

	void setStrength(int);
	void setArmour(int);
	void setAccuracy(double);
	void setDodge(double);
};
#endif
