/* TODO:
    - ||done|| lower central calculation
    - ||done|| derived series calculation
    - ||done|| quotient groups
    - group actions
    - representation with trace calculation
    - Tietze transformations
    - Knuth–Bendix completion algorithm
*/

#include <iostream>
#include <set>
#include <optional>


using std::string;
using std::set;


// printing sets
//{
template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::set<T>& s)
{
    os << "{";
    bool first = true;
    for(const auto& elem : s)
    {
        if(!first)
        {
            os << ", ";
        }
        os << elem;
        first = false;
    }
    os << "}";
    return os;
}
//}

#include "word.cpp"

class GroupPresentation
{

public:
    set<Generator> generators;
    set<Relation> relators;

    // Constructors
    //{

    // free group
    GroupPresentation(set<Generator> gen)
    {
        for(const auto& s : gen)
        {
            generators.insert(Generator(s));
        }
    }

    // presentation convention <a,b | ab> implies <a,b | ab = 1>
    GroupPresentation(set<Generator> gen, set<Relation> rel)
    {
        // TODO: include some run time type checks
        // - if rx \in r \in rel then rx \in gen
        for(const auto& s : gen)
        {
            generators.insert(s);
        }
        for(const auto& r : rel)
        {
            relators.insert(r);
        }

    }
    //}

    // NOTE: this only works for single character generators
    bool is_char_inv(char a, char b)
    {
        int d_caps = 'a' -
                     'A'; //distance of capital and lower letters in ASCII

        if(((a - b) == d_caps) or ((b - a) == d_caps))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Word reduce_inverses(Word w)
    {

        // aA = 1 (forall a)
        for(int i = 0 ; i < w.length() - 1 ; i++)
        {
            if(is_char_inv(w[i], w[i + 1]))
            {
                w[i] = ' ';
                w[i + 1] = ' ';
            }
        }
        Word new_w("");
        for(int i = 0 ; i < w.length() ; i++)
        {
            if(w[i] != ' ')
            {
                new_w +=
                    w[i];
            }
        }

        return new_w;
    }


    //bool areEqual(const Word& w1, const Word& w2) const;
    //bool areConjugate(const Word& w1, const Word& w2) const;


    friend std::ostream& operator<<(std::ostream& os,
                                    const GroupPresentation& obj);
};

std::ostream& operator<<(std::ostream& os,
                         const GroupPresentation& obj)
{
    os << "<";

    // Print generators
    bool first = true;
    for(const auto& g : obj.generators)
    {
        if(!first)
        {
            os << ",";
        }
        os << g;
        first = false;
    }

    os << " | ";

    // Print relators (relations)
    first = true;
    for(const auto& r : obj.relators)
    {
        if(!first)
        {
            os << ",";
        }
        os << r;
        first = false;
    }

    os << ">";
    return os;
}


    /* Donald Knuth Completion Algorithm

    s >e l in the encompassment ordering, or
    s and l are literally similar and t > r.

Delete 	 ‹ E∪{s = s} , R › 	       ⊢ ‹ E 	    , R ›
Compose  ‹ E 	     , R∪{s → t} › ⊢ ‹ E 	    , R∪{s → u} › if t ⟶R u
Simplify ‹ E∪{s = t} , R › 	       ⊢ ‹ E∪{s = u}, R › 	      if t ⟶R u
Orient 	 ‹ E∪{s = t} , R › 	       ⊢ ‹ E 	    , R∪{s → t} › if s > t
Collapse ‹ E 	     , R∪{s → t} › ⊢ ‹ E∪{u = t}, R › 	      if s ⟶R u by l → r with (s → t) ▻ (l → r)
Deduce 	 ‹ E 	     , R › 	       ⊢ ‹ E∪{s = t}, R › 	      if (s,t) is a critical pair of R


    */

// But will it build?
//    std::optional<std::pair<Word, Word>> orient(const Word& lhs, const Word& rhs);

//    void delete_trivial_equalities( std::set<std::pair<Word, Word>>& equations);

    void delete_trivial_equalities(
        std::set<std::pair<Word, Word>>& equations)
    {
        for(auto it = equations.begin(); it != equations.end();)
        {
            if(it->first == it->second)
            {
                it = equations.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }



// Quotient on group presentation is adding more relators
GroupPresentation quotient(const GroupPresentation& G,
                           const std::set<Relation>& new_relators)
{
    GroupPresentation result = G;
    result.relators.insert(new_relators.begin(),
                           new_relators.end());
    return result;
}

// The inverse of a word is a^-1 => a, not reversing the string
Word inverse(const Word& w)
{
    std::string rev;
    for(int i = 0 ; i < w.length() ; i++)
    {
        char c = w[i];
        rev += islower(c) ? toupper(c) : tolower(c);
    }
    return Word(rev);
}

Word commutator(const Word& a, const Word& b)
{
    return inverse(a) + inverse(b) + a + b;
}

GroupPresentation commutator(
    const GroupPresentation& A,
    const GroupPresentation& B)
{
    std::set<Relation> new_relators;

    for(const auto& a : A.generators)
    {
        for(const auto& b : B.generators)
        {
            Word comm = commutator(a, b);
            new_relators.insert(Relation(comm));
        }
    }

    return quotient(A, new_relators);
}


GroupPresentation LowerCentralStep(
    const GroupPresentation& A,
    const GroupPresentation& An
)
{
    return commutator(A, An);
}


GroupPresentation derived(const GroupPresentation& G)
{
    return commutator(G, G);
}
