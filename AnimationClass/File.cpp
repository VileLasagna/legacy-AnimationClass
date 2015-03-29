/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#include <iostream>
#include "File.h"

bool File::OpenRead(const std::string& filename)
{
    m_ifstream.open(filename.c_str());
    if (!m_ifstream.good())
    {
        return false;
    }
    int ver;
    if (!GetInteger(&ver))
    {
        ReportError("Expected version");
        return false;
    }
    return true;
}

bool File::GetString(std::string* pResult)
{
    char buf[1000];
    while (true)
    {
        if (m_ifstream.eof() || !m_ifstream.good())
        {
            return false;
        }
        m_ifstream.getline(buf, 1000);
        int len = strlen(buf);
        if (len == 0)
        {
            continue;
        }
        else if (len < 2)
        {
            *pResult = buf;
            return true;
        }
        else
        {
            if (buf[0] != '/' || buf[1] != '/')
            {
                *pResult = buf;
                return true;
            }
        }
    }
    // Never reached
    return true;
}

bool File::GetInteger(int* pResult)
{
    std::string s;
    if (!GetString(&s))
    {
        return false;
    }
    *pResult = atoi(s.c_str());
    return true;
}

bool File::GetFloat(float* pResult)
{
    std::string s;
    if (!GetString(&s))
    {
        return false;
    }
    *pResult = (float)atof(s.c_str());
    return true;
}

void File::ReportError(const std::string& s)
{
    std::cout << s << "\n";
}
