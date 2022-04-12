#ifndef DEBUGCSPH
#define DEBUGCSPH

#ifdef DEBUGCSP
#define DEBUGCSP(t) (debugcsp::debug(t))
#else
#define DEBUGCSP(t)
#endif

namespace debugcsp {

    using namespace std;

    void debug (std::string message) {
        std::cout << message << "\n";
    }

}

#endif