#include <iostream>
#include <fstream>
#include <filesystem>
#include "Groups.h"
#include "Relations.h"

#include <chrono>

using namespace std;
namespace fs = std::filesystem;

constexpr Long base = 2;
constexpr Long dim = 2;


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
		ostringstream name_dir_python;
		name_dir << "Wyniki/txt/Z-" << base;
		name_dir_python << "Wyniki/txt-python/Z-" << base;		


		if (!fs::exists("Wyniki")) {
			fs::create_directory("Wyniki");
		}	
		if (!fs::exists("Wyniki/txt")) {
			fs::create_directory("Wyniki/txt");
		}
		if (!fs::exists("Wyniki/txt-python")) {
			fs::create_directory("Wyniki/txt-python");
		}		
		
		if (!fs::exists(name_dir.str())) {
			fs::create_directory(name_dir.str());
		}
		if (!fs::exists(name_dir_python.str())) {
			fs::create_directory(name_dir_python.str());
		}
		
		ostringstream name;
		ostringstream name_python;		
		name << "Wyniki/txt/Z-" << base << "/Z-" << base << "-attempt-" << dim << ".txt";
		name_python << "Wyniki/txt-python/Z-" << base << "/Z-" << base << "-attempt-" << dim << ".txt";

		ofstream file;
		file.open(name.str());
		file << Relation::output(base, dim);
		file.close();			

		ofstream file_python;
		file_python.open(name_python.str());
		file_python << Relation::output_python(base, dim);
		file_python.close();		
}

