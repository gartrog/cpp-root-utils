#include "file.h"

File::File(const std::string& name, const std::string& option) :
  m_file(TFile::Open(name.c_str(), option.c_str()))
{}

TFile* File::operator->() const {
  return m_file.get();
}

TFile& File::operator*() const {
  return *m_file;
}
