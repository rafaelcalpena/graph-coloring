#ifndef AC3H
#define AC3H

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include "../utils/vector-utils.h"
#include "./constraint.h"
#include "./csp.h"
#include "./csp-utils.h"
#include "./ops.h"
#include "../utils/debug-csp.h"

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

            DEBUGCSP("Check constraint " + c.toString());
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
                    DEBUGCSP("checking variable " + c.a + " = " + v);
                    /* if value is not valid, remove it from the domain, and mark domain as changed */
                    
                    if (!cspUtils::valueConsistent(csp, c, c.a, v)) {
                        isDirty = true;
                        DEBUGCSP(c.a + "=" + v + " is not consistent");
                        /* remove value */
                        cspUtils::removeValue(csp, c.a, v);
                        /* Stop earlier if some variable domain becomes empty */
                        DEBUGCSP("removed inconsistent value");

                        if (csp.domains.at(c.a).size() == 0) {
                            DEBUGCSP(c.a + " EMPTY size");
                            return false;
                        }
                    }
                }
            }

            DEBUGCSP("removed arc from agenda" + c.toString());

            if (isDirty) {
                cspUtils::markVariableDirty(constraintList, c.a, initialConstraintList);
            }

            /* Remove arc from agenda, needs to come after mark dirty to avoid loops */
            cspUtils::removeArcFromAgenda(constraintList, c); 
        }

        return true;
    }
}

#endif