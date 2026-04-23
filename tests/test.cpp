#include "../src/group_presentation.hpp"

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

    CHECK((w.inverse()).str() == "CBAcba");
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

TEST_CASE("Generator rejects multi-character strings")
{
    CHECK_THROWS_AS(Generator("ab"), std::invalid_argument);
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
    CHECK(result.str() == "bbBaA");
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

TEST_CASE("Word inverse simplification reduces non-adjacent inverses to empty")
{
    Word w("abBA");
    Word reduced = GroupPresentation({}).reduce_inverses(w);
    CHECK(reduced.str() == "");
}

TEST_CASE("Word inverse simplification removes multiple adjacent pairs in one pass")
{
    Word w("aAbB");
    Word reduced = GroupPresentation({}).reduce_inverses(w);
    CHECK(reduced.str() == "");
}

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
        {Word("abc"), Word("bcd")},     // not reducible
        {Word("xyz"), Word("t")}
    };

    bool changed = collapse(E, R, {Word("xyz"), Word("t")});

    CHECK(changed == false);
//for (const auto& rule : R) {
 //  std::cout << "Rule: " << rule.first << " → " << rule.second << "\n";
//}


    CHECK(R.find({Word("xyz"), Word("t")}) != R.end());   // rule still there
    CHECK(E.empty());                                     // no new equations
}

TEST_CASE("find_overlaps between lhs1 and lhs2") {
    Word lhs1("abc");
    Word lhs2("bcd");

    auto overlaps = find_overlaps(lhs1, lhs2);

    CHECK(overlaps.size() == 1);
    CHECK(overlaps[0].str() == "abcd");

    Word lhs3("xyz");
    Word lhs4("uvw");

    auto no_overlap = find_overlaps(lhs3, lhs4);
    CHECK(no_overlap.empty());
}

TEST_CASE("delete_trivial_equalities removes only equal pairs") {
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("a")},
        {Word("ab"), Word("ab")},
        {Word("a"), Word("b")},
        {Word("abc"), Word("ab")}
    };

    delete_trivial_equalities(E);

    CHECK(E.size() == 2);
    CHECK(E.find({Word("a"), Word("b")}) != E.end());
    CHECK(E.find({Word("abc"), Word("ab")}) != E.end());
}

TEST_CASE("reduce applies rewrite rules until fixed point") {
    std::set<std::pair<Word, Word>> R = {
        {Word("ab"), Word("c")},
        {Word("c"), Word("d")}
    };

    Word w("xabyz");
    Word reduced = reduce(w, R);

    CHECK(reduced.str() == "xdyz");  // ab → c → d
}

TEST_CASE("reduce leaves word unchanged when no rule matches") {
    std::set<std::pair<Word, Word>> R = {
        {Word("ab"), Word("c")}
    };

    Word w("xyz");
    Word reduced = reduce(w, R);

    CHECK(reduced.str() == "xyz");
}

TEST_CASE("reduce restarts after each replacement using updated word") {
    std::set<std::pair<Word, Word>> R = {
        {Word("ab"), Word("ba")},
        {Word("ba"), Word("c")}
    };

    Word reduced = reduce(Word("zab"), R);
    CHECK(reduced.str() == "zc");
}

TEST_CASE("compose upgrades a rule rhs using existing rules") {
    std::set<std::pair<Word, Word>> R = {
        {Word("Y"), Word("Z")}
    };

    R.insert({Word("abc"), Word("xY")});

    bool changed = compose(R, {Word("abc"), Word("xY")});

    CHECK(changed == true);
    CHECK(R.find({Word("abc"), Word("xZ")}) != R.end());
}

TEST_CASE("simplify updates rhs of equation when reducible") {
    std::set<std::pair<Word, Word>> E = {
        {Word("lhs"), Word("MZ")}
    };
    std::set<std::pair<Word, Word>> R = {
        {Word("Z"), Word("T")}
    };

    bool changed = simplify(E, R, {Word("lhs"), Word("MZ")});

    CHECK(changed == true);
    CHECK(E.find({Word("lhs"), Word("MT")}) != E.end());
}

TEST_CASE("orient flips and inserts properly") {
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("abc")}
    };
    std::set<std::pair<Word, Word>> R;

    bool changed = orient(E, R, {Word("a"), Word("abc")});

    CHECK(changed == true);
    CHECK(E.empty());
    CHECK(R.find({Word("abc"), Word("a")}) != R.end());
}

TEST_CASE("compose does nothing when rhs is already irreducible") {
    std::set<RewriteRule> R = {
        {Word("abc"), Word("xy")}
    };

    bool changed = compose(R, {Word("abc"), Word("xy")});

    CHECK(changed == false);
    CHECK(R.find({Word("abc"), Word("xy")}) != R.end());
}

TEST_CASE("simplify does nothing when equation rhs is irreducible") {
    std::set<std::pair<Word, Word>> E = {
        {Word("lhs"), Word("rhs")}
    };
    std::set<std::pair<Word, Word>> R = {
        {Word("ab"), Word("c")}
    };

    bool changed = simplify(E, R, {Word("lhs"), Word("rhs")});

    CHECK(changed == false);
    CHECK(E.find({Word("lhs"), Word("rhs")}) != E.end());
}

TEST_CASE("quotient preserves generators and adds only new relators") {
    GroupPresentation G(
        set{Generator("a"), Generator("b")},
        set{Relation("abAB")}
    );

    GroupPresentation Q = quotient(G, set{Relation("aa"), Relation("bb")});
    std::ostringstream oss;
    oss << Q;

    CHECK(oss.str() == "<a,b | aa,abAB,bb>");
    CHECK(G.relators.size() == 1);
}

TEST_CASE("Presentation commutator adds pairwise commutator relators") {
    GroupPresentation A(set{Generator("a"), Generator("b")});
    GroupPresentation B(set{Generator("x"), Generator("y")});

    GroupPresentation C = commutator(A, B);

    CHECK(C.generators.size() == 2);
    CHECK(C.relators.size() == 4);
    CHECK(C.relators.find(Relation("AXax")) != C.relators.end());
    CHECK(C.relators.find(Relation("AYay")) != C.relators.end());
    CHECK(C.relators.find(Relation("BXbx")) != C.relators.end());
    CHECK(C.relators.find(Relation("BYby")) != C.relators.end());
}

TEST_CASE("LowerCentralStep keeps first presentation generators") {
    GroupPresentation A(set{Generator("a"), Generator("b")});
    GroupPresentation An(set{Generator("x")});

    GroupPresentation step = LowerCentralStep(A, An);

    CHECK(step.generators == A.generators);
    CHECK(step.relators.size() == 2);
    CHECK(step.relators.find(Relation("AXax")) != step.relators.end());
    CHECK(step.relators.find(Relation("BXbx")) != step.relators.end());
}

TEST_CASE("find_overlaps returns multiple overlaps when they exist") {
    auto overlaps = find_overlaps(Word("aaaa"), Word("aaab"));

    CHECK(overlaps.size() == 3);
    CHECK(overlaps[0].str() == "aaaab");
    CHECK(overlaps[1].str() == "aaaaab");
    CHECK(overlaps[2].str() == "aaaaaab");
}

TEST_CASE("find_overlaps does not count a complete overlap as a critical overlap") {
    auto overlaps = find_overlaps(Word("abc"), Word("abc"));

    CHECK(overlaps.empty());
}

TEST_CASE("reduce handles overlapping and nested rewrites") {
    std::set<std::pair<Word, Word>> R = {
        {Word("aba"), Word("x")},
        {Word("x"), Word("y")}
    };

    Word w("ababa");  // contains overlapping aba twice

    Word reduced = reduce(w, R);
    CHECK(reduced.str().find("y") != std::string::npos);
}

TEST_CASE("compose supports multi-step reduction of rhs") {
    std::set<std::pair<Word, Word>> R = {
        {Word("B"), Word("C")},
        {Word("C"), Word("D")}
    };

    R.insert({Word("X"), Word("AB")});

    compose(R, {Word("X"), Word("AB")});

    CHECK(R.find({Word("X"), Word("AD")}) != R.end());
}

TEST_CASE("simplify does nothing if rhs is irreducible") {
    std::set<std::pair<Word, Word>> E = {
        {Word("x"), Word("q")}
    };
    std::set<std::pair<Word, Word>> R = {
        {Word("a"), Word("b")}
    };

    bool changed = simplify(E, R, {Word("x"), Word("q")});
    CHECK(changed == false);
    CHECK(E.find({Word("x"), Word("q")}) != E.end());
}

TEST_CASE("collapse only uses other rules, not itself") {
    std::set<std::pair<Word, Word>> E;
    std::set<std::pair<Word, Word>> R = {
        {Word("X"), Word("Y")},
        {Word("XX"), Word("X")}  // should collapse
    };

    bool changed = collapse(E, R, {Word("XX"), Word("X")});
    CHECK(changed == true);
    CHECK(R.find({Word("XX"), Word("X")}) == R.end());

    CHECK(E.find({Word("YY"), Word("X")}) != E.end());
}

// ============================================================
// Word — additional construction
// ============================================================

TEST_CASE("Word strips commas from input")
{
    Word w("a,b");
    CHECK(w.str() == "ab");
}

TEST_CASE("Word from all-whitespace and commas becomes empty")
{
    Word w("  ,\t,\n");
    CHECK(w.str() == "");
    CHECK(w.length() == 0);
}

TEST_CASE("Word construction from uppercase char")
{
    Word w('A');
    CHECK(w.str() == "A");
}

// ============================================================
// Word — inverse additional cases
// ============================================================

TEST_CASE("Word inverse of empty word")
{
    Word w("");
    CHECK(w.inverse().str() == "");
}

TEST_CASE("Word inverse of single char")
{
    CHECK(Word('a').inverse().str() == "A");
    CHECK(Word('A').inverse().str() == "a");
}

// ============================================================
// Word — comparison operators additional cases
// ============================================================

TEST_CASE("Word < and > on equal words")
{
    Word a("abc"), b("abc");
    CHECK((a < b) == false);
    CHECK((a > b) == false);
}

TEST_CASE("Word > true case")
{
    Word a("b"), b("a");
    CHECK((a > b) == true);
}

TEST_CASE("Word comparison: empty word is less than any non-empty word")
{
    Word empty(""), a("a");
    CHECK((empty < a) == true);
    CHECK((a < empty) == false);
}

TEST_CASE("Word == on two empty words")
{
    Word a(""), b("");
    CHECK((a == b) == true);
}

TEST_CASE("Word != on equal words returns false")
{
    Word a("abc"), b("abc");
    CHECK((a != b) == false);
}

// ============================================================
// Word — operator+ additional cases
// ============================================================

TEST_CASE("Word addition with empty operands")
{
    Word empty(""), a("abc");
    CHECK((empty + a).str() == "abc");
    CHECK((a + empty).str() == "abc");
    CHECK((empty + empty).str() == "");
}

// ============================================================
// Word — implicit conversion and mutable subscript
// ============================================================

TEST_CASE("Word implicit conversion to string")
{
    Word w("hello");
    std::string s = w;
    CHECK(s == "hello");
}

TEST_CASE("Word mutable subscript allows in-place mutation")
{
    Word w("abc");
    w[1] = 'X';
    CHECK(w.str() == "aXc");
}

// ============================================================
// Word — find additional cases
// ============================================================

TEST_CASE("Word find returns npos when pattern is absent")
{
    Word w("hello");
    CHECK(w.find(Word("xyz")) == std::string::npos);
    CHECK(w.find("xyz") == std::string::npos);
}

TEST_CASE("Word find pattern at end of word")
{
    Word w("abcXYZ");
    CHECK(w.find(Word("XYZ")) == 3);
}

// ============================================================
// Word — erase additional cases
// ============================================================

TEST_CASE("Word erase at position zero")
{
    Word w("abcdef");
    w.erase(0, 3);
    CHECK(w.str() == "def");
}

TEST_CASE("Word erase to end with default length")
{
    Word w("abcdef");
    w.erase(3);
    CHECK(w.str() == "abc");
}

TEST_CASE("Word erase zero length is no-op")
{
    Word w("abc");
    w.erase(1, 0);
    CHECK(w.str() == "abc");
}

// ============================================================
// Word — replace string overload (untested overload)
// ============================================================

TEST_CASE("Word replace with std::string replacement")
{
    Word w("abcdef");
    w.replace(2, 3, std::string("XYZ"));
    CHECK(w.str() == "abXYZf");
}

// ============================================================
// Word — substr and subword with default length
// ============================================================

TEST_CASE("Word substr with default length reads to end")
{
    Word w("abcdef");
    CHECK(w.substr(3) == "def");
}

TEST_CASE("Word subword with default length reads to end")
{
    Word w("abcdef");
    CHECK(w.subword(3).str() == "def");
}

// ============================================================
// Relation — additional construction
// ============================================================

TEST_CASE("Empty relator construction")
{
    Relation r("");
    CHECK(r.str() == "");
    CHECK(r.length() == 0);
}

// ============================================================
// Generator — additional construction
// ============================================================

TEST_CASE("Generator from uppercase char")
{
    Generator g('A');
    CHECK(g.str() == "A");
}

TEST_CASE("Generator rejects empty string")
{
    CHECK_THROWS_AS(Generator(""), std::invalid_argument);
}

TEST_CASE("Generator rejects string with embedded whitespace")
{
    // The length check is on the raw parameter before Word strips whitespace
    CHECK_THROWS_AS(Generator("a "), std::invalid_argument);
}

// ============================================================
// GroupPresentation — additional construction
// ============================================================

TEST_CASE("GroupPresentation with empty generator set prints correctly")
{
    GroupPresentation g({});
    std::ostringstream oss;
    oss << g;
    CHECK(oss.str() == "< | >");
}

TEST_CASE("GroupPresentation deduplicates relators")
{
    GroupPresentation p(
        set{Generator("a")},
        set{Relation("aa"), Relation("aa")}
    );
    CHECK(p.relators.size() == 1);
}

// ============================================================
// is_char_inv — direct tests
// ============================================================

TEST_CASE("is_char_inv identifies inverse character pairs")
{
    GroupPresentation gp({});
    CHECK(gp.is_char_inv('a', 'A') == true);
    CHECK(gp.is_char_inv('A', 'a') == true);
    CHECK(gp.is_char_inv('z', 'Z') == true);
    CHECK(gp.is_char_inv('a', 'a') == false);
    CHECK(gp.is_char_inv('a', 'b') == false);
    CHECK(gp.is_char_inv('A', 'B') == false);
}

// ============================================================
// reduce_inverses — additional cases
// ============================================================

TEST_CASE("reduce_inverses on empty word")
{
    Word reduced = GroupPresentation({}).reduce_inverses(Word(""));
    CHECK(reduced.str() == "");
}

TEST_CASE("reduce_inverses with no cancellations leaves word unchanged")
{
    Word reduced = GroupPresentation({}).reduce_inverses(Word("abc"));
    CHECK(reduced.str() == "abc");
}

TEST_CASE("reduce_inverses on single character")
{
    Word reduced = GroupPresentation({}).reduce_inverses(Word("a"));
    CHECK(reduced.str() == "a");
}

TEST_CASE("reduce_inverses partial cancellation")
{
    Word reduced = GroupPresentation({}).reduce_inverses(Word("abBc"));
    CHECK(reduced.str() == "ac");
}

// ============================================================
// delete_trivial_equalities — additional cases
// ============================================================

TEST_CASE("delete_trivial_equalities on empty set is a no-op")
{
    std::set<std::pair<Word, Word>> E;
    delete_trivial_equalities(E);
    CHECK(E.empty());
}

TEST_CASE("delete_trivial_equalities removes all pairs when all are trivial")
{
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("a")},
        {Word("b"), Word("b")}
    };
    delete_trivial_equalities(E);
    CHECK(E.empty());
}

TEST_CASE("delete_trivial_equalities does nothing when no pairs are trivial")
{
    std::set<std::pair<Word, Word>> E = {
        {Word("a"), Word("b")},
        {Word("c"), Word("d")}
    };
    delete_trivial_equalities(E);
    CHECK(E.size() == 2);
}

// ============================================================
// reduce — additional cases
// ============================================================

TEST_CASE("reduce empty word with non-empty rules returns empty word")
{
    std::set<RewriteRule> R = {{Word("a"), Word("b")}};
    CHECK(reduce(Word(""), R).str() == "");
}

TEST_CASE("reduce non-empty word with empty rule set returns word unchanged")
{
    std::set<RewriteRule> R;
    CHECK(reduce(Word("abc"), R).str() == "abc");
}

TEST_CASE("reduce rule with empty RHS deletes matched subword")
{
    std::set<RewriteRule> R = {{Word("ab"), Word("")}};
    CHECK(reduce(Word("xabyz"), R).str() == "xyz");
}

// ============================================================
// collapse — additional case
// ============================================================

TEST_CASE("collapse when LHS reduces to empty string")
{
    std::set<std::pair<Word, Word>> E;
    std::set<std::pair<Word, Word>> R = {
        {Word("ab"), Word("")},
        {Word("abab"), Word("c")}
    };

    bool changed = collapse(E, R, {Word("abab"), Word("c")});

    CHECK(changed == true);
    CHECK(R.find({Word("abab"), Word("c")}) == R.end());
    // "abab" --(ab→"")→ "ab" --(ab→"")→ "" so u=""
    CHECK(E.find({Word(""), Word("c")}) != E.end());
}

// ============================================================
// find_overlaps — additional cases
// ============================================================

TEST_CASE("find_overlaps with single-char lhs1 finds no overlaps")
{
    // Loop starts at i=1; for a 1-char string i < s1.size() is immediately false
    auto overlaps = find_overlaps(Word("a"), Word("ab"));
    CHECK(overlaps.empty());
}

TEST_CASE("find_overlaps with empty lhs1 finds no overlaps")
{
    auto overlaps = find_overlaps(Word(""), Word("abc"));
    CHECK(overlaps.empty());
}

// ============================================================
// quotient — additional cases
// ============================================================

TEST_CASE("quotient with empty new relators leaves presentation unchanged")
{
    GroupPresentation G(set{Generator("a")}, set{Relation("aa")});
    GroupPresentation Q = quotient(G, {});
    CHECK(Q.relators == G.relators);
    CHECK(Q.generators == G.generators);
}

TEST_CASE("quotient deduplicates relators already present in G")
{
    GroupPresentation G(set{Generator("a")}, set{Relation("aa")});
    GroupPresentation Q = quotient(G, set{Relation("aa"), Relation("aaa")});
    CHECK(Q.relators.size() == 2);
}

// ============================================================
// free inverse — additional cases
// ============================================================

TEST_CASE("free inverse of empty word")
{
    CHECK(inverse(Word("")).str() == "");
}

TEST_CASE("free inverse of single char")
{
    CHECK(inverse(Word("a")).str() == "A");
    CHECK(inverse(Word("A")).str() == "a");
}

// ============================================================
// commutator — multi-char words
// ============================================================

TEST_CASE("Word commutator with multi-char words")
{
    // commutator(ab, cd) = inverse(ab) + inverse(cd) + ab + cd
    // inverse("ab") = reverse("ab") + flip = "ba" -> "BA"
    // inverse("cd") = reverse("cd") + flip = "dc" -> "DC"
    Word a("ab"), b("cd");
    CHECK(commutator(a, b).str() == "BADCabcd");
}

// ============================================================
// derived — additional cases
// ============================================================

TEST_CASE("derived of single-generator group")
{
    GroupPresentation G(set{Generator("a")});
    GroupPresentation D = derived(G);
    // commutator(a, a) = inverse(a) + inverse(a) + a + a = "AAaa"
    CHECK(D.relators.size() == 1);
    CHECK(D.relators.find(Relation("AAaa")) != D.relators.end());
}

TEST_CASE("derived preserves existing relators")
{
    GroupPresentation G(
        set{Generator("a"), Generator("b")},
        set{Relation("aaa")}
    );
    GroupPresentation D = derived(G);
    CHECK(D.relators.find(Relation("aaa")) != D.relators.end());
    CHECK(D.relators.size() == 5);  // 1 existing + 4 commutators
}

// ============================================================
// print_rules — output tests
// ============================================================

TEST_CASE("print_rules outputs each rule to stdout")
{
    std::set<std::pair<Word, Word>> rules = {{Word("a"), Word("b")}};
    std::ostringstream oss;
    auto* old_buf = std::cout.rdbuf(oss.rdbuf());
    print_rules(rules);
    std::cout.rdbuf(old_buf);
    CHECK(oss.str() == "a → b\n");
}

TEST_CASE("print_rules on empty set produces no output")
{
    std::ostringstream oss;
    auto* old_buf = std::cout.rdbuf(oss.rdbuf());
    print_rules({});
    std::cout.rdbuf(old_buf);
    CHECK(oss.str() == "");
}

// ============================================================
// set operator<< — direct tests
// ============================================================

TEST_CASE("set printing operator for a non-empty set of Words")
{
    std::set<Word> s = {Word("a"), Word("b")};
    std::ostringstream oss;
    oss << s;
    CHECK(oss.str() == "{a, b}");
}

TEST_CASE("set printing operator for an empty set")
{
    std::set<Word> s;
    std::ostringstream oss;
    oss << s;
    CHECK(oss.str() == "{}");
}
