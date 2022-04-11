#ifndef OPSH
#define OPSH

#include <iostream>

namespace ops {

    using namespace std;

    bool passes(string op, int v, int o) {
        /* TODO: Maybe refactor to switch */
        if (op == "!=") {
            return v != o;
        } else if (op == ">") {
            return v > o;
        } else if (op == ">=") {
            return v >= o;
        } else if (op == "<") {
            return v < o;
        } else if (op == "<=") {
            return v <= o;
        } else if (op == "=") {
            return v == o;
        }
        /* TODO: Analyze */
        return false;
    }

    string invertOperation(string op) {
        if (op == "!=") {
            return "!=";
        } else if (op == ">") {
            return "<";
        } else if (op == ">=") {
            return "<=";
        } else if (op == "<") {
            return ">";
        } else if (op == "<=") {
            return ">=";
        } else if (op == "=") {
            return "=";
        }
        /* TODO: Analyze */
        return "";
    }   

}

#endif