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
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 1);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);
	RestoreShadows = iniReader.ReadInteger("GENERAL", "RestoreShadows", 1);
	RestoreVisualTreatment = iniReader.ReadInteger("GENERAL", "RestoreVisualTreatment", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	RestoreBackfaceCulling = iniReader.ReadInteger("GENERAL", "RestoreBackfaceCulling", 1);
	RestoreWaterReflections = iniReader.ReadInteger("GENERAL", "RestoreWaterReflections", 0);
	RemoveLastGenTextures = iniReader.ReadInteger("GENERAL", "RemoveLastGenTextures", 1);
	OptimizeRenderDistance = iniReader.ReadInteger("GENERAL", "OptimizeRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	ExpandMemoryPools = iniReader.ReadInteger("EXTRA", "ExpandMemoryPools", 1);
	DisableBackFaceCulling = iniReader.ReadInteger("EXTRA", "DisableBackFaceCulling", 0);

	if (HDReflections)
	{
		RoadResX = GetSystemMetrics(SM_CXSCREEN);
		RoadResY = GetSystemMetrics(SM_CYSCREEN);
		VehicleRes = GetSystemMetrics(SM_CYSCREEN);
		MirrorResX = GetSystemMetrics(SM_CYSCREEN) / 2;
		MirrorResY = GetSystemMetrics(SM_CYSCREEN) / 6;
	}

	// Writes Resolution Values
	{
		// Road Reflection
		RoadResX = (RoadResX * RoadScale);
		RoadResY = (RoadResY * RoadScale);
		injector::WriteMemory<uint32_t>(0x6CFC26, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, RoadResY, true);
		injector::MakeJMP(0x6BCDEE, RoadReflectionResCodeCave1, true);
		injector::MakeJMP(0x6BCE32, RoadReflectionResCodeCave2, true);
		injector::MakeNOP(0x6BCE37, 1, true);
		// Rearview Mirror
		MirrorResX = (MirrorResX * MirrorScale);
		MirrorResY = (MirrorResY * MirrorScale);
		injector::WriteMemory<uint32_t>(0x8F9008, MirrorResX, true);
		injector::WriteMemory<uint32_t>(0x8F900C, MirrorResY, true);
		// Vehicle Reflection
		VehicleRes = (VehicleRes * VehicleScale);
		injector::MakeJMP(0x6BD542, VehicleReflectionResCodeCave1, true);
		injector::MakeJMP(0x6BD58F, VehicleReflectionResCodeCave2, true);
		injector::MakeNOP(0x6BD594, 1, true);
		injector::MakeJMP(0x6BD344, VehicleReflectionResCodeCave3, true);
		injector::MakeNOP(0x6BD349, 1, true);
	
		if (OldGPUCompatibility)
		{
			// Rounds vehicle resolution down to the nearest power of two
			static int VehicleRes_POT = VehicleRes;
			VehicleRes_POT--;
			VehicleRes_POT |= VehicleRes_POT >> 1;
			VehicleRes_POT |= VehicleRes_POT >> 2;
			VehicleRes_POT |= VehicleRes_POT >> 4;
			VehicleRes_POT |= VehicleRes_POT >> 8;
			VehicleRes_POT |= VehicleRes_POT >> 16;
			VehicleRes_POT++;
			if (VehicleRes_POT > GetSystemMetrics(SM_CYSCREEN))
			{VehicleRes_POT = VehicleRes_POT / 2;}
			VehicleRes = VehicleRes_POT;
		}
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

		if (RestoreDetails >= 2)
		// Adds missing particle effects to the rearview mirror
		injector::MakeJMP(0x6DE9E3, EnableParticlesCodeCave, true);
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
		// Corrects vehicle reflection world brightness
		injector::MakeJMP(0x7696A4, VehicleReflBrightnessCodeCave, true);
		// Fixes flashing when low reflection update rate is used
		injector::MakeJMP(0x6DA528, VehicleReflBrightnessBugFixCodeCave, true);
		// Corrects mirror brightness
		injector::MakeJMP(0x6E7120, MirrorRGBCodeCave, true);
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

	if (RemoveLastGenTextures)
	{
		// Removes the "2PLAYERHEADLIGHT" texture from planar reflections
		injector::MakeNOP(0x738207, 5, true);
		// Removes sun flare texture from reflections and rearview mirror
		injector::MakeNOP(0x504C56, 5, true);
	}

	if (OptimizeRenderDistance)
	{
		injector::MakeJMP(0x6E73A1, RenderDistanceCodeCave, true);
		injector::MakeNOP(0x6DB5AB, 6, true);
		injector::MakeJMP(0x6DB5BE, SkyboxRenderDistanceCodeCave, true);
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
		injector::WriteMemory<uint32_t>(0x5009D2, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x5009DC, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A01, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A12, 0xFA000, true);
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