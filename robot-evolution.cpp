//final.cpp - A program that simulates evolution in robots
//Joshua Aymett - CISP 400
//5/9/18
/*
  Robots with randomly generated genes that dictate behavior are run through a randomly generated map with obstacles and battery packs (extra life). The top scoring robots will reproduce and create a new generation with their genes and evolution will occur.
*/

//--------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

//--------------------------------------------------------------------------------------------------------------------------

using namespace std;


class Robot
{
private:
	string name; //Feature 2 - Robots have names
	int energy; //Robot energy level
	int location; //Robot's location
	int numMoves = 0; //Move counter
	char direction; //Direction facing
	string *genes; //Array of genes

public:
	friend ostream &operator<<(ostream& output, Robot& r); //Writes robot info to log
	friend bool operator<(const Robot &R1, const Robot &R2); //Returns true if left robot is worse than right robot

	//Constructors
	//Empty constructor for creating temp robot
	Robot()
	{

	}

	//Constructs robots from parents
	Robot(string rname, int rlocation, string *parent1, string *parent2, char rdirection)
	{
		//Arrays are used for random mutation
		char sensors[4] = {'0','1','2','9'};
		char directions[4] = {'N', 'S', 'E', 'W'};
		char  actions[4] = {'M', 'R', 'L', 'X'};

		//Initializing class values
		direction = rdirection;
		int random;
		genes = new string[24];
		name = rname;
		location = rlocation;
		energy = 5;

		//Randomly selecting genes from parents
		for(int x = 0; x < 24; x++)
		{
			random = rand()% 2 + 1;
			if(random == 1)
			{
				*(genes+x) = *(parent1 + x);
			}
			else if(random == 2)
			{
				*(genes + x) = *(parent2 + x);
			}
		}
		
		random = rand()% 100; //5% chance of mutation
		if(random < 5)
		{
			random = rand()%24; //Picks which gene to change
			int random2 = rand() % 9; //Picks which character to change
			if(random2 < 4)
			{
				genes[random][random2] = sensors[rand()%4]; //Changes sensor reading in gene.
			}
			else if(random2 == 5)
			{
				genes[random][random2] = directions[rand()%4]; //Changes direction in gene
			}
			else
			{
				genes[random][random2] = actions[rand()%4]; //Changes action in gene
			}
		}
	}

	//Constructs robots through random generation
	Robot(string rname, int rlocation, string rgenes[], char rdirection)
	{
		genes = new string[24];
		name = rname;
		location = rlocation;
		energy = 5;
		direction = rdirection;
		for(int x = 0; x < 24; x++)
		{
			*(genes+x) = rgenes[x];
		}
	}

	string getName()
	{
		return name;
	}

	int getEnergy()
	{
		return energy;
	}

	int getLocation()
	{
		return location;
	}

	string* getGenes()
	{
		return genes;
	}

	void energize()
	{
		energy += 5;
	}
	//Specification 5 - Robots lose power when they hit walls
	void hitWall()
	{
		energy -= 1;
	}

	int getNumMoves()
	{
		return numMoves;
	}

	void reset()
	{
		numMoves = 0;
		energy = 5;
		do
		{
			location = rand()%166 +15;
		}
		while(location % 14 == 0 || location % 14 == 13);

	}


	//Runs robots through the map
	void test(char map[], int moves)
	{
		string sensors = "----"; //Holds sensor reading
		while(moves > 0 && energy > 0)
		{
			sensors[0] = getSensorReading('N', map); //Reads for N sensor
			sensors[1] = getSensorReading('S', map); //Reads for S sensor
			sensors[2] = getSensorReading('E', map); //Reads for E sensor
			sensors[3] = getSensorReading('W', map); //Reads for W sensor

			//This line is potentially confusing 
			execute(getAction(getBestGene(sensors)), map); //1. Decides which gene to execute | 2. Decides action to execute from gene | 3. Executes

			moves--;
		}

	}

	//Returns sensor reading for a sensor
	char getSensorReading(char sensor, char map[])
	{
		int checkNum; //Used for checking coordinates | Coordinate to check = location + checkNum

		//Should never return f. F was used for debugging
		if(sensor == 'N')
		{
			checkNum = -14;
		}
		else if(sensor == 'S')
		{
			checkNum = 14;
		}
		else if(sensor == 'E')
		{
			checkNum = 1;
		}
		else if(sensor == 'W')
		{
			checkNum = -1;
		}
		else
		{
			cout<<"-----------------------------------Error in getSensorReading()\n";
			return 'f';
		}
		if(map[location+checkNum] == '-')
		{
			return '0';
		}
		else if(map[location+checkNum] == 'B')
		{
			return '1';
		}
		else if(map[location+checkNum] == 'X')
		{
			return '9';
		}
		else
		{
			return 'f';
		}
	}

	//Returns index of best gene to execute
	int getBestGene(string sensors)
	{
		int bestGene = 0; //Index of the best gene
		int bestMatch = 0; //Rating of how closely the gene matches

		for(int i = 0; i<24; i++)
		{
			int match=0; //Current match rating of gene being examined
			for(int j = 0; j<4; j++)
			{
				if(genes[i][j] == '2')
				{
					match++;
				}
				else if(sensors[j] == genes[i][j])
				{
					//Feature 1 - Prioritizes sensor readings over direction. Matching a 0, 1, or 9 (nothing, battery, or wall) increases match rating by 2 while matching direction or 2 (not caring) increase by 1.
					match+=2;
				}
			}
			if(direction == genes[i][4])
			{
				match++;
			}
			if(match > bestMatch)
			{
				bestMatch = match;
				bestGene = i;
			}
		}
		return bestGene;
	}

	//Returns action to execute
	char getAction(int gene)
	{
		//Searches the gene to get the action to execute
		if(direction == 'N')
		{
			return genes[gene][5];
		}
		else if(direction == 'S')
		{
			return genes[gene][6];
		}
		else if(direction == 'E')
		{	
			return genes[gene][7];
		}
		else if(direction == 'W')
		{
			return genes[gene][8];
		}
		cout<<"Error in getAction()\n";
		return 'X';
	}

	//Executes action
	void execute(char action, char map[])
	{
		if(action == 'R')
		{
			if(direction == 'S')
			{
				direction = 'W';
			}
			else if(direction == 'E')
			{
				direction = 'S';
			}
			else if(direction == 'W')
			{
				direction = 'N';
			}
			else if(direction == 'N')
			{
				direction = 'E';
			}
		}
		else if(action == 'L')
		{
			if(direction == 'N')
			{
				direction = 'W';
			}
			else if(direction == 'S')
			{
				direction = 'E';
			}
			else if(direction == 'E')
			{
				direction = 'N';
			}
			else if(direction == 'W')
			{
				direction = 'S';
			}
		}
		move(map);
	}

	//Causes robot to move
	void move(char map[])
	{
		//Changes robot location
		int moveIndex;
		if(direction == 'N')
		{		
			moveIndex = -14;
		}
		else if(direction == 'S')			
		{
			moveIndex = 14;
		}
		else if(direction == 'W')
		{
			moveIndex = -1;		
		}
		else
		{
			moveIndex = 1;
		}
		if(map[location + moveIndex] == 'X')
		{
			hitWall();
		}
		else if(map[location+moveIndex] == 'B')
		{
			energize();
			location+=moveIndex;
			map[location] = '-';
		}
		else
		{
			location += moveIndex;
		}
		energy--;
		numMoves++;
	}


};

//Writes to log
ostream &operator<<(ostream& output, Robot& r)
{
	output<<"\n----Name: "<<r.name<<endl;
	output<<"Energy: "<<r.energy<<endl;
	output<<"Genes: "<<endl;
	for(int x = 0; x < 24; x++)
	{
		output<<to_string(x+1)<<". "<<r.genes[x]<<endl;
	}
	return output;
}

//Returns true if R1 is worse than R2
bool operator<(const Robot &R1, const Robot &R2)
{
	if(R1.energy < R2.energy)
	{
		return true;
	}
	else if(R1.energy == R2.energy)
	{
		if(R1.numMoves < R2.numMoves)
		{
			return true;
		}
	}
	return false;
}

//Program Greeting
void programGreeting();
//Displays Menu
string displayMenu();


void generateMap(char[]); //Generates map
void displayMap(char[]); //Displays map
void initializeRobots(vector<Robot>&, int population, int generations); //Randomly generates robots
void testRobots(vector<Robot>&, int population, char map[], int &moves); //Runs robots through map
float getFitness(vector<Robot>&); //Returns average energy of robots 
void sortRobots(vector<Robot>&); //Sorts using bubble sort
void breed(vector<Robot>&, int generations); //Breeds robots

int main()
{
	srand(time(NULL));
	programGreeting();

	//Log of robot fitness and genes
	ofstream log;
	log.open("log.txt");


	while(displayMenu() == "1")
	{
		//Specification 1 - 12x12 map (with walls which is why it is 196 elements long, not 144)
		char map[196];
		//Specification 6 - Fixed population of 200 robots.
		const int POPULATION = 200;
		int generations;
		int currentGen = 0;
		vector<Robot> robots;

    // Band aid for filtering inputs (bad code, I know. :0 
    // But if it works, SHIP IT :D)
    string genStr;
    string moveStr;

		//Gets number of generations
		cout<<"Enter the number of generations you would like to test:\n";
		cout<<"-----:";
		cin>>genStr;
    generations = stoi(genStr);

		//Gets number of moves for each robot
		int moves;
		//Specification 4 - Robots move for a specified number of times
		cout<<"Enter the number of moves each robot makes:\n";
		cout<<"-----:";
		cin>>moveStr;
    moves = stoi(moveStr);

		//Initializations
		generateMap(map);
		cout<<"Map generated: \n";
		displayMap(map);
		initializeRobots(robots, POPULATION, currentGen);

		//Runs for specified number of generations
		while(generations > 0)
		{
			cout<<"Current generation: "<<currentGen<<endl;
			testRobots(robots, POPULATION, map, moves);
			sortRobots(robots);


			log<<"---------------------------------------------------Generation "<<currentGen<<"\n\n";
			for(int x = 0; x < POPULATION; x++)
			{
				log<<x<<". "<<robots[x]<<endl;
			}
			float fitness = getFitness(robots);
			log<<"\nGeneration "<<currentGen<<" Energy: "<<fitness<<endl;
			cout<<"-----------------------------Energy: "<<fitness<<endl;

			currentGen++;
			breed(robots,currentGen);
			generations--;

			for(int x = 0; x < 200; x++)
			{
				robots[x].reset();
			}
		}
		cin.ignore();
	}
	
	log.close();
	cout<<"Exiting program...\n";
	return 0;
}

void programGreeting()
{
	cout<<"Welcome! This program simulates the evolution of robots.\n";
}

string displayMenu()
{
	cout<<"1. Run simulation\n2. Quit\n";
	cout<<"Choose an action: ";
	string choice;
	getline(cin, choice);
	while(choice != "1" && choice != "2")
	{
		cout<<"Invalid selection\n";
		getline(cin, choice);
	}
	return choice;
}


void generateMap(char map[])
{
	//Specification 3 - Generates map with 40% of the squares being batteries
  int numBatteries = 58;
  for(int x = 0; x < 196; x++)
  {
  	if(x < 14 || x > 182)
  	{
  		map[x] = 'X';
  	}
  	else if(x%14 == 0 || x%14 ==13)
  	{
  		map[x] = 'X';
  	}
  	else
  	{
  		map[x] = '-';
  	}
  }
  while(numBatteries > 0)
  {
    int location = rand() % 196 + 1;
    if(map[location] == '-')
    {
      map[location] = 'B';
      numBatteries--;
    }
  }
  
}

void displayMap(char map[])
{
  for(int x = 0; x < 196; x++)
  {
    if(x != 0 && x % 14 == 0)
    {
      cout<<endl;
    }
    cout<<map[x]<<" ";
  }
  cout<<endl;
}

void initializeRobots(vector<Robot> &robots, int population, int generations)
{
	char sensors[4] = {'0','1','2','9'};
	char directions[4] = {'N', 'S', 'E', 'W'};
	char  actions[4] = {'M', 'R', 'L', 'X'};
	string genes[24];
	string gene = "---------";


	//Initializes each robot
	for(int i = 0; i < population; i++)
	{
		//Initializes each chromosome
		for(int j = 0; j < 24; j++)
		{
			//Initializes each gene
			for(int x = 0; x < 4; x++)
			{
				gene[x] = sensors[rand()%4];
			}
			gene[4] = directions[rand()%4];
			
			for(int x = 5; x < 9; x++)
			{
				gene[x] = actions[rand()%4];
			}
			genes[j] = gene;
		}
		//Specification 2 - Initializes robot on random square with 5 units of power. (Power is in constructor)
		int initialLocation = 0;
		while(initialLocation % 14 == 0 || initialLocation % 14 == 13)
		{
			initialLocation = rand()%166 +15;
		}
		Robot tempRobot(to_string(generations) + "-" + to_string(i),initialLocation , genes, directions[rand()%4]);
		robots.push_back(tempRobot);
	}
}

void testRobots(vector<Robot> &robots, int population, char map[], int &moves)
{
	char tempMap[196];
	while(moves < 12 || moves > 27)
	{
		cout<<"Error, enter a number in range 12-27\n";
		cin>>moves;
	}
	for(int x = 0; x < population; x++)
	{
		for(int x = 0; x< 196; x++)
		{
			tempMap[x] = map[x];
		}
		robots[x].test(tempMap, moves);
	}
}	

float getFitness(vector<Robot> &robots)
{
	float total = 0;
	for(unsigned int x = 0; x < robots.size(); x++)
	{
		total += robots[x].getEnergy();
	}
	return total/200;
}

void sortRobots(vector<Robot> & robots)
{
	Robot temp;
	bool sorted = false;
	bool swapped;
	while(!sorted)
	{
		swapped = false;
		for(int x = 0; x < 199; x++)
		{
			if(robots[x] < robots[x+1])
			{
				temp = robots[x];
				robots[x] = robots[x+1];
				robots[x+1] = temp;
				swapped = true;
			}
		}
		if(swapped == false)
		{
			sorted = true;
		}
	}
}

void breed(vector<Robot> &robots, int generations)
{

	char directions[4] = {'N', 'S', 'E', 'W'};
	for(int x = 0; x < 100; x+=2)
	{
		int initialLocation = 0;
		while(initialLocation % 14 == 0 || initialLocation % 14 == 13)
		{
			initialLocation = rand()%166 +15;
		}
		Robot temp1(to_string(generations) + "-" + to_string(x), initialLocation, robots[x].getGenes(), robots[x+1].getGenes(), directions[rand()%4]);
		Robot temp2(to_string(generations) + "-" + to_string(x+1), initialLocation, robots[x+1].getGenes(), robots[x+2].getGenes(), directions[rand()%4]);
		robots[198-x] = temp1;
		robots[199-x] = temp2;
	}

}