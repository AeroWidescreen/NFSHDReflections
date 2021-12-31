#pragma once

float BrightnessMultiplier = 2.0f;
float BrightnessDivider = 100.0f;
DWORD BrightnessResult;
DWORD BrightnessFixCodeCaveExit = 0x4B3E91;

void __declspec(naked) BrightnessFixCodeCave()
{
	_asm
	{
		fild dword ptr ds : [0xAC6F0C]              // Loads gamma integer
		fmul dword ptr ds : [BrightnessMultiplier]  // Multiplies by 2
		fstp dword ptr ds : [BrightnessResult]		// Stores result
		fld dword ptr ds : [BrightnessDivider]		// Loads a value of 100.0f
		fdiv dword ptr ds : [BrightnessResult]		// Divides by result
		fstp dword ptr ds : [0xAA9630]              // Stores new gamma float at 00AA9630
		movss xmm0, dword ptr ds : [0xAA9630]
		jmp BrightnessFixCodeCaveExit
	}
}

float FE_VehicleSkyboxBrightness = 0.75f;
DWORD VehicleSkyboxBrightnessCodeCaveExit = 0x4B209D;

void __declspec(naked) VehicleSkyboxBrightnessCodeCave()
{
	_asm
	{
		movss xmm0, dword ptr ds : [0x9EEECC]
		cmp dword ptr ds : [0xABB510] , 0x06 // checks if FrontEnd (0x03) or InGame (0x06)
		je VehicleSkyboxBrightnessCodeCaveInGame
		movss xmm0, dword ptr ds : [FE_VehicleSkyboxBrightness]

		VehicleSkyboxBrightnessCodeCaveInGame :
		jmp VehicleSkyboxBrightnessCodeCaveExit
	}
}

float FE_VehicleWorldBrightness = 0.5f;
DWORD VehicleWorldBrightnessCodeCaveExit = 0x4CA487;

void __declspec(naked) VehicleWorldBrightnessCodeCave()
{
	_asm
	{
		fld dword ptr ds : [0x9EEECC]
		cmp dword ptr ds : [0xABB510] , 0x06 // checks if FrontEnd (0x03) or InGame (0x06)
		je VehicleWorldBrightnessCodeCaveInGame
		fld dword ptr ds : [FE_VehicleWorldBrightness]

		VehicleWorldBrightnessCodeCaveInGame :
		jmp VehicleWorldBrightnessCodeCaveExit
	}
}

DWORD VehicleHorizonQualityCodeCaveExit = 0x1A11345;

void __declspec(naked) VehicleHorizonQualityCodeCave()
{
	_asm
	{
		push eax
		lea eax, dword ptr ds : [esi + 0x50]
		cmp dword ptr ds : [eax + 0x10] , 0x33524143 // checks if "PAN_CAR360_01_D"
		jne VehicleHorizonQualityCodeCaveNone
		pop eax
		mov dword ptr ds : [esi + 0x50] , 0x00000000 // nulls texture pointer
		mov cl, 0x01
		jmp VehicleHorizonQualityCodeCaveExit

	VehicleHorizonQualityCodeCaveNone :
		pop eax
		mov dword ptr ds : [esi + 0x50] , eax  // writes texture pointer
		mov cl, 0x01
		jmp VehicleHorizonQualityCodeCaveExit
	}
}

DWORD VehicleSkyboxQualityCodeCaveExit1 = 0x77710A;
DWORD VehicleSkyboxQualityCodeCaveExit2 = 0x7771E0;

void __declspec(naked) VehicleSkyboxQualityCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [0xABB510] , 0x06 // checks if FrontEnd (0x03) or InGame (0x06)
		je VehicleSkyboxQualityCodeCaveInGame
		cmp ebx, 0x12
		jne VehicleSkyboxQualityCodeCaveFrontEnd

	VehicleSkyboxQualityCodeCaveInGame :
		jmp VehicleSkyboxQualityCodeCaveExit1

	VehicleSkyboxQualityCodeCaveFrontEnd :
		jmp VehicleSkyboxQualityCodeCaveExit2
	}
}

DWORD VehicleReflectionLODCodeCaveExit = 0x7448A3;

void __declspec(naked) VehicleReflectionLODCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [0xABB510] , 0x06 // checks if FrontEnd (0x03) or InGame (0x06)
		je VehicleReflectionLODCodeCaveInGame
		jmp VehicleReflectionLODCodeCaveFrontEnd

	VehicleReflectionLODCodeCaveInGame :
		cmp byte ptr ds : [ImproveReflectionLOD] , 0x01 // checks if ImproveReflectionLOD is enabled
		jge VehicleReflectionLODCodeCaveEnable
		jmp VehicleReflectionLODCodeCaveDisable

	VehicleReflectionLODCodeCaveFrontEnd :
		cmp byte ptr ds : [RealFrontEndReflections] , 0x01 // checks if RealFrontEndReflections is enabled
		jge VehicleReflectionLODCodeCaveEnable
		jmp VehicleReflectionLODCodeCaveDisable

	VehicleReflectionLODCodeCaveEnable :
		or edx, 0x00008002
		jmp VehicleReflectionLODCodeCaveExit

	VehicleReflectionLODCodeCaveDisable :
		or edx, 0x08002180
		jmp VehicleReflectionLODCodeCaveExit
	}
}

DWORD RealisticChromeCodeCaveExit = 0x4B7939;

void __declspec(naked) RealisticChromeCodeCave()
{
	_asm
	{
		mov esi, dword ptr ds : [0xA9E578]
		mov dword ptr ds : [esi + 0x2E9C] , 0x3F800000 // chrome materiel reflectivity (1.0)
		mov dword ptr ds : [esi + 0x2E8C] , 0x3F800000 // chrome materiel reflectivity (1.0)
		mov dword ptr ds : [esi + 0x2E10] , 0x3E800000 // chrome materiel brightness (0.25)
		mov dword ptr ds : [esi + 0x2E00] , 0x00000000 // chrome materiel brightness (0.0)
		mov esi, dword ptr ds : [ecx + 0x08]
		fld dword ptr ds : [esi + 0x64]
		jmp RealisticChromeCodeCaveExit
	}
}

DWORD ExtendRenderDistanceCodeCaveExit = 0x4BDEA8;

void __declspec(naked) ExtendRenderDistanceCodeCave()
{
	_asm
	{
		mov dword ptr ds : [esi + 0x164] , 0x461C4000 // 10000.0f
		jmp ExtendRenderDistanceCodeCaveExit
	}
}