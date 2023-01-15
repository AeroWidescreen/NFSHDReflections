#pragma once
#include "settings.h"

CIniReader iniReader("NFSCHDReflections.ini");

DWORD RoadReflectionCodeCave1Exit = 0x71AA2B;
DWORD RoadReflectionRes1;

void __declspec(naked) RoadReflectionCodeCave1()
{
	_asm 
	{
		// for widescreen fix compatibility
		fild dword ptr ds : [RoadResX]
		fmul dword ptr ds : [RoadScale]
		fistp dword ptr ds : [RoadReflectionRes1]
		mov edx, dword ptr ds : [RoadReflectionRes1]
		push edx
		push eax
		call dword ptr ds : [ecx + 0x5C]
		jmp RoadReflectionCodeCave1Exit
	}
}

DWORD RoadReflectionCodeCave2Exit = 0x71AA7B;
DWORD RoadReflectionRes2;

void __declspec(naked) RoadReflectionCodeCave2()
{
	_asm 
	{
		// for widescreen fix compatibility
		fild dword ptr ds : [RoadResX]
		fmul dword ptr ds : [RoadScale]
		fistp dword ptr ds : [RoadReflectionRes2]
		mov ecx, dword ptr ds : [RoadReflectionRes2]
		push ecx
		push eax
		call dword ptr ds : [edx + 0x74]
		jmp RoadReflectionCodeCave2Exit
	}
}

DWORD RoadReflectionCodeCave3Exit = 0x71A9FE;
DWORD RoadReflectionRes3;

void __declspec(naked) RoadReflectionCodeCave3()
{
	_asm 
	{
		// for widescreen fix compatibility
		mov eax, dword ptr ds : [0xAB0ABC]
		fild dword ptr ds : [RoadResY]
		fmul dword ptr ds : [RoadScale]
		fistp dword ptr ds : [RoadReflectionRes3]
		mov edx, dword ptr ds : [RoadReflectionRes3]
		jmp RoadReflectionCodeCave3Exit
	}
}

float DOFStrength = 1.0f;
float BloomBlurStrength = 1.0f;
float ReflectionBlurStrength = 1.0f;

void __declspec(naked) DownScale4x4StrengthCodeCave()
{
	_asm 
	{
		cmp dword ptr ds : [ebp + 0x04] , 0x72E22E // Road Reflection Return Address
		je DownScale4x4StrengthCodeCaveRoad
		cmp dword ptr ds : [ebp + 0x04] , 0x726B17 // Bloom Return Address
		je DownScale4x4StrengthCodeCaveBloom

		// restores horizontal depth of field
		fdivr dword ptr ds : [DOFStrength]
		ret

	DownScale4x4StrengthCodeCaveBloom:
		// restores horizontal frontend blur
		fdivr dword ptr ds : [BloomBlurStrength]
		ret

	DownScale4x4StrengthCodeCaveRoad:
		// horizontal road reflection
		fdivr dword ptr ds : [ReflectionBlurStrength]
		ret
	}
}

DWORD TrafficSignFixCodeCaveExit = 0x71E085;

void __declspec(naked) TrafficSignFixCodeCave()
{
	_asm 
	{
		mov ecx, dword ptr ds : [0xA6B910]
		mov dword ptr ds : [ecx + 0x74] , 0x41000000 // Traffic sign materiel brightness
		fld dword ptr ds : [esi + 0x6C]
		fmul dword ptr ds : [esi + 0x64]
		jmp TrafficSignFixCodeCaveExit
	}
}

DWORD RealisticChromeCodeCaveExit = 0x71E08F;

void __declspec(naked) RealisticChromeCodeCave()
{
	_asm 
	{
		cmp byte ptr ds : [0xA63E60] , 0x03 // checks if Shader Detail = High
		jne NoRealisticChromeCodeCave
		cmp byte ptr ds : [0xA65340] , 0x00 // checks if Reflection Update Rate is greater than 0
		jle NoRealisticChromeCodeCave
		cmp byte ptr ds : [0xA63E77] , 0x00 // checks if vehicle reflections are enabled
		jle NoRealisticChromeCodeCave
		mov ecx, dword ptr ds : [0xA6B910]
		mov dword ptr ds : [ecx - 0xB90] , 0x40C00000 // Chrome Materiel Intensity: 6.0f
		mov dword ptr ds : [ecx - 0xBF0] , 0x3E000000 // Chrome Materiel Brightness: 0.125f
		mov dword ptr ds : [ecx - 0xC00] , 0x3E800000 // Chrome Materiel Reflect Brightness: 0.25f
		jmp RealisticChromeCodeCavePart2

	NoRealisticChromeCodeCave :
		mov ecx, dword ptr ds : [0xA6B910]
		mov dword ptr ds : [ecx - 0xB90] , 0x3F800000 // Chrome Materiel Intensity
		mov dword ptr ds : [ecx - 0xBF0] , 0x3F800000 // Chrome Materiel Brightness
		mov dword ptr ds : [ecx - 0xC00] , 0x3F800000 // Chrome Materiel Reflect Brightness

	RealisticChromeCodeCavePart2 :
		fld dword ptr ds : [esi + 0x70]
		fmul dword ptr ds : [esi + 0x64]
		jmp RealisticChromeCodeCaveExit
	}
}

DWORD ImproveReflectionLODCodeCave1Exit = 0x79FB65;

void __declspec(naked) ImproveReflectionLODCodeCave1()
{
	_asm 
	{

		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks game state
		jne ImproveReflectionLODCodeCave1FrontEnd

		// ImproveReflectionLODCodeCave1InGame
		cmp byte ptr ds : [ImproveReflectionLOD] , 0x00
		jg ImproveReflectionLODCodeCave1On
		jmp ImproveReflectionLODCodeCave1Off

	ImproveReflectionLODCodeCave1FrontEnd :
		cmp byte ptr ds : [RealFrontEndReflections] , 0x00
		jg ImproveReflectionLODCodeCave1On
		jmp ImproveReflectionLODCodeCave1Off

	ImproveReflectionLODCodeCave1Off :
		test ch, 0x30
		mov dword ptr ds : [esp + 0x14] , 0x00000000
		jmp ImproveReflectionLODCodeCave1Exit

	ImproveReflectionLODCodeCave1On :
		test ch, 0x10
		mov dword ptr ds : [esp + 0x14] , 0x00000000
		jmp ImproveReflectionLODCodeCave1Exit
	}
}

DWORD ImproveReflectionLODCodeCave2Exit = 0x79AEE1;

void __declspec(naked) ImproveReflectionLODCodeCave2()
{
	_asm 
	{

		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks game state
		jne ImproveReflectionLODCodeCave2FrontEnd

		// ImproveReflectionLODCodeCave2InGame
		cmp byte ptr ds : [ImproveReflectionLOD] , 0x00
		jg ImproveReflectionLODCodeCave2On
		jmp ImproveReflectionLODCodeCave2Off

	ImproveReflectionLODCodeCave2FrontEnd :
		cmp byte ptr ds : [RealFrontEndReflections] , 0x00
		jg ImproveReflectionLODCodeCave2On
		jmp ImproveReflectionLODCodeCave2Off

	ImproveReflectionLODCodeCave2Off :
		or dx, 0x08002180
			jmp ImproveReflectionLODCodeCave2Exit

	ImproveReflectionLODCodeCave2On :
		or edx, 0x00008002
		jmp ImproveReflectionLODCodeCave2Exit
	}
}

DWORD ImproveReflectionLODCodeCave3Exit = 0x79AE9A;

void __declspec(naked) ImproveReflectionLODCodeCave3()
{
	_asm 
	{
		cmp dword ptr ds : [ebx + 0x08] , 0x03
		je ImproveReflectionLODCodeCave3Part2
		mov edi, dword ptr ds : [ebx + 0x08]
		lea ecx, dword ptr ds : [edx + edx * 0x04]
		jmp ImproveReflectionLODCodeCave3Exit

	ImproveReflectionLODCodeCave3Part2 :
		// for mirror LOD
		mov edi, 0x01
		lea ecx, dword ptr ds : [edx + edx * 0x04]
		jmp ImproveReflectionLODCodeCave3Exit
	}
}

DWORD VehicleReflShaderCodeCaveExit = 0x729461;

void __declspec(naked) VehicleReflShaderCodeCave()
{
	_asm 
	{
		mov ecx, dword ptr ds : [edi + 0x04]
		// excludes objects
		cmp ecx, 0x01
		je VehicleReflShaderCodeCavePart2
		jmp VehicleReflShaderCodeCaveExit

	VehicleReflShaderCodeCavePart2 :
		push edi
		mov edi, [esp + 0x0C]
		mov edi, [edi + 0x08]
		// checks for vehicle reflection
		cmp edi, 0x16
		jnl VehicleReflShaderCodeCavePart3
		pop edi
		jmp VehicleReflShaderCodeCaveExit

	VehicleReflShaderCodeCavePart3 :
		pop edi
		movzx ecx, ax
		mov edi, dword ptr ds : [ecx * 0x4 + 0xB1F66C]
		jmp VehicleReflShaderCodeCaveExit
	}
}

DWORD VehicleReflShaderSettingCodeCaveExit = 0x7236D5;

void __declspec(naked) VehicleReflShaderSettingCodeCave()
{
	_asm
	{
		mov eax, dword ptr ds : [0xB42FC8]
		mov dword ptr ds : [eax + 0x1774] , 0x02 // lowers shader detail of traffic signs
		mov eax, dword ptr ds : [ebx + 0x1774]
		jmp VehicleReflShaderSettingCodeCaveExit
	}
}

static float AlignmentMultiplier = 1.333333373f;
DWORD AlignmentFixCodeCaveExit = 0x71B8A6;

void __declspec(naked) AlignmentFixCodeCave()
{
	_asm
	{
		cmp eax, 0x16
		jl AlignmentFixCodeCaveConditional
		fld dword ptr ds : [esp + 0x24]
		fmul dword ptr[AlignmentMultiplier]
		fstp dword ptr ds : [esp + 0x24]

		AlignmentFixCodeCaveConditional :
		movzx ecx, word ptr ds : [edi + 0xE4]
		jmp AlignmentFixCodeCaveExit
	}
}

static float AspectRatio_1 = 0.6f;
static float AspectRatio_2 = 0.0f;
static float AspectRatio_3 = -0.5f;
DWORD VehicleReflAspectRatioCodeCaveExit = 0x73E1BA;

void __declspec(naked) VehicleReflAspectRatioCodeCave()
{
	_asm
	{
		cmp byte ptr ds : [AspectRatioFix] , 0x01
		jge AspectRatioFix_AlignmentFix // jumps if AspectRatioFix is enabled
		// if only AlignmentFix
		mov eax, dword ptr ds : [AspectRatio_1]
		jmp ExitCode

		// if AlignmentFix and AspectRatio are enabled
	AspectRatioFix_AlignmentFix :
		cmp byte ptr ds : [AlignmentFix] , 0x01
		jl AspectRatioFix_NoAlignnmentFix
		mov eax, dword ptr ds : [AspectRatio_2]
		jmp ExitCode
		// if only AspectRatioFix
	AspectRatioFix_NoAlignnmentFix :
		mov eax, dword ptr ds : [AspectRatio_3]

	ExitCode :
		push eax
		mov ecx, esi
		push 0x37
		jmp VehicleReflAspectRatioCodeCaveExit
	}
}

float VehicleReflSkyboxRenderDistance = 0.25f;
DWORD VehicleReflSkyboxCodeCaveExit = 0x7AFCAA;
DWORD VehicleReflSkyboxCodeCavePart2Exit = 0x7AFCD5;
DWORD VehicleReflSkyboxCodeCavePart2Jump = 0x7AFCAA;

void __declspec(naked) VehicleReflSkyboxCodeCave()
{
	_asm
	{
		cmp eax, 0x03
		jg VehicleReflSkyboxCodeCavePart2
		jmp VehicleReflSkyboxCodeCaveExit

	VehicleReflSkyboxCodeCavePart2 :
		mov eax, dword ptr ds : [0xA7335C]
		test eax, eax
		je VehicleReflSkyboxCodeCavePart2Conditional
		test ecx, ecx
		fld dword ptr ds : [VehicleReflSkyboxRenderDistance]
		fmul st(0), st(1)
		fmul dword ptr ds : [esi]
		fstp dword ptr ds : [esi]
		fld dword ptr ds : [VehicleReflSkyboxRenderDistance]
		fmul st(0), st(1)
		fmul dword ptr ds : [esi + 0x14]
		fstp dword ptr ds : [esi + 0x14]
		fld dword ptr ds : [VehicleReflSkyboxRenderDistance]
		jmp VehicleReflSkyboxCodeCavePart2Exit

	VehicleReflSkyboxCodeCavePart2Conditional :
		jmp VehicleReflSkyboxCodeCavePart2Jump
	}
}

__int64 VehicleReflSkyboxBrightness;
__int64 FEVehicleReflBrightness;
__int64 VehicleReflBrightness;
__int64 VehicleReflBrightness2;
float VehicleReflSkyboxBrightnessSubtraction = 1.75f;
float VehicleReflBrightnessAddition = 0.0f;
float VehicleReflBrightnessSubtraction = 1.6f;
float VehicleReflBrightnessAddition2 = 0.0f;
float VehicleReflBrightnessSubtraction2 = 0.95f;
float FEVehicleReflBrightnessAddition = 2.5f;
float FEVehicleReflBrightnessSubtraction = 1.0f;
DWORD VehicleReflBrightnessCodeCaveExit = 0x748A9D;

void __declspec(naked) VehicleReflBrightnessCodeCave()
{
	if (ReflectionContrast > 1)
	{
		VehicleReflBrightnessSubtraction = 0.5f;
	}
	_asm 
	{
		push edx
		mov edx, dword ptr ds : [0xAB08F4]
		mov edx, dword ptr ds : [edx + 0x04]
		cmp edx, 0x16
		jl VehicleReflBrightnessCodeCavePart2
		push ebx
		lea ebx, dword ptr ds : [VehicleReflBrightness]
		fld dword ptr ds : [eax]
		fadd dword ptr ds : [VehicleReflBrightnessAddition]
		fstp dword ptr ds : [ebx]
		fld dword ptr ds : [eax + 0x4]
		fsub dword ptr ds : [VehicleReflBrightnessSubtraction]
		fstp dword ptr ds : [ebx + 0x04]
		mov eax, ebx
		pop ebx
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflBrightnessCodeCaveExit

	VehicleReflBrightnessCodeCavePart2 :
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflBrightnessCodeCaveExit
	}
}

DWORD VehicleReflBrightnessCodeCave2Exit = 0x7497E4;

void __declspec(naked) VehicleReflBrightnessCodeCave2()
{

	if (ReflectionContrast > 1)
	{
		VehicleReflBrightnessSubtraction = 0.5f;
	}
	__asm {
		push edx
		mov edx, dword ptr ds : [0xAB08F4]
		mov edx, dword ptr ds : [edx + 0x04]
		cmp edx, 0x16
		jl VehicleReflBrightnessCodeCave2Part2
		push ebx
		lea ebx, dword ptr ds : [FEVehicleReflBrightness]
		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks gmae state
		je InGameVehicleReflBrightnessCodeCave
		cmp byte ptr ds : [ReflectionContrast] , 0x00
		jle VehicleReflBrightnessCodeCave2Part2

		// FEVehicleReflBrightnessCodeCave
		fld dword ptr ds : [eax]
		fadd dword ptr ds : [FEVehicleReflBrightnessAddition]
		fstp dword ptr ds : [ebx]
		fld dword ptr ds : [eax + 0x04]
		fsub dword ptr ds : [FEVehicleReflBrightnessSubtraction]
		fstp dword ptr ds : [ebx + 0x04]
		mov eax, ebx
		pop ebx
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflBrightnessCodeCave2Exit

	InGameVehicleReflBrightnessCodeCave :
		lea ebx, dword ptr ds : [VehicleReflBrightness2]
		fld dword ptr ds : [eax]
		fadd dword ptr ds : [VehicleReflBrightnessAddition2]
		fstp dword ptr ds : [ebx]
		fld dword ptr ds : [eax + 0x04]
		fsub dword ptr ds : [VehicleReflBrightnessSubtraction2]
		fstp dword ptr ds : [ebx + 0x04]
		mov eax, ebx
		pop ebx
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflBrightnessCodeCave2Exit

	VehicleReflBrightnessCodeCave2Part2 :
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflBrightnessCodeCave2Exit
	}
}

DWORD VehicleReflSkyboxBrightnessCodeCaveExit = 0x7498B0;

void __declspec(naked) VehicleReflSkyboxBrightnessCodeCave()
{
	_asm 
	{
		push edx
		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks game state
		je VehicleReflSkyboxBrightnessCodeCavePart2
		mov edx, dword ptr ds : [0xAB08F4]
		mov edx, dword ptr ds : [edx + 0x04]
		cmp edx, 0x16
		jl VehicleReflSkyboxBrightnessCodeCavePart2
		push ebx
		lea ebx, dword ptr ds : [VehicleReflSkyboxBrightness]
		fld dword ptr ds : [eax]
		fstp dword ptr ds : [ebx]
		fld dword ptr ds : [eax + 0x04]
		fsub dword ptr ds : [VehicleReflSkyboxBrightnessSubtraction]
		fstp dword ptr ds : [ebx + 0x04]
		mov eax, ebx
		pop ebx
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflSkyboxBrightnessCodeCaveExit

	VehicleReflSkyboxBrightnessCodeCavePart2 :
		pop edx
		push eax
		push 0x8B
		jmp VehicleReflSkyboxBrightnessCodeCaveExit
	}
}

float FEReflBrightness = 0.15f;
DWORD FEReflBrightnessCodeCaveExit = 0x71E24F;
DWORD FEReflBrightnessCodeCavePart2Exit = 0x71E1A8;

void __declspec(naked) FEReflBrightnessCodeCave()
{
	_asm 
	{
		fmul dword ptr ds : [esi + 0xC4]
		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks game state
		je FEReflBrightnessCodeCavePart2
		cmp byte ptr ds : [ReflectionContrast] , 0x00
		jg FEReflBrightnessCodeCavePart2
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		fstp dword ptr ds : [esp + 0x30]
		fld dword ptr ds : [esi + 0xCC]
		fmul dword ptr ds : [esi + 0xC4]
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		fstp dword ptr ds : [esp + 0x34]
		fld dword ptr ds : [esi + 0xD0]
		fmul dword ptr ds : [esi + 0xC4]
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		fstp dword ptr ds : [esp + 0x38]
		fld dword ptr ds : [esi + 0xD8]
		fmul dword ptr ds : [esi + 0xD4]
		mov eax, dword ptr ds : [esi + 0x8C]
		mov edx, dword ptr ds : [esi + 0x54]
		mov ecx, dword ptr ds : [esi + 0xB8]
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		mov dword ptr ds : [esp + 0x54] , eax
		mov dword ptr ds : [esp + 0x7C] , 0x00
		mov dword ptr ds : [esp + 0x50] , edx
		fsub dword ptr ds : [esp + 0x30]
		mov dword ptr ds : [esp + 0x58] , ecx
		mov dword ptr ds : [esp + 0x5C] , 0x00
		fstp dword ptr ds : [esp + 0x70]
		fld dword ptr ds : [esi + 0xDC]
		fmul dword ptr ds : [esi + 0xD4]
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		fsub dword ptr ds : [esp + 0x34]
		fstp dword ptr ds : [esp + 0x74]
		fld dword ptr ds : [esi + 0xE0]
		fmul dword ptr ds : [esi + 0xD4]
		fmul dword ptr ds : [FEReflBrightness]
		fmul dword ptr ds : [VehicleReflectionBrightness]
		jmp FEReflBrightnessCodeCaveExit

	FEReflBrightnessCodeCavePart2 :
		jmp FEReflBrightnessCodeCavePart2Exit
	}
}

DWORD VehicleReflRenderDistanceCodeCaveExit = 0x721DEB;

void __declspec(naked) VehicleReflRenderDistanceCodeCave()
{
	_asm 
	{
		mov eax, dword ptr ds : [esp + 0x6C]
		cmp dword ptr ds : [0xA99BBC] , 0x06
		je VehicleReflRenderDistanceCodeCaveConditional
		mov eax, 0x481C4000

	VehicleReflRenderDistanceCodeCaveConditional :
		mov edx, dword ptr ds : [esp + 0x64]
		jmp VehicleReflRenderDistanceCodeCaveExit
	}
}

void __declspec(naked) VehicleReflFlareRotationCodeCave()
{
	__asm 
	{
		push eax
		mov dword ptr ds : [esp + 0x1C], 0x3F800000
		mov eax, dword ptr ds : [ebp + 0x08]
		mov eax, dword ptr ds : [eax + 0x08]
		cmp eax, 0x16
		jl ExitCode
		cmp eax, 0x1B
		jg ExitCode
		mov dword ptr ds : [esp + 0x1C], 0xBF800000

	ExitCode:
		pop eax
		cmp edi, 0x02
		ret
	}
}

void __declspec(naked) CubemapFixCodeCave()
{
	_asm
	{
		mov eax, 0x9D083C // "QRACE"
		mov dword ptr ds : [0x9E87E4] , 0x47454E58 // ZNEG = XNEG
		mov dword ptr ds : [0x9E87EC] , 0x534F5058 // ZPOS = XPOS
		mov dword ptr ds : [0x9E8804] , 0x47454E5A // XNEG = ZNEG
		mov dword ptr ds : [0x9E880C] , 0x534F505A // XPOS = ZPOS
		ret 0x04
	}
}

float PauseBlurValue1 = 1.0f;

void __declspec(naked) PauseBlurCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [0xA8AD18] , 0x00 // checks for pause
		je ExitCode
		cmp dword ptr ds : [0xA590A0] , 0x1F41 // checks for photo mode
		je ExitCode
		fadd dword ptr ds : [PauseBlurValue1] // 1.0f

	ExitCode :
		ret
	}
}

float PauseBlurValue2 = 10.0f;

void __declspec(naked) PauseBlurCodeCave2()
{
	_asm
	{
		cmp dword ptr ds : [0xA8AD18] , 0x00 // checks for pause
		je ExitCode
		cmp dword ptr ds : [0xA590A0] , 0x1F41 // checks for photo mode
		je ExitCode
		fadd dword ptr ds : [PauseBlurValue2] // 10.0f

	ExitCode :
		ret
	}
}

DWORD sub_72C9B0 = 0x72C9B0;
DWORD sub_7CCA20 = 0x7CCA20;
DWORD RoadReflectionCarModelCodeCaveExit = 0x72E193;

void __declspec(naked) RoadReflectionCarModelCodeCave()
{
	_asm
	{
		call sub_72C9B0
		mov dword ptr ds : [0xA6523C], 0x03
		push 0x01
		push 0xB4B110
		call sub_7CCA20
		add esp, 0x08
		call sub_72C9B0
		mov dword ptr ds : [0xA6523C], 0x02
		jmp RoadReflectionCarModelCodeCaveExit
	}
}

DWORD sub_7507D0 = 0x7507D0;
DWORD sub_7D5DC0 = 0x7D5DC0;
DWORD loc_72E1E5 = 0x72E1E5;

void __declspec(naked) RoadReflectionVehicleFlaresCodeCave()
{
	_asm
	{
		call sub_7507D0
		push 0x00
		push 0x02 // for headlight exclusion
		push 0xB4B110
		call sub_7D5DC0
		add esp, 0x0C
		call sub_72C9B0
		jmp loc_72E1E5
	}
}

DWORD loc_7CBDD8 = 0x7CBDD8;
DWORD loc_7CBDC2 = 0x7CBDC2;

void __declspec(naked) HeadlightExclusionCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [ebp + 0xC8] , 0xB4B110
		jne RenderHeadlights // jumps if not Road Reflection
		cmp byte ptr ds : [ebp + 0xCC] , 0x02
		jne RenderHeadlights // jumps if not Push 02
		jmp loc_7CBDD8

	RenderHeadlights :
		mov eax, dword ptr ds : [edi + 0x30C0]
		jmp loc_7CBDC2
	}
}

DWORD sub_74D330 = 0x74D330;
DWORD loc_7CC646 = 0x7CC646;

void __declspec(naked) FlareExclusionCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [ebp + 0xC8], 0xB4B110
		jne RenderFlares // jumps if not Road Reflection
		cmp byte ptr ds : [ebp + 0xCC], 0x02
		je RenderFlares // jumps if push 02
		jmp ExitCode

	RenderFlares :
		call sub_74D330

	ExitCode :
		jmp loc_7CC646
	}
}


DWORD sub_75AA10 = 0x75AA10;
DWORD MirrorParticleEffectsCodeCaveExit = 0x72E45D;

void __declspec(naked) MirrorParticleEffectsCodeCave()
{
	_asm
	{
		cmp byte ptr ds : [0xA6536C],0x00
		jng ExitCode
		push 0xB4B090
		mov ecx, 0xB4BE70
		call sub_75AA10
		call sub_72C9B0

	ExitCode:
		mov dword ptr ds : [0xAB0B98], 0x00
		jmp MirrorParticleEffectsCodeCaveExit
	}
}

DWORD sub_754BF0 = 0x754BF0;
DWORD MirrorParticleEffectsCodeCave2Exit = 0x75AB3A;

void __declspec(naked) MirrorParticleEffectsCodeCave2()
{
	_asm
	{
		push edi
		cmp dword ptr ds : [edi + 0x08], 0x01
		jne ExitCode // jumps if not Main render
		call sub_754BF0

	ExitCode:
		jmp MirrorParticleEffectsCodeCave2Exit
	}
}

void __declspec(naked) VehicleReflBrakeFlareSizeCodeCave()
{
	_asm
	{
		cmp eax, 0x01
		je BrakeFlareSize_Main
		cmp eax, 0x16
		jl ExitCode
		cmp eax, 0x1B
		jg ExitCode

	BrakeFlareSize_Reflection :
		mov dword ptr ds : [esp + 0x1C] , 0x43340000 // 180.0f
		ret

	BrakeFlareSize_Main :
		mov dword ptr ds : [esp + 0x1C] , 0x43B40000 // 360.0f
		ret

	ExitCode:
		ret
	}
}

DWORD loc_74DB98 = 0x74DB98;
DWORD loc_74DB92 = 0x74DB92;

void __declspec(naked) VehicleReflFlareSizeCodeCave()
{
	_asm
	{
		mov edx, dword ptr ds : [ebp + 0x08]
		mov edx, dword ptr ds : [edx + 0x08]
		cmp edx, 0x16
		jl ExitCode
		cmp edx, 0x1B
		jg ExitCode
		lea edx, dword ptr ds : [esi + 0x04]
		cmp edx, 0xA6BB0C // Blue Flare
		je VehicleReflFlareSize
		cmp edx, 0xA6BB38 // Red Flare
		je VehicleReflFlareSize
		//cmp edx, 0xA6BB64 // White Flare
		//je VehicleReflFlareSize
		jmp ExitCode

	VehicleReflFlareSize:
		fld dword ptr ds : [esi + 0x04]
		fsub dword ptr ds : [esi]
		fmul dword ptr ds : [CopFlareScale]
		jmp loc_74DB98

	ExitCode:
		fld dword ptr ds : [esi + 0x04]
		fsub dword ptr ds : [esi]
		jmp loc_74DB92
	}
}

DWORD VisualTreatmentCodeCaveExit = 0x71D706;

void __declspec(naked) VisualTreatmentCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [0xA99BBC] , 0x06 // checks game state
		jne VisualTreatmentFrontEnd

VisualTreatmentInGame :
		cmp byte ptr ds : [ReplaceVisualTreatmentIG] , 00
		jle LUT0_IG
		cmp byte ptr ds : [ReplaceVisualTreatmentIG] , 01
		je LUT1_IG
		cmp byte ptr ds : [ReplaceVisualTreatmentIG] , 02
		je LUT2_IG
		cmp byte ptr ds : [ReplaceVisualTreatmentIG] , 03
		jge LUT3_IG

	LUT0_IG:
		mov edx, dword ptr ds : [0xB43068] // FILTER0
		jmp VisualTreatmentCodeCaveExit

	LUT1_IG :
		mov edx, dword ptr ds : [0xB4306C] // FILTER1
		jmp VisualTreatmentCodeCaveExit

	LUT2_IG :
		mov edx, dword ptr ds : [0xB43070] // FILTER2
		jmp VisualTreatmentCodeCaveExit

	LUT3_IG :
		mov edx, dword ptr ds : [0xB43074] // FILTER3
		jmp VisualTreatmentCodeCaveExit


VisualTreatmentFrontEnd:
		cmp byte ptr ds : [ReplaceVisualTreatmentFE] , 00
		jle LUT0_FE
		cmp byte ptr ds : [ReplaceVisualTreatmentFE] , 01
		je LUT1_FE
		cmp byte ptr ds : [ReplaceVisualTreatmentFE] , 02
		je LUT2_FE
		cmp byte ptr ds : [ReplaceVisualTreatmentFE] , 03
		jge LUT3_FE

	LUT0_FE :
		mov edx, dword ptr ds : [0xB43068] // FILTER0
		jmp VisualTreatmentCodeCaveExit

	LUT1_FE :
		mov edx, dword ptr ds : [0xB4306C] // FILTER1
		jmp VisualTreatmentCodeCaveExit

	LUT2_FE :
		mov edx, dword ptr ds : [0xB43070] // FILTER2
		jmp VisualTreatmentCodeCaveExit

	LUT3_FE :
		mov edx, dword ptr ds : [0xB43074] // FILTER3
		jmp VisualTreatmentCodeCaveExit
	}
}