#include "includes.hpp"
#include "Proc.hpp"

typedef unsigned int uint;

int main()
{
	Proc a(11728);
	std::vector<uint8_t> godmodeAOB{ 0x29, 0x82, 0x08, 0x04, 0x00, 0x00 };

	/*

	unsigned char buf{};

	if (!ReadProcessMemory(a.getHandle(), readFrom, &buf, 1, NULL))
		std::cerr << "No returned\n";
	std::cout << std::hex << "0x" << (unsigned int)(buf) << "  /  " << std::dec << (unsigned int)(buf) << std::endl;

	*/

	/*

	int buf{};
	if (!ReadProcessMemory(a.getHandle(), readFrom, &buf, 4, NULL))
		std::cerr << "NO!\n";

	int out{};
	if (!a.ReadFromProcessMemoryExplicit<int>(readFrom, out, 4))
		std::cerr << "no2\n";


	std::cout << buf << std::endl;
	std::cout << out << std::endl;
	*/

	return 0;
}