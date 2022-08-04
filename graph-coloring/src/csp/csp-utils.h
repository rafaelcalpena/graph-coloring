#ifndef CSPUTILSH
#define CSPUTILSH

#include <iostream>
#include <set>
#include "./ops.h"

namespace cspUtils {

    using namespace std; 

    set<int> domain(csp::CSP & csp, string name) {
        return csp.domains.at(name);
    }    

    bool valueConsistent(csp::CSP & csp, csp::Constraint& c, string varName, int value) {
        bool isConsistent = false;
        for (int o: cspUtils::domain(csp, c.b)) {
            if (ops::passes(c.op, value, o)) {
                isConsistent = true;
                break;
            }
        }
        return isConsistent;
    }

    void removeValue(csp::CSP & csp, string & name, int v) {
        set<int> & domain = csp.domains.at(name);
        domain.erase(v);
    }      

    bool inVector(vector<csp::Constraint> vector, csp::Constraint c) {
        std::vector<csp::Constraint>::iterator itr = find(vector.begin(), vector.end(), c);
        return itr != vector.end();
    }

    void markVariableDirty(vector<csp::Constraint> & constraintList, string name, vector<csp::Constraint> & initialConstraintList) {

        set<csp::Constraint> dirtyConstraints;

        /* Find all constraints with right hand side equal to the domain */
        for (csp::Constraint const c: initialConstraintList) {
            if (c.b == name) {
                dirtyConstraints.insert(c);
            }
        }

        for (csp::Constraint c: dirtyConstraints) {
            if (!inVector(constraintList, c)) {
                constraintList.push_back(c);
            }
        }
        
    }

    vector<csp::Constraint> getInitialConstraintsList (vector<csp::Constraint> & constraints) {
        vector<csp::Constraint> result = vectorUtils::copiarVetor(constraints);

        for (csp::Constraint c: constraints) {
            csp::Constraint inverted(c.b, ops::invertOperation(c.op), c.a);
            result.push_back(inverted);
        }

        return result;
    }

    void printConstraintVector(vector<csp::Constraint> & v) {
        cout << "[" << endl;
        for (csp::Constraint c: v) {
            cout << c.toString() << endl;
        }
        cout << "]" << endl;
    }


    void removeArcFromAgenda (vector<csp::Constraint> & agenda, csp::Constraint const & c) {
        std::vector<csp::Constraint>::iterator itr = find(agenda.begin(), agenda.end(), c);
        if (itr != agenda.end()) {
            int index = std::distance(agenda.begin(), itr);
            agenda.erase(agenda.begin() + index);
        }
    }

    void printDomains(const csp::CSP & csp) {
        cout << "---------------" << endl;    
        for (auto const & [k, vs]: csp.domains) {
            cout << "| " << k << ": ";
            for (int v: vs) {
                cout << v << " ";
            }
            cout << endl;
            cout << "---------------" << endl;
        }
    }    

    void printDomain(const csp::CSP & csp, string k) {
        cout << "---------------" << endl;    
            cout << "| " << k << ": ";
            for (int v: csp.domains.at(k)) {
                cout << v << " ";
            }
            cout << endl;
            cout << "---------------" << endl;
    }   

}

#endif