#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>
#include "settings.h"
#include "assembly.h"

void Init()
{

	// Read values from .ini
	CIniReader iniReader("NFSMWHDReflections.ini");

	// Resolution
	AutoRes = iniReader.ReadInteger("RESOLUTION", "AutoRes", 1);
	Scale = iniReader.ReadFloat("RESOLUTION", "Scale", 1.0f);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	FECubemapRes = iniReader.ReadInteger("RESOLUTION", "FECubemapRes", 256);

	// Custom Resolution
	CubemapRes = iniReader.ReadInteger("CUSTOM RESOLUTION", "CubemapRes", 256);
	MirrorResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResX", 256);
	MirrorResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResY", 256);
	RoadResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResX", 320);
	RoadResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResY", 240);

	// General
	CubemapBrightnessFix = iniReader.ReadInteger("GENERAL", "CubemapBrightnessFix", 1);
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 1);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);
	RestoreShadows = iniReader.ReadInteger("GENERAL", "RestoreShadows", 1);
	RestoreVisualTreatment = iniReader.ReadInteger("GENERAL", "RestoreVisualTreatment", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	RestoreCars = iniReader.ReadInteger("GENERAL", "RestoreCars", 0);
	RestoreBackfaceCulling = iniReader.ReadInteger("GENERAL", "RestoreBackfaceCulling", 1);
	RestoreWaterReflections = iniReader.ReadInteger("GENERAL", "RestoreWaterReflections", 0);
	RemoveLastGenEffects = iniReader.ReadInteger("GENERAL", "RemoveLastGenEffects", 1);
	OptimizeRenderDistance = iniReader.ReadInteger("GENERAL", "OptimizeRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	ExpandMemoryPools = iniReader.ReadInteger("EXTRA", "ExpandMemoryPools", 1);
	DisableBackFaceCulling = iniReader.ReadInteger("EXTRA", "DisableBackFaceCulling", 0);

	if (AutoRes)
	{
		CubemapRes = GetSystemMetrics(SM_CYSCREEN);
		MirrorResX = GetSystemMetrics(SM_CXSCREEN) / 2;
		MirrorResY = GetSystemMetrics(SM_CYSCREEN) / 6;
		RoadResX = GetSystemMetrics(SM_CXSCREEN);
		RoadResY = GetSystemMetrics(SM_CYSCREEN);
	}

	// Resolution Multiplier
	{
		CubemapRes = CubemapRes * Scale;
		MirrorResX = MirrorResX * Scale;
		MirrorResY = MirrorResY * Scale;
		RoadResX = RoadResX * Scale;
		RoadResY = RoadResY * Scale;
	}

	if (OldGPUCompatibility)
	{
		// Rounds the cubemap resoltution down to the nearest power of two
		static int CubemapRes_POT = CubemapRes;
		CubemapRes_POT--;
		CubemapRes_POT |= CubemapRes_POT >> 1;
		CubemapRes_POT |= CubemapRes_POT >> 2;
		CubemapRes_POT |= CubemapRes_POT >> 4;
		CubemapRes_POT |= CubemapRes_POT >> 8;
		CubemapRes_POT |= CubemapRes_POT >> 16;
		CubemapRes_POT++;

		if (CubemapRes_POT > CubemapRes)
		{CubemapRes_POT = CubemapRes_POT >> 1;}
		CubemapRes = CubemapRes_POT;
	}

	// Writes Resolution Values
	{
		// Road Reflection
		injector::WriteMemory<uint32_t>(0x6CFC26, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, RoadResY, true);
		injector::MakeJMP(0x6BCDEE, RoadReflectionResCodeCave1, true);
		injector::MakeJMP(0x6BCE32, RoadReflectionResCodeCave2, true);
		injector::MakeNOP(0x6BCE37, 1, true);
		// Rearview Mirror
		injector::WriteMemory<uint32_t>(0x8F9008, MirrorResX, true);
		injector::WriteMemory<uint32_t>(0x8F900C, MirrorResY, true);
		// Vehicle Reflection
		injector::MakeJMP(0x6BD542, VehicleReflectionResCodeCave1, true);
		injector::MakeJMP(0x6BD58F, VehicleReflectionResCodeCave2, true);
		injector::MakeNOP(0x6BD594, 1, true);
		injector::MakeJMP(0x6BD344, VehicleReflectionResCodeCave3, true);
		injector::MakeNOP(0x6BD349, 1, true);
		// Front-End Cubemap
		injector::WriteMemory(0x6BD4FE, &FECubemapRes, true);

	}

	if (ImproveReflectionLOD)
	{
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x6BFEBD, 0x00006002, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x6BFE58, 0x00006002, true);
		// Bypasses old Widescreen Fix LOD changes
		injector::MakeJMP(0x6BFE3D, LODBypassCodeCave, true);
		injector::MakeNOP(0x6BFE42, 1, true);
		// Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x4FAE9A, 0xEB, true);
		// Full RVM LOD
		injector::WriteMemory<uint8_t>(0x4FAEB0, 0xEB, true);
		// Full Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x6BFF21, 0x06, true);
		injector::WriteMemory<uint8_t>(0x7293DB, 0x06, true);
	}

	if (RestoreDetails >= 1)
	{
		// Adds missing traffic lights to the rearview mirror
		injector::MakeJMP(0x6DE9F0, TrafficLightRestorationCodeCave, true);
		// Solves flare and car shadow layering issue
		// Adds missing sunflare to the rearview mirror
		// Adds missing destroyable objects to the rearview mirror
		injector::MakeJMP(0x6DB260, SunflareCodeCave1, true);
		injector::MakeJMP(0x6E6DAD, SunflareCodeCave2, true);
		injector::MakeJMP(0x6DB0FE, SunflareCodeCave3, true);
		injector::MakeJMP(0x6DE96D, RestoreDetailsCodeCave, true);
		// Solves flare culling issue 
		injector::WriteMemory<uint8_t>(0x729479, 0xEB, true);
		// Adds missing car shadow to the rearview mirror
		injector::MakeJMP(0x74E812, EnableCarShadowCodeCave, true);
		// Adds missing car flares to cubemap
		injector::WriteMemory<uint8_t>(0x6DE81D, 0x00, true);

		if (RestoreDetails >= 2)
		// Adds missing particle effects to the rearview mirror
		injector::MakeJMP(0x6DE9E3, EnableParticlesCodeCave, true);
	}

	if (RestoreCars)
	{
		injector::MakeJMP(0x6DE7CA, DrawCarsForCubemap, true);
	}

	if (ForceEnableMirror)
	{
		// Enables the mirror for all camera views
		injector::MakeNOP(0x6CFB72, 2, true);
		injector::MakeNOP(0x6CFBC5, 2, true);
		injector::WriteMemory<uint16_t>(0x595DDD, 0x14EB, true);
	}

	if (RestoreBackfaceCulling)
	{
		// Solves backface culling issue for upside down car model in the front-end
		injector::MakeJMP(0x6DEF3D, FECarModelFixCodeCave, true);
	}

	if (RestoreShaders)
	{
		// Enables all shader effects for reflections
		// Credit to osdever for discovery
		injector::WriteMemory<uint8_t>(0x6DABD6, 0xEB, true);
		// Removes road reflection from other reflections
		injector::MakeJMP(0x6D7289, RemoveRoadReflectionCodeCave, true);

		if (CubemapBrightnessFix)
		{
			// Corrects environment brightness
			injector::MakeCALL(0x6DE6B0, EnvmapBrightnessCodeCave, true);
		}
	}

	if (RestoreShadows)
	{
		// Enables Shadows for Mirror
		injector::MakeJMP(0x6DE88E, ShadowMirrorEnablerCodeCave, true);
	}

	if (RestoreVisualTreatment)
	{
		// Enables Visual Treatment for Mirror
		injector::MakeJMP(0x6DE9F8, VisualTreatmentMirrorEnablerCodeCave, true);
		injector::MakeNOP(0x6DE9FD, 1, true);
	}

	if (RestoreWaterReflections)
	{
		// Enables rendering for planar reflections at all times
		injector::MakeNOP(0x6BFEF7, 2, true);
		injector::MakeNOP(0x6DE508, 2, true);
	}

	if (RemoveLastGenEffects)
	{
		// Removes sun flare texture from reflections and rearview mirror
		injector::MakeNOP(0x504C56, 5, true);
		if (RemoveLastGenEffects >= 2)
		// Removes the "2PLAYERHEADLIGHT" texture from planar reflections
		{injector::MakeNOP(0x738207, 5, true);}
	}

	if (OptimizeRenderDistance)
	{
		injector::MakeJMP(0x6E73A1, RenderDistanceCodeCave, true);
		injector::MakeNOP(0x6DB5AB, 6, true);
		injector::MakeJMP(0x6DB5BE, SkyboxRenderDistanceCodeCave, true);
		injector::MakeNOP(0x6DB5C3, 1, true);
	}

	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity1 = (0.5f * VehicleReflectionBrightness);
		static float VehicleReflectionIntensity2 = (1.0f * VehicleReflectionBrightness);
		injector::WriteMemory(0x6C77CE, &VehicleReflectionIntensity1, true);
		injector::WriteMemory(0x6C77D6, &VehicleReflectionIntensity2, true);
	}

	if (ExpandMemoryPools)
	{
		// GManager::PreloadTransientVaults (0x2C8000)
		injector::WriteMemory<short>(0x5F7396, 0x2C80, true);
		injector::WriteMemory<short>(0x5F73B2, 0x2C80, true);
		// InitializeEverything (0x8000)
		injector::WriteMemory<BYTE>(0x665FDC, 0x80, true);
		// FEngMemoryPoolSize (InitFEngMemoryPool)
		injector::WriteMemory<DWORD>(0x8F5790, 0x0BE6E0, true);
		// CarLoaderPoolSizes
		injector::WriteMemory<DWORD>(0x8F7EF0, 0x01CC00, true);
		// Fixes disappearing objects
		injector::WriteMemory<int>(0x5009D2, 2048000, true);
		injector::WriteMemory<int>(0x5009DC, 2048000, true);
		injector::WriteMemory<int>(0x500A01, 2048000, true);
		injector::WriteMemory<int>(0x500A12, 2048000, true);
	}

	if (DisableBackFaceCulling)
	{
		injector::WriteMemory<byte>(0x8FAE44, 0x00, true);
	}
}
	
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x7C4040) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.3 SPEED.exe from RELOADED (5.75 MB (6,029,312 bytes)).", "NFSMW HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}