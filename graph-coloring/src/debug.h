
#ifndef DEBUGH
#define DEBUGH

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