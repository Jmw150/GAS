#include "../src/group_presentation.cpp"
#define CATCH_CONFIG_MAIN
#include "../src/catch.hpp" 

TEST_CASE("Word inverse simplification works", "[word]") {
    Word w("aA");
    Word reduced = GroupPresentation({}).reduce_inverses(w);
    REQUIRE(reduced.str() == "");  // empty string after reduction
}

TEST_CASE("Commutator is correct", "[group]") {
    Word a("a"), b("b");
    Word comm = commutator(a, b);
    REQUIRE(comm.str() == "ABab");  // example value
}


TEST_CASE("Word construction and printing") {
    Word w("l ov    \t\ne");
    std::ostringstream oss;
    oss << w;
    REQUIRE(oss.str() == "love");
}

TEST_CASE("Generator printing") {
    Generator g("cat generator");
    std::ostringstream oss;
    oss << g;
    REQUIRE(oss.str() == "catgenerator");
}

TEST_CASE("Free group presentation printing") {
    GroupPresentation g({"cat"});
    std::ostringstream oss;
    oss << g;
    REQUIRE(oss.str() == "<cat | >");
}

TEST_CASE("Presentation with relation printing") {
    GroupPresentation p({"a", "b"}, {"abAB"});
    std::ostringstream oss;
    oss << p;
    REQUIRE(oss.str() == "<a,b | abAB>");
}

TEST_CASE("Derived group presentation") {
    // free group of one variable "cat"
    GroupPresentation g({"cat"});

    // attempt at derived series
    GroupPresentation Gn = g;
    for(int i = 0; i < 2; i++)
    {
        Gn = derived(Gn);
    }
    std::ostringstream oss;
    oss << Gn;
    REQUIRE(oss.str() == "<cat | CATCATcatcat>"); // You may want to generalize this later
}

TEST_CASE("Inverse of Word") {
    Word w("aAbBB");
    Word result = inverse(w);
    REQUIRE(result.str() == "AaBbb");
}

TEST_CASE("Reduction by single pass") {
    GroupPresentation p({"a", "b"}, {"abAB"});
    Word w("abABaaaa");
    Word reduced = p.reduce_singlepass(w);
    REQUIRE(reduced.str() == "aaaa"); 
}


