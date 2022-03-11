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
	CIniReader iniReader("NFSU2HDReflections.ini");

	// Resolution
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	AspectRatioFix = iniReader.ReadInteger("GENERAL", "AspectRatioFix", 1);
	RemoveParticleEffects = iniReader.ReadInteger("GENERAL", "RemoveParticleEffects", 1);
	FrontEndReflectionBlur = iniReader.ReadInteger("GENERAL", "FrontEndReflectionBlur", 1);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 1);
	RestoreSkybox = iniReader.ReadInteger("GENERAL", "RestoreSkybox", 1);
	ExtendRenderDistance = iniReader.ReadInteger("GENERAL", "ExtendRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0);

	// Extra
	NewMotionBlur = iniReader.ReadInteger("EXTRA", "NewMotionBlur", 0);
	RealisticChrome = iniReader.ReadInteger("EXTRA", "RealisticChrome", 0);

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
		// Jumps
		injector::MakeJMP(0x5BA50D, RestoreFEReflectionCodeCave, true);
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x5BA08F, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x5BA0D1, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x5C2366, RoadResX * RoadScale, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x5BA08A, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x5BA0CC, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x5C236D, RoadResY * RoadScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x7FEE6C, VehicleRes * VehicleScale, true);
		// RVM Reflection
		// Aspect ratio without RVM_MASK is 3:1
		injector::WriteMemory<uint32_t>(0x7FEE80, MirrorResX * MirrorScale, true);
		injector::WriteMemory<uint32_t>(0x7FEE84, MirrorResY * MirrorScale, true);

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
			if (VehicleRes_POT > GetSystemMetrics(SM_CYSCREEN))
			{VehicleRes_POT = VehicleRes_POT / 2;}
			injector::WriteMemory<uint32_t>(0x7FEE6C, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD)
	{		
		// Road Reflection LOD
		injector::MakeJMP(0x631665, VehicleLODCodeCave, true);
		injector::MakeJMP(0x6311E4, FEVehicleLODCodeCave, true);
		injector::WriteMemory<uint32_t>(0x5B9D30, 0x00000100, true);
		injector::WriteMemory<uint8_t>(0x5D67A6, 0x01, true);
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x5B9C96, 0x00000000, true);
		// Rearview Mirror LOD
		injector::WriteMemory<uint32_t>(0x5B9CE1, 0x00000000, true);
		injector::WriteMemory<uint8_t>(0x4888F3, 0xEB, true);
	}

	if (AspectRatioFix)
	{
		injector::WriteMemory<float>(0x78D78C, 8.0f, true);
	}

	if (RemoveParticleEffects)
	{
		injector::MakeCALL(0x5CAEC3, ParticleEffectNullCodeCave, true);
	}

	if (ForceEnableMirror)
	{
		// Enables mirror for all camera views
		injector::MakeNOP(0x5C2230, 2, true);
		injector::WriteMemory<uint8_t>(0x6004A0, 0xEB, true);

		// Restores light trails
		injector::MakeCALL(0x5CAE6E, RestoreLightTrailsCodeCave, true);
	}

	if (FrontEndReflectionBlur)
	{
		// Enables gaussian blur in the front end
		injector::MakeNOP(0x5CAC3A, 2, true);
	}

	if (RestoreSkybox)
	{
		// Restores skybox for RVM
		injector::MakeJMP(0x5CAE61, RestoreMirrorSkyboxCodeCave, true);
		// Restores skybox for vehicle reflection
		injector::MakeJMP(0x5CAD7E, RestoreVehicleSkyboxCodeCave, true);
		// Restores skybox for road reflection
		injector::MakeJMP(0x5CAB59, RestoreRoadSkyboxCodeCave, true);
		// Flips skybox so it's visible in road reflection
		injector::MakeJMP(0x60F955, FlipRoadSkyboxCodeCave, true);
		// Enables skybox
		injector::WriteMemory<uint8_t>(0x60F9F6, 0xEB, true);
		injector::MakeJMP(0x60F9D6, SkyboxRenderDistanceCodeCave, true);
	}

	if (ExtendRenderDistance)
	{
		// Extends vehicle reflection distance so skybox is visible
		injector::MakeJMP(0x5C4FAE, ExtendVehicleRenderDistanceCodeCave, true);
		// Extends render RVM distance so skybox is visible
		injector::WriteMemory<uint32_t>(0x7870D8, 0x461C4000, true);
	}

	if (NewMotionBlur)
	{
		// Force Enable Depth of Field
		// Required for New Motion Blur
		injector::WriteMemory<uint8_t>(0x49C23D, 0xB9, true); // mov ecx, 00000000 (part 1)
		injector::WriteMemory<uint32_t>(0x49C23E, 0x00000000, true); // mov ecx, 00000000 (part 2)
		injector::MakeNOP(0x49C242, 1, true); // nop
		injector::MakeNOP(0x5BE8B4, 5, true); // nop
		injector::WriteMemory<uint8_t>(0x870CF0, 0x01, true); // Depth of Field Bool
		// Enable New Motion Blur
		injector::WriteMemory<uint8_t>(0x5D29CF, 0xEB, true); // jmp 0x5D29DD
		// Motion Blur Distance
		static float MotionBlurDistance = 0.0100f;
		injector::WriteMemory<float>(0x5D2A2B, MotionBlurDistance, true);
		// Depth of Field Distance
		static float DepthOfFieldDistance = 0.0009765625f / MotionBlurDistance;
		injector::WriteMemory(0x5D2A70, &DepthOfFieldDistance, true);
		// Speed For Motion Blur Min
		injector::WriteMemory<float>(0x7FF7B0, 1.0f, true); // 25.0f = original
		// Speed For Motion Blur Max
		injector::WriteMemory<float>(0x7FF7B4, 250.0f, true); // 125.0f = original
	}

	if (RealisticChrome)
	{
		// Changes the chrome material
		injector::MakeJMP(0x5BD4B1, RealisticChromeCodeCave, true);
		injector::MakeNOP(0x5BD4B6, 1, true);
	}


	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity = (0.5f * VehicleReflectionBrightness);

		injector::WriteMemory(0x5BD57A, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x5BD590, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x5BD5A6, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x5BD5BC, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x5BD5E4, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x5BD5FE, &VehicleReflectionIntensity, true);
	}
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x75BCC7) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.2 NTSC speed2.exe (4,57 MB (4.800.512 bytes)).", "NFSU2 HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;	
}