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
    
    void ac3 (csp::CSP & csp) {
        /* Run AC3 algorithm to make the network arc-consistent */
        /* TODO: May be converted into set as long as loop behaves predictably */
        vector<csp::Constraint> initialConstraintList = cspUtils::getInitialConstraintsList(csp.constraints);
        vector<csp::Constraint> constraintList = vectorUtils::copiarVetor(initialConstraintList);

        auto itr = constraintList.begin();


        while (itr != constraintList.end()) {

            csp::Constraint c = *itr;
            bool isDirty = false;

            cout << "Check constraint " << c.toString() << endl;
            /* for each value in the left side of the constraint, check if valid */
            
            for (int v: cspUtils::domain(csp, c.a)) {
                std::cout << "checking variable " << c.a << " = " << v << endl;
                    /* if value is not valid, remove it from the domain, and mark domain as changed */
                if (!cspUtils::valueConsistent(csp, c, c.a, v)) {
                    isDirty = true;
                    std::cout << c.a << "=" << v << " is not consistent" << endl;
                    /* remove value */
                    cout << "v2 outer " << csp.domains.at(c.a).size() << endl;
                    cspUtils::removeValue(csp, c.a, v);
                    cout << "v2 outer after " << csp.domains.at(c.a).size() << endl;
                    std::cout << "removed inconsistent value" << endl;
                }
            }

            std::cout << "removed arc from agenda" << endl;        

            if (isDirty) {
                cspUtils::markVariableDirty(constraintList, c.a, initialConstraintList);
            }

            /* Remove arc from agenda, needs to come after mark dirty to avoid loops */
            cspUtils::removeArcFromAgenda(constraintList, c); 

            cspUtils::printDomains(csp);       

        }
    }
}

#endif