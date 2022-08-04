#ifndef OPSH
#define OPSH

#include <iostream>

namespace ops {

    using namespace std;

    bool passes(string op, int v, int o) {
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
        return "";
    }   

}

#endif