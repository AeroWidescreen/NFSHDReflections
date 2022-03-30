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
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 2);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);
	RestoreShadows = iniReader.ReadInteger("GENERAL", "RestoreShadows", 1);
	RestoreVisualTreatment = iniReader.ReadInteger("GENERAL", "RestoreVisualTreatment", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	RestoreBackfaceCulling = iniReader.ReadInteger("GENERAL", "RestoreBackfaceCulling", 1);
	OptimizeRenderDistance = iniReader.ReadInteger("GENERAL", "OptimizeRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	ExpandMemoryPool = iniReader.ReadInteger("EXTRA", "ExpandMemoryPool", 1);

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
		injector::MakeJMP(0x6BD4FC, RestoreFEReflectionCodeCave, true);
		injector::MakeJMP(0x6BD52D, VehicleReflectionCodeCave, true);
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x6CFC26, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDF5, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BD17D, RoadResX * RoadScale, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x6BD184, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDEF, RoadResY * RoadScale, true);
		// Rearview Mirror
		// Aspect ratio is based on NFSU2 because true aspect ratio is unknown
		injector::WriteMemory<uint32_t>(0x8F9008, MirrorResX * MirrorScale, true);
		injector::WriteMemory<uint32_t>(0x8F900C, MirrorResY * MirrorScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x8F8FF4, VehicleRes * VehicleScale, true);
		
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
			injector::WriteMemory<uint32_t>(0x8F8FF4, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD)
	{
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x6BFEBD, 0x00006002, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x6BFE58, 0x00006002, true);
		// Bypasses widescreen fix LOD changes
		injector::MakeJMP(0x6BFE3D, LODBypassCodeCave, true);
		injector::MakeNOP(0x6BFE42, 1, true);
		// Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x4FAE9A, 0xEB, true);
		// Full RVM LOD
		injector::WriteMemory<uint8_t>(0x4FAEB0, 0xEB, true);
		// Full Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x6BFF21, 0x06, true);
		injector::WriteMemory<uint8_t>(0x7293DB, 0x06, true);
		// Fixes Car Model Position in Road Reflection
		injector::MakeJMP(0x74E709, CarModelRoadReflectionCodeCave, true);
		injector::MakeNOP(0x74E70E, 1, true);
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

	if (OptimizeRenderDistance)
	{
		injector::MakeJMP(0x6E73A1, RenderDistanceCodeCave, true);
		injector::MakeNOP(0x6DB5AB, 6, true);
		injector::MakeJMP(0x6DB5BE, SkyboxRenderDistanceCodeCave, true);
	}

	/*
	if (NewMotionBlur)
	{
		// Force Enable Depth of Field
		// Required for New Motion Blur
		injector::MakeNOP(0x6C3284, 6, true); // nop
		injector::MakeNOP(0x6C2FBA, 6, true); // nop
		injector::MakeNOP(0x6C1B14, 6, true); // nop
		injector::MakeNOP(0x6C1A7B, 6, true); // nop
		injector::MakeNOP(0x6C19F8, 6, true); // nop
		injector::WriteMemory<uint8_t>(0x9017F0, 0x01, true); // Depth of Field Bool
		// Enable New Motion Blur
		injector::WriteMemory<uint8_t>(0x6E73FD, 0xEB, true); // jmp 0x6E741E
		// Motion Blur Distance
		static float MotionBlurDistance = 0.0100f;
		injector::WriteMemory<float>(0x6E746B, MotionBlurDistance, true);
		// Depth of Field Distance
		static float DepthOfFieldDistance = 0.0009765625f / MotionBlurDistance;
		injector::WriteMemory(0x6E74B0, &DepthOfFieldDistance, true);
		// Speed For Motion Blur Min
		injector::WriteMemory<float>(0x8F9B10, 1.0f, true); // 25.0f = original
		// Speed For Motion Blur Max
		injector::WriteMemory<float>(0x8F9B14, 250.0f, true); // 125.0f = original
	}
	*/

	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity1 = (0.5f * VehicleReflectionBrightness);
		static float VehicleReflectionIntensity2 = (1.0f * VehicleReflectionBrightness);
		injector::WriteMemory(0x6C77CE, &VehicleReflectionIntensity1, true);
		injector::WriteMemory(0x6C77D6, &VehicleReflectionIntensity2, true);
	}

	if (ExpandMemoryPool)
	{
		// Fixes disappearing objects
		injector::WriteMemory<uint32_t>(0x5009D2, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x5009DC, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A01, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A12, 0xFA000, true);
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