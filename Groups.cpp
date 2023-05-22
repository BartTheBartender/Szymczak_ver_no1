#include "Groups.h"

using namespace std;

#include <iostream>
#include <vector>

using Int = uint_fast8_t;
using Long = uint_fast32_t;

using namespace std;

template<typename T0_0>
using holder = vector<T0_0>;

template<typename T0_1>
using container = vector<T0_1>;



using element = holder<Int>;
using group = holder<element>;
using groupRanks = holder<Int>;

using coset = holder<element>;
using quotientGroup = holder<coset>;
	

ostream& operator <<(ostream& out, const element& g) {
	out << "(";
	for (Int i = 0; i < (Int)g.size() - 1; ++i) out << (short)g[i] << ",";
	if(!g.empty()) out << (short)g.back();
	out << ")";
	return out;
}

ostream& operator <<(ostream& out, const group& G) {
	for(element g : G) out << g << " ";
	return out;
}

ostream& operator <<(ostream& out, const quotientGroup& Q) {
	for(coset G_ : Q) out << "[" << G_ << "]" << endl;
	return out;
}

bool operator==(const coset& A, const coset& B){
	
	bool isFound;
	for(element a : A){
		isFound = false;
		
		for(element b : B){
			if(b == a){
			isFound = true; break;}
		}
		
		if(!isFound)
		return false;
	}
	
	return true;
}

bool operator !=(const coset& A, const coset &B){
	return !(A==B);
}
	
Int GroupTheory::gcd(const Int a, const Int b){
	Int r, u=a, v=b;
	while(true){
		if(v==0) return u;
		r=u%v;
		if(r==0) return v;
		u=v;
		v=r;
	}
}

holder<Int> GroupTheory::sigma(Int n){
	holder<Int>sigma;
	for(Int i=1; i<=n; ++i){
		if(n%i==0) sigma.push_back(i);
	}
	return sigma;
}
	
holder<Int> GroupTheory::phi(Int n){
	holder<Int> result;
	if(n == 1){result.push_back(0); return result;}
	
	for(Int i = 1; i < n; ++i){
		if(gcd(i,n) == 1) result.push_back(i);
	}

  return result;
}
	

Long GroupTheory::gcd(const Long a, const Long b){
	Long r, u=a, v=b;
	while(true){
		if(v==0) return u;
		r=u%v;
		if(r==0) return v;
		u=v;
		v=r;
	}
}

holder<Long> GroupTheory::sigma(Long n){
	holder<Long>sigma;
	for(Long i=1; i<=n; ++i){
		if(n%i==0) sigma.push_back(i);
	}
	return sigma;
}
	

holder<Long> GroupTheory::phi(Long n){
	holder<Long> result;

	if(n == 1){result.push_back(0); return result;}
	
	for(Long i = 1; i < n; ++i){
		if(gcd(i,n) == 1) result.push_back(i);
	}

  return result;
}	

	
element GroupTheory::sum(const element& a, const element& b, groupRanks ranks){
	if(a.size() != b.size())throw invalid_argument("Nie można dodać elementów różnej wielkości\n");
	element c(a.size());
	for(Int i = 0; i<(Int)a.size(); ++i){c[i] = a[i]+b[i]; if(c[i]>=ranks[i]) c[i]-=ranks[i];}
	return c;
}
	
coset GroupTheory::sum(const coset& a, const coset& b, groupRanks ranks){
	if(a.size() != b.size())throw invalid_argument("Nie można dodać cosetów różnej wielkości\n");
	
	coset c;
	element s; bool isFound;
	for(Long i = 0; i<(Long)a.size(); ++i){
	for(Long j = 0; j<(Long)b.size(); ++j){
		s = sum(a[i], b[j], ranks);

		isFound = false;
		
		for(element s_ : c){
			if(s_ == s){isFound = true; break;}
		}
		if(!isFound) c.push_back(s);
	}
	}
	
	return c;
}	
	
void GroupTheory::generateGroup(const groupRanks& ranks, group& G, element g, Int index){
	if(index != (Int)ranks.size()){
		for(Int r = 0; r < ranks[index]; ++r){
			g.push_back(r);

			GroupTheory::generateGroup(ranks, G, g, index+1);
			g.pop_back();
		}
	}
	else G.push_back(g);
	
}
	
group GroupTheory::generateGroup(const groupRanks& ranks){
	group G;
	element g;
	GroupTheory::generateGroup(ranks, G, g, 0);
	
	return G;
}
	
group GroupTheory::maxElement(const group& G, groupRanks ranks){
	group a;
	group max;
	if(G.size() == 0)throw invalid_argument("Nie można znaleźć największego elementu w pustej grupie\n");

	element e(G[0].size());
	element h;
	for(element g : G){

		a.clear();
		h = e;
		do{
			a.push_back(h);
			h = GroupTheory::sum(h,g,ranks);
		}while(h != e);
		
		if(a.size() == G.size()){return a;}
		if(a.size() > max.size()){max = a;}
	}
	return max;
	
}
	
quotientGroup GroupTheory::maxElement(const quotientGroup& G, groupRanks ranks){

	if(G.size() == 0)throw invalid_argument("Nie można znaleźć największego elementu w pustej grupie ilorazowej\n");

	coset e = G[0];
	quotientGroup a;
	quotientGroup max; max.push_back(e);
	coset h;
	
	for(coset g : G){
		a.clear();
		h = e;
		
		do{
			a.push_back(h);
			h = GroupTheory::sum(h,g,ranks);
		}while(h != e);

		
		if(a.size() == G.size()){

			return a;
		}
		if(a.size() > max.size()){max = a;}
	}
	
	return max;
	
}

group GroupTheory::closure(group& B, const groupRanks ranks){
	
	if(B.size() == 0) throw invalid_argument("Nie da sie domknąć pustej grupy!\n");
	bool isGroup = false, isFound = false; element s = {};
	while(!isGroup){
		isGroup = true;
		
		for(Long i = 0; i < (Long)B.size(); ++i){
		for(Long j = 0; j < (Long)B.size(); ++j){
			
			isFound = false;
			s = GroupTheory::sum(B[i], B[j], ranks);
			
			for(element g : B){
				if(g == s){
					isFound = true; break;
				}
			}
			
			if(!isFound){
				B.push_back(s); isGroup=false; break;
			}
		}
		}
	}
	
	return B;
}

quotientGroup GroupTheory::quotient(const group& G, const group& H, const groupRanks& ranks){
	quotientGroup Q;
	bool isFound;
	for(element g : G){
		isFound = false;
		
		for(coset H_ : Q){
			for(element h : H_){
				if(g == h){isFound = true; break;}
			}
			if(isFound) break;
		}
		
		if(!isFound){
			coset H_;
			for(element h : H){
				H_.push_back(sum(g,h,ranks));
			}
			Q.push_back(H_);
		}
	}
	
	return Q;
}

group GroupTheory::split(group G, const group& A, const groupRanks& ranks){
	element a;
	Long j; auto begin = G.begin(); 
	
	for(Long i=0; i<(Long)A.size(); ++i){
		a = A[i]; j=0; 
		while(j<(Long)G.size()){
			if(a == G[j]){G.erase(begin+j);}
			++j;
		}
	}		
	group B;
	element e(A[0].size());
	B.push_back(e);
	
	bool fullOrbit, isLinearlyIndependent;
	element g, h;

	
	group C;

	holder<Long> indicesToBeRemoved; begin = G.begin();

	j=0;
	while(j<(Long)G.size()){
		
		g=G[j];
		isLinearlyIndependent = true;
		if(B.size() != 1){
			for(element a : A){
			for(element b : B){
				if(g == GroupTheory::sum(a,b,ranks)){
					isLinearlyIndependent = false;

					G.erase(begin+j);
					begin = G.begin();
					break;
				}
					
			}
					if(!isLinearlyIndependent)break;
			}
		}
		
		

		if(isLinearlyIndependent){
			
			fullOrbit = true;
			h = g;
			
			while(h != e){

				fullOrbit = false;
				for(Long i = 0; i<(Long)G.size(); ++i){
					if(G[i] == h){
						indicesToBeRemoved.push_back(i); 
						C.push_back(h);
						
						fullOrbit = true;
						break;
					}
				}
				
				if(!fullOrbit) break;	
				
				h = GroupTheory::sum(h,g,ranks);
			}
			
			if(fullOrbit){
				B.insert(B.end(), C.begin(), C.end());
				
				begin = G.begin();
				for(Long i=(Long)indicesToBeRemoved.size()-1; i != (Long)-1; --i){
					G.erase(begin + indicesToBeRemoved[i]);
				}
				
			}
			
			begin = G.begin();
			C.clear();
			indicesToBeRemoved.clear();
			
		}
		
		++j;
		closure(B, ranks);
	}

	return B;
}
	
group GroupTheory::epimorphismToSubgroup(const group& A, const group& B, const quotientGroup& Q, const coset& p, const groupRanks& ranks){
			  
	group G;

	for(const auto& b : B){

		coset bC_;	
		bool isFound = false;
		
		for(coset q_ : Q){

			for(const auto& c : q_){

				if(c == b){
					bC_ = q_;

					isFound = true; break;
				}
			}
			if(isFound) break;
			
		}
		
		coset q = Q[0];
		for(const auto& a : A){
			
			if(q == bC_){

				G.push_back(GroupTheory::sum(a,b,ranks));
			}
			q = GroupTheory::sum(q, p, ranks);
		}
		
			
	}
	return G;
}

//~ group GroupTheory::epimorphismToSubgroup(const group& A, const element& a_, const group& B, const quotientGroup& Q, const coset& q_, const groupRanks& ranks){

	//~ group G;
	//~ element a = a_;
	//~ coset q = q_;
	
	//~ while(a != A[0]){
		
		//~ for(auto b : q){
			//~ G.push_back(GroupTheory::sum(a,b,ranks));
		//~ }
		
		//~ q = GroupTheory::sum(q, q_, ranks);
		//~ a = GroupTheory::sum(a, a_, ranks);
		
	//~ }
	
		//~ for(auto b : q){
			//~ G.push_back(GroupTheory::sum(a,b,ranks));
		//~ }
		
		//~ q = GroupTheory::sum(q, q_, ranks);
		//~ a = GroupTheory::sum(a, a_, ranks);
	
	//~ return G;
//~ }
	
container<group> GroupTheory::goursat(const group& A, const group& B, const groupRanks& ranks){
	container<group> subgroups;
	container<group> subgroupsOfB;
	container<group> subgroupsOfA;
	
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			subgroupsOfB = generateSubgroups(B, ranks);
		}
		
		#pragma omp section
		{
			subgroupsOfA = generateSubgroups(A, ranks);
		}
	}
	
		for(const auto& B_ : subgroupsOfB){

			container<group> subgroupsOfB_ = subgroupsViaSuperGroup(B_, subgroupsOfB);
			
		for(const auto& A_ : subgroupsOfA){
			
			Long domainOrder = A_.size();
			
			for(const auto& C_ : subgroupsOfB_){
				
				Long codomainOrder = (Long) (B_.size()/C_.size());
				if(domainOrder%codomainOrder != 0) continue;
				
				quotientGroup Q_ = quotient(B_, C_, ranks);
				quotientGroup q = maxElement(Q_, ranks);
				if((Long) q.size() != codomainOrder ) continue;

				
				if(domainOrder == 1){
					subgroups.push_back(epimorphismToSubgroup(A_, B_, q,q[0],ranks));
				}
				
				else{
					
					for(Long i : phi(codomainOrder)){
						subgroups.push_back(epimorphismToSubgroup(A_, B_,q,q[i],ranks)); //każdy homomorfizm
					}
				}				
			}
		}
		}
	
	return subgroups;
}
	
container<group> GroupTheory::generateSubgroups(const group& G, const groupRanks& ranks){;
	group A = GroupTheory::maxElement(G, ranks);
	
	if(A.size() == G.size()){
		container<group> subgroups;
		for(Long div: GroupTheory::sigma(G.size())){
			group H;
			for(Long k = 0; k != (Long)G.size(); k+=div) H.push_back(A[k]);
			subgroups.push_back(H);
			
		}
		return subgroups;
	}
	group B = GroupTheory::split(G, A, ranks);
	return GroupTheory::goursat(A, B, ranks);
}


group GroupTheory::cap(const group& A, const group& B){

	group C;
	
	for(const auto& a : A){
	
		for(const auto& b : B){
		
			if(a == b){C.push_back(a); break;}
		}
	}
	
	return C;
}

container<group> GroupTheory::subgroupsViaSuperGroup(const group&C, const container<group> subgroupsOfSuperGroup){
	
	container<group> subgroups;
	
	for(const auto& H: subgroupsOfSuperGroup){
	
		group G = cap(C, H); bool isFound = false;
		
		for(const auto& K : subgroups){
			if(K == G){isFound = true; break;}
		}
		
		if(!isFound){subgroups.push_back(G);}
	}
	
	return subgroups;
}
