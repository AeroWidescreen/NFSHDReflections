#pragma once

DWORD RestoreFEReflectionCodeCaveExit = 0x5BA513;

void __declspec(naked) RestoreFEReflectionCodeCave()
{
	_asm
	{
		mov edx, 0x80
		jmp RestoreFEReflectionCodeCaveExit
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

float VehichleSkyboxRenderDist = 0.00625;
float RVMSkyboxRenderDist = 0.0185;
DWORD SkyboxRenderDistanceCodeCaveExit = 0x60F9DE;

void __declspec(naked) SkyboxRenderDistanceCodeCave()
{
	_asm
	{
		cmp byte ptr ds : [ExtendRenderDistance] , 0x00
		jg SkyboxRenderDistanceCodeCavePart3 // jumps if extend render distance is enabled
		cmp edx, 0x03
		je SkyboxRenderDistanceCodeCavePart2 // jumps if rearview mirror
		cmp edx, 0x01
		je SkyboxRenderDistanceCodeCavePart3 // jumps if main render
		cmp edx, 0x04
		je SkyboxRenderDistanceCodeCavePart3 // jumps if road reflection

		fld dword ptr ds : [VehichleSkyboxRenderDist] // render distance: 0.00625
		jmp SkyboxRenderDistanceCodeCaveExit

	SkyboxRenderDistanceCodeCavePart2 :
		fld dword ptr ds : [RVMSkyboxRenderDist] // render distance: 0.01825
		jmp SkyboxRenderDistanceCodeCaveExit

	SkyboxRenderDistanceCodeCavePart3 :
		fld dword ptr ds : [0x7A6768] // default render distance: 1.0
		jmp SkyboxRenderDistanceCodeCaveExit
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