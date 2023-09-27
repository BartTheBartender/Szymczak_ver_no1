#pragma once

#include "Groups.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <functional>
//#include <omp.h>
#include <chrono>

class Relation;
using Large = uint_fast32_t;
using dimensions = groupRanks;

template<typename T1_0>
using matrix = holder<holder<T1_0>>;

template<typename T1_1>
using table = container<container<T1_1>>;

struct DimensionHasher {
    std::size_t operator()(const dimensions& d) const {
        std::size_t seed = d.size();
        for (const Int& val : d) {
            seed ^= std::hash<Int>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct DimensionPairHasher {
    std::size_t operator()(const std::pair<dimensions, dimensions>& pair) const {
        std::size_t seed = 0;
        seed ^= DimensionHasher()(pair.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= DimensionHasher()(pair.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct DimensionQuadrupleHasher {
    std::size_t operator()(const std::pair<std::pair<dimensions, dimensions>, std::pair<dimensions, dimensions>>& pair) const {
        std::size_t seed = 0;
        seed ^= DimensionPairHasher()(pair.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= DimensionPairHasher()(pair.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

class Relation : public matrix<bool> {
public:
	
	dimensions domain;
	dimensions codomain;
	container < Relation* > orbit;
	
    std::size_t hash() const;
    
    static container < dimensions >	all_partitions;
	static std::unordered_map < std::pair < dimensions, dimensions >, container < Relation > , DimensionPairHasher > all_relations;
	static std::unordered_map < std::pair < std::pair < dimensions, dimensions >,  std::pair < dimensions, dimensions > > , table < Relation* >, DimensionQuadrupleHasher > multiplication_table;
	static container < std::unordered_map < dimensions, container < Relation* >, DimensionHasher > > all_szymczak_classes;
	
	Relation(const group& G, const dimensions& domain, const dimensions& codomain, const dimensions& domain_prod, const dimensions& codomain_prod, const dimensions& merged);
	
	Relation(const Relation& source);
	
	bool operator == (const Relation& source) const;
	
	bool operator != (const Relation& source) const;
	
	Relation& operator= (const Relation& source);
	
	Relation(const dimensions& domain, const dimensions& codomain, const Long rows, const Long columns);

	static void generate_partitions(container < dimensions >& partitions, Long base, Long size, dimensions partition, Long index);
	
	static container < dimensions > generate_partitions(Long base, Long size);
		
	static void generate_all_partitions(Long base, Long size);
	
	static void generate_all_relations();
	
	static container < Relation > generate_relations(const dimensions& domain, const dimensions& codomain);
	
	static Relation* find(const Relation& R);
	
	static Long find_Long(const Relation& R);
	
	static table < Relation* > generate_multiplication_table(const dimensions& left_domain, const dimensions& left_codomain, const dimensions& right_domain, const dimensions& right_codomain);
	
	static void generate_multiplication_table();
	
	Relation& operator*= (const Relation& right);
	
	friend Relation operator*(const Relation& left, const Relation& right);
	
	std::string toString() const;
	
	std::string toString_python() const;	

	bool is_a_map() const;

	static void generate_orbits(); //metoda niebezpieczna
	
	void generate_orbit();
	
	static bool are_isomorphic_thread(const Relation& A, const Relation& B, const Relation& C, const Relation& D);
	
	static bool are_isomorphic(const Relation& A, const Relation& B);
	
	static void generate_szymczak_classes();
	
	static std::string toString(const dimensions& dimensions_);
	
	static std::string output(Long base, Long size);

	static std::string output_python(Long base, Long size);
	
	static void generate(Long base, Long size);
	
	static void reset();
};


