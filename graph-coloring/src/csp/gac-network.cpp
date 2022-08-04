
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include "./csp.h"
#include "./ac3.h"

using namespace std;

/* Main function for constraint solver */
int main(int argc, char** argv) {

    map< string, vector<int> > domains = {
        // {"a", {0, 1, 2, 3, 4}}, 
        // {"b", {0, 1, 2, 3, 4}}
        {"a", {1, 2, 3}},
        {"b", {1, 2, 3}},
        {"c", {1, 2, 3}}
    };
    vector< vector<string> > constraints = {
        // {"a", "<", "b"}
        {"a", ">", "b"},
        {"b", "=", "c"}
    };
    csp::CSP test(domains, constraints);

    ac3::ac3(test);

    // printDomains(test);

    cout << "done" << endl;
}