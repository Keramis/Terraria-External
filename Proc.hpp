#pragma once
#include "includes.hpp"

class Proc
{
private:
	DWORD pid{};
	std::wstring name{};
	HANDLE hProc{};
	HANDLE snapshot{};
	std::vector<MODULEENTRY32W> entries{};
	MODULEENTRY32W primaryEntry{};
	std::vector<MEMORY_BASIC_INFORMATION> allRegions{};
	std::vector<MEMORY_BASIC_INFORMATION> usefulRegions{};
	bool doDebugPrints{ true };
	SYSTEM_INFO systemInfo{};
	DWORD pageSize{};
	BYTE* primaryModuleBaseAddress{};
public:
	Proc(DWORD pid);
	void CloseAndError(std::string error);
	~Proc();
	DWORD getPID();
	std::wstring getName();
	HANDLE getHandle();
	HANDLE getSnapshot();
	std::vector<MODULEENTRY32W> getEntries();
	MODULEENTRY32W getSpecificEntry(std::wstring name);
	std::vector<MEMORY_BASIC_INFORMATION> getRegions();
	MEMORY_BASIC_INFORMATION getSpecificRegion(unsigned int addr);
	bool getDebugPrintsStatus();
	void setDebugPrints(bool);
	void debugPrint(std::wstring);
	SYSTEM_INFO getSystemInfo();
	inline DWORD getPageSize() { return this->pageSize; }
	std::vector<MEMORY_BASIC_INFORMATION> getAllRegions();
	std::vector<MEMORY_BASIC_INFORMATION> getUsefulRegions();
	template <typename T>
	inline bool ReadFromProcessMemoryExplicit(void* baseAddr, T &output, size_t numBytesRead) //wrapper for ReadProcessMemory with our own handle.
	{
		return ReadProcessMemory(hProc, baseAddr, &output, numBytesRead, NULL);
	}
	void DebugScanPrimaryModule();
	bool AOBScanUsefulRegions(std::vector<uint8_t>, size_t&);
};