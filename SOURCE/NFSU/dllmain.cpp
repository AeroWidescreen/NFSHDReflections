#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"
#include "settings.h"
#include "assembly.h"
#include "stdafx.h"

void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSUHDReflections.ini");

	// Resolution
	AutoRes = iniReader.ReadInteger("RESOLUTION", "AutoRes", 1);
	CubemapScale = iniReader.ReadFloat("RESOLUTION", "CubemapScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);

	// Custom Resolution
	CubemapRes = iniReader.ReadInteger("CUSTOM RESOLUTION", "CubemapRes", 128);
	MirrorResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResX", 256);
	MirrorResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResY", 64);
	RoadResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResX", 320);
	RoadResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResY", 240);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	AspectRatioFix = iniReader.ReadInteger("GENERAL", "AspectRatioFix", 1);
	RealFrontEndReflections = iniReader.ReadInteger("GENERAL", "RealFrontEndReflections", 0);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 0);
	RestoreSkybox = iniReader.ReadInteger("GENERAL", "RestoreSkybox", 1);
	DisableRoadReflection = iniReader.ReadInteger("GENERAL", "DisableRoadReflection", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0);

	// Extra
	RealisticChrome = iniReader.ReadInteger("EXTRA", "RealisticChrome", 0);
	ExtendTunnelDistance = iniReader.ReadInteger("EXTRA", "ExtendTunnelDistance", 0);

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
		CubemapRes = CubemapRes * CubemapScale;
		MirrorResX = MirrorResX * MirrorScale;
		MirrorResY = MirrorResY * MirrorScale;
		RoadResX = RoadResX * RoadScale;
		RoadResY = RoadResY * RoadScale;
	}

	if (OldGPUCompatibility)
	{
		// Rounds the cubemap resolution down to the nearest power of two
		static int CubemapRes_POT = CubemapRes;
		CubemapRes_POT--;
		CubemapRes_POT |= CubemapRes_POT >> 1;
		CubemapRes_POT |= CubemapRes_POT >> 2;
		CubemapRes_POT |= CubemapRes_POT >> 4;
		CubemapRes_POT |= CubemapRes_POT >> 8;
		CubemapRes_POT |= CubemapRes_POT >> 16;
		CubemapRes_POT++;

		if (CubemapRes_POT > CubemapRes)
		{
			CubemapRes_POT = CubemapRes_POT >> 1;
		}
		CubemapRes = CubemapRes_POT;
	}

	// Writes Resolution Values
	{
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x40A8BB, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x40A8F1, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x40854C, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x40AACA, RoadResX, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x40A8B6, RoadResY, true);
		injector::WriteMemory<uint32_t>(0x40A8EC, RoadResY, true);
		injector::WriteMemory<uint32_t>(0x408553, RoadResY, true);
		injector::WriteMemory<uint32_t>(0x40AAD1, RoadResY, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x702A84, CubemapRes, true);
		// Rearview Mirror
		injector::WriteMemory<uint32_t>(0x702A9C, MirrorResX, true);
		injector::WriteMemory<uint32_t>(0x702AA0, MirrorResY, true);
	}

	if (ImproveReflectionLOD)
	{
		// Road Reflection LOD
		injector::MakeJMP(0x570FEA, VehicleLODCodeCave, true);
		injector::MakeJMP(0x5708F0, FEVehicleLODCodeCave, true);
		injector::MakeJMP(0x445F89, RoadReflectionLODCodeCave, true);
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x408FEC, 0x00000000, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x408F94, 0x00000000, true);
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
		// Restores skybox for vehicle reflection
		injector::MakeJMP(0x40970D, RestoreVehicleSkyboxCodeCave, true);
		// Restores skybox for road reflection
		injector::MakeJMP(0x4095EA, RestoreRoadSkyboxCodeCave, true);
		// Enables skybox
		injector::MakeNOP(0x57187B, 2, true);
		// Extends vehicle reflection render distance so skybox is visible
		injector::MakeJMP(0x40B2EC, ExtendVehicleRenderDistanceCodeCave, true);
		injector::MakeNOP(0x40B2F1, 3, true);
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

	if (RealisticChrome)
	{
		// Changes the chrome material
		injector::MakeJMP(0x40FB13, RealisticChromeCodeCave, true);
		injector::MakeNOP(0x40FB18, 1, true);
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
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 SPEED.exe from Drunk! (3.03 MB (3,178,496 bytes)).", "NFSU HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}