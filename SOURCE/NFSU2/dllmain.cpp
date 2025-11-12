#include "..\includes\injector\injector.hpp"
#include "..\includes\IniReader.h"
#include "settings.h"
#include "assembly.h"
#include "stdafx.h"

void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSU2HDReflections.ini");

	// Resolution
	AutoRes = iniReader.ReadInteger("RESOLUTION", "AutoRes", 1);
	CubemapScale = iniReader.ReadFloat("RESOLUTION", "CubemapScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	FECubemapRes = iniReader.ReadInteger("RESOLUTION", "FECubemapRes", 128);

	// Custom Resolution
	CubemapRes = iniReader.ReadInteger("CUSTOM RESOLUTION", "CubemapRes", 128);
	MirrorResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResX", 256);
	MirrorResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "MirrorResY", 256);
	RoadResX = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResX", 320);
	RoadResY = iniReader.ReadInteger("CUSTOM RESOLUTION", "RoadResY", 240);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	AspectRatioFix = iniReader.ReadInteger("GENERAL", "AspectRatioFix", 1);
	RemoveParticleEffects = iniReader.ReadInteger("GENERAL", "RemoveParticleEffects", 1);
	FrontEndReflectionBlur = iniReader.ReadInteger("GENERAL", "FrontEndReflectionBlur", 1);
	ForceEnableMirror = iniReader.ReadInteger("GENERAL", "ForceEnableMirror", 0);
	RestoreEnhancedContrast = iniReader.ReadInteger("GENERAL", "RestoreEnhancedContrast", 1);
	RestoreSkybox = iniReader.ReadInteger("GENERAL", "RestoreSkybox", 1);
	RestoreHeadlights = iniReader.ReadInteger("GENERAL", "RestoreHeadlights", 1);
	ExtendRenderDistance = iniReader.ReadInteger("GENERAL", "ExtendRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	ImproveMotionBlur = iniReader.ReadInteger("EXTRA", "ImproveMotionBlur", 0);
	MotionBlurStrength = iniReader.ReadFloat("EXTRA", "MotionBlurStrength", 1.0f);
	ExpandMemoryPools = iniReader.ReadInteger("EXTRA", "ExpandMemoryPools", 1);
	RealisticChrome = iniReader.ReadInteger("EXTRA", "RealisticChrome", 0);

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
		// Front-End Cubemap
		injector::MakeJMP(0x5BA50D, FECubemapResCodeCave, true);
		injector::MakeNOP(0x5BA512, 1, true);
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x5BA08F, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x5BA0D1, RoadResX, true);
		injector::WriteMemory<uint32_t>(0x5C2366, RoadResX, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x5BA08A, RoadResY, true);
		injector::WriteMemory<uint32_t>(0x5BA0CC, RoadResY, true);
		injector::WriteMemory<uint32_t>(0x5C236D, RoadResY, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x7FEE6C, CubemapRes, true);
		// RVM Reflection
		// Aspect ratio without RVM_MASK is 3:1
		injector::WriteMemory<uint32_t>(0x7FEE80, MirrorResX, true);
		injector::WriteMemory<uint32_t>(0x7FEE84, MirrorResY, true);
		// Gaussian Blur Effect (Road Reflections)
		//injector::WriteMemory<uint32_t>(0x5BA100, RoadResX / 2, true);
		//injector::WriteMemory<uint32_t>(0x5BA105, RoadResY / 2, true);
	}

	if (ImproveReflectionLOD)
	{		
		// Road Reflection LOD
		injector::MakeJMP(0x631665, VehicleLODCodeCave, true);
		injector::MakeNOP(0x63166A, 3, true);
		injector::MakeJMP(0x6311E4, FEVehicleLODCodeCave, true);
		injector::MakeNOP(0x6311E9, 3, true);
		injector::WriteMemory<uint32_t>(0x5B9D30, 0x00000100, true);
		injector::WriteMemory<uint8_t>(0x5D67A6, 0x01, true);
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x5B9CE1, 0x00000000, true);
		// Rearview Mirror LOD
		injector::WriteMemory<uint32_t>(0x5B9C96, 0x00000000, true);
		injector::WriteMemory<uint8_t>(0x4888F7, 0x00, true);
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
		// Enables blur in the front end
		injector::MakeNOP(0x5CAC3A, 2, true);
	}

	if (RestoreEnhancedContrast)
	{
		// Creates a new "EVIEW" for the rearview mirror
		injector::MakeJMP(0x5BA1DF, MirrorEnhancedContrastCodeCavePart1, true);
		injector::MakeNOP(0x5BA1E4, 1, true);
		injector::MakeJMP(0x5BA766, MirrorEnhancedContrastCodeCavePart2, true);
		// Calls necessary functions
		injector::MakeJMP(0x5CAEED, RestoreEnhancedContrastCodeCave, true);
		injector::MakeNOP(0x5CAEF2, 1, true);
	}

	if (RestoreSkybox)
	{
		// Restores skybox for RVM
		injector::MakeJMP(0x5CAE61, RestoreMirrorSkyboxCodeCave, true);
		injector::MakeNOP(0x5CAE66, 2, true);
		// Restores skybox for vehicle reflection
		injector::MakeJMP(0x5CAD7E, RestoreVehicleSkyboxCodeCave, true);
		injector::MakeNOP(0x5CAD83, 1, true);
		// Restores skybox for road reflection
		injector::MakeJMP(0x5CAB59, RestoreRoadSkyboxCodeCave, true);
		injector::MakeNOP(0x5CAB5E, 2, true);
		// Flips skybox so it's visible in road reflection
		injector::MakeJMP(0x60F955, FlipRoadSkyboxCodeCave, true);
		injector::MakeNOP(0x60F95A, 3, true);
		// Enables skybox
		injector::WriteMemory<uint8_t>(0x60F9F6, 0xEB, true);
		// Resizes skybox to match render distance
		injector::MakeJMP(0x60F9D6, SkyboxSizeCodeCave, true);
	}

	if (RestoreHeadlights)
	{
		// Renders headlights for road reflections
		injector::MakeJMP(0x5CACA5, RestoreHeadlightsCodeCave, true);
		// Removes the original function call
		injector::MakeNOP(0x5CAC07, 5, true);
	}

	if (ExtendRenderDistance)
	{
		// Extends vehicle reflection distance
		injector::MakeJMP(0x5C4FAE, ExtendVehicleRenderDistanceCodeCave, true);
	}

	// Thanks to Berkay and nlgzrgn
	if (ExpandMemoryPools)
	{
		// FEngMemoryPoolSize (InitFEngMemoryPool)
		injector::WriteMemory<int>(0x8F5790, 800000, true);

		// CarLoaderPoolSizesD
		injector::WriteMemory<int>(0x7FA9C8, 22000, true);
		injector::WriteMemory<int>(0x7FA9CC, 18000, true);
		injector::WriteMemory<int>(0x7FA9D0, 32000, true);

		// CarLoaderPoolSizesR
		injector::WriteMemory<int>(0x7FA9D4, 22000, true);
		injector::WriteMemory<int>(0x7FA9D8, 18000, true);
		injector::WriteMemory<int>(0x7FA9DC, 32000, true);

		// TrackStreamingPoolSizes
		injector::WriteMemory<int>(0x79DC54, 120000, true);
		injector::WriteMemory<int>(0x79DC58, 86000, true);
		injector::WriteMemory<int>(0x79DC5C, 72000, true);

		// Fixes disappearing objects (ePolySlotPool)
		injector::WriteMemory<uint32_t>(0x48CD62, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CD6C, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CD91, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x48CDA2, 0xFA000, true);
	}

	if (ImproveMotionBlur)
	{
		// Force Enable Depth of Field
		// Required for better "Xbox 360" Motion Blur, so the entire screen isn't blurred.
		injector::WriteMemory<uint8_t>(0x49C23D, 0xB9, true); // mov ecx, 00000000
		injector::WriteMemory<uint32_t>(0x49C23E, 0x00000000, true); // mov ecx, 00000000
		injector::MakeNOP(0x49C242, 1, true); // nop
		injector::MakeNOP(0x5BE8B4, 5, true); // nop
		injector::WriteMemory<uint8_t>(0x870CF0, 0x01, true); // Depth of Field Bool
		// Enables better "Xbox 360" Motion Blur
		injector::MakeJMP(0x5D29CF, ImproveMotionBlurEnablerCodeCave, true);
		injector::MakeNOP(0x5D29D4, 4, true);
		// Motion Blur Distance
		static float MotionBlurDistance = 0.0100f;
		injector::WriteMemory<float>(0x5D2A2B, MotionBlurDistance, true);
		// Depth of Field Distance
		static float DepthOfFieldDistance = 0.0009765625f / MotionBlurDistance;
		injector::WriteMemory(0x5D2A70, &DepthOfFieldDistance, true);
		// Speed For Min Motion Blur
		injector::WriteMemory<float>(0x7FF7B0, 1.0f, true); // 25.0f = original
		// Speed For Max Motion Blur
		injector::WriteMemory<float>(0x7FF7B4, 250.0f, true); // 125.0f = original
	}

	if (MotionBlurStrength > 0)
	{
		// Controls the amount of motion blur
		static float MotionBlurFloat = MotionBlurStrength * 1.20f;
		injector::WriteMemory(0x5CC72A, &MotionBlurFloat, true);
		injector::WriteMemory(0x5CC73A, &MotionBlurFloat, true);
		injector::WriteMemory(0x5CC74A, &MotionBlurFloat, true);
		injector::WriteMemory(0x5CC758, &MotionBlurFloat, true);
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
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.2 SPEED2.exe from HOODLUM (4.57 MB (4,800,512 bytes)).", "NFSU2 HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;	
}