/* TODO:
    - ||done|| lower central calculation
    - ||done|| derived series calculation
    - ||done|| quotient groups
    - group actions
    - representation with trace calculation
    - Tietze transformations
    - Knuth–Bendix completion algorithm
*/

#pragma once
#include <iostream>
#include <set>
#include <optional>
#include <vector>
#include "word.hpp"

using std::set;


// printing sets
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
{
    os << "{";
    bool first = true;
    for(const auto& elem : s)
    {
        if(!first) os << ", ";
        os << elem;
        first = false;
    }
    os << "}";
    return os;
}


class GroupPresentation
{
public:
    set<Generator> generators;
    set<Relation> relators;

    GroupPresentation(set<Generator> gen) : generators(std::move(gen)) {}

    GroupPresentation(set<Generator> gen, set<Relation> rel)
        : generators(std::move(gen)), relators(std::move(rel)) {}

    // NOTE: this only works for single character generators
    bool is_char_inv(char a, char b) const
    {
        int d_caps = 'a' - 'A';
        return ((a - b) == d_caps) || ((b - a) == d_caps);
    }

    Word reduce_inverses(Word w) const
    {
        // aA = 1 (forall a); iterate until no adjacent pair cancels
        bool changed;
        do {
            changed = false;
            for(size_t i = 0; i + 1 < w.length(); i++)
            {
                if(is_char_inv(w[i], w[i + 1]))
                {
                    w[i]     = ' ';
                    w[i + 1] = ' ';
                    changed = true;
                }
            }
            Word new_w("");
            for(size_t i = 0; i < w.length(); i++)
                if(w[i] != ' ') new_w += w[i];
            w = new_w;
        } while(changed);

        return w;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const GroupPresentation& obj);
};

inline std::ostream& operator<<(std::ostream& os, const GroupPresentation& obj)
{
    os << "<";
    bool first = true;
    for(const auto& g : obj.generators)
    {
        if(!first) os << ",";
        os << g;
        first = false;
    }
    os << " | ";
    first = true;
    for(const auto& r : obj.relators)
    {
        if(!first) os << ",";
        os << r;
        first = false;
    }
    os << ">";
    return os;
}


/* Donald Knuth Completion Algorithm
//{

    s >e l in the encompassment ordering, or
    s and l are literally similar and t > r.

Delete   ‹ E∪{s = s} , R ›        ⊢ ‹ E      , R ›
Compose  ‹ E          , R∪{s → t} › ⊢ ‹ E      , R∪{s → u} › if t ⟶R u
Simplify ‹ E∪{s = t} , R ›        ⊢ ‹ E∪{s = u}, R ›          if t ⟶R u
Orient   ‹ E∪{s = t} , R ›        ⊢ ‹ E      , R∪{s → t} ›   if s > t
Collapse ‹ E          , R∪{s → t} › ⊢ ‹ E∪{u = t}, R ›         if s ⟶R u by l → r with (s → t) ▻ (l → r)
Deduce   ‹ E          , R ›        ⊢ ‹ E∪{s = t}, R ›          if (s,t) is a critical pair of R

//}
*/


inline void delete_trivial_equalities(std::set<std::pair<Word, Word>>& equations)
{
    for(auto it = equations.begin(); it != equations.end();)
    {
        if(it->first == it->second)
            it = equations.erase(it);
        else
            ++it;
    }
}

using RewriteRule = std::pair<Word, Word>; // s → t

inline Word reduce(const Word& w, const std::set<RewriteRule>& rules)
{
    Word current = w;
    bool changed;

    do {
        changed = false;
        for(const auto& [lhs, rhs] : rules)
        {
            size_t pos = current.find(lhs);
            if(pos != std::string::npos)
            {
                current = current.erase(pos, lhs.length()).insert(pos, rhs);
                changed = true;
                break; // restart left-to-right
            }
        }
    } while(changed);

    return current;
}

inline bool compose(std::set<RewriteRule>& R, const RewriteRule& rule)
{
    const auto& [s, t] = rule;
    Word u = reduce(t, R);

    if(u == t) return false;

    R.erase(rule);
    R.insert({s, u});
    return true;
}

inline bool simplify(std::set<std::pair<Word, Word>>& E,
                     const std::set<std::pair<Word, Word>>& R,
                     const std::pair<Word, Word>& eq)
{
    const auto& [s, t] = eq;
    Word u = reduce(t, R);

    if(u == t) return false;

    E.erase(eq);
    E.insert({s, u});
    return true;
}

inline bool orient(std::set<std::pair<Word, Word>>& E,
                   std::set<std::pair<Word, Word>>& R,
                   const std::pair<Word, Word>& eq)
{
    const auto& [s, t] = eq;

    if(s == t) return false;

    Word lhs = s;
    Word rhs = t;
    if(t > s) std::swap(lhs, rhs);

    E.erase(eq);
    R.insert({lhs, rhs});
    return true;
}

inline bool collapse(std::set<std::pair<Word, Word>>& E,
                     std::set<std::pair<Word, Word>>& R,
                     const std::pair<Word, Word>& rule)
{
    const auto& [s, t] = rule;

    std::set<std::pair<Word, Word>> R_prime = R;
    R_prime.erase(rule);

    Word u = reduce(s, R_prime);

    if(u == s) return false;

    R.erase(rule);
    E.insert({u, t});
    return true;
}

inline std::vector<Word> find_overlaps(const Word& lhs1, const Word& lhs2)
{
    std::vector<Word> overlaps;
    std::string s1 = lhs1.str();
    std::string s2 = lhs2.str();

    for(size_t i = 1; i < s1.size(); ++i)
    {
        std::string suffix = s1.substr(i);
        // check if s2 starts with suffix
        if(s2.rfind(suffix, 0) == 0)
        {
            std::string overlap_str = s1 + s2.substr(suffix.length());
            overlaps.emplace_back(overlap_str);
        }
    }

    return overlaps;
}


inline GroupPresentation quotient(const GroupPresentation& G,
                                  const std::set<Relation>& new_relators)
{
    GroupPresentation result = G;
    result.relators.insert(new_relators.begin(), new_relators.end());
    return result;
}

inline Word inverse(const Word& w)
{
    std::string result;
    for(int i = (int)w.length() - 1; i >= 0; i--)
    {
        char c = w[i];
        result += islower(c) ? toupper(c) : tolower(c);
    }
    return Word(result);
}

inline Word commutator(const Word& a, const Word& b)
{
    return inverse(a) + inverse(b) + a + b;
}

inline GroupPresentation commutator(const GroupPresentation& A,
                                    const GroupPresentation& B)
{
    std::set<Relation> new_relators;
    for(const auto& a : A.generators)
        for(const auto& b : B.generators)
            new_relators.insert(Relation(commutator(a, b)));

    return quotient(A, new_relators);
}

inline GroupPresentation LowerCentralStep(const GroupPresentation& A,
                                          const GroupPresentation& An)
{
    return commutator(A, An);
}

inline GroupPresentation derived(const GroupPresentation& G)
{
    return commutator(G, G);
}

inline void print_rules(const std::set<std::pair<Word, Word>>& rules)
{
    for(const auto& [lhs, rhs] : rules)
        std::cout << lhs.str() << " → " << rhs.str() << '\n';
}
