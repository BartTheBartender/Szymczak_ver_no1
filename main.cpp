#include <iostream>
#include <fstream>
#include <filesystem>
#include "Groups.h"
#include "Relations.h"
#include "Maps.h"
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

constexpr Long base = 3;
constexpr Long dim = 1;


int main() {
	
	//~ Long base;
	//~ Long size;
	
	//~ cout << "Generuj relacje w modułach nad Z/";
	//~ cin >> base;
	//~ cout << "do wymiaru ";
	//~ cin >> size;
	//~ cout << endl;

	Relation::generate(base, dim);
	
		ostringstream name_dir;
		ostringstream name_dir_Filip;
		name_dir << "Wyniki/txt/Z-" << base;
		name_dir_Filip << "Wyniki/txt-Filip/Z-" << base;		


		if (!fs::exists("Wyniki")) {
			fs::create_directory("Wyniki");
		}	
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
		name << "Wyniki/txt/Z-" << base << "/Z-" << base << "-attempt-" << dim << ".txt";
		name_Filip << "Wyniki/txt-Filip/Z-" << base << "/Z-" << base << "-attempt-" << dim << ".txt";

		ofstream file;
		file.open(name.str());
		file << Relation::output(base, dim);
		file.close();			

		ofstream file_Filip;
		file_Filip.open(name_Filip.str());
		file_Filip << Relation::output_Filip(base, dim);
		file_Filip.close();		
}

