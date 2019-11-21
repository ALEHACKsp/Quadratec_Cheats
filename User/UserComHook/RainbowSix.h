#pragma once
#include <cstdint>
#include <vector>
#include "Vectors.h"
#include "MemoryFunctions.h"
#include <Windows.h>
#include <atomic>

#define cauto const auto
class RainbowSix {

public:

	RainbowSix();
	RainbowSix(uintptr_t bAddress);
	void setBaseAddress(uintptr_t bAddress);
	bool getEnemyPlayers(std::vector<uintptr_t>& players);
	void setLocalPlayer(uintptr_t local_player);
	uintptr_t getBaseAddress();
	//bool glow(bool enable); //not the same
	bool glow(Vector3 RGB, float Opacity, Vector2 Distance);
	void NoClip(bool enable);
	void setOutline(bool update, bool visible);
	void outlineEsp(bool enable);
	uintptr_t getOutlineComponent();
	void setOutlineColor(bool team, Vector4 color);
	bool noRecoil(bool enable);
	bool noFlash(bool enable);
	void teleport();
	//bool speedHack(bool enable);
	bool patch_terminate();

	bool enablefovchanger();
	bool disablefovchanger();



	bool damageMultiplier(int dmg);
	std::byte GameStateCheck();

	Vector4 teamOutlineColor = Vector4(0.0, 1.0, 0.0, 1.0); //0.0, 1.0, 0.0, 1.0
	Vector4 enemyOutlineColor = Vector4(1.0, 0, 0, 1.0); //1.0, 0, 0, 1.0
	uintptr_t getLocalPlayer();
	void testFunction();

	void invisible();
	void impactBullets(bool enable);


	unsigned int playerCount;
	unsigned int playerTeam;

	uintptr_t entityContainer;
	uintptr_t baseAddress;


	uintptr_t statusManager;
	uintptr_t networkManager;
	uintptr_t gameManager;
	std::atomic<std::uintptr_t> localPlayer;
	uintptr_t cameraManager;

	uintptr_t getPlayerByID(unsigned int i);
	uintptr_t getSpottedMarker(uintptr_t player);
	unsigned long getPlayerCount();
	std::uint8_t getPlayerTeam(uintptr_t entity);
	uintptr_t getPlayerById(unsigned int id);
	bool setLocalPlayer();
	void init();
	//uintptr_t getOutlineComponent();
	Vector3 getViewTranslation();
	Vector3 getViewRight();
	Vector3 getViewUp();
	Vector3 getViewForward();
	Vector3 getPlayerFeet(uintptr_t entity);
	Vector3 getPlayerHead(uintptr_t player);
	//Vector2 getDisplaySize();
	uintptr_t getClosestEnemy();
	uintptr_t getClosestEnemyToLocalHeadPos();

	std::uint32_t getPlayerHealth(std::uintptr_t entity)
	{
		auto r1 = Read<uintptr_t>(entity + 0x28);
		auto r2 = Read<uintptr_t>(r1 + 0xd8);
		auto r3 = Read<uintptr_t>(r2 + 0x8);
		return Read<std::uint32_t>(r3 + 0x148);
	}
	float getViewFovX();
	float getViewFovY();

	bool worldToScreen(Vector3 position, Vector2* screen);

	float recoilNumberOne;
	float recoilNumberTwo;
	float almost_zero = 0.f;
	float normalSpeedWalking = 1.45;
	float normalSpeedRunning = 1.93;
	float speedAddition = 0.5;
	Vector3 CalcAngle(Vector3 enemy, Vector3 local);
	Vector4 calculate_quaternion(Vector3 euler)
	{
		Vector4 result{};

		cauto yaw = (euler.z * 0.01745329251f) * 0.5f;
		cauto sy = std::sin(yaw);
		cauto cy = std::cos(yaw);

		cauto roll = (euler.x * 0.01745329251f) * 0.5f;
		cauto sr = std::sin(roll);
		cauto cr = std::cos(roll);

		constexpr auto sp = 0.f;
		constexpr auto cp = 1.f;

		result.x = cy * sr * cp - sy * cr * sp;
		result.y = cy * cr * sp + sy * sr * cp;
		result.z = sy * cr * cp - cy * sr * sp;
		result.w = cy * cr * cp + sy * sr * sp;

		return result;
	}
	float closest_to_fov(Vector3 source_head, Vector3 entity_head, Vector3 source_angle)
	{
		const auto calc_angle = [](Vector3 src, Vector3 dest)
		{
			cauto delta = src - dest;

			cauto y = -asin(delta.z / src.Distance(dest)) * 57.2957795131f;
			auto x = (atan2(delta.y, delta.x) * 57.2957795131f) + 90.f;

			if (x > 180.f) x -= 360.f;
			else if (x < -180.f) x += 360.f;

			cauto clamp = [](Vector3& angle)
			{
				if (angle.y > 75.f) angle.y = 75.f;
				else if (angle.y < -75.f) angle.y = -75.f;
				if (angle.x < -180.f) angle.x += -360.f;
				else if (angle.x > 180.f) angle.x -= 360.f;

				angle.z = 0.f;
			};

			auto angle = Vector3(x, y, 0.f);
			clamp(angle);

			return angle;
		};

		cauto aim_angle = calc_angle(source_head, entity_head);

		cauto calc_fov = [](Vector3 src, Vector3 aim)
		{
			aim -= src;

			if (aim.x > 180.f)
				aim.x -= 360.f;
			else if (aim.x < -180.f)
				aim.x += 360.f;
			if (aim.y > 180.f)
				aim.y -= 360.f;
			else if (aim.y < -180.f)
				aim.y += 360.f;

			return aim;
		};

		auto angle = calc_fov(source_angle, aim_angle);

		angle.x = std::abs(angle.x);
		angle.y = std::abs(angle.y);

		return angle.x + angle.y;
	}
	Vector3 calculate_euler(Vector4 quat)
	{
		auto y_p2 = quat.y * quat.y;

		auto x = std::atan2(2.f * (quat.w * quat.z + quat.x * quat.y), (1.f - 2.f * (y_p2 + quat.z * quat.z))) * 57.2957795131f;
		auto y = std::atan2(2.f * (quat.w * quat.x + quat.y * quat.z), (1.f - 2.f * (quat.x * quat.x + y_p2))) * 57.2957795131f;

		return Vector3(x, y, 0.f);
	}
	uintptr_t getRandomPlayer();

	void set_viewangle(std::uintptr_t entity, std::uintptr_t offset, Vector4 value)
	{
		auto r1 = Read<uintptr_t>(entity + 0x20);
		auto r2 = Read<uintptr_t>(r1 + 0x1170);

		Write<Vector4>(r2 + offset, value);
	}
};