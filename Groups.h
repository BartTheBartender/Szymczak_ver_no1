#pragma once

#include <iostream>
#include <vector>

using Int = uint_fast8_t;
using Long = uint_fast32_t;

template<typename T0_0>
using holder = std::vector<T0_0>;

template<typename T0_1>
using container = std::vector<T0_1>;

template<typename T0_2>
using matrix = holder<holder<T0_2>>;

template<typename T0_3>
using table = container<container<T0_3>>;

using element = holder<Int>;
using group = holder<element>;
using groupRanks = holder<Int>;

using coset = holder<element>;
using quotientGroup = holder<coset>;

class GroupTheory{
public:

	friend class Relation;
	
	~GroupTheory() = default;
	
	static Int gcd(const Int a, const Int b);
	
	static holder<Int> sigma(Int n);
	
	static holder<Int> phi(Int n);	
	
	static Long gcd(const Long a, const Long b);
	
	static holder<Long> sigma(Long n);
	
	static holder<Long> phi(Long n);

//-------------------------------------------------------------------------------------------	
	static element sum(const element& a, const element& b, groupRanks ranks);
	
	static coset sum(const coset& a, const coset& b, groupRanks ranks);
	
	static void generateGroup(const groupRanks& ranks, group& G, element g, Int index);
	
	static group generateGroup(const groupRanks& ranks);
		
	static group maxElement(const group& G, groupRanks ranks);
	
	static quotientGroup maxElement(const quotientGroup& G, groupRanks ranks);

	static group closure(group& B, const groupRanks ranks);
	
	static quotientGroup quotient(const group& G, const group& H, const groupRanks& ranks);

	static group split(group G, const group& A, const groupRanks& ranks);
	
	static group epimorphismToSubgroup(const group& A, const group& B,	const quotientGroup& Q, const coset& p, const groupRanks& ranks);
	
	static container<group> goursat(const group& A, const group& B, const groupRanks& ranks);
	
	static container<group> generateSubgroups(const group& G, const groupRanks& ranks);
	
	static group cap(const group& A, const group& B);
	
	static container<group> subgroupsViaSuperGroup(const group&C, const container<group> subgroupsOfSuperGroup);
};

	std::ostream& operator <<(std::ostream& out, const element& g);

	std::ostream& operator <<(std::ostream& out, const group& G);

	std::ostream& operator <<(std::ostream& out, const quotientGroup& Q);

	bool operator==(const coset& A, const coset& B);

	bool operator !=(const coset& A, const coset &B);
