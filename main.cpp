#include "includes.hpp"
#include "Proc.hpp"
#include "HackModule.hpp"
#include "byteArrays.hpp"

typedef unsigned int uint;

#define TEST false

int main()
{
	Proc a(L"Terraria.exe");

#if TEST

	std::vector<uint8_t> nop{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	HackModule godModule(byteArrays::takeDamage, nop, "Godmode", a, false, '1');

	while (1)
	{
		Sleep(1000);
	}

#else

	size_t add;
	if (a.AOBScanUsefulRegions(byteArrays::takeDamage, add))
		std::cout << std::hex << add << std::dec << std::endl;
	else
		std::cout << "no\n";

	while (1)
	{
		if (_getch() == '1')
		{
			//research memset
			uint8_t b[6]{ 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
			if (WriteProcessMemory(a.getHandle(), (void*)(add), &b, 6, NULL))
			//if (a.WriteToProcessMemoryExplicit((void*)add, b, 6)) //for some reason it's crashing with the wrapper function.
				std::cout << "Write godmode enable successful!\n";
			else
				std::cout << "Unsucessful.\n";
		}
		else if (_getch() == '2')
		{
			uint8_t b[6]{ 0x29, 0x82, 0x08, 0x04, 0x00, 0x00 }; //fix this, this shit don't write lmfao
			if (WriteProcessMemory(a.getHandle(), (void*)(add), &b, 6, NULL))
			//if (a.WriteToProcessMemoryExplicit((void*)(add), b, 6))
				std::cout << "Write godmode disable successful!\n";
			else
				std::cout << "Unsucessful.\n";
		}
		else if (_getch() == 'n')
		{
			a.~Proc();
			exit(0);
			//break;
		}
		Sleep(300);
	}

	return 0;
#endif
}