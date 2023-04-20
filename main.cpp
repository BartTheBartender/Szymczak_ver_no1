#include "Groups.h"
#include <iostream>
#include <chrono>

using namespace std;
int main(){ 
	
	auto start_time = chrono::high_resolution_clock::now();
	
	groupRanks ranks = {2,2,2};
	for(int r : ranks) cout << "Z/" << r << " ";
	cout << endl;
	
	group G = GroupTheory::generateGroup(ranks);
	
	container<group> subgroups = GroupTheory::generateSubgroups(G, ranks);

    auto end_time = chrono::high_resolution_clock::now();
    auto time_taken = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();	
	
	cout << "grup jest:\t" << subgroups.size() << "\ntime:\t" << time_taken << " miliseconds\n--------------------------\n";
}
