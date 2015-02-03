#ifndef container_utils_h
#define container_utils_h

#include <vector>
#include <map>

namespace util {

  template<class T>
    std::map<std::string, T*> map(const std::vector<T*>& v);

};

#include "container_utils.icc"

#endif


