#include <iostream>
#include <msxml.h>
#include <thread>         
#include <chrono>

std::atomic_bool silent_aim(false);
std::atomic_bool aimbot_on(false);

int main()
{
	uintptr_t modulebase;

	SetConsoleTitleW(L"Hack Provided By https://discord.gg/ArPNUXd"); //*sigh* great now we need to update the usermode with the loader 

	//for hardware lock
	//This shows how dumb pasted is

	/*HW_PROFILE_INFO hwProfileInfo;
	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		std::string guid = hwProfileInfo.szHwProfileGuid;

		if (guid.compare("{878a8ba5-b33a-11e9-b887-806e6f6e6963}") != 0) { //3

			std::cout << "Developer Mode Only, Your Not a Developer!?" << std::endl;
			std::cin.get();
			exit(1);

		}

	}*/

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 12;
	std::string input;
	std::cout << "Join discord for help?(Y/N)"<<std::endl;
	std::cin >> input;
	if (input == "Y" || input == "y") {
		ShellExecute(0, 0, "https://discord.gg/ArPNUXd", 0, 0, SW_SHOW);
	}
	else {
		std::cout << "https://discord.gg/ArPNUXd"<<std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15); //set back to black background and white text
	//This shows how dumb pasted is and cringe
	std::cout << "[+] Establishing Connection with Driver " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "[+] Connected " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(4));





	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, 8);

	std::cout << "[-] Quadratec Cheat Has Been Loaded!" << std::endl;
	std::cout << "[-] Version:  Beta " << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	PID = GetProcessID("RainbowSix.exe");
	modulebase = GetModuleBaseAddr("RainbowSix.exe");
	SetConsoleTextAttribute(hConsole, 15); //set back to black background and white text
	std::cout << "[+] Base Address : " << std::hex << modulebase << std::endl;
	LoadLibrary("user32.dll");
	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	RainbowSix rainbowSix;
	RainbowSix::RainbowSix();
	bool glow = false;
	bool esp = false;
	bool outlineEsp = false;
	bool noRecoil = false;
	bool speedHack = false;
	bool noFlash = false;
	bool damageMul = false;
	bool impactBullets = false;
	bool NoClip = false;
	bool glowPlayers = false;
	bool all_unlocks = false;
	bool triggerbot = false;
	bool SetFOV = false;
	bool fovchanger = false;


	//F1: ESP
	//F2: setoutline        
	//F3: Glow
	//F4: no recoil
	//F5: No Flash
	//F6: Aimbot
	//F7: Damage Multiplier
	//F8: Impact bullets
	//F9: No Clip
	//F10 Unlock All
	//F11 Invisible
	//F12 High FOV

	//Doesnt do anything
	std::cout << "[+] Use the Quadratec.txt file for key binds!" << std::endl;
	SetConsoleTextAttribute(hConsole, col);

	rainbowSix.patch_terminate();
	Vector2 GlowDistance = Vector2(5, 35);
	Vector3 GlowColours = Vector3(0.5372549295, 0.5372549295, 0.5372549295);

	while (true)
	{

		if (GetAsyncKeyState(VK_F3) & 1)
		{

			if (glow == true)
			{

				std::cout << "[GLOW] Disabled " << std::endl;
				glow = false;
				rainbowSix.glow(GlowColours,1.0, GlowDistance);

			}
			else
			{

				std::cout << "[GLOW] Enabled " << std::endl;
				glow = true;

				rainbowSix.glow(GlowColours, 0.0, GlowDistance);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F1) & 1)
		{

			if (esp == true)
			{

				std::cout << "[CAV ESP] Disabled " << std::endl;
				esp = false;

				rainbowSix.cav(esp);

			}
			else
			{

				std::cout << "[CAV ESP] Enabled " << std::endl;
				esp = true;

				rainbowSix.cav(esp);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F12) & 1)
		{

			if (SetFOV == true)
			{

				std::cout << "[HIGHFOV] Disabled " << std::endl;
				SetFOV = false;

				rainbowSix.disablefovchanger();

			}
			else
			{

				std::cout << "[HIGHFOV] Enabled " << std::endl;
				SetFOV = true;

				rainbowSix.enablefovchanger();

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F9) & 1)
		{

			if (NoClip == true)
			{

				std::cout << "[NOCLIP] Disbaled" << std::endl;
				NoClip = false;

				rainbowSix.NoClip(NoClip);

			}
			else
			{

				std::cout << "[NOCLIP] Enabled" << std::endl;
				NoClip = true;

				rainbowSix.NoClip(NoClip);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F4) & 1)
		{

			if (noRecoil == true)
			{

				std::cout << "[NORECOIL/NOSPREAD] Disbaled" << std::endl;
				noRecoil = false;

				rainbowSix.noRecoil(noRecoil);

			}
			else
			{

				std::cout << "[NORECOIL/NOSPREAD] Enabled" << std::endl;
				noRecoil = true;

				rainbowSix.noRecoil(noRecoil);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F5) & 1)
		{

			if (noFlash == true)
			{
				std::cout << "[NO FLASH] Disbaled" << std::endl;
				noFlash = false;

				rainbowSix.noFlash(noFlash);

			}
			else
			{

				std::cout << "[NO FLASH] Enabled" << std::endl;
				noFlash = true;

				rainbowSix.noFlash(noFlash);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F7) & 1)
		{

			if (damageMul == true)
			{
				std::cout << "[DAMAGE MUTIPLIER] Disbaled" << std::endl;
				damageMul = false;

				rainbowSix.damageMultiplier(1);

			}

			else
			{

				damageMul = true;
				std::cout << "[DAMAGE MUTIPLIER] Enabled" << std::endl;

				rainbowSix.damageMultiplier(10);

			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15)); \
		}

		if (GetAsyncKeyState(VK_F8) & 1)
		{

			if (impactBullets == true)
			{

				std::cout << "[SHOTGUN PELLETS] Disabled" << std::endl;
				impactBullets = false;
				rainbowSix.impactBullets(impactBullets);
			}
			else
			{

				std::cout << "[SHOTGUN PELLETS] Enabled" << std::endl;
				impactBullets = true;

				rainbowSix.impactBullets(impactBullets);

			}
			Sleep(250);
		}

		if (GetAsyncKeyState(VK_F2) & 1)
		{

			if (outlineEsp == true)
			{

				std::cout << "[OUTLINE] Disabled" << std::endl;
				outlineEsp = false;
				rainbowSix.outlineEsp(outlineEsp);
			}
			else
			{

				std::cout << "[OUTLINE] Enabled" << std::endl;
				outlineEsp = true;

				rainbowSix.outlineEsp(outlineEsp);

			}
			Sleep(250);
		}


		if (GetAsyncKeyState(VK_F10) & 1)
		{
			if (all_unlocks == true)
			{

				std::cout << "[UNLOCK ALL] Disabled" << std::endl;
				all_unlocks = false;
				rainbowSix.all_unlocks(all_unlocks);
			}
			else
			{

				std::cout << "[UNLOCK ALL] Enabled" << std::endl;
				all_unlocks = true;
				rainbowSix.all_unlocks(all_unlocks);
				rainbowSix.patch_terminate();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}

		if (GetAsyncKeyState(VK_F11) & 1)
		{
			std::cout << "[INVISIBLE] Enabled" << std::endl;
			rainbowSix.invisible();
		}

		/*if ( GetAsyncKeyState( VK_F2 ) & 1 )
		{
			silent_aim.store( !silent_aim.load( ) );
			std::printf( "[SILENT AIM] %s\n", silent_aim.load( ) ? "ENABLED" : "DISABLED" );

			rainbowSix.silent_aim( silent_aim.load( ) );
		}
		 */
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			aimbot_on.store(!aimbot_on.load());
			std::printf("[AIMBOT] %s\n", aimbot_on.load() ? "ENABLED" : "DISABLED");
		}

		static auto last_entity = 0ull;
		static auto last_key = false;

		if (GetAsyncKeyState(VK_RBUTTON) && aimbot_on.load())
		{
			auto entity = 0ull;

			if (last_entity && last_key && rainbowSix.getPlayerHealth(last_entity))
				entity = last_entity;
			else
				entity = rainbowSix.getClosestEnemy();

			if (!entity)
				continue;

			auto angle = rainbowSix.CalcAngle(rainbowSix.getPlayerHead(entity), rainbowSix.getPlayerHead(rainbowSix.localPlayer.load()));

			rainbowSix.set_viewangle(rainbowSix.localPlayer.load(), silent_aim.load() ? 0x134 : 0xc0, rainbowSix.calculate_quaternion(angle));

			last_entity = entity;
			last_key = true;
		}
		else { last_key = false; }

		rainbowSix.localPlayer.store(rainbowSix.getLocalPlayer());

		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}
}