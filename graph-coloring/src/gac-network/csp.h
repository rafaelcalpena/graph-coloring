#ifndef CSPH
#define CSPH

#include <iostream>
#include <set>
#include <map>
#include "./constraint.h"

namespace csp {

    /* Constraint satisfaction problem */
    struct CSP {
        map< string, set<int> > domains;
        /* If possible, use set instead (must keep iteration order on removals) */
        vector<csp::Constraint> constraints;

        CSP (map < string, vector<int> > const& initialDomains, vector<vector<string> > const& initialConstraints) {

            /* TODO: Use (or not) const here? */
            for (auto const& [key, val] :initialDomains) {
                set<int> numbers;
                for (int v: val) {
                    numbers.insert(v);
                }
                domains.insert(pair<string, set<int> >(key, numbers));
            };

            for (vector<string> cv: initialConstraints) {
                csp::Constraint newConstraint(cv[0], cv[1], cv[2]);
                constraints.push_back(newConstraint);
            };
        }

    };

}

#endif