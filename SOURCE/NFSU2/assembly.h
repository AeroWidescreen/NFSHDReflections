#pragma once

DWORD FECubemapResCodeCaveExit = 0x5BA513;

void __declspec(naked) FECubemapResCodeCave()
{
	_asm
	{
		mov edx, dword ptr ds : [FECubemapRes] // Front-End Cubemap Resolution
		jmp FECubemapResCodeCaveExit
	}
}

DWORD VehicleLODCodeCaveExit = 0x63166D;

void __declspec(naked) VehicleLODCodeCave()
{
	_asm
	{
		mov ecx, 0x0 // Road Reflection (Vehicle) LOD setting
		mov edx, 0x0 // Road Reflection (Wheels) LOD setting
		jmp VehicleLODCodeCaveExit
	}
}

DWORD FEVehicleLODCodeCaveExit = 0x6311EC;

void __declspec(naked) FEVehicleLODCodeCave()
{
	_asm
	{
		mov eax, 0x0 // FE Road Reflection (Vehicle) LOD setting
		mov ecx, 0x0 // FE Road Reflection (Wheels) LOD setting
		jmp FEVehicleLODCodeCaveExit
	}
}

DWORD RoadReflectionLODCodeCaveExit = 0x5D665F;
DWORD RoadReflectionLODCodeCavePart2Exit = 0x5D6720;
DWORD RoadReflectionLODCodeCavePart2Jump = 0x5D672E;

void __declspec(naked) RoadReflectionLODCodeCave()
{
	_asm
	{
		cmp dword ptr ds : [edi + 0x3C] , 0x40E96E29 // freeway sign
		je RoadReflectionLODCodeCavePart2
		cmp dword ptr ds : [edi + 0x3C] , 0x45976DD4 // hills
		je RoadReflectionLODCodeCavePart2
		cmp dword ptr ds : [edi + 0x3C] , 0x42212D15 // white box
		je RoadReflectionLODCodeCavePart2
		jmp RoadReflectionLODCodeCaveExit

	RoadReflectionLODCodeCavePart2 :
		test byte ptr ds : [esi + 0x1B] , 0x04
		je RoadReflectionLODCodeCavePart2Condtional
		jmp RoadReflectionLODCodeCavePart2Exit

	RoadReflectionLODCodeCavePart2Condtional :
		jmp RoadReflectionLODCodeCavePart2Jump
	}
}

DWORD RestoreLightTrailsCodeCaveExit = 0x6316F5;
DWORD RestoreLightTrailsCodeCaveJump = 0x63198C;

void __declspec(naked) RestoreLightTrailsCodeCave()
{
	_asm
	{
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF6
		sub esp, 0x0314
		mov eax, dword ptr ds : [0x8026C8]
		test eax, eax
		push ebx
		push esi
		push edi
		je RestoreLightTrailsCodeCavePart2
		mov edi, dword ptr ds : [ebp + 0x08]
		push edi
		jmp RestoreLightTrailsCodeCaveExit

	RestoreLightTrailsCodeCavePart2 :
		jmp RestoreLightTrailsCodeCaveJump
	}
}

DWORD RestoreMirrorSkyboxCodeCaveExit = 0x5CAE68;
DWORD sub_60F880 = 0x60F880;
DWORD sub_5CF420 = 0x5CF420;

void __declspec(naked) RestoreMirrorSkyboxCodeCave()
{
	_asm
	{
		push esi
		call sub_60F880 // Skybox function call
		add esp, 0x04
		call sub_5CF420
		push 0x01
		push esi
		lea ecx, dword ptr ds : [esp + 0x78]
		jmp RestoreMirrorSkyboxCodeCaveExit
	}
}

DWORD RestoreVehicleSkyboxCodeCaveExit = 0x5CAD84;

void __declspec(naked) RestoreVehicleSkyboxCodeCave()
{
	_asm
	{
		push esi
		call sub_60F880 // Skybox function call
		add esp, 0x04
		call sub_5CF420
		push ebx
		push esi
		lea ecx, dword ptr ds : [esp + 0x78]
		jmp RestoreVehicleSkyboxCodeCaveExit
	}
}

DWORD RestoreRoadSkyboxCodeCaveExit = 0x5CAB60;

void __declspec(naked) RestoreRoadSkyboxCodeCave()
{
	_asm
	{
		push esi
		call sub_60F880 // Skybox function call
		add esp, 0x04
		call sub_5CF420
		push 0x08
		push esi
		lea ecx, dword ptr ds : [esp + 0x78]
		jmp RestoreRoadSkyboxCodeCaveExit
	}
}

float SkyboxOrientation = -8000;
DWORD FlipRoadSkyboxCodeCaveExit = 0x60F95D;

void __declspec(naked) FlipRoadSkyboxCodeCave()
{
	_asm
	{
		fld dword ptr ds : [eax + 0x08]
		cmp edx, 0x04
		jne FlipRoadSkyboxCodeCavePart2
		fadd dword ptr ds : [SkyboxOrientation]
	FlipRoadSkyboxCodeCavePart2 :
		mov eax, dword ptr ds : [0x82DA5C]
		jmp FlipRoadSkyboxCodeCaveExit
	}
}

DWORD sub_615490 = 0x615490;
DWORD RestoreHeadlightsCodeCaveExit = 0x5CACAA;

void __declspec(naked) RestoreHeadlightsCodeCave()
{
	_asm
	{
		push 0x832FA0
		call sub_615490
		add esp,0x04
		jmp RestoreHeadlightsCodeCaveExit
	}
}

DWORD ExtendVehicleRenderDistanceCodeCaveExit = 0x5C4FB5;

void __declspec(naked) ExtendVehicleRenderDistanceCodeCave()
{
	_asm
	{
		mov edx, 0x461C4000
		jmp ExtendVehicleRenderDistanceCodeCaveExit
	}
}

float Road_SkyboxSize = 0.00625;
float Mirror_SkyboxSize = 0.0185;
DWORD SkyboxSizeCodeCaveExit = 0x60F9DE;

void __declspec(naked) SkyboxSizeCodeCave()
{
	_asm
	{
		cmp edx, 0x01
		je Main // jumps if main render
		cmp edx, 0x03
		je Mirror // jumps if rearview mirror
		cmp edx, 0x04
		je Road // jumps if road reflection

	Main :
		fld dword ptr ds : [0x7A6768] // default skybox size: 1.0f
		jmp SkyboxSizeCodeCaveExit

	Mirror :
		fld dword ptr ds : [Mirror_SkyboxSize] // default skybox size: 0.01825f
		jmp SkyboxSizeCodeCaveExit

	Road:
		cmp byte ptr ds : [ExtendRenderDistance] , 0x00
		jg Main // jumps if extend render distance is enabled
		fld dword ptr ds : [Road_SkyboxSize] // default skybox size: 0.00625f
		jmp SkyboxSizeCodeCaveExit
	}
}

DWORD RealisticChromeCodeCaveExit = 0x5BD4B8;

void __declspec(naked) RealisticChromeCodeCave()
{
	_asm
	{
		push eax
		mov eax, dword ptr ds : [0x7F399C] // Material Pointer
		mov dword ptr ds : [eax - 0xE88] , 0x3ECCCCCD // 4.0f
		mov dword ptr ds : [eax - 0xE98] , 0x3ECCCCCD // 4.0f
		pop eax
		fld dword ptr ds : [esi + 0x34]
		push edi
		fmul dword ptr ds : [esi + 0x30]
		jmp RealisticChromeCodeCaveExit
	}
}

void __declspec(naked) ParticleEffectNullCodeCave()
{
	_asm
	{
		ret 4
	}
}

DWORD loc_5BA1E5 = 0x5BA1E5;
int RVM_POINTER[100];

void __declspec(naked) MirrorEnhancedContrastCodeCavePart1()
{
	_asm
	{
		mov eax, dword ptr ds : [0x8708A4]
		mov ecx, dword ptr ds : [eax]
		lea edx, dword ptr ds : [RVM_POINTER]
		push edx
		push 0x01
		push eax
		call dword ptr ds : [ecx + 0x48]

	ExitCode:
		mov ecx, dword ptr ds : [0x7FEE7C]
		jmp loc_5BA1E5
	}
}

DWORD loc_5BA76B = 0x5BA76B;

void __declspec(naked) MirrorEnhancedContrastCodeCavePart2()
{
	_asm
	{
		cmp dword ptr ds : [0x8654A4], 0x06
		jne ExitCode
		mov eax, dword ptr ds : [RVM_POINTER]
		cmp eax, edi
		je ExitCode
		mov ecx, dword ptr ds : [eax]
		push eax
		call dword ptr ds : [ecx + 0x08]
		mov dword ptr ds : [RVM_POINTER] , edi

	ExitCode :
		mov eax, dword ptr ds : [0x8708A4]
		jmp loc_5BA76B
	}
}

void __declspec(naked) sub_5CC8B0()
{
	_asm
	{
		mov eax, dword ptr ds : [0x870D00] // Enhanced Contrast Bool
		test eax, eax
		jne loc_5CC914
		jmp loc_5CC982

	loc_5CC914 :
		mov ecx, dword ptr ds : [0x8708A8] // 008708A8
		mov eax, dword ptr ds : [0x870974]
		mov edx, dword ptr ds : [eax]
		push 0x00
		push 0x00
		push ecx
		mov ecx, dword ptr ds : [0x8708AC] // 008708AC
		push 0x00
		push ecx
		push eax
		call dword ptr ds: [edx + 0x88]
		jmp loc_5CC985

	loc_5CC982 :
		add esp, 0x14

	loc_5CC985 :
		mov eax, dword ptr ds : [0x870974]
		mov edx, dword ptr ds : [eax]
		push 0x07
		push 0xA8
		push eax
		call dword ptr ds : [edx + 0xE4]
		ret
	}
}

DWORD loc_5C6600 = 0x5C6600;

void __declspec(naked) sub_5C6520()
{
	_asm
	{
		push ebp
		mov ebp, esp
		and esp, -10
		sub esp, 0xC4
		mov ecx, dword ptr ds : [ebp + 0x08]
		push ebx
		push esi
		push edi
		xor edi, edi
		cmp ecx, edi
		je loc_5C6557
		mov eax, dword ptr ds : [0x870974]
		mov edx, dword ptr ds : [eax]
		push ecx
		push edi
		push eax
		call dword ptr ds : [edx + 0x94]
		mov eax, dword ptr ds : [0x870974]
		mov ecx, dword ptr ds : [eax]
		push edi
		push eax
		call dword ptr ds : [ecx + 0x9C]

	loc_5C6557:
		fild dword ptr ds : [MirrorResX]
		mov ebx, dword ptr ds : [ebp + 0x10]
		mov esi, dword ptr ds : [ebx * 4 + 0x86EB28]
		mov eax, 0xFF7F7F7F
		fdivr dword ptr ds : [0x7A27CC]
		mov dword ptr ds : [esp + 0x30], 0xBF800000
		mov dword ptr ds : [esp + 0x34], 0x3F800000
		mov dword ptr ds : [esp + 0x38], 0x00000000
		mov dword ptr ds : [esp + 0x3C], eax
		mov dword ptr ds : [esp + 0x48], 0x3F800000
		mov dword ptr ds : [esp + 0x4C], 0x3F800000
		mov dword ptr ds : [esp + 0x50], 0x00000000
		mov dword ptr ds : [esp + 0x54], eax
		mov dword ptr ds : [esp + 0x60], 0x3F800000
		mov dword ptr ds : [esp + 0x64], 0xBF800000
		mov dword ptr ds : [esp + 0x68], 0x00000000
		mov dword ptr ds : [esp + 0x6C], eax
		mov dword ptr ds : [esp + 0x78], 0xBF800000
		mov dword ptr ds : [esp + 0x7C], 0xBF800000
		mov dword ptr ds : [esp + 0x80], 0x00000000
		mov dword ptr ds : [esp + 0x84], eax
		mov dword ptr ds : [0x870910], esi
		mov ecx, esi
		fstp dword ptr ds : [esp + 0x14]
		fild dword ptr ds : [MirrorResY]
		fdivr dword ptr ds : [0x7A27CC]
		jmp loc_5C6600
	}
}

DWORD sub_5BBE80 = 0x5BBE80;

void __declspec(naked) sub_5C68D0()
{
	_asm
	{
		mov eax, dword ptr ds : [esp + 0x04]
		mov dword ptr ds : [0x86EC98], eax
		test byte ptr ds : [0x876860], 0x01
		jne loc_5C6987
		mov eax, dword ptr ds : [0x876860]
		mov ecx, dword ptr ds : [0x7FF7AC]
		or eax, 0x01
		mov dword ptr ds : [0x876860], eax
		mov dword ptr ds : [0x87685C], ecx
		fcomp dword ptr ds : [0x86EB14]

	loc_5C6987 :
	    fcomp dword ptr ds : [0x86EB14]
	    fnstsw ax
	    test ah, 0x44
	    jp loc_5C69FC
		fld dword ptr ds : [0x7FF158]
		fcomp dword ptr ds : [0x86EB20]
		fnstsw ax
		test ah, 0x44
		jp loc_5C69FC
		fld dword ptr ds : [0x87685C]
		fcomp dword ptr ds : [0x7FF7AC]
		fnstsw ax
		test ah, 0x44
		jp loc_5C69FC
		fld dword ptr ds : [0x8708F0]
		fcomp dword ptr ds : [0x7FF18C]
		fnstsw ax
		test ah, 0x44
		jp loc_5C69FC
		mov eax, dword ptr ds : [0x876858]
		test eax, eax
		jne loc_5C6A17

	loc_5C69FC :
		mov edx, dword ptr ds : [0x7FF7AC]
		mov dword ptr ds : [0x87685C], edx
		call sub_5BBE80
		mov dword ptr ds : [0x876858], 0x01

	loc_5C6A17 :
		mov eax, dword ptr ds : [0x870974]
		mov ecx, dword ptr ds : [eax]
		push esi
		push 0x0F
		push 0xA8
		push eax
		call dword ptr ds : [ecx + 0xE4]
		mov al, byte ptr ds : [esp + 0x0C]
		test al, al
		mov eax, 0x00
		lea esi, dword ptr ds : [eax + 0x01]
		and esi, 0x80000001
		jns loc_5C6A4E
		dec esi
		or esi, -02
		inc esi

	loc_5C6A4E :
		mov edx, dword ptr ds : [0x86EB6C]
		mov ecx, dword ptr ds : [edx + 0x0C]
		test ecx, ecx
		push 0x00
		push 0x01
		push 0x02
		push 0x00
		push 0x11
		mov eax, dword ptr ds : [eax * 4 + 0x8708A4] // 008708A4
		mov ecx, dword ptr ds : [esi * 4 + 0x8708A4] // +4 = 008708A8
		push eax
		push ecx
		call sub_5C6520
		mov edx, dword ptr ds : [esi * 4 + 0x8708A0] // +4 = 008708A4
		mov eax, 0x00
		mov ecx, dword ptr ds : [eax * 4 + RVM_POINTER] // 008708A8
		push 0x01
		push 0x01
		push 0x02
		push 0x00
		push 0x11
		push edx
		push ecx
		call sub_5C6520
		mov edx, 0x00
		mov eax, dword ptr ds : [edx * 4 + 0x8708A4] // 008708A4
		mov ecx, dword ptr ds : [0x8708AC] // 008708AC
		push 0x02
		push 0x01
		push 0x02
		push 0x00
		push 0x11
		push eax
		push ecx
		call sub_5C6520
		add esp, 0x54
		pop esi
		ret
	}
}

DWORD loc_5CAEF3 = 0x5CAEF3;

void __declspec(naked) RestoreEnhancedContrastCodeCave()
{
	_asm
	{
		cmp byte ptr ds : [0x870D00] , 0x00 // Enhanced Contrast Bool
		je ExitCode
		call sub_5CC8B0
		mov byte ptr ds : [esp + 0x1C] , 0x01 // Enables Enhanced Contrast
		mov eax, dword ptr ds : [esp + 0x1C]
		push eax
		push ecx
		call sub_5C68D0
		add esp, 0x08

	ExitCode :
		mov ecx, dword ptr ds : [0x86EB5C]
		jmp loc_5CAEF3
	}
}

DWORD loc_5D29DD = 0x5D29DD;
DWORD ImproveMotionBlurEnablerCodeCaveExit = 0x5D29D8;

void __declspec(naked) ImproveMotionBlurEnablerCodeCave()
{
	_asm
	{
		cmp byte ptr ds : [0x8654A4], 0x06 // jumps if not frontend
		je Enable
		mov byte ptr ds : [0x8707CC], 0x00
		jmp ImproveMotionBlurEnablerCodeCaveExit

	Enable:
		jmp loc_5D29DD
	}
}