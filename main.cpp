#include "includes.hpp"
#include "Proc.hpp"

typedef unsigned int uint;

int main()
{
	Proc a(21108);
	std::vector<uint8_t> godmodeAOB{ 0x29, 0x82, 0x08, 0x04, 0x00, 0x00 };
	size_t add;
	if (a.AOBScanUsefulRegions(godmodeAOB, add))
		std::cout << std::hex << add << std::dec << std::endl;
	else
		std::cout << "no\n";

	while (1)
	{
		if (_getch() == '1')
		{
			uint8_t b[6]{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			//if (WriteProcessMemory(a.getHandle(), (void*)(add), &b, 6, NULL))
			if (a.WriteToProcessMemoryExplicit((void*)add, b, 6))
				std::cout << "Write godmode enable successful!\n";
			else
				std::cout << "Unsucessful.\n";
		}
		else if (_getch() == '2')
		{
			uint8_t b[6]{ 0x29, 0x82, 0x08, 0x04, 0x00, 0x00 }; //fix this, this shit don't write lmfao
			//if (WriteProcessMemory(a.getHandle(), (void*)(add), &b, 6, NULL))
			if (a.WriteToProcessMemoryExplicit((void*)(add), b, 6))
				std::cout << "Write godmode disable successful!\n";
			else
				std::cout << "Unsucessful.\n";
		}
		else if (_getch() == 'n')
		{
			a.~Proc();
			break;
		}
		Sleep(300);
	}


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