#pragma once
#include "includes.hpp"
#include "Proc.hpp"

class HackModule
{
private:
	std::vector<uint8_t> aob{}; //array-of-bytes to scan for
	std::vector<uint8_t> replace{}; //replace the AOB with these
	size_t address{}; //set when scanning
	bool enabled{}; //later for gui
	std::string moduleName{};
	char keyActivate{};
public:
	HackModule(std::vector<uint8_t> aob, std::vector<uint8_t> replacementBytes, std::string moduleName, Proc& proc, bool enabled, char keyActivate);
	void Close(int code);
	inline void toggle() { enabled = !enabled; }

};