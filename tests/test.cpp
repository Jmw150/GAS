#include "../src/group_presentation.cpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/doctest.hpp"


// Word properties
//{

TEST_CASE("Word construction from string, and printing")
{
    Word w("l ov    \t\ne");
    std::ostringstream oss;
    oss << w;
    CHECK(oss.str() == "love");
}
TEST_CASE("Word construction when empty")
{
    Word w("");
    std::ostringstream oss;
    oss << w;
    CHECK(oss.str() == "");
}
TEST_CASE("Word construction when char input")
{
    Word w('a');
    std::ostringstream oss;
    oss << w;
    CHECK(oss.str() == "a");
}

TEST_CASE("Word length")
{
    Word w("12345");
    CHECK(w.length() == 5);

    Word i("");
    CHECK(i.length() == 0);
}

TEST_CASE("Word.inverse")
{
    Word w("ABCabc");

    CHECK((w.inverse()).str() == "abcABC");
}

TEST_CASE("Word < comparison")
{
    Word a("a");
    Word b("ab");

    CHECK((a < b) == true);
    CHECK((a > b) == false);
}

TEST_CASE("Word == comparison")
{
    Word a("a");
    Word b("ab");
    Word c("ab");

    CHECK((a == b) == false);
    CHECK((b == c) == true);
    CHECK((a != b) == true);
}

TEST_CASE("Word addition")
{
    Word a("a"), b("b");

    CHECK((a + b).str() == "ab");
}

TEST_CASE("Word addition with other objects")
{
    Word a("a");

    CHECK((a + "b").str() == "ab");
    CHECK(("b" + a).str() == "ba");
}

TEST_CASE("Word access")
{
    Word w("Hello Word");

    CHECK(w[0] == 'H');
    CHECK(w[w.length() - 1] == 'd');

}

TEST_CASE("Word += operator")
{
    Word w("");
    w += 'a';
    CHECK(w.str() == "a");
    w += "b";
    CHECK(w.str() == "ab");
    Word c("c");
    w += c;
    CHECK(w.str() == "abc");
}

TEST_CASE("Word find function")
{
    Word w("123needle in a haystack");
    Word n("needle");
    CHECK(w.find(n) == 3);
    CHECK(w.find("needle") == 3);
}

TEST_CASE("Word erase function")
{
    Word w("The answer is REDACTED");
    int pos = w.find("REDACTED");

    // Don't forget Words cut whitespace, for sake of algebra
    CHECK(w.erase(pos, w.length() - 1).str()  == "Theansweris");

}

TEST_CASE("Word letter replacement")
{
    Word w("abcdefgg");

    CHECK(w.replace(0, 3, "123").str() == "123defgg");
    CHECK(w.replace(0, 0, "HELLO").str() == "HELLO123defgg");
}

TEST_CASE("Word to substring")
{
    Word w("What do you mean?");

    CHECK(w.substr(0, 4) == "What");

    Word what("What");
    CHECK(w.subword(0, 4).str() == "What");
}
//}

TEST_CASE("Generator from string")
{
    Generator g("c");
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "c");
}

TEST_CASE("Generator from char")
{
    Generator g('c');
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "c");
}

TEST_CASE("Relator printing")
{
    Relation g("cat relator");
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "catrelator");
}

// Group Presentation stuff
//{
TEST_CASE("Free group presentation printing")
{
    GroupPresentation g(set{Generator("c")});
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "<c | >");
}

TEST_CASE("Presentation with relation printing")
{
    GroupPresentation p(set{Generator("a"), Generator("b")}, set{Relation("abAB")});
    std::ostringstream oss;
    oss << p;
    CHECK(oss.str() == "<a,b | abAB>");
}

//}





TEST_CASE("Presentation with relation printing: 2")
{
    GroupPresentation p(
        set {
            Generator("a"),
            Generator("b")},
        set{Relation("abAB"),
            Relation("aaa")});

    std::ostringstream oss;
    oss << p;
    CHECK(((oss.str() == "<a,b | abAB,aaa>")
           or
           (oss.str() == "<a,b | aaa,abAB>")) == true);
}

TEST_CASE("Inverse of Word")
{
    Word w("aAbBB");
    Word result = inverse(w);
    CHECK(result.str() == "AaBbb");
}

TEST_CASE("Word Commutator")
{
    Word a("a"), b("b");
    Word comm = commutator(a, b);
    CHECK(comm.str() == "ABab");  
}

TEST_CASE("Derived group presentation")
{
    // free group of one variable "cat"
    GroupPresentation g(
    {
        Generator("b"),
        Generator("a")
    });

    GroupPresentation Gn = g;
    for(int i = 0; i < 2; i++) // saturates
    {
        Gn = derived(Gn);
    }
    std::ostringstream oss;
    oss << Gn;
    CHECK(oss.str() ==
          "<a,b | AAaa,ABab,BAba,BBbb>"); 
}

TEST_CASE("Lower central group presentation")
{
    // free group of one variable "cat"
    GroupPresentation G(
    {
        Generator("b"),
        Generator("a")
    });

    GroupPresentation Gn = G;
    for(int i = 0; i < 2; i++) // saturates
    {
        Gn = LowerCentralStep(G, Gn);
    }
    std::ostringstream oss;
    oss << Gn;
    CHECK(oss.str() ==
          "<a,b | AAaa,ABab,BAba,BBbb>"); 
}

TEST_CASE("Word inverse simplification works")
{
    Word w("aA");
    Word reduced = GroupPresentation({}).reduce_inverses(w);
    CHECK(reduced.str() == "");  // empty string after reduction
}

/*
TEST_CASE("Orient simple word pairs") 
{
    Word a("xyz"), b("xy");

    auto oriented = orient(a, b);
    REQUIRE(oriented.has_value());
    CHECK(oriented->first.str() == "xyz");
    CHECK(oriented->second.str() == "xy");

    Word same("abc");
    CHECK(!orient(same, same).has_value());
}
*/

TEST_CASE("Delete trivial equalities") 
{
    std::set<std::pair<Word, Word>> E = {
        {Word("abc"), Word("abc")},
        {Word("a"), Word("b")},
        {Word("xyz"), Word("xyz")}
    };

    delete_trivial_equalities(E);

    // Should only keep the non-trivial one
    CHECK(E.size() == 1);
    auto remaining = *E.begin();
    CHECK(remaining.first.str() == "a");
    CHECK(remaining.second.str() == "b");
}


