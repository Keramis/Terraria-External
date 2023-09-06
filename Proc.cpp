#include "Proc.hpp"

void Proc::CloseAndError(std::string error)
{
	std::cerr << error << std::endl;
	this->~Proc();
	Sleep(5000);
	exit(-1);
}

void Proc::debugPrint(std::wstring print)
{
	if (this->doDebugPrints)
		std::wcout << print << std::endl;
}

Proc::Proc(DWORD pid)
{
	this->pid = pid;
	//get the handle to the process
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
	if (hProc == INVALID_HANDLE_VALUE)
		CloseAndError("Failed to get Process Handle!");

	debugPrint(L"Successfully gotten process handle...");



	//get the name of the process
	WCHAR n[MAX_PATH];
#pragma warning( push )
#pragma warning( disable : 6386)
	if (!GetModuleBaseNameW(hProc, NULL, n, sizeof(n)))
#pragma warning( pop )
	{
		CloseAndError("Failed to get the name of the processs!");
	}
	this->name = n;
	debugPrint(L"Successfully gotten process name...");
	std::wstring p = L">> Working with primary module " + (std::wstring)(n);
	debugPrint(p);
	


	//get a snapshot
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snapshot == INVALID_HANDLE_VALUE)
		CloseAndError("Failed to get Snapshot Handle!");

	debugPrint(L"Successfully gotten snapshot handle...");



	//get all the modules
	MODULEENTRY32W entry{};
	entry.dwSize = sizeof(MODULEENTRY32W);
	if (Module32FirstW(snapshot, &entry))
	{
		entries.push_back(entry);
		while (Module32NextW(snapshot, &entry))
		{
			entries.push_back(entry);
		}
	}
	else
		CloseAndError("Failed getting the first snapshot!");
	//get primary entry
	primaryEntry = getSpecificEntry(name);

	//get primary module base address
	size_t addedSize = 0;
	while (1)
	{
		MEMORY_BASIC_INFORMATION info{};
		if (!VirtualQueryEx(hProc, primaryEntry.modBaseAddr + addedSize, &info, sizeof(MEMORY_BASIC_INFORMATION)))
		{
			std::cerr << "Could not get query at 0x" << std::hex << (size_t)(primaryEntry.modBaseAddr + addedSize) << std::dec << std::endl;
			break;
		}
		//if it succeeds
		
		//std::cout << "Succeeded in querying 0x" << std::hex << (size_t)(primaryEntry.modBaseAddr + addedSize) << std::dec << std::endl;

		addedSize += info.RegionSize;
		allRegions.push_back(info);
		if (info.Protect == PAGE_EXECUTE_READ || info.Protect == PAGE_EXECUTE_READWRITE || info.Protect == PAGE_EXECUTE_WRITECOPY || info.Protect == PAGE_READONLY)
			usefulRegions.push_back(info);
	}
	debugPrint(L"Successfully queried all modules.");
	


	CloseHandle(snapshot); //we don't need snapshot anymore
	snapshot = INVALID_HANDLE_VALUE;

	//get system information
	GetSystemInfo(&this->systemInfo);
	pageSize = systemInfo.dwPageSize;

	primaryModuleBaseAddress = getSpecificEntry(name).modBaseAddr;

	debugPrint(L"Successfully gotten all modules...");
}

DWORD Proc::getPID()
{
	return pid;
}

std::wstring Proc::getName()
{
	return name;
}

HANDLE Proc::getHandle()
{
	return hProc;
}

HANDLE Proc::getSnapshot()
{
	return snapshot;
}

std::vector<MODULEENTRY32> Proc::getEntries()
{
	return entries;
}

MODULEENTRY32 Proc::getSpecificEntry(std::wstring name)
{
	for (auto entry : this->entries)
	{
		if (!(wcscmp(name.c_str(), entry.szModule))) //zero value = equal
		{
			return entry;
		}
	}
}

bool Proc::getDebugPrintsStatus()
{
	return doDebugPrints;
}

void Proc::setDebugPrints(bool set)
{
	doDebugPrints = set;
}

SYSTEM_INFO Proc::getSystemInfo()
{
	return systemInfo;
}

std::vector<MEMORY_BASIC_INFORMATION> Proc::getAllRegions()
{
	return allRegions;
}

std::vector<MEMORY_BASIC_INFORMATION> Proc::getUsefulRegions()
{
	return usefulRegions;
}

bool Proc::AOBScanUsefulRegions(std::vector<uint8_t>toScanFor, size_t& addr)
{
	for (auto const& info : getUsefulRegions())
	{
		//std::cout << "Scanning 0x" << std::hex << (size_t)(info.BaseAddress) << std::dec << std::endl;
		//std::cout << "REGIONSIZE: " << info.RegionSize << std::endl;
		uint8_t* buf = new uint8_t[info.RegionSize];
		if (buf == nullptr)
		{
			std::cerr << "Nullptr\n";
			break;
		}

		ReadFromProcessMemoryExplicit(info.BaseAddress, *buf, info.RegionSize);

		unsigned int count = 0;
		bool matched = true;
		for (size_t i = 0; i < info.RegionSize - toScanFor.size(); ++i)
		{
			matched = true;
			for (size_t j = 0; j < toScanFor.size(); ++j)
			{
#pragma warning (push)
#pragma warning( disable : 6385 )
				if (buf[i + j] != toScanFor[j])
#pragma warning (pop)
				{
					matched = false;
					break;
				}
			}
			if (matched)
			{
				debugPrint(L"Address found!");
				addr = (size_t)(info.BaseAddress) + i;
				std::cout << ">>>>FOUND AT 0x" << std::hex << (size_t)(info.BaseAddress) + i << std::dec << std::endl;
				//don't return here, as we will have a memory leak via buf.
				break;
			}
		}

		delete[] buf;
		if (matched)
			return 1;
	}
	return 0;
}

Proc::~Proc()
{
	std::cout << "Closed the PROC!\n";
	if (snapshot != INVALID_HANDLE_VALUE)
		CloseHandle(snapshot);

	CloseHandle(hProc);
}