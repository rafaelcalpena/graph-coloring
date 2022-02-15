
#ifndef DEBUGH
#define DEBUGH

#ifdef DEBUG
#define DEBUG(t, l) (debug::debug(t, l))
#else
#define DEBUG(t, l)
#endif

namespace debug {

    using namespace std;

    void debug (std::string message, fstream& logFile) {
        const std::string flag = getenv("DEBUG");
        if (flag == "2") {
            cout << message << "\n";
        }
        if (flag == "1" || flag == "2") {
            logFile << message << ",\n";
        }
    }

}

#endif