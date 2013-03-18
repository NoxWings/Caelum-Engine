#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "EnginePrerequisites.h"

#include <list>
#include <vector>
#include <memory>


namespace Caelum {

typedef std::vector<String> StringVector;
typedef std::list<String>   StringList;


class StringUtils {
  public:
    static bool equals(const String& s1, const String& s2) {
        return (s1.compare(s2) == 0);
    }

  private:
    StringUtils() {}
    ~StringUtils() {}
};

}


#endif // STRINGUTILS_H
