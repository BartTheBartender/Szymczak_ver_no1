#include <iostream>
#include <fstream>
#include <filesystem>
#include "Groups.h"
#include "Relations.h"
#include <chrono>

using namespace std;


//~ int main() {
	
	//~ Long base = 2;
	
	//~ while(true){

		//~ ostringstream name_dir;
		//~ name_dir << "Wyniki/txt/Z-" << base;
		//~ filesystem::create_directory(name_dir.str());
		
		//~ for(Long size = 1; size < (Long)10000; ++size){

		    //~ auto start_time = std::chrono::high_resolution_clock::now();
			
			//~ ostringstream name;
			//~ name << "Wyniki/txt/Z-" << base << "/Z-" << base << "-attempt-" << size << ".txt";

			//~ ofstream file;
			//~ file.open(name.str());

			//~ file << Relation::generate(base,size);
			//~ file.close();			
			
			//~ auto end_time = std::chrono::high_resolution_clock::now();
			//~ auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			
			//~ Relation::reset();
			//~ if(duration.count() > 3600000) break;
		//~ }
		
		//~ ++base;
	//~ }

//~ }

int main() {
	
	Long base = 5;
	


		ostringstream name_dir;
		name_dir << "Wyniki/txt/Z-" << base;
		filesystem::create_directory(name_dir.str());
		
		for(Long size = 1; size < (Long)10000; ++size){

		    auto start_time = std::chrono::high_resolution_clock::now();
			
			ostringstream name;
			name << "Wyniki/txt/Z-" << base << "/Z-" << base << "-attempt-" << size << ".txt";


			string result = Relation::generate(base,size);
			
			ofstream file;
			file.open(name.str());
			file << result;
			file.close();			
			
			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			
			Relation::reset();
			if(duration.count() > 3600000) break;
		}
			

}
