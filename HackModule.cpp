#include "HackModule.hpp"

constexpr auto sleepTime = std::chrono::milliseconds(200);

void threadWorker(bool& enabled, std::string moduleName, char& activateKey, Proc& proc, size_t& address, std::vector<uint8_t>& replacement)
{
	bool previousEnable = enabled;
	while (1)
	{
		if (proc.closed)
		{
			std::cout << "Proc closed, closing " << moduleName << '\n';
			break;
		}
		if (_getch() == activateKey)
		{
			std::cout << "Toggled " << moduleName << '\n';
			enabled = !enabled;
		}
		else if (_getch() == 'n')
		{
			proc.~Proc();
		}

		if (previousEnable != enabled)
		{
			switch (enabled)
			{
			case true:
				//switch the feature on
				

				std::cout << "1\n";
				break;
			case false:
				//switch feature off
				std::cout << "0\n";
				break;
			}
			//optimization to put it here
			previousEnable = enabled;
		}
		std::this_thread::sleep_for(sleepTime);
	}
}

HackModule::HackModule(std::vector<uint8_t> aob, std::vector<uint8_t> replacementBytes, std::string moduleName, Proc& proc, bool enabled, char keyActivate) : aob(aob), replace(replacementBytes), enabled(enabled), keyActivate(keyActivate)
{
	//aob scan when we init the module
	if (proc.AOBScanUsefulRegions(aob, this->address))
	{
		//successfully scanned, proceed with everything.
		std::cout << "Successfully scanned with address " << std::hex << this->address << std::dec << ", creating thread...\n";
		std::thread worker(threadWorker, std::ref(enabled), moduleName, std::ref(keyActivate),
			std::ref(proc), std::ref(address), std::ref(replacementBytes));
		worker.detach();
	}
	else
	{
		//not scanned.
		Close(1);
		return;
	}
}

void HackModule::Close(int code = 0)
{
	switch (code)
	{
	case 0: //this is regular
		break;
	case 1: //AOB scan went wrong
		//handle it
		std::cerr << "AOB scan went wrong!\n";
		break;
	}
}