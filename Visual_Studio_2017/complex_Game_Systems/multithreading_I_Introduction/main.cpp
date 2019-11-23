#include <iostream>

// *****----- Mutex Locks -----*****
#include <mutex>
// *****-----------------------*****

// *****----- Hello Thread Part One / Two -----*****
#include <thread>
#include <vector>
// *****---------------------------------------*****

// *****----- Logger -----*****
#include <cstring>
// Stream class to write on files
#include <fstream>
#include <string>
// *****------------------*****

using namespace std;
/*
void print() {
	// *****----- Hello Thread Part One / Two -----*****
	// cout << "Howdy Partner\n"; 
	// *****---------------------------------------*****

	// *****----- Mutex Locks -----*****
	// Function-static mutex ensures that this will be initialized only once
	// but, also across any thread that calls this function
	static mutex printMutex;

	// If a thread has already locked this, this will block execution until unlocked
	printMutex.lock();

	cout << "Howdy Partner\n";
	cout << "Looks like this town ain't big enough for the two of us...\n";
	cout << "Town square, high noon, be there or be square...\n";
	
	// Unlocks, allowing a diffrent thread to lock this
	printMutex.unlock();
	// *****-----------------------*****

	// *****----- Lock Guard -----*****
	// Function-static mutex ensures that this will be initialized only once
	// but, also across any thread that calls this function
	static mutex printMutex;

	// Created when this statement is executed, which attempts to lock
	// Destroyed when this function exits, which unlocks
	lock_guard<mutex> guard(printMutex);

	cout << "So, in about nine hours, I will be going to see my favorite artist\n";
	cout << "The only thing, is that I've never been to an event such as that before\n";
	cout << "With that being said, I am definitely nervous but excited \n";
	// *****----------------------*****
}


void print(int SJ) {
	// *****----- Passing Arguments -----*****
	// Function-static mutex ensures that this will be initialized only once
	// but, also across any thread that calls this function
	static mutex printMutex;

	// Created when this statement is executed, which attempts to lock
	// Destroyed when this function exits, which unlocks
	lock_guard<mutex> guard(printMutex);

	cout << SJ << " person / people used to live here, now... It's a ghost town\n";
	// *****-----------------------------*****
}
*/

// *****----- Logger -----*****
void logger(string file, string text) {
	// Function-static mutex ensures that this will be initialized only once
	// but, also across any thread that calls this function
	static mutex loggerMutex;

	// Created when this statement is executed, which attempts to lock
	// Destroyed when this function exits, which unlocks
	lock_guard<mutex> guard(loggerMutex);

	if (remove(file.c_str()) + ".txt" != 0) { cout << "Error - No file to remove\n"; }
	else { cout << file << ".txt successfully removed\n"; }

	// Opens file to add data and all output operations are performed at the end of the file appending the content to the current content of the file. 
	ofstream selectedFile(file + ".txt", ios::app | ios::out | ios::trunc);

	if (selectedFile.is_open()) {
		selectedFile << text << endl;
		selectedFile.close();
	}
	else { cout << "Error - Unable to open file."; }
}
// *****------------------*****

int main() {
	/*
	// *****----- Hello Thread Part One -----*****
	/ *
	// Create a thread that will execute the print function
	thread printThread(print);

	// Blocks execution on the main thread until printThread exits
	printThread.join();
	* /
	// *****---------------------------------*****

	// *****----- Hello Thread Part Two / Mutex Locks / Passing Arguments -----*****
	// Create a thread that will execute the print function
	vector<thread> threads;

	for (int SJ = 0; SJ < 50; SJ++) { 
		// *****----- Hello Thread Part Two / Mutex Locks -----*****
		// threads.push_back(thread(print)); 
		// *****-----------------------------------------------*****

		// *****----- Passing Arguments -----*****
		threads.push_back(thread(print, SJ)); 
		// *****-----------------------------*****
	}

	// Blocks execution on the main thread until all threads exit
	for (auto& thread : threads) { thread.join(); }
	// *****-------------------------------------------------------------------*****
	*/

	// *****----- Logger -----*****
	bool shouldGameLoop = true;
	vector<thread> loggerThreads;
	string exitString[2] = { "Exit", "exit" };
	while (shouldGameLoop)
	{
		string createdString;

		cout << "Enter some text..." << endl;
		// Grabs all characters in a string / character array etc.
		getline(cin, createdString);

		for (int SJ = 0; SJ < exitString->size() - 1; SJ++) { if (createdString == exitString[SJ]) { cout << "Who you calling pinhead?" << endl; shouldGameLoop = false; } }

		loggerThreads.push_back(thread(logger, "SJ", createdString));
	}
	for (auto& thread : loggerThreads) { thread.join(); }
	// *****------------------*****
	return 0;
}