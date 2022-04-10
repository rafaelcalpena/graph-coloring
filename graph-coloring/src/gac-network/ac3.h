#ifndef AC3H
#define AC3H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include "../vector-utils.h"
#include "./constraint.h"
#include "./csp.h"
#include "./csp-utils.h"
#include "./ops.h"

namespace ac3 {

    using namespace std;
    
    /* Return true if consistent, false if some variable had domain emptied */
    bool ac3 (csp::CSP & csp) {
        /* Run AC3 algorithm to make the network arc-consistent */
        /* TODO: May be converted into set as long as loop behaves predictably */
        vector<csp::Constraint> initialConstraintList = cspUtils::getInitialConstraintsList(csp.constraints);
        vector<csp::Constraint> constraintList = vectorUtils::copiarVetor(initialConstraintList);

        auto itr = constraintList.begin();


        while (itr != constraintList.end()) {

            csp::Constraint c = *itr;
            bool isDirty = false;

            // cout << "Check constraint " << c.toString() << endl;
            /* for each value in the left side of the constraint, check if valid */
            
            
            /* Optimization */
            if (c.op == "!=") {

                set<int> d = cspUtils::domain(csp, c.b);

                if (d.size() == 1) {
                    auto elItr = d.begin();
                    int el = *elItr;

                    auto da = cspUtils::domain(csp, c.a);

                    if (da.find(el) != da.end()) {
           
                        isDirty = true;
                        cspUtils::removeValue(csp, c.a, el);

                        if (csp.domains.at(c.a).size() == 0) {
                            return false;
                        }

                    }
                }
            } else {
                for (int v: cspUtils::domain(csp, c.a)) {
                    // std::cout << "checking variable " << c.a << " = " << v << endl;
                        /* if value is not valid, remove it from the domain, and mark domain as changed */
                    if (!cspUtils::valueConsistent(csp, c, c.a, v)) {
                        isDirty = true;
                        // std::cout << c.a << "=" << v << " is not consistent" << endl;
                        /* remove value */
                        // cout << "v2 outer " << csp.domains.at(c.a).size() << endl;
                        cspUtils::removeValue(csp, c.a, v);
                        // cout << "v2 outer after " << csp.domains.at(c.a).size() << endl;
                        /* TODO: Stop earlier if some variable domain becomes empty! */
                        // std::cout << "removed inconsistent value" << endl;
                        if (csp.domains.at(c.a).size() == 0) {
                            // cout << c.a << " EMPTY size" << endl;
                            return false;
                        }
                    }
                }
            }

            // std::cout << "removed arc from agenda" << endl;        

            if (isDirty) {
                cspUtils::markVariableDirty(constraintList, c.a, initialConstraintList);
            }

            /* Remove arc from agenda, needs to come after mark dirty to avoid loops */
            cspUtils::removeArcFromAgenda(constraintList, c); 

            // cspUtils::printDomains(csp);       

        }

        return true;
    }
}

#endif