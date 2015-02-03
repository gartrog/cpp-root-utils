#include "file.h"

#include <TList.h>

#include <regex>

File::File(const std::string& name, std::string option) :
  m_file(TFile::Open(name.c_str(), option.c_str()))
{}

TFile* File::operator->() const {
  return m_file.get();
}

TFile& File::operator*() const {
  return *m_file;
}

void File::findObjects(std::vector<TKey*>& objs, TDirectory* dir, const std::string& pattern, TClass* objClass) {
  // First: find the case we are in
  size_t pos = pattern.find_first_of('/');

  // Case #1: we have given a path with directories in it
  if(pos != std::string::npos) {
    std::string dir_patt = pattern.substr(0, pos);
    std::string rest_patt = pattern.substr(pos+1);
    std::vector<TKey*> dirKeys;
    // Case #1.5: directory "**" means we want to glob all hierarchy
    if(dir_patt == "**") {
      // first, add matching objects inside current directory
      findObjects(objs, dir, rest_patt, objClass);
      // then find all subdirectories
      findObjects(dirKeys, dir, "", TDirectory::Class());
      // and recurse through them
      for(auto& k : dirKeys) {
        findObjects(objs, dynamic_cast<TDirectory*>(k->ReadObj()), pattern, objClass);
      }
    }
    // Standard Case #1
    else {
      findObjects(dirKeys, dir, dir_patt, TDirectory::Class());
      for(auto& k : dirKeys) {
        findObjects(objs, dynamic_cast<TDirectory*>(k->ReadObj()), rest_patt, objClass);
      }
    }
  }

  // Case #2: empty string: get all objects of the correct type
  else {
    if( pattern == "" ) {
      TListIter it(dir->GetListOfKeys());
      TKey* k;
      while((k = (TKey*)(it.Next()))) {
        TClass* kclass = TClass::GetClass(k->GetClassName());
        if(kclass->InheritsFrom(objClass)) {
          objs.emplace_back(k);
        }
      }
    }

    // Case #3: there is a regexp to match
    else {
      std::regex regexp(pattern);
      TListIter it(dir->GetListOfKeys());
      TKey* k;
      while((k = (TKey*)(it.Next()))) {
        TClass* kclass = TClass::GetClass(k->GetClassName());
        if(kclass->InheritsFrom(objClass)) {
          if(std::regex_match(k->GetName(), regexp)) {
            objs.emplace_back(k);
          }
        }
      }
    }
  }

}
