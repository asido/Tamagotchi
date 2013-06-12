// Win32Project1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <string>

class Resource
{
public:
	Resource(const std::wstring &name) : name(name) {}
	const std::wstring &	GetName() const { return this->name; }
	void				SetName(const std::wstring &name) { this->name = name; }

private:
	std::wstring name;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Resource res(L"1");

	OutputDebugString(res.GetName().c_str());

	if (argc)
	{
		std::wstring newname = L"2";
		res.SetName(newname);

		OutputDebugString(res.GetName().c_str());
	}

	OutputDebugString(res.GetName().c_str());

	return 0;
}

