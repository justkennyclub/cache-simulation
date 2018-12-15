#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<vector>
#include<fstream>
#include<locale>
#include <codecvt>
#include<iomanip>

//function prototype to start the LRU/FIFO algorithm based on program input
void simulate(int total_address, int frame_size, char choice, int data[]);

//function prototype to display address input by user or randomly generated address
void displayAddress(int total_address, int data[]);

using namespace std;

//global variable for the file output
const locale for_utf8 = locale(locale(), new codecvt_utf8<wchar_t>());
wofstream outputFile;

//global variable for the border of table
char horizontal = 196;				//symbol ─
char vertical = 179;				//symbol │
char upleftcorner = 218;			//symbol ┌
char verticalmidleft = 195;			//symbol ├
char bottomleftcorner = 192;		//symbol └
char middle = 197;					//symbol ┼
char topmiddle = 194;				//symbol ┬
char bottommiddle = 193;			//symbol ┴
char toprightcorner = 191;			//symbol ┐
char bottomrightcorner = 217;		//symbol ┘
char verticalmidright = 180;		//symbol ┤

//function to check if an input string is an integer
bool isInteger(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

int main()
{
	//drawing title box
	cout << upleftcorner;
	for (int i = 0; i < 25; i++)
		cout << horizontal;
	cout << toprightcorner << endl;
	cout << vertical << " CACHE MEMORY SIMULATION " << vertical << endl;
	cout << bottomleftcorner;
	for (int i = 0; i < 25; i++)
		cout << horizontal;
	cout << bottomrightcorner << endl;

	//create or open the output.txt file for printing program output results
	outputFile.open(L"output.txt", ios_base::app);
	outputFile.imbue(for_utf8);
	outputFile << "CACHE MEMORY SIMULATION\nby Kenny\n-----------------------\n";

	cout << "Input\n" << horizontal << horizontal << horizontal << horizontal << horizontal << endl;

	//input string to be processed before assigned to used variable
	string input_number;					
	//variable for number of possible addresses
	int total_address;						

	//repeat until correct integer input
	do
	{
		cout << "Number of Possible Addresses: ";
		getline(cin, input_number);
		if (!isInteger(input_number))
			cout << "Please input POSITIVE INTEGER values\n\n";
	} while (!isInteger(input_number));
	total_address = stoi(input_number);		//convert string to integer
	if (total_address == 0)
	{
		cout << "\nNo Possible Addresses\nProgram terminates...\n\n";
		return 0;
	}
	//array to store input addresses
	int* data = new int[total_address];

	//variable for size number of the frames
	int frame_size;							

	//repeat until correct integer input
	do
	{
		cout << "Size of Frames              : ";
		getline(cin, input_number);
		if (!isInteger(input_number))
			cout << "Please input POSITIVE INTEGER values\n\n";
	} while (!isInteger(input_number));
	frame_size = stoi(input_number);

	//input string to be processed before assigned to used variable
	string input;
	//variable for choice of replacement policy initialised with unusual character not l or f
	char choice = 'z';						

	//repeat until input is the character l or f
	do
	{
		cout << "Algorithm LRU/FIFO (l/f)    : ";
		getline(cin, input);
		if (input.length() != 1)		//prevent accepting string>1 character with first letter l or f
		{
			cout << "Please type the replacement policy choice l or f accordingly\n\n";
		}

		else
		{
			choice = input[0];
			if (choice != 'l' && choice != 'f')
				cout << "Please type the replacement policy choice l or f accordingly\n\n";
		}

	} while (choice != 'l' && choice != 'f');

	//variable for choosing address input source type initialised with unusual character not y or n
	char reply = 'z';

	//repeat until input is the character y or n
	do
	{
		cout << "User Input Address/Not (y/n): ";
		getline(cin, input);
		if (input.length() != 1)			//prevent accepting string>1 character with first letter y or n
		{
			cout << "Please type the choice y or n accordingly\n\n";
		}
		else
		{
			reply = input[0];
			if (reply != 'y' && reply != 'n')
				cout << "Please type the choice y or n accordingly\n\n";
		}

	} while (reply != 'y' && reply != 'n');

	//seed the rand() function with computer time
	srand(static_cast<unsigned int>(time(0)));	

	//if user reply 'n', generate random number based on user specified amount
	if (reply == 'n')
	{
		//variable for total random numbers to be generated
		int total_random;

		//repeat until user inputs a valid number
		do
		{
			cout << "\nHow many addresses should be generated? (max " << total_address << ") ";
			getline(cin, input_number);
			if (!isInteger(input_number))
				cout << "Please input POSITIVE INTEGER values <=" << total_address << "\n";
			else
			{
				total_random = stoi(input_number);
				if (total_random > total_address)
					cout << "Max possible addresses generated is " << total_address
					<< ". Please input <=" << total_address << endl;
			}
		} while (!isInteger(input_number) || total_random > total_address || total_random < 0);

		//process check user input number before generating random number and call simulate
		if (total_random == 0)
		{
			cout << "\nNo random address generated\nProgram terminates...\n\n";
			return 0;
		}
		else
		{
			for (int i = 0; i < total_random; i++)
			{
				unsigned int number = 0 + rand() % 10;
				data[i] = number;
			}
			cout << "\n\nOutput\n" << horizontal << horizontal << horizontal << horizontal << horizontal << horizontal << endl;
			cout << "[Randomly Generated]\n";
			outputFile << "\n[Randomly Generated]\n";
			displayAddress(total_random, data);
			simulate(total_random, frame_size, choice, data);
		}
	}
	else if (reply == 'y')		//allow user to key in each input address till invalid or max num of address
	{
		int input_index = 0;
		char term;
		//repeat until number of possible address or invalid user input
		do
		{
			if (data[input_index] >= 0 && data[input_index] <= 9)
			{
				input_index++;
			}
			else
				cout << "\n-ONLY INPUT INTEGER FROM 0 to 9 FOR ADDRESS-\n\n";
			cout << "Input address " << input_index + 1 << "= ";
			cin >> input_number;
			if (!isInteger(input_number))
				break;
			else
				data[input_index] = stoi(input_number);
		} while (data[input_index] >= 0 && data[input_index] <= 9 && input_index<total_address - 1);

		//process check input before passing to call simulate
		if (input_index == 0 && data[input_index] >= 0 && data[input_index] <= 9)
		{
			cout << "\n\nOutput\n" << horizontal << horizontal << horizontal << horizontal << horizontal << horizontal << endl;
			cout << "[From User Input]\n";
			outputFile << "\n[From User Input]\n";
			displayAddress(input_index + 1, data);
			simulate(input_index + 1, frame_size, choice, data);	//input_index + 1 = total number of user input
		}
		else if (input_index == 0)
		{
			cout << "\nInput " << input_index + 1 << " is not a valid address. User input stops there.\n\n";
			cout << "No valid address input\nProgram terminates...\n\n";
			return 0;
		}
		else if ((input_index != total_address - 1) || (!(data[input_index] >= 0 && data[input_index] <= 9)))
		{
			cout << "\nInput " << input_index + 1 << " is not a valid address. User input stops there.\n";
			cout << "\n\nOutput\n" << horizontal << horizontal << horizontal << horizontal << horizontal << horizontal << endl;
			cout << "[From User Input]\n";
			outputFile << "\n[From User Input]\n";
			displayAddress(input_index, data);
			simulate(input_index, frame_size, choice, data);
		}
		else
		{
			cout << "\n\nOutput\n" << horizontal << horizontal << horizontal << horizontal << horizontal << horizontal << endl;
			cout << "[From User Input]\n";
			outputFile << "\n[From User Input]\n";
			displayAddress(input_index + 1, data);
			simulate(input_index + 1, frame_size, choice, data);	//input_index + 1 = total number of user input
		}

	}

	cout << endl;
	outputFile << endl;
	outputFile.close();
	return 0;
}

//function definition to display address input by user or randomly generated address
void displayAddress(int total_address, int data[])
{
	cout << "\nAddress:  ";
	outputFile << "\nAddress:  ";
	for (int i = 0; i < total_address; i++)
	{
		cout << data[i] << "   ";
		outputFile << data[i] << "   ";
	}
	cout << endl << endl;
	outputFile << endl << endl;
}

//function definition to start the LRU/FIFO algorithm based on program input
void simulate(int total_address, int frame_size, char choice, int data[])
{
	int* stack = new int[frame_size];

	//size of table is 1 more than the frames

	//initialize table
	//add one more row for the hit *

	//vector of vector
	vector<vector<int>> table(frame_size + 1, vector<int>(total_address));	//frame size + 1 for 'hit' row

	for (int i = 0; i <= frame_size; i++)			//initialize vector table
	{
		for (int j = 0; j < total_address; j++)
		{
			table[i][j] = -1;
		}
	}

	int stack_counter = 0;
	for (int i = 0; i < total_address; i++)
	{
		//variables to mark an occurrence of hit
		bool flag = false;
		bool isHit = false;


		//basically till size of stack
		for (int j = 0; j < stack_counter; j++)
		{
			if (data[i] == stack[j])
			{
				flag = true;
				isHit = true;
				if (choice == 'l')			//if choice is using LRU
				{
					for (int k = j; k < stack_counter - 1; k++)
					{
						stack[k] = stack[k + 1];
					}
					stack[stack_counter - 1] = data[i];
				}
				break;
			}
		}
		if (flag != true)
		{
			//if less than number of frames
			if (stack_counter < frame_size)
			{
				stack[stack_counter] = data[i];
				stack_counter++;
			}
			else
			{
				//shift stack
				for (int l = 0; l < stack_counter - 1; l++)
				{
					stack[l] = stack[l + 1];
				}
				stack[stack_counter - 1] = data[i];
			}

		}

		int countd = stack_counter - 1;	//for array index is always one less
		//loop till number of frames, inserting per column
		for (int k = 0; k < frame_size; k++)		
		{
			//fill in table per column based on current stack
			table[k][i] = stack[countd];

			if (countd == 0)
				break;
			countd--;
		}
		if (isHit == true)
			table[frame_size][i] = -2;	//insert unusual value to mark hit
	}
	if (choice == 'l')
	{
		cout << "LRU\n";
		outputFile << "LRU\n";
	}
	else
	{
		cout << "FIFO\n";
		outputFile << "FIFO\n";
	}
	int hit_count = 0;

	int table_width = 9 + total_address * 4;	//the length of table width when output, accounting the spacing etc
	cout << upleftcorner;
	outputFile << L"\u250c";
	int counttop = 7;							//for counting the position where ┬ is used

	for (int i = 0; i < table_width - 2; i++)	//print the top border of table
	{
		if (i == counttop)
		{
			cout << topmiddle;
			outputFile << L"\u252c";
			counttop += 4;
		}
		else
		{
			cout << horizontal;
			outputFile << L"\u2500";
		}
	}

	cout << toprightcorner;
	outputFile << L"\u2510";
	cout << endl;
	outputFile << endl;

	//loop till the number of frames, printing per column
	for (int i = 0; i <= frame_size; i++)	
	{
		//print the title information of each row
		if (i != frame_size)
		{
			cout << vertical << " i=" << setw(2) << i << ": " << vertical << " ";
			outputFile << L"\u2502" << " i=" << setw(2) << i << ": " << L"\u2502" << " ";
		}
		else
		{
			cout << vertical << " Hits: " << vertical << " ";
			outputFile << L"\u2502" << " Hits: " << L"\u2502" << " ";
		}

		//print the content of table
		for (int j = 0; j < total_address; j++)
		{
			if (table[i][j] == -2)
			{
				cout << "*" << " " << vertical << " ";
				outputFile << "*" << " " << L"\u2502" << " ";
				hit_count++;
			}
			else if (table[i][j] == -1)
			{
				cout << " " << " " << vertical << " ";
				outputFile << " " << " " << L"\u2502" << " ";
			}
			else
			{
				cout << table[i][j] << " " << vertical << " ";
				outputFile << table[i][j] << " " << L"\u2502" << " ";
			}
		}
		cout << endl;
		outputFile << endl;

		//print the left border of next line
		if (i != frame_size)
		{
			cout << verticalmidleft;
			outputFile << L"\u251c";
		}

		//for counting the position where ┼ is used
		int counter = 7;

		//print the border below table content
		for (int j = 0; j < table_width - 2; j++)
		{
			if (j == counter && i != frame_size)
			{
				counter = j + 4;
				cout << middle;
				outputFile << L"\u253c";
			}
			else if (i != frame_size)
			{
				cout << horizontal;
				outputFile << L"\u2500";
			}
		}
		if (i != frame_size)
		{
			cout << verticalmidright << endl;
			outputFile << L"\u2524" << endl;
		}
	}
	cout << bottomleftcorner;
	outputFile << L"\u2514";
	int countbottom = 7;							//for counting the position where ┴ is used
	for (int i = 0; i < table_width - 2; i++)		//print the bottom border of table
	{
		if (i == countbottom)
		{
			cout << bottommiddle;
			outputFile << L"\u2534";
			countbottom += 4;
		}
		else
		{
			cout << horizontal;
			outputFile << L"\u2500";
		}
	}
	cout << bottomrightcorner;
	outputFile << L"\u2518";

	//print the hit ratio information
	cout << "\n\nTotal = " << total_address << endl;
	cout << "Hit = " << hit_count << endl;
	cout << "Hit ratio = " << hit_count << "/" << total_address << " = " << double(hit_count) / double(total_address) << endl;
	cout << endl;

	//print the hit ratio information to a .txt file
	outputFile << "\n\nTotal = " << total_address << endl;
	outputFile << "Hit = " << hit_count << endl;
	outputFile << "Hit ratio = " << hit_count << "/" << total_address << " = " << double(hit_count) / double(total_address) << endl;
	outputFile << endl;
}
