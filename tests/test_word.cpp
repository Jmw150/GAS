#include "../src/group_presentation.cpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/doctest.hpp"

TEST_CASE("Word reduction removes adjacent inverse pairs")
{
    Word w("aA");
    Word reduced = GroupPresentation({}).reduce_inverses(w);
    CHECK(reduced.str() == "");
}

TEST_CASE("Word construction strips formatting whitespace")
{
    Word w("l ov    \t\ne");
    std::ostringstream oss;
    oss << w;
    CHECK(oss.str() == "love");
}

TEST_CASE("Generator accepts single character names")
{
    Generator g("c");
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "c");
}

TEST_CASE("Free group presentation prints generators")
{
    GroupPresentation g(set{Generator("a"), Generator("b")});
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "<a,b | >");
}

TEST_CASE("Inverse flips case for each letter")
{
    Word w("aAbBB");
    Word result = inverse(w);
    CHECK(result.str() == "AaBbb");
}

TEST_CASE("Derived presentation on two generators has four commutator relators")
{
    GroupPresentation g(set{Generator("a"), Generator("b")});
    GroupPresentation d = derived(g);

    CHECK(d.generators == g.generators);
    CHECK(d.relators.size() == 4);
    CHECK(d.relators.find(Relation("AAaa")) != d.relators.end());
    CHECK(d.relators.find(Relation("ABab")) != d.relators.end());
    CHECK(d.relators.find(Relation("BAba")) != d.relators.end());
    CHECK(d.relators.find(Relation("BBbb")) != d.relators.end());
}
