/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>

class File
{
public:
    bool OpenRead(const std::string& filename);
    bool GetString(std::string*);
    bool GetInteger(int*);
    bool GetFloat(float*);
    void ReportError(const std::string&);
private:
    std::ifstream m_ifstream;
};

#endif
