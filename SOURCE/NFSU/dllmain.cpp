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
	CIniReader iniReader("NFSUHDReflections.ini");

	// Resolution
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	AspectRatioFix = iniReader.ReadInteger("GENERAL", "AspectRatioFix", 1);
	RealFrontEndReflections = iniReader.ReadInteger("GENERAL", "RealFrontEndReflections", 0);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 1);
	RestoreSkybox = iniReader.ReadInteger("GENERAL", "RestoreSkybox", 1);
	DisableRoadReflection = iniReader.ReadInteger("GENERAL", "DisableRoadReflection", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0);

	// Extra
	ExtendTunnelDistance = iniReader.ReadInteger("EXTRA", "ExtendTunnelDistance", 1);

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
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x40A8BB, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x40A8F1, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x40854C, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x40AACA, RoadResX * RoadScale, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x40A8B6, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x40A8EC, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x408553, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x40AAD1, RoadResY * RoadScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x702A84, VehicleRes * VehicleScale, true);
		// RVM Reflection
		// Aspect ratio without RVM_MASK is 3:1
		injector::WriteMemory<uint32_t>(0x702A9C, MirrorResX, true);
		injector::WriteMemory<uint32_t>(0x702AA0, MirrorResY, true);

		if (OldGPUCompatibility)
		{
			// Rounds vehicle resolution down to the nearest power of two
			static int VehicleRes_POT = (VehicleRes * VehicleScale);
			VehicleRes_POT--;
			VehicleRes_POT |= VehicleRes_POT >> 1;
			VehicleRes_POT |= VehicleRes_POT >> 2;
			VehicleRes_POT |= VehicleRes_POT >> 4;
			VehicleRes_POT |= VehicleRes_POT >> 8;
			VehicleRes_POT |= VehicleRes_POT >> 16;
			VehicleRes_POT++;
			injector::WriteMemory<uint32_t>(0x702A84, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD)
	{
		// Road Reflection (Vehicle) LOD
		injector::MakeJMP(0x570FEA, VehicleLODCodeCave, true);
		injector::MakeJMP(0x5708F0, FEVehicleLODCodeCave, true);
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x408FEC, 0x00006002, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x408F94, 0x00006002, true);
		// Road Reflection LOD
		injector::MakeJMP(0x445F89, RoadReflectionLODCodeCave, true);
	}

	if (AspectRatioFix)
	{
		injector::WriteMemory<float>(0x6B6348, 8.0f, true);
		injector::WriteMemory<float>(0x6B634C, 8.0f, true);
	}

	if (RealFrontEndReflections)
	{
		injector::WriteMemory<uint32_t>(0x4AE59F, 0x00000000, true);
	}

	if (ForceEnableMirror)
	{
		// Enables mirror for all camera views
		injector::MakeJMP(0x4C1F43, ForceEnableMirrorCodeCave1, true);
		// Enables mirror option for all camera views
		injector::MakeJMP(0x40843F, ForceEnableMirrorCodeCave2, true);
	}

	if (RestoreSkybox)
	{
		// Restores skybox for RVM
		injector::MakeJMP(0x409784, RestoreMirrorSkyboxCodeCave, true);
		// Extends RVM distance so skybox is visible
		injector::WriteMemory<uint32_t>(0x6B6CC0, 0x461C4000, true);
		// Restores skybox for vehicle reflection
		injector::MakeJMP(0x40970D, RestoreVehicleSkyboxCodeCave, true);
		// Extends vehicle reflection render distance so skybox is visible
		injector::MakeJMP(0x40B2EC, ExtendVehicleRenderDistanceCodeCave, true);
		// Restores skybox for road reflection
		injector::MakeJMP(0x4095EA, RestoreRoadSkyboxCodeCave, true);
		// Enables skybox
		injector::MakeNOP(0x57187B, 2, true);
	}

	if (DisableRoadReflection)
	{
		// Disables road reflection that appears in vehicle and mirror
		injector::MakeNOP(0x40C95D, 2, true);
	}

	if (ExtendTunnelDistance)
	{
		static int TunnelDistance = 24;
		// Main
		injector::WriteMemory(0x4AE5BE, &TunnelDistance, true);
		// Road Reflection
		injector::WriteMemory(0x4AE57E, &TunnelDistance, true);
		// Vehicle Reflection
		injector::WriteMemory(0x4AE59A, &TunnelDistance, true);
	}

	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity = (0.5f * VehicleReflectionBrightness);
		injector::WriteMemory(0x40FBB9, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x40FBCF, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x40FBE5, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x40FBFB, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x40FC20, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x40FC3A, &VehicleReflectionIntensity, true);
	}
}
	
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x670CB5) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English speed.exe (3,03 MB (3.178.496 bytes)).", "NFSU HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}