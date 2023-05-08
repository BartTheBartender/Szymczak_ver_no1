#include "Relations.h"
#include <omp.h>

using namespace std;

	

    std::size_t Relation::hash() const {
        std::size_t seed = size();
        for (const std::vector<bool>& row : *this) {
            seed ^= std::hash<std::vector<bool>>()(row) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
    
	Relation::Relation(const group& G, const dimensions& domain, const dimensions& codomain, const dimensions& domain_prod, const dimensions& codomain_prod, const dimensions& merged): domain(domain), codomain(codomain){

		this -> resize(codomain_prod[0]);
		for(Int i = 0; i < codomain_prod[0]; ++i) ((*this)[i]).resize(domain_prod[0]);
		for(Long i = 0; i < G.size(); ++i){
			
			Long x = 0, y = 0;
			
			for(Int j = 0; j < domain_prod.size() - 1; ++j){ x += (Long)(domain_prod[j+1]*G[i][j]);} x += (Long)(G[i][domain_prod.size() - 1]); //BOOST IT
			for(Int j = 0; j < codomain_prod.size() - 1; ++j){ y += (Long)(codomain_prod[j+1]*G[i][domain_prod.size() + j]); } y += (Long)G[i][domain_prod.size() + codomain_prod.size() - 1];
			(*this)[y][x] = true;

		}
	}
	
	Relation::Relation(const Relation& source): domain(source.domain), codomain(source.codomain), orbit(source.orbit){
		matrix<bool>::operator=(source);
	}
	
	bool Relation::operator == (const Relation& source) const {
		if(this -> domain != source.domain || this -> codomain != source.codomain) return false;
		
		return ((matrix<bool>)(*this) == (matrix<bool>)(source));
	}
	
	bool Relation::operator != (const Relation& source) const {
		if(this -> domain != source.domain || this -> codomain != source.codomain) return false;
		
		return ((matrix<bool>)(*this) != (matrix<bool>)(source));
	}	
	
	Relation& Relation::operator= (const Relation& source) {
		if(*this == source) return *this;
		
		matrix<bool>::operator=(source);
		this -> domain = source.domain;
		this -> codomain = source.codomain;
		this -> orbit = source.orbit;
		return *this;
	}
	
	Relation::Relation(const dimensions& domain, const dimensions& codomain, const Long rows, const Long columns): domain(domain), codomain(codomain){
		this -> resize(rows);
		for(Long i = 0; i < rows; ++i) ((*this)[i]).resize(columns);
	}

	void Relation::generate_partitions(container < dimensions >& partitions, Long base, Long size, dimensions partition, Long index){
		if(index == size){partitions.push_back(partition); return;}
		
		for(Long k : GroupTheory::sigma(base)){
			if(k == 1) continue;
			else if(partition.size() != 0 && k < partition.back()) continue;
			partition.push_back(k);
			Relation::generate_partitions(partitions, base, size, partition, index + 1);
			partition.pop_back();
		}
		
	}
	
	container < dimensions > Relation::generate_partitions(Long base, Long size){
		
		container < dimensions > partitions;
		dimensions partition;
		Relation::generate_partitions(partitions, base, size, partition, 0);			
		return partitions;
	}
	
	void Relation::generate_all_partitions(Long base, Long size){
		
		for(Long k = 1; k <= size; ++k){
			container < dimensions > partitions = Relation::generate_partitions(base, k);
			
			for(auto d : partitions) Relation::all_partitions.push_back(d);
		}
	}	
	
	
	void Relation::generate_all_relations(){
		
		for(dimensions domain : Relation::all_partitions){
		for(dimensions codomain : Relation::all_partitions){
				Relation::all_relations.insert(make_pair( make_pair(domain, codomain), Relation::generate_relations(domain, codomain)));
		}	
		}
		
	}
	
	container < Relation > Relation::generate_relations(const dimensions& domain, const dimensions& codomain){
		
		container < Relation > relations;
		
		dimensions domain_temp(domain.size());
		for(Long i = 0; i < (Long)domain.size(); ++i)domain_temp[domain.size()-1-i] = domain[i];
		dimensions domain_prod(domain.size());
		for(Long i = 1; i < (Long)domain_temp.size(); ++i){
			domain_temp[i] *= domain_temp[i-1];
		}
		for(Long i = 0; i < (Long)domain.size(); ++i)domain_prod[domain.size()-1-i] = domain_temp[i];
		
		dimensions codomain_temp(codomain.size());
		for(Long i = 0; i < (Long)codomain.size(); ++i)codomain_temp[codomain.size()-1-i] = codomain[i];
		dimensions codomain_prod(codomain.size());
		for(Long i = 1; i < (Long)codomain_temp.size(); ++i){
			codomain_temp[i] *= codomain_temp[i-1];
		}
		for(Long i = 0; i < (Long)codomain.size(); ++i)codomain_prod[codomain.size()-1-i] = codomain_temp[i];
		
		dimensions merged(domain.size() + codomain.size());
		for(Long i = 0; i < (Long)(domain.size()); ++i) merged[i] = domain[i];
		for(Long i = 0; i < (Long)(codomain.size()); ++i) merged[i + (Long)domain.size()] = codomain[i];		
		
		//~ cout << "domain_prod: " << domain_prod << " codomain_prod: " << codomain_prod << " merged: " << merged << endl;			
		group G = GroupTheory::generateGroup(merged);	
		container <group> subgroups_of_G = GroupTheory::generateSubgroups(G, merged);
		
			cout << "\twygenerowano podgrupy G: ";
			
			if(domain.size() == 0) throw invalid_argument("Pusta dziedzina w generacji");
			cout << "(Z/" << (short)domain[0];
			for(Long i = 1; i < domain.size(); ++i) cout << "+Z/" << (short)domain[i];
			
			if(codomain.size() == 0) throw invalid_argument("Pusta przeciwdziedzina w generacji");
			cout << ") x (Z/" << (short)codomain[0];
			for(Long i = 1; i < codomain.size(); ++i) cout << "+Z/" << (short)codomain[i];
			cout << ")";

		for(Large i = 0; i < (Large)subgroups_of_G.size(); ++i){
			relations.push_back(Relation(subgroups_of_G[i], domain, codomain, domain_prod, codomain_prod, merged));
		}
			cout << "\t\twygenerowano relacje\n";
					
		return relations;
	}
	
	Relation* Relation::find(const Relation& R){
		
		pair < dimensions, dimensions > domain_and_codomain = make_pair(R.domain, R.codomain);
		
		for(Long i = 0; i < Relation::all_relations[domain_and_codomain].size(); ++i){
			if(Relation::all_relations[domain_and_codomain][i] == R) return &(Relation::all_relations[domain_and_codomain][i]);
		}
			
		throw out_of_range("nie znaleziono R w tablicy\n");
	}
	
	Long Relation::find_Long(const Relation& R){
		pair < dimensions, dimensions > domain_and_codomain = make_pair(R.domain, R.codomain);
		
		for(Long i = 0; i < Relation::all_relations[domain_and_codomain].size(); ++i){
			if(all_relations[domain_and_codomain][i] == R) return i;
		}
			
		throw out_of_range("nie znaleziono R w tablicy\n");
	}
	
	table < Relation* > Relation::generate_multiplication_table(const dimensions& left_domain, const dimensions& left_codomain, const dimensions& right_domain, const dimensions& right_codomain){
		table < Relation* > multiplication_table(Relation::all_relations[make_pair(left_domain, left_codomain)].size());

		pair < dimensions , dimensions > left = make_pair(left_domain, left_codomain);
		pair < dimensions , dimensions > right = make_pair(right_domain, right_codomain);
		
		for(Long i = 0; i < (Long)all_relations[left].size(); ++i){

			multiplication_table[i].resize(all_relations[right].size());
		}		
		
		#pragma omp parallel for shared(left, right)
		for(Long i = 0; i < (Long)all_relations[left].size(); ++i){
		#pragma omp parallel for shared(left, right)			
		for(Long j = 0; j < (Long)all_relations[right].size(); ++j){
			
			Relation product = times(all_relations[left][i], all_relations[right][j]);
			
			multiplication_table[i][j] = find(product);//UWAGA
		}			
		}
		
		return multiplication_table;
	}
	
	void Relation::generate_multiplication_table(){
		#pragma omp parallel for
		for(auto left_domain : Relation::all_partitions){
		#pragma omp parallel for
		for(auto left_codomain : Relation::all_partitions){
			#pragma omp parallel for
			for(auto right_domain : Relation::all_partitions){
			#pragma omp parallel for
			for(auto right_codomain : Relation::all_partitions){
				
				cout << "\t" << left_domain << " " << left_codomain << " " << right_domain << " " << right_codomain << " ";
				if(right_codomain != left_domain){
					cout << "nie\n";
					continue;
				}
				cout << "tak\n";
				Relation::multiplication_table.insert(
					make_pair(
						make_pair(make_pair(left_domain, left_codomain), make_pair(right_domain, right_codomain)),
						Relation::generate_multiplication_table(left_domain, left_codomain, right_domain, right_codomain))
				);
			}
			}		
		}
		}
	}
	
	Relation Relation::times(const Relation& left, const Relation& right) {
		if (left.size() == 0 || right.size() == 0 || left[0].size() == 0 || right[0].size() == 0){
			throw invalid_argument("Empty relation in multiplication");
		}
		
		else if (right.size() != left[0].size()){
			throw out_of_range("Cannot compose relations");
		}

		Relation product(right.domain, left.codomain, left.size(), right[0].size());
		#pragma omp parallel for shared(product)
		for (Long i = 0; i < product.size(); ++i) {
			#pragma omp parallel for shared(product)
			for (Long j = 0; j < product[0].size(); ++j) {
				for (Long k = 0; k < right.size(); ++k) {
					product[i][j] = product[i][j] || (left[i][k] && right[k][j]);
				}
			}
		}

		return product;
	}
	
	Relation& Relation::operator*= (const Relation& right){
		
		Long i = find_Long(*this);
		Long j = find_Long(right);	
		*this = *multiplication_table[make_pair(make_pair(this -> domain, this -> codomain), make_pair(right.domain, right.codomain))][i][j];
		return *this;
	}
	
	
	const Relation& operator*(const Relation& left, const Relation& right){
		
		Long i = Relation::find_Long(left), j = Relation::find_Long(right);	
		return *(Relation::multiplication_table[make_pair(make_pair(left.domain, left.codomain), make_pair(right.domain, right.codomain))][i][j]);
	}
	
	
	string Relation::toString() const {
		ostringstream out;
			
		for(Int i = 0; i < this -> size(); ++i){
			
			for(Int j = 0; j < (*this)[i].size() - 1; ++j){
			
				out << (short)(*this)[i][j] << " ";
			}
			out << (short)(*this)[i][(*this)[i].size() - 1] << endl;
		}
		
		return out.str();	
	}
	
	bool Relation::is_a_map() const {
		
		for(Int i = 0; i < this -> size(); ++i){
			Int row = 0;
			for(Int j = 0; j < (*this)[0].size(); ++j){
				row += (Int)(*this)[i][j];
			}
			
			if(row != 1) return false;
		}
				
		return true;
	}
	
	string Relation::toString_Filip() const {
		ostringstream out;
			
		for(Int i = 0; i < this -> size(); ++i){
			
			for(Int j = 0; j < (*this)[i].size(); ++j){
			
				out << (short)(*this)[i][j];
			}
		}
		
		return out.str();	
	}	

	void Relation::generate_orbits(){//metoda niebezpieczna
		for(auto d : Relation::all_partitions){
			//~ cout << d << endl;
			pair < dimensions , dimensions > domain_and_codomain = make_pair(d,d);
			for(Long i = 0; i < (Long)all_relations[domain_and_codomain].size(); ++i){
				//~ cout << "generowanie orbity dla:\n" << (all_relations[domain_and_codomain][i]).toString() << endl;
				(all_relations[domain_and_codomain][i]).generate_orbit();
				//~ cout <<"------------------\n\n";
			}
			
			cout << endl;
		}
	}
	
	void Relation::generate_orbit(){
		
		Relation R = *this; 
		Long begin = 0; bool generated_full_orbit = false;
		container < Relation* > temp;
		
		while(!generated_full_orbit){
			
			temp.push_back(find(R));
			for(Large i = 0; i < (Large)temp.size() -1 ; ++i){
				if(*temp[i] == R){
					begin = i; generated_full_orbit = true; break;
				}
			}
			
			R *= *this;
		}
		//~ for(auto R : temp) cout << R -> toString() << endl;
		
		if(temp.size() == 0 ) throw out_of_range("orbita nie jest pusta!");
		//~ cout << "przepisanie\n";
		orbit.resize(temp.size() - 1 - begin);
		for(Large i = 0; i < (Large)(temp.size() - 1 - begin); ++i){
			orbit[i] = temp[begin + i];
		}
		
		//~ for(auto R : orbit) cout << R -> toString() << endl;	
	}
	
bool Relation::are_isomorphic_thread(const Relation& A, const Relation& B, const Relation& C, const Relation& D){
		
    bool DC_is_trivial = false, CD_is_trivial = false;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // thread 1 checks DC
            for(const auto& An : A.orbit){
                for(const auto& Am : A.orbit){
                    if((D*C) * (*An) == *Am){DC_is_trivial = true; break;}
                }
                if(DC_is_trivial) break;
            }
        }

        #pragma omp section
        {
            // thread 2 checks CD
            for(const auto& Bn : B.orbit){
                for(const auto& Bm : B.orbit){
                    if((C*D) * (*Bn) == *Bm){CD_is_trivial = true; break;}
                }
                if(CD_is_trivial) break;
            }
        }
    }

    if(CD_is_trivial && DC_is_trivial) return true;

    return false;
}

	
	//~ bool Relation::are_isomorphic_thread(const Relation& A, const Relation& B, const Relation& C, const Relation& D){
		
		//~ bool DC_is_trivial = false, CD_is_trivial = false;
		

		//~ for(const auto& An : A.orbit){
		//~ for(const auto& Am : A.orbit){
			
			//~ if((D*C) * (*An) == *Am){DC_is_trivial = true; break;}
		//~ }
		//~ if(DC_is_trivial) break;
		//~ }

		//~ if(!DC_is_trivial) return false;
		
		//~ for(const auto& Bn : B.orbit){
		//~ for(const auto& Bm : B.orbit){
			
			//~ if((C*D) * (*Bn) == *Bm){CD_is_trivial = true; break;}
		//~ }
		//~ if(CD_is_trivial) break;
		//~ }		
			
		//~ if(CD_is_trivial && DC_is_trivial)return true;
		//~ return false;
	//~ }	

	bool Relation::are_isomorphic(const Relation& A, const Relation& B){
		if(A.domain != A.codomain || B.domain != B.codomain) throw invalid_argument("A lub B nie jest endomorfizmem!");

		pair<dimensions, dimensions> C_dimensions = make_pair(A.codomain, B.domain);
		pair<dimensions, dimensions> D_dimensions = make_pair(B.codomain, A.domain);
		bool found_isomorphic = false;

		#pragma omp parallel for shared(A,B,C_dimensions,D_dimensions,found_isomorphic)
		for(Large i = 0; i < (Large)Relation::all_relations[C_dimensions].size(); ++i){

			#pragma omp parallel for shared(A,B,C_dimensions,D_dimensions,found_isomorphic)
			for(Large j = 0; j < (Large)Relation::all_relations[D_dimensions].size(); ++j){




				const auto& C = Relation::all_relations[C_dimensions][i];
				const auto& D = Relation::all_relations[D_dimensions][j];
				if(D * B != A * D) continue;
				if(C * A != B * C) continue;

				if(are_isomorphic_thread(A,B,C,D)){
					#pragma omp critical
					{
						found_isomorphic = true;					
					}
				}
			}
		}

		return found_isomorphic;
	}

	
	//~ bool Relation::are_isomorphic(const Relation& A, const Relation& B){
		//~ if(A.domain != A.codomain || B.domain != B.codomain) throw invalid_argument("A lub B nie jest endomorfizmem!");
			
		//~ pair < dimensions, dimensions > C_dimensions = make_pair(A.codomain, B.domain);
		//~ pair < dimensions, dimensions > D_dimensions = make_pair(B.codomain, A.domain);
		
		//~ for(const auto& C : Relation::all_relations[C_dimensions]){
			//~ if(C * A != B * C) continue;
		//~ for(const auto& D : Relation::all_relations[D_dimensions]){		
			//~ if(D * B != A * D) continue;

			//~ if(are_isomorphic_thread(A,B,C,D)) return true;
		//~ }
		//~ }
		
		//~ return false;
	//~ }
	
	//~ bool Relation::are_isomorphic(const Relation& A, const Relation& B){
		//~ if(A.domain != A.codomain || B.domain != B.codomain) throw invalid_argument("A lub B nie jest endomorfizmem!");
		
		//~ bool DC_is_trivial = false, CD_is_trivial = false;		
		//~ pair < dimensions, dimensions > C_dimensions = make_pair(A.codomain, B.domain);
		//~ pair < dimensions, dimensions > D_dimensions = make_pair(B.codomain, A.domain);
		
		//~ for(const auto& C : Relation::all_relations[C_dimensions]){
			//~ if(C * A != B * C) continue;
		//~ for(const auto& D : Relation::all_relations[D_dimensions]){		
			//~ if(D * B != A * D) continue;
			//~ DC_is_trivial = false, CD_is_trivial = false;

			//~ for(const auto& An : A.orbit){
			//~ for(const auto& Am : A.orbit){
				
				//~ if((D*C) * (*An) == *Am){DC_is_trivial = true; break;}
			//~ }
			//~ if(DC_is_trivial) break;
			//~ }

			//~ if(!DC_is_trivial) continue;
			
			//~ for(const auto& Bn : B.orbit){
			//~ for(const auto& Bm : B.orbit){
				
				//~ if((C*D) * (*Bn) == *Bm){CD_is_trivial = true; break;}
			//~ }
			//~ if(CD_is_trivial) break;
			//~ }		
				
			//~ if(CD_is_trivial && DC_is_trivial) return true;
		//~ }
		//~ }
		
		//~ return false;
	//~ }
	
	void Relation::generate_szymczak_classes(){
	
		for(auto& dimension : Relation::all_partitions){

			pair < dimensions, dimensions > domain_and_codomain = make_pair(dimension, dimension);		
			for(const Relation& R : Relation::all_relations[domain_and_codomain]){
				
				bool szymczak_class_found = false;	
				
				for(auto& szymczak_class : Relation::all_szymczak_classes){
					
					if(szymczak_class.size() == 0) continue;
					
					Long i = 0;
					for(Long j = 0; j < (Long)all_partitions.size(); ++j){
						if(szymczak_class[all_partitions[j]].size() !=0){i = j; break;}
					}
					if(are_isomorphic(R, *(szymczak_class[all_partitions[i]][0]))){
						
						szymczak_class_found = true;
						
						if(szymczak_class[dimension].size() != 0){

							szymczak_class[dimension].push_back(find(R));	
						}

						else{

							container < Relation* > szymczak_class_new_section;

							szymczak_class.insert(	make_pair(	dimension, szymczak_class_new_section)	);
							szymczak_class[dimension].push_back(find(R));
						}
					}
					if(szymczak_class_found) break;
				}
				
				if(!szymczak_class_found){//jeśli R nie jest w żadnej klasie
					
				container < Relation* > new_szymczak_class_section;
				new_szymczak_class_section.push_back(find(R));

				unordered_map < dimensions, container < Relation* >, DimensionHasher > new_szymczak_class;
				new_szymczak_class.insert(make_pair(dimension, new_szymczak_class_section));
				all_szymczak_classes.push_back(new_szymczak_class);
					
				}
			}
		}	
	}
	
	string Relation::output(Long base, Long size){
		
		ostringstream out;
		out << base << " " << size << endl;
		out << "===\n";
		for(auto& szymczak_class : Relation::all_szymczak_classes){
			
			for(auto& dimensions_ : Relation::all_partitions){
				
				out << dimensions_ << endl;
				out << "---\n";
				for(const auto& R : szymczak_class[dimensions_]){
					
					out << R -> toString() << endl;
				};
				
				out << "---\n";
			}
			
			out << "===\n";
		}
		
		return out.str();
	}
	
	string Relation::output_Filip(Long base, Long size){
		
		ostringstream out;
		out << base << " " << size << endl;
		out << "===\n";
		for(auto& szymczak_class : Relation::all_szymczak_classes){
			
			for(auto& dimensions_ : Relation::all_partitions){
				out << "---\n";
				for(const auto& R : szymczak_class[dimensions_]){
					
					out << R -> toString_Filip();
					if(R -> is_a_map()) out << " y\n";
					else out << " n\n";
					
				};
				
				out << "---\n";
			}
			
			out << "===\n";
		}
		
		return out.str();
	}	
	
	
	void Relation::generate(Long base, Long size){
		cout << "base = " << base << " size = " << size << endl;
		cout << "generowanie podziałów\n";
		Relation::generate_all_partitions(base,size);
		cout << "generowanie relacji\n";
		Relation::generate_all_relations();
		cout << "generowanie tabliczki mnożenia\n";
		Relation::generate_multiplication_table();
		cout << "generowanie orbit\n";
		Relation::generate_orbits();
		cout << "generowanie klas\n";
		Relation::generate_szymczak_classes();
		cout << "zakończono generowanie\n";
		
	}
	
	void Relation::reset(){
		
		Relation::all_partitions.clear();
		Relation::all_relations.clear();
		Relation::multiplication_table.clear();
		Relation::all_szymczak_classes.clear();
	}

	container < dimensions > Relation::all_partitions;
	unordered_map < pair < dimensions, dimensions >, container < Relation > , DimensionPairHasher > Relation::all_relations;
	unordered_map < pair < pair < dimensions, dimensions >,  pair < dimensions, dimensions > > , table < Relation* >, DimensionQuadrupleHasher > Relation::multiplication_table;
	container < unordered_map < dimensions , container < Relation* >, DimensionHasher > > Relation::all_szymczak_classes;


