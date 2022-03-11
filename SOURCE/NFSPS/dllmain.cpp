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
	CIniReader iniReader("NFSPSHDReflections.ini");

	// Resolution
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	Scale = iniReader.ReadFloat("RESOLUTION", "Scale", 1.0f);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	ImproveReflectionSkybox = iniReader.ReadInteger("GENERAL", "ImproveReflectionSkybox", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	MissingReflectionFix = iniReader.ReadInteger("GENERAL", "MissingReflectionFix", 1);
	ExtendRenderDistance = iniReader.ReadInteger("GENERAL", "ExtendRenderDistance", 0);
	RealFrontEndReflections = iniReader.ReadInteger("GENERAL", "RealFrontEndReflections", 0);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	GammaFix = iniReader.ReadInteger("EXTRA", "GammaFix", 1);
	RealisticChrome = iniReader.ReadInteger("EXTRA", "RealisticChrome", 0);

	if (HDReflections)
	{
		VehicleRes = ::GetSystemMetrics(SM_CYSCREEN);
	}

	// Writes Resolution Values
	{
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x4BD062, VehicleRes * Scale, true);
		injector::WriteMemory<uint32_t>(0x4BD24D, VehicleRes * Scale, true);
		injector::WriteMemory<uint32_t>(0x4BD283, VehicleRes * Scale, true);
		injector::WriteMemory<uint32_t>(0x4BD288, VehicleRes * Scale, true);

		if (OldGPUCompatibility)
		{
			// Rounds vehicle resolution down to the nearest power of two
			static int VehicleRes_POT = (VehicleRes * Scale);
			VehicleRes_POT--;
			VehicleRes_POT |= VehicleRes_POT >> 1;
			VehicleRes_POT |= VehicleRes_POT >> 2;
			VehicleRes_POT |= VehicleRes_POT >> 4;
			VehicleRes_POT |= VehicleRes_POT >> 8;
			VehicleRes_POT |= VehicleRes_POT >> 16;
			VehicleRes_POT++;
			if (VehicleRes_POT > GetSystemMetrics(SM_CYSCREEN))
			{VehicleRes_POT = VehicleRes_POT / 2;}
			injector::WriteMemory<uint32_t>(0x4BD062, VehicleRes_POT, true);
			injector::WriteMemory<uint32_t>(0x4BD24D, VehicleRes_POT, true);
			injector::WriteMemory<uint32_t>(0x4BD283, VehicleRes_POT, true);
			injector::WriteMemory<uint32_t>(0x4BD288, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD)
	{
		// Vehicle Reflection LOD
		injector::MakeJMP(0x74489D, VehicleReflectionLODCodeCave, true);
		injector::MakeNOP(0x7448A2, 1, true);
	}

	if (ImproveReflectionSkybox)
	{
		// Vehicle Reflection Horizon Quality
		injector::MakeJMP(0x1A11340, VehicleHorizonQualityCodeCave, true);

		if (ImproveReflectionSkybox >= 2)
		{
			// Vehicle Reflection Skybox Quality
			injector::MakeJMP(0x777104, VehicleSkyboxQualityCodeCave, true);
			injector::MakeNOP(0x777109, 1, true);
		}
	}

	if (RestoreDetails)
	{
		// Refelction Shaders, Shadows, and Markings.
		injector::WriteMemory<uint32_t>(0x4CA478, 0x00000000, true);
	}

	if (MissingReflectionFix)
	{
		static float RenderValue = 0.5f;
		injector::WriteMemory(0x4BDE3E, &RenderValue, true);
	}

	if (ExtendRenderDistance)
	{
		// Vehicle Reflection Render Distance
		injector::MakeJMP(0x4BDEA0, ExtendRenderDistanceCodeCave, true);
		injector::MakeNOP(0x4BDEA5, 3, true);
	}

	if (RealFrontEndReflections)
	{
		// Enables reflection mapping 
		injector::MakeNOP(0x4B8D1B, 2, true);
		injector::MakeNOP(0x4B8B8B, 2, true);
		injector::MakeNOP(0x4B8C66, 2, true);
		// Corrects brightness
		injector::WriteMemory<uint8_t>(0x701606, 0xEB, true);
		injector::MakeJMP(0x4B2095, VehicleSkyboxBrightnessCodeCave, true);
		injector::MakeNOP(0x4B209A, 3, true);
		injector::MakeJMP(0x4CA481, VehicleWorldBrightnessCodeCave, true);
		injector::MakeNOP(0x4CA486, 1, true);
		// Vehicle Reflection LOD
		injector::MakeJMP(0x74489D, VehicleReflectionLODCodeCave, true);
		injector::MakeNOP(0x7448A2, 1, true);
	}

	if (VehicleReflectionBrightness)
	{
		static double VehicleReflectionIntensity = (0.5f * VehicleReflectionBrightness);
		injector::WriteMemory(0x4B7AB3, &VehicleReflectionIntensity, true);
	}

	if (GammaFix)
	{
		// Sets default brightness to 50%
		injector::WriteMemory<float>(0xAA9630, 1.0f, true);
		// Custom brightness code
		injector::MakeJMP(0x4B3E89, BrightnessFixCodeCave, true);
		injector::MakeNOP(0x4B3E8E, 1, true);
	}

	if (RealisticChrome)
	{
		// Chrome materiel
		injector::MakeJMP(0x4B7933, RealisticChromeCodeCave, true);
		injector::MakeNOP(0x4B7938, 1, true);
	}
}
	
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x00828C25) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.1 NFS.exe from BATTERY (27,4 MB (28.739.656 bytes)).", "NFSPS HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}