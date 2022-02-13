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
	CIniReader iniReader("NFSCHDReflections.ini");

	// Resolution
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0);
	PIPScale = iniReader.ReadFloat("RESOLUTION", "PIPScale", 1.0);
	CubemapRes = iniReader.ReadInteger("RESOLUTION", "CubemapRes", 256);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	ReflectionContrast = iniReader.ReadInteger("GENERAL", "ReflectionContrast", 1);
	ForceEnvironmentMap = iniReader.ReadInteger("GENERAL", "ForceEnvironmentMap", 0);
	AlignmentFix = iniReader.ReadInteger("GENERAL", "AlignmentFix", 1);
	AspectRatioFix = iniReader.ReadInteger("GENERAL", "AspectRatioFix", 1);
	CubemapFix = iniReader.ReadInteger("GENERAL", "CubemapFix", 1);
	RestoreSkybox = iniReader.ReadInteger("GENERAL", "RestoreSkybox", 1);
	EnableMirror = iniReader.ReadInteger("GENERAL", "EnableMirror", 1);
	RealFrontEndReflections = iniReader.ReadInteger("GENERAL", "RealFrontEndReflections", 0);
	ReflectionBlurStrength = iniReader.ReadFloat("GENERAL", "ReflectionBlurStrength", 1.0f);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0);

	// Extra
	ReplaceVisualTreatmentFE = iniReader.ReadInteger("EXTRA", "ReplaceVisualTreatmentFE", 0);
	ReplaceVisualTreatmentIG = iniReader.ReadInteger("EXTRA", "ReplaceVisualTreatmentIG", 0);
	TrafficSignFix = iniReader.ReadInteger("EXTRA", "TrafficSignFix", 1);
	PauseBlur = iniReader.ReadInteger("EXTRA", "PauseBlur", 0);
	RealisticChrome = iniReader.ReadInteger("EXTRA", "RealisticChrome", 0);

	if (HDReflections)
	{
		RoadResX = GetSystemMetrics(SM_CXSCREEN);
		RoadResY = GetSystemMetrics(SM_CYSCREEN);
		VehicleRes = GetSystemMetrics(SM_CYSCREEN);
		MirrorResX = GetSystemMetrics(SM_CYSCREEN);
		MirrorResY = GetSystemMetrics(SM_CYSCREEN) / 3;
		PIPRes = GetSystemMetrics(SM_CYSCREEN) / 3;
	}

	// Writes Resolution Values
	{
		// Road Reflection X
		injector::MakeJMP(0x71AA26, RoadReflectionCodeCave1, true);
		injector::MakeJMP(0x71AA76, RoadReflectionCodeCave2, true);
		injector::WriteMemory<uint32_t>(0x71BE28, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x71BDF1, RoadResX * RoadScale, true);
		// Road Reflection Y
		injector::MakeJMP(0x71A9F2, RoadReflectionCodeCave3, true);
		injector::WriteMemory<uint32_t>(0x71BE2F, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x71BDF8, RoadResY * RoadScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x70DE39, VehicleRes * VehicleScale, true);
		// Rearview Mirror
		// Aspect ratio is based on NFSU2 because true aspect ratio is unknown
		injector::WriteMemory<uint32_t>(0x70DB04, MirrorResX * MirrorScale, true);
		injector::MakeNOP(0x70DB08, 2, true);
		injector::WriteMemory<uint32_t>(0x70DB62, MirrorResY * MirrorScale, true);
		injector::MakeNOP(0x70DB66, 2, true);
		// Picture-In-Picture
		injector::WriteMemory<uint32_t>(0xA63B00, PIPRes * PIPScale, true);
		injector::WriteMemory<uint32_t>(0xA63B04, PIPRes * PIPScale, true);
		// Front-End Vehicle Cubemap
		if (CubemapRes < 256){CubemapRes = 256;}
		if (CubemapRes > 2048){CubemapRes = 2048;}
		injector::WriteMemory<uint32_t>(0x70DE50, CubemapRes, true);


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
			injector::WriteMemory<uint32_t>(0x70DE39, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD)
	{
		// Partial RVM and Vehicle Reflection LOD
		injector::MakeJMP(0x79FB5A, ImproveReflectionLODCodeCave1, true);
		// Full Vehicle Reflection LOD
		injector::MakeJMP(0x79AEDB, ImproveReflectionLODCodeCave2, true);
		// Full RVM LOD
		injector::MakeJMP(0x79AE94, ImproveReflectionLODCodeCave3, true);
	}

	if (RestoreDetails)
	{
		// Adds car model to Road Reflection
		injector::MakeJMP(0x72E18E, RoadReflectionCarModelCodeCave, true);
	}

	if (ForceEnvironmentMap)
	{
		static int EnvMapTexture = (ForceEnvironmentMap - 1);
		if (EnvMapTexture > 4){EnvMapTexture = 4;}
		if (EnvMapTexture < 0){EnvMapTexture = 0;}
		injector::WriteMemory(0x748AE1, &EnvMapTexture, true);
	}

	if (ReflectionContrast)
	{
		// Changes the vehicle reflection shader
		injector::MakeJMP(0x729445, VehicleReflShaderCodeCave, true);
		injector::MakeJMP(0x7236CF, VehicleReflShaderSettingCodeCave, true);
		injector::MakeJMP(0x748A97, VehicleReflBrightnessCodeCave, true);
		injector::MakeJMP(0x7497DE, VehicleReflBrightnessCodeCave2, true);
		injector::MakeJMP(0x7498AA, VehicleReflSkyboxBrightnessCodeCave, true);
		// Adjusts the size of flares in the vehicle reflection
		injector::MakeJMP(0x74D9D5, VehicleReflFlareSizeCodeCave, true);
	}

	if (AlignmentFix)
	{
		// Corrects the FOV so all segments of the vehicle reflection align properly
		injector::MakeJMP(0x71B89F, AlignmentFixCodeCave, true);
		// Restores the original aspect ratio
		injector::MakeJMP(0x73E1B5, VehicleReflAspectRatioCodeCave, true);
	}
	if (AspectRatioFix)
	{
		// Restores the original aspect ratio
		injector::MakeJMP(0x73E1B5, VehicleReflAspectRatioCodeCave, true);
	}

	if (CubemapFix)
	{
		// Fixes the "QRACE" cubemap
		injector::MakeJMP(0x572C30, CubemapFixCodeCave, true);
	}

	if (RestoreSkybox)
	{
		// Increases the skybox render distance for vehicle reflections
		injector::MakeJMP(0x7AFCA1, VehicleReflSkyboxCodeCave, true);
		// Fixes skybox position for road reflections
		injector::WriteMemory<uint32_t>(0x9EF050, 0x40000000, true);
		injector::WriteMemory<uint32_t>(0x9EF05C, 0x3F800000, true);
		injector::WriteMemory<uint32_t>(0x7AFC35, 0xBF800000, true);
	}

	if (EnableMirror)
	{
		// Enables the rearview mirror by default
		injector::WriteMemory<uint32_t>(0x4BD511, 0x00000001, true);
	}

	//if (ReplaceVisualTreatmentIG || ReplaceVisualTreatmentFE)
	{
		// Main
		injector::MakeCALL(0x726D84, VisualTreatmentCodeCave, true);
		// Rearview Mirror
		injector::MakeCALL(0x750CAB, VisualTreatmentCodeCave, true);
		// Picture-In-Picture
		injector::MakeCALL(0x750D9F, VisualTreatmentCodeCave, true);
	}

	if (RealFrontEndReflections)
	{
		// Enables real reflections
		injector::MakeNOP(0x72ECE8, 2, true);
		// Enables reflection mapping 
		injector::MakeNOP(0x73E19F, 2, true);
		// Enables vehicle skybox
		injector::MakeNOP(0x72E4E6, 2, true);
		// Increases render distance
		injector::MakeJMP(0x721DE3, VehicleReflRenderDistanceCodeCave, true);
		// Reduces reflection brightness if XB360Reflections = 0
		injector::MakeJMP(0x71E1A2, FEReflBrightnessCodeCave, true);
		// Allows geometry to render
		injector::MakeJMP(0x79FB5A, ImproveReflectionLODCodeCave1, true);
		injector::MakeJMP(0x79AEDB, ImproveReflectionLODCodeCave2, true);
	}

	if (TrafficSignFix)
	{
		// Restores the traffic sign brightness
		injector::MakeJMP(0x71E07F, TrafficSignFixCodeCave, true);
		// TrafficSignDistance
		static float TrafficSignDistance = 45000.0f;
		injector::WriteMemory(0x79B257, &TrafficSignDistance, true);
	}

	if (PauseBlur)
	{
		injector::MakeJMP(0x73ECA6, PauseBlurCodeCave, true);
		injector::MakeJMP(0x73ECB6, PauseBlurCodeCave, true);
		injector::MakeJMP(0x73ECC6, PauseBlurCodeCave2, true);
		injector::MakeJMP(0x73ECD6, PauseBlurCodeCave2, true);
	}

	if (RealisticChrome)
	{
		// Changes Chrome Materiel values
		injector::MakeJMP(0x71E089, RealisticChromeCodeCave, true);
	}
	
	if (ReflectionBlurStrength >= 0)
	{
		// Calls
		injector::MakeCALL(0x73CC29, DownScale4x4StrengthCodeCave, true);
		injector::MakeNOP(0x73CC2E, 1, true);
		injector::MakeCALL(0x73CC3D, DownScale4x4StrengthCodeCave, true);
		injector::MakeNOP(0x73CC42, 1, true);
		// Allows blur to render at full resolution
		injector::MakeNOP(0x713108, 2, true);
		injector::MakeNOP(0x713141, 2, true);
		// TwoPassBlur
		injector::WriteMemory(0x73C97E, &ReflectionBlurStrength, true);
		// GaussBlur5x5
		injector::WriteMemory(0x73C68E, &DOFStrength, true);
		injector::WriteMemory(0x73C6A8, &DOFStrength, true);
	}

	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity = (0.5f * VehicleReflectionBrightness);
		injector::WriteMemory(0x71E1AA, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x71E1C0, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x71E1D6, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x71E1FB, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x71E231, &VehicleReflectionIntensity, true);
		injector::WriteMemory(0x71E24B, &VehicleReflectionIntensity, true);
	}
}	

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC HD Reflections by Aero_", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}
