#include "Offsets.h"
#include "RainbowSix.h"
#include <random>
#include <cstdint>
#include <windows.h>
#include <memoryapi.h>
#include <string>
#include <cstdint>
#include <thread>
#include <chrono>

uintptr_t baseAddress;


/*
Default constructor
*/
RainbowSix::RainbowSix() {

	PID = GetProcessID("RainbowSix.exe");
	baseAddress = GetModuleBaseAddr("RainbowSix.exe");
	init();

}

/*
Provided a unsigned integer pointer, set base address accordingly

@param base address
@return null
*/
void RainbowSix::setBaseAddress(uintptr_t bAddress) {

	baseAddress = bAddress;

}

/*
Initialize managers (status manager, network manager, etc)
*/
void RainbowSix::init() {

	statusManager = Read<uintptr_t>(baseAddress + OFFSET_STATUS_MANAGER);
	networkManager = Read<uintptr_t>(baseAddress + OFFSET_NETWORK_MANAGER);
	gameManager = Read<uintptr_t>(baseAddress + OFFSET_GAME_MANAGER);
	cameraManager = Read<uintptr_t>(baseAddress + OFFSET_CAMERA_MANAGER);

	this->localPlayer = getLocalPlayer();

}

/*
Provided an address of a vector of type unsigned int pointer, fill the vector with players addresses.

@param empty vector<unintptr_t>&
*/
bool RainbowSix::getEnemyPlayers(std::vector<uintptr_t>& players) {


	unsigned short local_team = getPlayerTeam(getLocalPlayer());
	std::cout << "[+] Local team: " << local_team << std::endl;
	unsigned int count = getPlayerTeam(baseAddress);
	std::cout << "[+] count: " << count << std::endl;

	if (count > 255)
		return false;

	for (unsigned int i = 0; i < count; i++)
	{
		uintptr_t target_player = getPlayerById(i);

		if (!target_player)
			continue;

		if (!localPlayer)
			continue;

		if (target_player == localPlayer)
			continue;

		if (getPlayerTeam(target_player) == local_team)
			continue;

		players.push_back(target_player);
	}

	return true;

}

uintptr_t RainbowSix::getPlayerById(unsigned int id) {

	uintptr_t entity_list = Read<uintptr_t>(gameManager + OFFSET_GAME_MANAGER_ENTITY_LIST);
	if (!entity_list)
		return NULL;

	uintptr_t entity = Read<uintptr_t>(entity_list + (sizeof(PVOID) * id));
	if (!entity)
		return NULL;

	return entity;
}

/*
Get player team provided player

@return unsigned short containing team id
*/
std::uint8_t RainbowSix::getPlayerTeam(uintptr_t entity)
{
	uintptr_t replication = Read<uintptr_t>(entity + OFFSET_ENTITY_REPLICATION);

	if (!replication)
	{
		return 0xFF;
	}
	return Read<std::uint8_t>(replication + OFFSET_ENTITY_REPLICATION_TEAM);
}






/*
Set local player

@return true or false if it worked.
*/
bool RainbowSix::setLocalPlayer()
{

	uintptr_t status_manager = Read<uintptr_t>(baseAddress + OFFSET_STATUS_MANAGER);

	if (!status_manager)
		return 0;

	uintptr_t entity_container = Read<uintptr_t>(status_manager + OFFSET_STATUS_MANAGER_CONTAINER);

	if (!entity_container)
		return 0;

	entity_container = Read<uintptr_t>(entity_container);
	if (!entity_container)
		return 0;


	entity_container = Read<uintptr_t>(entity_container + OFFSET_STATUS_MANAGER_LOCALENTITY);

	if (!entity_container)
		return 0;

	localPlayer = entity_container;

	return 1;

}

/*
Set local player provided address

@return void
*/
void RainbowSix::setLocalPlayer(uintptr_t local_player) {

	localPlayer = local_player;

}

uintptr_t RainbowSix::getBaseAddress()
{
	return uintptr_t();
}

uintptr_t RainbowSix::getPlayerByID(unsigned int i)
{
	return uintptr_t();
}

uintptr_t RainbowSix::getSpottedMarker(uintptr_t player)
{
	return uintptr_t();
}

/*
Gets the amount of players as an unsigned long

@return the amount of players as an unsigned long
*/
unsigned long RainbowSix::getPlayerCount() {

	return Read<unsigned long>(gameManager + OFFSET_GAME_MANAGER_ENTITY_COUNT) & 0x3fffffff;

}



bool RainbowSix::glow(Vector3 RGB, float Opacity, Vector2 Distance) //glow/chams 
{
	uint64_t glowbase = Read<uint64_t>(baseAddress + OFFSET_GLOW_MANAGER);

	if (!glowbase)
		return false;

	uint64_t glowbase1 = Read<uint64_t>(glowbase + 0xB8);

	Write<Vector3>(glowbase1 + 0x110, RGB);

	Write<float>(glowbase1 + 0x13C, Opacity);

	Write<Vector2>(glowbase1 + 0x130, Distance);


	return true;
}


/*bool RainbowSix::speedHack(bool enable)
{
	uint64_t temp;
	temp = Read<uintptr_t>(getLocalPlayer() + 0x38);

	if (!temp)
		return;

	Write<float>(temp + 0x78, 1.5f);
} */


bool RainbowSix::patch_terminate()
{
	HMODULE module = GetModuleHandle(("kernelbase.dll"));

	if (!module)
		return false;

	uint64_t r6_module = GetModuleBaseAddr("kernelbase.dll");

	if (!r6_module)
		return false;

	uint64_t api = reinterpret_cast<uint64_t>(GetProcAddress(module, ("TerminateProcess"))) - reinterpret_cast<uint64_t>(module) + r6_module;

	printf("[+] Quadratec Threads: %llx\n", api);

	if (change_protection(PID, api, PAGE_EXECUTE_READWRITE, 0x64) == 0)
	{
		uint8_t ret_stub[] = { 0xC3, 0x90, 0x90, 0x90 };

		WriteVirtualMemoryRaw(api, reinterpret_cast<uintptr_t>(ret_stub), sizeof(ret_stub));
		change_protection(PID, api, PAGE_EXECUTE_READ, 0x64);
	}

	return true;
}

//unlock all
bool RainbowSix::all_unlocks(bool enabled)
{
	/* uint64_t instruction = this->baseAddress + OFFSET_UNLOCKS;
	 uint8_t operand = Read<uint8_t>( instruction + 3 );

	 if ( change_protection( PID, instruction, PAGE_EXECUTE_READWRITE, 0x64 ) == 0 )
	 {
		 uint8_t unlocks_on[]  = { 0xC6, 0x47, 0x38, 0x00 };
		 uint8_t unlocks_off[] = { 0xC6, 0x47, 0x38, 0x01 };

		 if ( enabled )
			 WriteVirtualMemoryRaw( instruction, reinterpret_cast<uintptr_t>( unlocks_on ), sizeof( unlocks_on ) );
		 else
			 WriteVirtualMemoryRaw( instruction, reinterpret_cast<uintptr_t>( unlocks_off ), sizeof( unlocks_off ) );

		 change_protection( PID, instruction, PAGE_EXECUTE_READ, 0x64 );
	 }*/

	auto instruction = this->baseAddress + OFFSET_UNLOCKS;

	if (change_protection(PID, instruction, PAGE_EXECUTE_READWRITE, 4) == 0)
	{
		Write<std::uint8_t>(instruction + 3, enabled ? 0 : 1);

		change_protection(PID, instruction, PAGE_EXECUTE_READ, 4);
	}

	return true;
}




void RainbowSix::invisible()
{
	auto profile_manager = Read<uintptr_t>(this->baseAddress + 0x51b87b0);

	if (!profile_manager)
		return;

	auto entity_list = Read<uintptr_t>(Read<uintptr_t>(profile_manager + 0x68));

	if (!entity_list)
		return;

	auto operator_list = Read<uintptr_t>(entity_list + 0x2d0);

	if (!operator_list)
		return;

	for (auto i = 0; i < 100; i++)
	{
		auto coperator = Read<uintptr_t>(operator_list + (i * 8));

		if (!coperator)
			continue;

		auto headgear = Read<uintptr_t>(coperator + 0x68);

		if (!headgear)
			continue;

		Write<uintptr_t>(coperator + 0x98, headgear);
	}
}



bool RainbowSix::cav(bool enable)
{
	BYTE spotted = enable;

	uintptr_t entity_list = Read<uintptr_t>(gameManager + OFFSET_GAME_MANAGER_ENTITY_LIST);
	int entity_count = Read<DWORD>(gameManager + OFFSET_GAME_MANAGER_ENTITY_COUNT) & 0x3fffffff;
	if (entity_count == NULL) return false;

	for (int i = 0; i < entity_count; i++)
	{
		uintptr_t entity_object = Read<uintptr_t>(entity_list + i * 0x8);

		uintptr_t addr2 = Read<uintptr_t>(entity_object + 0x28);
		if (addr2 == NULL) continue;
		uintptr_t addr3 = Read<uintptr_t>(addr2 + 0xD8);
		if (addr3 == NULL) continue;

		for (auto current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t))
		{
			uintptr_t addr4 = Read<uintptr_t>(addr3 + current_component);
			if (addr4 == NULL) continue;

			if (Read<uintptr_t>(addr4) != (baseAddress + ENTITY_MARKER_VT_OFFSET)) //vt marker
				continue;

			Write<BYTE>(addr4 + 0x532, spotted);
			Write<BYTE>(addr4 + 0x534, spotted);
		}
	}
}


bool RainbowSix::enablefovchanger()
{
	float W, P;
	W = 1.800f; // RAGE FOV
	P = 2.400f; // RAGE FOV

	uint64_t OFFSET_FOV = Read<uint64_t>(baseAddress + OFFSET_FOVMANAGER);

	if (!OFFSET_FOV)
		return false;

	uint64_t weapon_fov = Read<uint64_t>(OFFSET_FOV + 0x28);
	uint64_t class1_unknown = Read<uint64_t>(weapon_fov + 0x0);

	Write<float>(class1_unknown + 0x3C, W);
	Write<float>(class1_unknown + 0x38, P);


}


bool RainbowSix::disablefovchanger()
{
	float W, P;
	W = 0.900f; // default weapon
	P = 1.550f; // default player FOV  

	uint64_t OFFSET_FOV = Read<uint64_t>(baseAddress + OFFSET_FOVMANAGER);

	if (!OFFSET_FOV)
		return false;

	uint64_t weapon_fov = Read<uint64_t>(OFFSET_FOV + 0x28);
	uint64_t class1_unknown = Read<uint64_t>(weapon_fov + 0x0);

	Write<float>(class1_unknown + 0x3C, W);
	Write<float>(class1_unknown + 0x38, P);


}




/*uintptr_t RainbowSix::getOutlineComponent() {

	uintptr_t temp;
	temp = Read<uintptr_t>(gameManager + 0x320); //read address of gamemanager + 0x320
	temp = Read<uintptr_t>(temp + 0x968); // read address of (gamemanager + 0x320) + 0x968
	return temp;

}

void RainbowSix::outlineEsp(bool enable) {


	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	Write<BYTE>(outlineComponent + 0x310, enable);


}

std::byte RainbowSix::GameStateCheck() {

	uintptr_t gameState = Read<uintptr_t>(baseAddress + OFFSET_ROUNDMANAGER);
	std::byte readByte = Read<std::byte>(gameState + 0x2E8);
	return readByte;


}

void RainbowSix::setOutlineColor(bool team, Vector4 color)
{
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	Write<Vector4>(outlineComponent + (team ? 0x48 : 0x58), color);
}


Set outline color


void RainbowSix::setOutline(bool update, bool visible)
{
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	uintptr_t objectArray = Read<uintptr_t>(outlineComponent + 0x2f0); //list
	uint32_t objectArraySize = Read<uint32_t>(outlineComponent + 0x2f8); //size
	for (size_t offset = 0x0; offset < objectArraySize * 0x68; offset += 0x90)
	{
		Write<BYTE>(objectArray + offset + 0x0, visible);
		Write<BYTE>(objectArray + offset + 0x2, update);
	}
}

/*
Get localplayer

@return uintptr_t of localplayer
*/
uintptr_t RainbowSix::getLocalPlayer() {

	uint64_t status_manager = Read<uint64_t>(baseAddress + OFFSET_PROFILE_MANAGER); //offset profile  or stat
	uint64_t entity_container = Read<uint64_t>(status_manager + 0x68);
	entity_container = Read<uint64_t>(entity_container + 0x0);
	return Read<uint64_t>(entity_container + 0x28);
}


bool RainbowSix::noRecoil(bool enable)
{
	uintptr_t LpVisualCompUnk = Read<uintptr_t>(getLocalPlayer() + 0x78);

	if (!LpVisualCompUnk)
		return false;

	uintptr_t LpWeapon = Read<uintptr_t>(LpVisualCompUnk + 0xC8);

	if (!LpWeapon)
		return false;

	uintptr_t LpCurrentDisplayWeapon = Read<uintptr_t>(LpWeapon + 0x208);

	if (!LpCurrentDisplayWeapon)
		return false;

	if (enable == true) {

		recoilNumberOne = Read<float>(LpCurrentDisplayWeapon + 0x50); //spread
		recoilNumberTwo = Read<float>(LpCurrentDisplayWeapon + 0xB0); //recoil

		Write<float>(LpCurrentDisplayWeapon + 0x50, almost_zero); //spread
		Write<float>(LpCurrentDisplayWeapon + 0xB0, almost_zero); //recoil

	}
	else {

		Write<float>(LpCurrentDisplayWeapon + 0x50, recoilNumberOne); //spread
		Write<float>(LpCurrentDisplayWeapon + 0xB0, recoilNumberTwo); //recoil

	}

	return true;
}


bool RainbowSix::noFlash(bool enable) {

	uint8_t Activate = 0;
	uintptr_t lpEventManager = Read<uintptr_t>(getLocalPlayer() + 0x30);
	uintptr_t lpFxArray = Read<uintptr_t>(lpEventManager + 0x30);
	const UINT uStunIndex = 5;
	uintptr_t lpFxStun = Read<uintptr_t>(lpFxArray + (uStunIndex * sizeof(PVOID)));
	Write<uint8_t>(lpFxStun + 0x40, Activate);
	return true;
}


void RainbowSix::NoClip(bool enable) {

	uintptr_t networkManager = Read<uintptr_t>(baseAddress + OFFSET_NETWORK_MANAGER); //network M 
	uintptr_t noclipPtr = Read<uintptr_t>(networkManager + 0xF8);

	noclipPtr = Read<uintptr_t>(noclipPtr + 0x8); //no clip 2


	noclipPtr += 0x530; //no clip 3

	Vector3 emptyVector = Vector3(0, 0, 0);
	Vector3 defaultVector = Vector3(0.0001788139343f, 0.0001788139343f, 0.03051757626f);

	Write<Vector3>(noclipPtr, (enable ? emptyVector : defaultVector));
}




/*
Test function
*/
void RainbowSix::testFunction() {

	uintptr_t entity_list = Read<uintptr_t>(gameManager + 0x1C8);
	int entity_count = Read<DWORD>(gameManager + 0x1D0);
	std::cout << "Entity count: " << entity_count << std::endl;
	Vector2 screen;
	for (int i = 0; i < entity_count; i++)
	{
		uintptr_t entity_object = Read<uintptr_t>(entity_list + i * 0x8);
		uintptr_t pawn = Read<uintptr_t>(entity_object + OFFSET_ENTITY_PAWN);
		Vector3 headLocation = Read<Vector3>(pawn + OFFSET_SKELETON_HEADPOSITION);

		Vector3 location = getPlayerHead(entity_object);

		//worldToScreen(headLocation, &screen);
		std::cout << "x: " << location.x << std::endl;
		std::cout << "y: " << location.y << std::endl;
		std::cout << "z: " << location.y << std::endl;
	}

}

uintptr_t RainbowSix::getOutlineComponent() {

	uintptr_t temp;
	temp = Read<uintptr_t>(gameManager + 0x320); //read address of gamemanager + 0x320
	temp = Read<uintptr_t>(temp + 0x968); // read address of (gamemanager + 0x320) + 0x968
	return temp;

}

void RainbowSix::outlineEsp(bool enable) {


	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	Write<BYTE>(outlineComponent + 0x310, enable);


}

std::byte RainbowSix::GameStateCheck() {

	uintptr_t gameState = Read<uintptr_t>(baseAddress + OFFSET_ROUNDMANAGER);
	std::byte readByte = Read<std::byte>(gameState + 0x2E8);
	return readByte;


}

void RainbowSix::setOutlineColor(bool team, Vector4 color)
{
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	Write<Vector4>(outlineComponent + (team ? 0x48 : 0x58), color);
}

/*
Set outline color

*/
void RainbowSix::setOutline(bool update, bool visible)
{
	uintptr_t outlineComponent = getOutlineComponent();
	if (!outlineComponent)
		return;

	uintptr_t objectArray = Read<uintptr_t>(outlineComponent + 0x2f0); //list
	uint32_t objectArraySize = Read<uint32_t>(outlineComponent + 0x2f8); //size
	for (size_t offset = 0x0; offset < objectArraySize * 0x68; offset += 0x90)
	{
		Write<BYTE>(objectArray + offset + 0x0, visible);
		Write<BYTE>(objectArray + offset + 0x2, update);
	}
}

void RainbowSix::teleport() {

	Vector3 High;
	High.y = 20;
	uint64_t r1 = Read<uint64_t>(getLocalPlayer() + 0x98);
	uint64_t aa = Read<uint64_t>(r1 + 0x148);

	uint8_t write1 = 3;
	int write2 = 3;
	uint8_t write3 = 3;

	Write<uint8_t>(aa + 0xA1, write1);

	Write<uint8_t>(aa + 0xA3, write1);
	Write<uint8_t>(aa + 0xA4, write1);
	Write<uint8_t>(aa + 0x13F, write1);
	Write<int>(aa + 0x140, write2);

	Write<uint8_t>(aa + 0x140, write3);

	Write<Vector3>(aa + 0x150, getPlayerHead(getRandomPlayer()));

	Write<Vector3>(aa + 0x180, getPlayerFeet(getLocalPlayer()));

}

uintptr_t RainbowSix::getRandomPlayer()
{
	int num = Read<UINT16>(gameManager + 0x1D0);
	uintptr_t entity_object;
	uint64_t EntList = Read<uint64_t>(gameManager + 0x1C8);


	for (int i = 0; i < num; i++)
	{

		entity_object = Read<uint64_t>(EntList + i * 0x8);
		if (entity_object != getLocalPlayer())
		{

			return entity_object;

		}

	}
}

uintptr_t RainbowSix::getClosestEnemy()
{
	const auto getViewangle = [](std::uintptr_t entity)
	{
		auto r1 = Read<uintptr_t>(entity + 0x20);
		auto r2 = Read<uintptr_t>(r1 + 0x1170);
		return Read<Vector4>(r2 + 0xc0);
	};

	std::uintptr_t resultant_entity = 0;
	static auto resultant_fov = 90.f;

	for (auto i = 0ul; i < this->getPlayerCount(); i++)
	{
		const auto entity = this->getPlayerById(i);

		const auto health = this->getPlayerHealth(entity);

		if (health <= 0 || health > 200)
			continue;

		if (this->getPlayerTeam(this->localPlayer.load()) == this->getPlayerTeam(entity))
			continue;

		const auto fov_result = closest_to_fov(this->getPlayerHead(this->localPlayer.load()), this->getPlayerHead(entity), calculate_euler(getViewangle(this->localPlayer.load())));

		if (fov_result < resultant_fov)
		{
			resultant_fov = fov_result;
			resultant_entity = entity;
		}
	}

	return resultant_entity;
}

uintptr_t RainbowSix::getClosestEnemyToLocalHeadPos() {

	int num = Read<DWORD>(gameManager + 0x1D0);

	Vector3 CurrentViewAngles = getPlayerHead(getLocalPlayer());
	uintptr_t entity_object;
	float bestDelta = FLT_MAX;
	uintptr_t BestEntity;
	uint64_t EntList = Read<uint64_t>(gameManager + 0x1C8);

	for (int i = 0; i < num; i++) {
		entity_object = Read<uint64_t>(EntList + i * 0x8);

		Vector3 angles = CalcAngle(getPlayerHead(getLocalPlayer()), getPlayerHead(entity_object));

		(angles - CurrentViewAngles).clamp();

		float Delta = angles.y;

		if (Delta < bestDelta && getLocalPlayer() != entity_object) {

			bestDelta = Delta;
			BestEntity = entity_object;
			return BestEntity;

		}

	}

	//return BestEntity;

}

Vector3 RainbowSix::getPlayerFeet(uintptr_t entity) {

	uint64_t addr1 = Read<uint64_t>(entity + OFFSET_ENTITY_PAWN);
	return Read<Vector3>(addr1 + OFFSET_ENTITYPAWN_FEET);
}

Vector3 RainbowSix::getPlayerHead(uintptr_t player)
{
	uint64_t pSkeleton = Read<uint64_t>(player + 0x20);

	if (!pSkeleton)
		return Vector3();

	return Read<Vector3>(pSkeleton + OFFSET_SKELETON_HEADPOSITION); // Ox6A0
}

/*bool RainbowSix::worldToScreen(Vector3 position, Vector2* screen)
{
	if (!cameraManager)
		return false;

	Vector3 temp = position - getViewTranslation();

	float x = temp.Dot(getViewRight());
	float y = temp.Dot(getViewUp());
	float z = temp.Dot(getViewForward() * -1.f);

	Vector2 DisplaySize = getDisplaySize();

	screen->x = (DisplaySize.x / 2.f) * (1.f + x / getViewFovX() / z);
	screen->y = (DisplaySize.y / 2.f) * (1.f - y / getViewFovY() / z);

	return z >= 1.0f ? true : false;
}*/

Vector3 RainbowSix::getViewTranslation()
{
	if (!cameraManager)
		return Vector3();

	return Read<Vector3>(cameraManager + OFFSET_CAMERA_VIEWTRANSLATION);
}
Vector3 RainbowSix::getViewRight()
{
	if (!cameraManager)
		return Vector3();

	return Read<Vector3>(cameraManager + OFFSET_CAMERA_VIEWRIGHT);
}
Vector3 RainbowSix::getViewUp()
{
	if (cameraManager)
		return Vector3();

	return Read<Vector3>(cameraManager + OFFSET_CAMERA_VIEWUP);
}

Vector3 RainbowSix::getViewForward()
{
	if (!cameraManager)
		return Vector3();

	return Read<Vector3>(cameraManager + OFFSET_CAMERA_VIEWFORWARD);
}

/*Vector2 RainbowSix::getDisplaySize()
{
	RECT rect;
	GetClientRect(targetHWND, &rect);
	return Vector2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
}*/

float RainbowSix::getViewFovX()
{
	if (!cameraManager)
		return 0.f;

	return Read<float>(cameraManager + OFFSET_CAMERA_VIEWFOVX);
}

float RainbowSix::getViewFovY()
{
	if (!cameraManager)
		return 0.f;

	return Read<float>(cameraManager + OFFSET_CAMERA_VIEWFOVY);
}

bool RainbowSix::worldToScreen(Vector3 position, Vector2* screen)
{
	return false;
}

Vector3 RainbowSix::CalcAngle(Vector3 enemy, Vector3 local)
{
	auto direction = enemy - local;

	auto x = std::asin(direction.z / direction.Length()) * 57.2957795131f;
	auto y = std::atan(direction.y / direction.x) * 57.2957795131f;

	if (direction.x >= 0.f) y += 180.f;
	if (x > 180.f) x -= 360.f;
	else if (x < -180.f) x += 360.f;

	auto angle = Vector3(x, 0.f, y + 90.f);
	angle.clamp();

	return angle;
}


bool RainbowSix::damageMultiplier(int dmg) {

	uint64_t damgebase = Read<uint64_t>(baseAddress + OFFSET_GAME_MANAGER); //game M

	if (!damgebase)
		return false;

	uint64_t dmage = Read<uint64_t>(damgebase + 0x1f8);
	uint64_t dmage1 = Read<uint64_t>(dmage + 0xD8);
	uint64_t dmage2 = Read<uint64_t>(dmage1 + 0x48);
	uint64_t dmage3 = Read<uint64_t>(dmage2 + 0x130);
	uint64_t dmage4 = Read<uint64_t>(dmage3 + 0x130);
	uint64_t dmage5 = Read<uint64_t>(dmage4 + 0x0);

	Write<unsigned int>(dmage5 + 0x40, dmg); //dmg
	return true;
}

void RainbowSix::impactBullets(bool enable) {

	if (enable == true) {

		//almost_zero = 2;
		damageMultiplier(10);


	}
	else {

		//almost_zero = 0.52;
		damageMultiplier(1);


	}


}

