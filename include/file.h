#ifndef file_h
#define file_h

#include <TFile.h>
#include <TKey.h>
#include <TClass.h>

#include <string>
#include <memory>
#include <vector>

class File
{
  private:
    std::shared_ptr<TFile> m_file;

  public:
    File() = delete;
    File(const std::string& name, std::string option="READ");
    File(const File&) = default;
    File(File&&) = default;
    ~File() = default;

    // nice stuff
    TFile* operator->() const;
    TFile& operator*() const;
    template<class T>
    T* getObject(const std::string& name);
    template<class T>
      std::vector<T*> getObjects(const std::string& pattern = ""); // if pattern empty, return all objects of type inheriting from T
    template<class T>
      std::vector<T*> getObjects(const std::vector<std::string>& patterns);

    // can getObject also return kind of iterator... something like an iterator on TKeys, that ReadObj automatically when dereferenced ?

  private:
    void findObjects(std::vector<TKey*>& objs, TDirectory* dir, const std::string& pattern, TClass* objClass);

};

#include "file.icc"

#endif
