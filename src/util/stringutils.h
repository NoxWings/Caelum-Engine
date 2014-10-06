#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "EnginePrerequisites.h"

#include <list>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdlib>

namespace Caelum {

typedef std::vector<String> StringVector;
typedef std::list<String>   StringList;


class StringUtils {
  public:
    static bool equals(const String& s1, const String& s2) {
        return (s1.compare(s2) == 0);
    }

    static String to_string(float number){
        std::ostringstream buff;
        buff << number;
        return buff.str();
    }

    static String to_string(int number) {
        std::ostringstream buff;
        buff << number;
        return buff.str();
    }

    static int to_int(const String& str) {
        return atoi(str.c_str());
    }

    static Real to_Real(const String &str) {
        return atof(str.c_str());
    }

    static Real to_Real2(const String &str) {
        std::istringstream is(str.c_str());
        double myfloat;
        is >> myfloat;
        return myfloat;
    }

  private:
    StringUtils() {}
    ~StringUtils() {}
};

}


#endif // STRINGUTILS_H
