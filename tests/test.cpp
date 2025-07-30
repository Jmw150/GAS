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

TEST_CASE("Word insert function") 
{
    Word w("abc");
    Word x("X");

    SUBCASE("Insert in middle") {
        Word w1 = w;
        w1.insert(1, x);  // aXbc
        CHECK(w1.str() == "aXbc");
    }

    SUBCASE("Insert at beginning") {
        Word w2 = w;
        w2.insert(0, x);  // Xabc
        CHECK(w2.str() == "Xabc");
    }

    SUBCASE("Insert at end") {
        Word w3 = w;
        w3.insert(w3.length(), x);  // abcX
        CHECK(w3.str() == "abcX");
    }

    SUBCASE("Insert empty word") {
        Word w4 = w;
        Word empty("");
        w4.insert(1, empty);  // no change
        CHECK(w4.str() == "abc");
    }

    SUBCASE("Insert past end clamps") {
        Word w5("hi");
        w5.insert(99, Word("Z"));
        CHECK(w5.str() == "hiZ");
    }
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

TEST_CASE("Compose step rewrites rhs using existing rules") {
    Word s("abc");
    Word t("xY");

    std::set<RewriteRule> R = {
        {Word("Y"), Word("Z")}  // Y → Z
    };

    R.insert({s, t}); // abc → xY

    bool changed = compose(R, {s, t});

    CHECK(changed == true);
    CHECK(R.find({s, Word("xZ")}) != R.end());
}

TEST_CASE("Simplify equation using existing rewrite rules") {
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("XY")}
    };

    std::set<std::pair<Word, Word>> R = {
        {Word("Y"), Word("Z")}
    };

    bool changed = simplify(E, R, {Word("a"), Word("XY")});

    CHECK(changed == true);
    CHECK(E.find({Word("a"), Word("XZ")}) != E.end());
}

TEST_CASE("Orient equation into rewrite rule when lhs > rhs") {
    std::set<std::pair<Word, Word>> E = {
        {Word("xyz"), Word("xy")}
    };

    std::set<std::pair<Word, Word>> R;

    bool changed = orient(E, R, {Word("xyz"), Word("xy")});

    CHECK(changed == true);
    CHECK(E.empty());
    CHECK(R.find({Word("xyz"), Word("xy")}) != R.end());
}

TEST_CASE("Orient flips direction if rhs > lhs") {
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("abc")}
    };

    std::set<std::pair<Word, Word>> R;

    bool changed = orient(E, R, {Word("a"), Word("abc")});

    CHECK(changed == true);
    CHECK(R.find({Word("abc"), Word("a")}) != R.end());
}

TEST_CASE("Orient does nothing for s == t") {
    std::set<std::pair<Word, Word>> E = {
        {Word("same"), Word("same")}
    };

    std::set<std::pair<Word, Word>> R;

    bool changed = orient(E, R, {Word("same"), Word("same")});

    CHECK(changed == false);
    CHECK(E.find({Word("same"), Word("same")}) != E.end());
    CHECK(R.empty());
}

TEST_CASE("Collapse rewrites lhs and moves rule to equation") {
    std::set<std::pair<Word, Word>> E;
    std::set<std::pair<Word, Word>> R = {
        {Word("AA"), Word("A")},       // used to reduce
        {Word("AAA"), Word("b")}       // candidate for collapse
    };

    bool changed = collapse(E, R, {Word("AAA"), Word("b")});

    CHECK(changed == true);
    CHECK(R.find({Word("AAA"), Word("b")}) == R.end());   // rule removed
    CHECK(E.find({Word("A"), Word("b")}) != E.end());     // u = t added
}

// This fails
TEST_CASE("Collapse does nothing if no reduction") {
    std::set<std::pair<Word, Word>> E;
    std::set<std::pair<Word, Word>> R = {
        {Word("abc"), Word("bcd")}     // not reducible
    };

    bool changed = collapse(E, R, {Word("xyz"), Word("t")});

    CHECK(changed == false);
//for (const auto& rule : R) {
 //   std::cout << "Rule: " << rule.first << " → " << rule.second << "\n";
//}


    CHECK(R.find({Word("xyz"), Word("t")}) != R.end());   // rule still there
    CHECK(E.empty());                                     // no new equations
}

