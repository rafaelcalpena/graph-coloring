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
        std::cout << "removing value " << name << "=" << v << endl;
        set<int> & domain = csp.domains.at(name);
        cout << "size was " << to_string(domain.size()) << endl;
        domain.erase(v);
        cout << "size is " << to_string(domain.size()) << endl; 
    }      

    bool inVector(vector<csp::Constraint> vector, csp::Constraint c) {
        std::vector<csp::Constraint>::iterator itr = find(vector.begin(), vector.end(), c);
        if (itr == vector.end()) {
            cout << "Did not find " << c.toString() << endl;
        }
        return itr != vector.end();
    }

    void markVariableDirty(vector<csp::Constraint> & constraintList, string name, vector<csp::Constraint> & initialConstraintList) {
        std::cout << "marking dependents of " << name << " as dirty" << endl;

        set<csp::Constraint> dirtyConstraints;

        /* Find all constraints with right hand side equal to the domain */
        for (csp::Constraint const c: initialConstraintList) {
            cout << "check if dirty " << c.toString() << endl;
            if (c.b == name) {
                cout << "constraint is dirty" << endl;
                dirtyConstraints.insert(c);
            } else {
                cout << "not dirty" << endl;
            }
        }

        for (csp::Constraint c: dirtyConstraints) {
            if (!inVector(constraintList, c)) {
                std::cout << "added " << c.toString() << " to constraintList" << endl; 
                constraintList.push_back(c);
            } else {
                cout << "skip adding " << c.toString() << " because already in the list" << endl;
            }
        }

        std::cout << "finish marking variable arcs as dirty" << endl;    

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
        cout << "before removal";
        printConstraintVector(agenda);    
        std::vector<csp::Constraint>::iterator itr = find(agenda.begin(), agenda.end(), c);
        if (itr != agenda.end()) {
            int index = std::distance(agenda.begin(), itr);
            std::cout << "Element present at index " << index << endl;
            agenda.erase(agenda.begin() + index);
        }
        else {
            std::cout << "Element not found" << endl;
        }    

        cout << "after removal ";
        printConstraintVector(agenda);    
    }

    void printDomains(const csp::CSP & csp) {
        /* TODO: Extract to function */
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

}

#endif