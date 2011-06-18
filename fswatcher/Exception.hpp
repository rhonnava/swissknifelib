#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>
#include <string>

using namespace std;

class Exception{
wstring mData;
public:
Exception(const wstring& param) : mData(param)
{}

virtual void print()
{
	wcout << mData << endl;
}

virtual ~Exception() 
{
}
};

class SizeExceededException : public Exception
{
public:
SizeExceededException(const wstring& param) : Exception(param)
{
}
};

#endif

