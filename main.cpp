#include <iostream>
#include <fstream>
#include <filesystem>
#include "Groups.h"
#include "Relations.h"
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

//~ int main() {
	
	//~ Long base = 4;
	
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
	
	Long base = 3;
	Long size = 2;

	Relation::generate(base, size);
	
		ostringstream name_dir;
		ostringstream name_dir_Filip;
		name_dir << "Wyniki/txt/Z-" << base;
		name_dir_Filip << "Wyniki/txt-Filip/Z-" << base;		
			
		if (!fs::exists("Wyniki/txt")) {
			fs::create_directory("Wyniki/txt");
		}
		if (!fs::exists("Wyniki/txt-Filip")) {
			fs::create_directory("Wyniki/txt-Filip");
		}		
		if (!fs::exists(name_dir.str())) {
			fs::create_directory(name_dir.str());
		}
		if (!fs::exists(name_dir_Filip.str())) {
			fs::create_directory(name_dir_Filip.str());
		}
		
		ostringstream name;
		ostringstream name_Filip;		
		name << "Wyniki/txt/Z-" << base << "/Z-" << base << "-attempt-" << size << ".txt";
		name_Filip << "Wyniki/txt-Filip/Z-" << base << "/Z-" << base << "-attempt-" << size << ".txt";

		ofstream file;
		file.open(name.str());
		file << Relation::output(base, size);
		file.close();			

		ofstream file_Filip;
		file_Filip.open(name_Filip.str());
		file_Filip << Relation::output_Filip(base, size);
		file_Filip.close();		
}

