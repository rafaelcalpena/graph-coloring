#ifndef CONSTRAINTH
#define CONSTRAINTH

#include "../utils/debug-csp.h"
#include <iostream>

namespace csp {

    using namespace std;

    /* Binary constraints */
    struct Constraint {
        string a;
        string op;
        string b;

        Constraint(string x, string y, string z) {
            DEBUGCSP("Create constraint " + x + " " + y + " " + z);
            a = x;
            op = y;
            b = z;
        }

        /* TODO: Does it need .Equals? */
        bool operator == (Constraint const &c1) {
            bool result = (a == c1.a) && (op == c1.op) && (b == c1.b);
            DEBUGCSP("Comparing == " + a + " " + op + " " + b + " to " + c1.a + " " + c1.op + " " + c1.b + " " + result);
            return result;
        }

        bool operator !=(Constraint const &c1) {
            bool result = !((a == c1.a) && (op == c1.op) && (b == c1.b));
            DEBUGCSP("Comparing != " + a + " " + op + " " + b + " to " + c1.a + " " + c1.op + " " + c1.b + " " + result);
            return result;
        }    

        /* Required for sets */ 
        bool operator <(Constraint const &c1) const {
            bool result = (a < c1.a) || (op < c1.op) || (b == c1.b);
            DEBUGCSP("Comparing < " + a + " " + op + " " + b + " to " + c1.a + " " + c1.op + " " + c1.b + " " + result);
            return result;
        }        

        string toString() const {
            return  a + " " + op + " " + b;
        }


    };


}



#endif