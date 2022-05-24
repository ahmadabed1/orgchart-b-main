#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

#include "OrgChart.hpp"

using namespace ariel;

#include "doctest.h"

TEST_CASE("OrgChart Constructing") {
    OrgChart orgchart;


    CHECK_NOTHROW(orgchart.add_root("node1"));
    CHECK_NOTHROW(orgchart.add_sub("Root", "A")); 
    CHECK_NOTHROW(orgchart.add_sub("A", "AA")); 
    CHECK_NOTHROW(orgchart.add_sub("Root", "B").add_sub("B", "BB"));
    CHECK_NOTHROW(orgchart.add_root("Z"));
    CHECK_THROWS(orgchart.add_sub("Root", "FAIL"));
    CHECK_NOTHROW(orgchart.add_sub("Z", "C"));
    CHECK_THROWS(orgchart.add_sub("UNKNOWN", "THROWS"));
    CHECK_THROWS(orgchart.add_sub("z", "THROWS"));
    CHECK_NOTHROW(orgchart.add_root("A"));
    CHECK_NOTHROW(orgchart.add_sub("A", "B").add_sub("A", "C").add_sub("A", "D"));
    CHECK_NOTHROW(orgchart.add_sub("C", "CC").add_sub("CC", "CCC"));
    CHECK_NOTHROW(orgchart.add_sub("B", "BB"));
    CHECK_NOTHROW(orgchart.add_root("non"));
    CHECK_NOTHROW(orgchart.add_sub("R", "A")); 
    CHECK_NOTHROW(orgchart.add_sub("Asas", "AA")); 
    CHECK_NOTHROW(orgchart.add_sub("asa", "e").add_sub("B", "BB"));
    CHECK_NOTHROW(orgchart.add_root("e"));
    CHECK_THROWS(orgchart.add_sub("Roo", "F"));
    CHECK_NOTHROW(orgchart.add_sub("Z", "Cabtin"));
    CHECK_THROWS(orgchart.add_sub("U", "THROW"));
    CHECK_THROWS(orgchart.add_sub("z", "THROW"));
    CHECK_NOTHROW(orgchart.add_root("A"));
    CHECK_NOTHROW(orgchart.add_sub("Aa", "Bb").add_sub("Aas", "Caa").add_sub("Aww", "Dww"));
    CHECK_THROWS(orgchart.add_sub("saeed", "THROW"));
    CHECK_THROWS(orgchart.add_sub("eres", "noth"));
}
