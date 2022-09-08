#pragma once

DWORD VehicleLODCodeCaveExit = 0x570FF2;

void __declspec(naked) VehicleLODCodeCave()
{
	_asm 
	{
		mov ecx, 0x00 // Road Reflection (Vehicle) LOD setting
		mov edx, 0x00 // Road Reflection (Vehicle) LOD setting
		jmp VehicleLODCodeCaveExit
	}
}

DWORD FEVehicleLODCodeCaveExit = 0x570906;

void __declspec(naked) FEVehicleLODCodeCave()
{
	_asm 
	{
		mov eax, 0x00 // FE Road Reflection (Vehicle) LOD setting
		mov edx, 0x00 // FE Road Reflection (Vehicle) LOD setting
		cmp eax, edx
		mov ecx, 0x00 // FE Road Reflection (Wheels) LOD setting
		jmp FEVehicleLODCodeCaveExit
	}
}

DWORD RoadReflectionLODCodeCaveExit = 0x445FEB;

void __declspec(naked) RoadReflectionLODCodeCave()
{
	_asm 
	{
		mov dword ptr ds : [esp + 0x18] , 0x04
		mov edx, dword ptr ds : [esi + ecx * 0x04 + 0x24]
		push edx
		mov edx, dword ptr ds : [edx]
		cmp dword ptr ds : [edx + 0x0C] , 0x43535254 // Excludes drain walls
		je RoadReflectionLODCodeCavePart2
		cmp dword ptr ds : [edx + 0x0C] , 0x535F5658 // Excludes truck
		je RoadReflectionLODCodeCavePart2
		cmp dword ptr ds : [edx + 0x08] , 0x536222EF // Excludes bridge
		je RoadReflectionLODCodeCavePart2
		pop edx
		mov dword ptr ds : [esp + 0x14] , edx
		mov dword ptr ds : [esp + 0x18] , ecx
		mov edi, dword ptr ds : [ebp + 0x08]
		jmp RoadReflectionLODCodeCaveExit

	RoadReflectionLODCodeCavePart2 :
		pop edx
		jmp RoadReflectionLODCodeCaveExit
	}
}

DWORD ForceEnableMirrorCodeCave1Exit = 0x4C1F49;
DWORD ForceEnableMirrorCodeCave1DriftModeJump = 0x4C1FA6;

void __declspec(naked) ForceEnableMirrorCodeCave1()
{
	_asm 
	{
		cmp dword ptr ds : [0x78A345] , 0x01
		je ForceEnableMirrorCodeCave1DriftMode
		cmp byte ptr ds : [eax + 0x24] , bl
		push 0x01
		push 0x03
		jmp ForceEnableMirrorCodeCave1Exit

	ForceEnableMirrorCodeCave1DriftMode :
		jmp ForceEnableMirrorCodeCave1DriftModeJump

	}
}

DWORD ForceEnableMirrorCodeCave2Exit = 0x408446;
DWORD ForceEnableMirrorCodeCave2DriftModeJump = 0x4084B1;

void __declspec(naked) ForceEnableMirrorCodeCave2()
{
	_asm 
	{
		mov eax, dword ptr ds : [0x536610] // 0078A344 (0078A334 = EU)
		add eax, 0x01
		cmp byte ptr ds : [eax] , 0x01
		je ForceEnableMirrorCodeCave2DriftMode
		mov eax, dword ptr ds : [0x740504]
		jmp ForceEnableMirrorCodeCave2Exit

	ForceEnableMirrorCodeCave2DriftMode :
		jmp ForceEnableMirrorCodeCave2DriftModeJump
	}
}

DWORD RestoreMirrorSkyboxCodeCaveExit = 0x409789;
DWORD sub_571870 = 0x571870;
DWORD sub_40ED50 = 0x40ED50;

void __declspec(naked) RestoreMirrorSkyboxCodeCave()
{
	_asm 
	{
		mov ecx, dword ptr ds : [0x4097F4] // 00740580 (00750570 = EU)
		call sub_571870 // Skybox function call
		call sub_40ED50
		mov ecx, dword ptr ds : [0x4097F4] // 00740580 (00750570 = EU)
		push ecx
		jmp RestoreMirrorSkyboxCodeCaveExit
	}
}

DWORD RestoreVehicleSkyboxCodeCaveExit = 0x409714;
DWORD RestoreVehicleSkyboxCodeCaveECX;

void __declspec(naked) RestoreVehicleSkyboxCodeCave()
{
	_asm 
	{
		mov dword ptr ds : [RestoreVehicleSkyboxCodeCaveECX] , ecx
		mov ecx, ebx // 00740820 (00740810 = EU)
		call sub_571870
		call sub_40ED50
		mov ecx, dword ptr ds : [RestoreVehicleSkyboxCodeCaveECX]
		push ebx
		xor ecx, ecx
		lea edx, dword ptr ds : [esp + 0x24]
		jmp RestoreVehicleSkyboxCodeCaveExit
	}
}

DWORD RestoreRoadSkyboxCodeCaveExit = 0x4095EF;
DWORD RestoreRoadSkyboxCodeCaveECX;

void __declspec(naked) RestoreRoadSkyboxCodeCave()
{
	_asm 
	{
		mov dword ptr ds : [RestoreRoadSkyboxCodeCaveECX] , ecx
		mov ecx, dword ptr ds : [0x409658] // 007405D0 (007505E0 = EU)
		call sub_571870
		call sub_40ED50
		mov ecx, dword ptr ds : [0x409658] // 007405D0 (007505E0 = EU)
		push ecx
		mov ecx, dword ptr ds : [RestoreRoadSkyboxCodeCaveECX]
		jmp RestoreRoadSkyboxCodeCaveExit
	}
}

DWORD ExtendVehicleRenderDistanceCodeCaveExit = 0x40B2F4;

void __declspec(naked) ExtendVehicleRenderDistanceCodeCave()
{
	_asm 
	{
		mov ecx, dword ptr ds : [0x44976C]
		cmp dword ptr ds : [ecx] , 0x03
		je ExtendVehicleRenderDistanceCodeCaveFrontEnd
		mov esi, 0x461C4000

	ExtendVehicleRenderDistanceCodeCaveFrontEnd :
		mov dword ptr ds : [eax + 0xC0] , esi
		jmp ExtendVehicleRenderDistanceCodeCaveExit
	}
}

DWORD RealisticChromeCodeCaveExit = 0x40FB19;

void __declspec(naked) RealisticChromeCodeCave()
{
	_asm
	{
		push ecx
		mov ecx, dword ptr ds : [0x743C18]
		mov dword ptr ds : [ecx + 0x71C], 0x3ECCCCCD // Chrome Material Max 0.4
		mov dword ptr ds : [ecx + 0x70C], 0x3ECCCCCD // Chrome Material Min 0.4
		pop ecx

		fld dword ptr ds : [esi + 0x38]
		fmul dword ptr ds : [esi + 0x30]
		jmp RealisticChromeCodeCaveExit
	}
}
