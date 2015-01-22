#ifndef file_h
#define file_h

#include <TFile.h>
#include <string>
#include <memory>

class File
{
  private:
    std::shared_ptr<TFile> m_file;

  public:
    File() = delete;
    File(const std::string& name, const std::string& option);
    File(const File&) = default;
    File(File&&) = default;
    ~File() = default;

    // nice stuff
    TFile* operator->() const;
    TFile& operator*() const;
    // template<class T> T* getObject(name)
    // template<class T> vector<T*> getObjects(pattern)
    // template<class T> vector<T*> getObjects(vector<pattern>)

};

#endif
