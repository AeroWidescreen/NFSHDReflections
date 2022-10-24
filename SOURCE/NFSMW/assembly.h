#pragma once

DWORD RestoreFEReflectionCodeCaveExit = 0x6BD502;

void __declspec(naked) RestoreFEReflectionCodeCave()
{
	_asm 
	{
		mov ecx, 0x100
		jmp RestoreFEReflectionCodeCaveExit
	}
}

DWORD VehicleReflectionCodeCaveExit = 0x6BD533;

void __declspec(naked) VehicleReflectionCodeCave()
{
	_asm 
	{
		mov edi, dword ptr ds : [0x8F8FF4]
		mov edx, dword ptr ds : [0x8F8FF8]
		jmp VehicleReflectionCodeCaveExit
	}
}

DWORD sub_505E80 = 0x505E80;
DWORD TrafficLightRestorationCodeCaveExit = 0x6DE9F8;
DWORD TrafficLightFunctionJump = 0x507781;

void __declspec(naked) TrafficLightRestorationCodeCave()
{
	_asm 
	{
		call TrafficLightFunction
		push 0x00919730
		call sub_505E80
		add esp, 0x0C
		jmp TrafficLightRestorationCodeCaveExit

	TrafficLightFunction :
		sub esp, 0x08
		push ebp
		push edi
		mov edi, dword ptr ds : [esp + 0x14]
		mov ecx, dword ptr ds : [edi + 0x04]
		mov eax, 0x02 // Removes vehicle sunflare from RVM
		jmp TrafficLightFunctionJump
	}
}

DWORD SunflareArray[50];
DWORD sub_7538D0 = 0x7538D0;
DWORD sub_6D04E0 = 0x6D04E0;
DWORD sub_723FA0 = 0x723FA0;
DWORD sub_750B10 = 0x750B10;
DWORD sub_6E2F50 = 0x6E2F50;
DWORD RestoreDetailsCodeCaveExit = 0x6DE990;

void __declspec(naked) RestoreDetailsCodeCave()
{
	_asm 
	{
		// world
		push 0x01
		push 0x919730
		lea ecx, dword ptr ds : [esp + 0xE8]
		call sub_723FA0
		call sub_6E2F50
		cmp byte ptr ds : [ImproveReflectionLOD] , 0x01
		jl RestoreDetailsCodeCave_NoDestroyableObjects
		// destroyable objects
		push 0x00
		push 0x919730
		call sub_7538D0
		add esp, 0x08

	RestoreDetailsCodeCave_NoDestroyableObjects:
		// car shadow
		push ebx
		push 0x00919730
		call sub_750B10
		call sub_6E2F50
		// sunflare
		push 0x919730
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6D04E0
		jmp RestoreDetailsCodeCaveExit
	}
}

DWORD EnableCarShadowCodeCaveExit = 0x74E843;
DWORD EnableCarShadowCodeCaveJump = 0x74E818;

void __declspec(naked) EnableCarShadowCodeCave()
{
	_asm 
	{
		cmp dword ptr ds : [edi + 0x04] , 0x01
		je EnableCarShadowCodeCaveConditional
		cmp dword ptr ds : [edi + 0x04] , 0x03
		je EnableCarShadowCodeCaveConditional
		jmp EnableCarShadowCodeCaveExit

	EnableCarShadowCodeCaveConditional :
		jmp EnableCarShadowCodeCaveJump
	}
}

DWORD sub_503D00 = 0x503D00;
DWORD EnableParticlesCodeCaveExit = 0x6DE9E8;

void __declspec(naked) EnableParticlesCodeCave()
{
	_asm 
	{
		add esp, 0x0C
		cmp eax, ebx
		push 0x919730
		mov ecx, 0x9166C0
		call sub_503D00
		jmp EnableParticlesCodeCaveExit
	}
}

DWORD RemoveRoadReflectionCodeCaveExit = 0x6D728F;

void __declspec(naked) RemoveRoadReflectionCodeCave()
{
	_asm
	{
		mov edi, dword ptr ds : [0x982A20]
		mov edi, dword ptr ds : [edi + 0x04]
		imul edi, edi, 0x70
		mov edi, dword ptr ds : [edi + 0x9195E4]
		cmp edi, 0x01
		je RemoveRoadReflectionCodeCave_IsMAIN

		mov edi, 0x00000000
		jmp RemoveRoadReflectionCodeCaveExit

	RemoveRoadReflectionCodeCave_IsMAIN :
		mov edi, dword ptr ds : [0x982A6C]
		jmp RemoveRoadReflectionCodeCaveExit
	}
}

float RGBAmbient = 0.5f;
float RGBDiffuse = 0.75f;
float RGBSpecular = 0.0f;

void __declspec(naked) VehicleReflBrightnessCodeCave()
{
	_asm 
	{
		cmp dword ptr ds : [0x925E90] , 0x06
		jne VehicleReflBrightnessCodeCavePart3
		push edx
		mov edx, dword ptr ds : [0x982A20]
		mov edx, dword ptr ds : [edx + 0x04]
		imul edx, edx, 0x70
		mov edx, dword ptr ds : [edx + 0x9195E4]
		cmp edx, 0x11
		jng VehicleReflBrightnessCodeCavePart2
		cmp edx, 0x16
		jg VehicleReflBrightnessCodeCavePart2
		fld dword ptr ds : [esi + 0x20]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x20]
		fld dword ptr ds : [esi + 0x24]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x24]
		fld dword ptr ds : [esi + 0x28]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x28]
		fld dword ptr ds : [esi + 0x30]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x30]
		fld dword ptr ds : [esi + 0x34]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x34]
		fld dword ptr ds : [esi + 0x38]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x38]
		fld dword ptr ds : [esi + 0x40]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x40]
		fld dword ptr ds : [esi + 0x44]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x44]
		fld dword ptr ds : [esi + 0x48]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x48]

	VehicleReflBrightnessCodeCavePart2 :
		pop edx
	VehicleReflBrightnessCodeCavePart3 :
		pop esi
		mov esp, ebp
		pop ebp
		ret 4
	}
}

DWORD VehicleReflBrightnessBugFixCodeCaveExit = 0x6DA52F;

void __declspec(naked) VehicleReflBrightnessBugFixCodeCave()
{
	_asm 
	{
		div dword ptr ds : [ecx * 0x04 + 0x8F9018]
		mov ecx, 0x00000003 // overwrites reflection setting
		jmp VehicleReflBrightnessBugFixCodeCaveExit
	}
}

DWORD MirrorRGBCodeCaveExit = 0x6E7126;

void __declspec(naked) MirrorRGBCodeCave()
{
	_asm
	{
		mov byte ptr ds : [eax] , 0xFF // Alpha: 255
		mov byte ptr ds : [eax - 0x03] , 0x80 // Red: 128
		mov byte ptr ds : [eax - 0x02] , 0x80 // Green: 128
		mov byte ptr ds : [eax - 0x01] , 0x80 // Blue: 128
		add eax, 0x04
		jmp MirrorRGBCodeCaveExit
	}
}

DWORD RenderDistanceCodeCaveExit = 0x6E73A6;
DWORD RenderDistanceCodeCaveExit2 = 0x6E73AB;

void __declspec(naked) RenderDistanceCodeCave()
{
	_asm 
	{
		push ebx
		push 0x00
		mov ecx, edi
		cmp dword ptr ds : [esi + 0x04] , 0x12 // Vehicle Reflection
		jnl RenderDistanceCodeCavePart2
		cmp dword ptr ds : [esi + 0x04] , 0x03 // RVM
		je RenderDistanceCodeCavePart3
		jmp RenderDistanceCodeCaveExit

	RenderDistanceCodeCavePart2 :
		cmp dword ptr ds : [esi + 0x04] , 0x03
		push 0x42C80000 // 100.0f
		jmp RenderDistanceCodeCaveExit2

	RenderDistanceCodeCavePart3 :
		push 0x43400000 // 192.0f
		jmp RenderDistanceCodeCaveExit2
	}
}

float SkyboxRenderDistance = 0.5f;
DWORD SkyboxRenderDistanceCodeCaveExit = 0x6DB5C4;

void __declspec(naked) SkyboxRenderDistanceCodeCave()
{
	_asm 
	{
		mov eax, dword ptr ds : [0x982A20]
		mov eax, dword ptr ds : [eax + 0x04]
		imul eax, eax, 0x70
		mov eax, dword ptr ds : [eax + 0x9195E4]

		cmp eax, 0x12 // checks for vehicle reflections
		jnl SkyboxRenderDistanceCodeCavePart2
		cmp eax, 0x03 // checks for rearview mirror
		je SkyboxRenderDistanceCodeCavePart2
		fld dword ptr ds : [0x8F935C]
		jmp SkyboxRenderDistanceCodeCaveExit

	SkyboxRenderDistanceCodeCavePart2 :
		fld dword ptr ds : [SkyboxRenderDistance]
		jmp SkyboxRenderDistanceCodeCaveExit
	}
}

DWORD SunflareCodeCave1Exit = 0x6DB265;
DWORD sub_6C00D0 = 0x6C00D0;

void __declspec(naked) SunflareCodeCave1()
{
	_asm 
	{
		call sub_6C00D0
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6C00D0
		jmp SunflareCodeCave1Exit
	}
}

DWORD SunflareCodeCave2Exit = 0x6E6DB2;

void __declspec(naked) SunflareCodeCave2()
{
	_asm 
	{
		call sub_6C00D0
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6C00D0
		jmp SunflareCodeCave2Exit
	}
}

DWORD SunflareCodeCave3Exit = 0x6DB120;

void __declspec(naked) SunflareCodeCave3()
{
	_asm 
	{
		lea esi, dword ptr ds : [SunflareArray]
		add esi, 0x24
		call SunflareCodeCave3Sub
		mov esi, 0x93E03C
		call SunflareCodeCave3Sub
		jmp SunflareCodeCave3Exit

	SunflareCodeCave3Sub :
		mov edi, esi
		add edi, 0x10

	SunflareCodeCave3SubConditional2 :
		mov eax, dword ptr ds : [esi]
		mov edx, dword ptr ds : [eax]
		test eax, eax
		je SunflareCodeCave3SubConditional1
		push eax
		call dword ptr ds : [edx + 0x08]
		mov dword ptr ds : [esi] , 0x00000000

	SunflareCodeCave3SubConditional1 :
		add esi, 0x04
		cmp esi, edi
		jl SunflareCodeCave3SubConditional2
		ret
	}
}

DWORD LODBypassCodeCaveExit1 = 0x6BFE44;
DWORD LODBypassCodeCaveExit2 = 0x6BFE57;

void __declspec(naked) LODBypassCodeCave()
{
	_asm 
	{
		cmp esi, 0x919730
		je LODBypassCodeCavePart2
		mov ecx, [0x982AF4]
		test ecx, ecx
		jne LODBypassCodeCavePart2
		xor ecx, ecx
		cmp ebx, 0x03
		jmp LODBypassCodeCaveExit1

	LODBypassCodeCavePart2 :
		jmp LODBypassCodeCaveExit2
	}
}

DWORD sub_6D2CD0 = 0x6D2CD0;
float VisualTreatmentArray[] = { 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f };

void __declspec(naked) VisualTreatmentSub1()
{
	_asm
	{
		mov eax, dword ptr ds : [0x901828] // Visual Treatment Bool
		test eax, eax
		jne loc_6DBF27
		mov al, byte ptr ds : [0x982B2A]
		test al, al
		jne loc_6DBF27
		mov eax, dword ptr ds : [0x9017D0]
		test eax, eax
		jne loc_6DBF27
		mov edx, dword ptr ds : [0x982A80] // Render 2
		mov eax, dword ptr ds : [0x982BDC]
		mov ecx, dword ptr ds : [eax]
		push 0x00
		push 0x00
		push edx
		mov edx, dword ptr ds : [0x982A84] // Render 3
		push 0x00
		push edx
		push eax
		call dword ptr ds : [ecx + 0x88]
		push 0x03
		lea eax, dword ptr ds : [VisualTreatmentArray]
		push eax
		mov eax, dword ptr ds : [0x982A7C] // Render 1
		mov ecx, dword ptr ds : [0x982A80] // Render 2
		push 0x01
		push eax
		push ecx
		call sub_6D2CD0
		mov dword ptr ds : [0x982C60] , 0xFFFFFFFF
		add esp, 0x14
		jmp loc_6DBFC8

	loc_6DBF27 :
		mov ecx, dword ptr ds : [0x982A80] // Render 2
		mov eax, dword ptr ds : [0x982BDC]
		mov edx, dword ptr ds : [eax]
		push 0x00
		push 0x00
		push ecx
		mov ecx, dword ptr ds : [0x982A84] // Render 3
		push 0x00
		push ecx
		push eax
		call dword ptr ds : [edx + 0x88]
		mov dword ptr ds : [0x982C60] , 0x00000000
		mov eax, dword ptr ds : [0x982BDC]
		mov edx, dword ptr ds : [eax]
		push 0x0F
		push 0xA8
		push eax
		call dword ptr ds : [edx + 0xE4]
		mov eax, dword ptr ds : [0x901828] // Visual Treatment Bool
		test eax, eax
		je loc_6DBFC8
		push 0x02
		lea eax, dword ptr ds : [VisualTreatmentArray]
		push eax
		mov eax, dword ptr ds : [0x982A7C] // Render 1
		mov ecx, dword ptr ds : [0x982A80] // Render 2
		push 0x04
		push eax
		push ecx
		call sub_6D2CD0
		add esp, 0x14
		jmp loc_6DBFC8

	loc_6DBFC8 :
		mov eax, dword ptr ds : [0x982BDC]
		mov ecx, dword ptr ds : [eax]
		push 0x07
		push 0xA8
		push eax
		call dword ptr ds : [ecx + 0xE4]
		ret
	}
}

DWORD sub_6D4EF0 = 0x6D4EF0;
DWORD VisualTreatmentSub2Exit = 0x6D54ED;

void __declspec(naked) VisualTreatmentSub2()
{
	_asm
	{
		mov al, byte ptr ds : [esp + 0x04]
		sub esp, 0xA0
		push ebx
		push ebp
		push esi
		mov esi, dword ptr ds : [0x93DEB8]
		mov ebp, ecx
		cmp dword ptr ds : [esi + 0x28] , 0x01
		sete bl
		test al, al
		push edi
		jne loc_6D540C
		mov eax, dword ptr ds : [0x982C60]
		inc eax
		and eax, 0x80000001
		jns loc_6D5403
		dec eax
		or eax, -02
		inc eax

	loc_6D5403 :
		mov edi, dword ptr ds : [0x982A80] // Render 2
		jmp loc_6D5433

	loc_6D540C :
		test bl, bl
		je loc_6D542D
		mov ecx, dword ptr ds : [0x982C60]
		inc ecx
		and ecx, 0x80000001
		jns loc_6D5424
		dec ecx
		or ecx, -02
		inc ecx

	loc_6D5424 :
		mov edi, dword ptr ds : [0x982A80] // Render 2
		jmp loc_6D5433

	loc_6D542D :
		mov edi, dword ptr ds : [0x982A84] // Render 3

	loc_6D5433 :
		mov edx, dword ptr ds : [edi]
		lea eax, dword ptr ds : [esp + 0x90]
		push eax
		push edi
		call dword ptr ds : [edx + 0x30]
		mov eax, dword ptr ds : [0x982BDC]
		mov ecx, dword ptr ds : [eax]
		push edi
		xor edi, edi
		push edi
		push eax
		call dword ptr ds : [ecx + 0x94]
		mov eax, dword ptr ds : [0x982BDC]
		mov edx, dword ptr ds : [eax]
		push edi
		push eax
		call dword ptr ds : [edx + 0x9C]
		mov dword ptr ds : [0x982C80] , esi
		mov eax, dword ptr ds : [esi]
		mov ecx, esi
		call dword ptr ds : [eax + 0x04]
		mov edx, dword ptr ds : [esi + 0x4C]
		mov eax, dword ptr ds : [0x982BDC]
		mov ecx, dword ptr ds : [eax]
		push edx
		push eax
		call dword ptr ds : [ecx + 0x15C]
		mov eax, dword ptr ds : [esi + 0x48]
		mov ecx, dword ptr ds : [eax]
		push edi
		lea edx, dword ptr ds : [esp + 0x14]
		push edx
		push eax
		call dword ptr ds : [ecx + 0xFC]
		mov edx, dword ptr ds : [0x982C60]
		mov edx, dword ptr ds : [0x982A7C] // Render 1
		mov eax, dword ptr ds : [esi + 0x48]
		mov ecx, dword ptr ds : [eax]
		push edx
		mov edx, dword ptr ds : [esi + 0x44]
		mov edx, dword ptr ds : [edx + 0x22C]
		push edx
		push eax
		call dword ptr ds : [ecx + 0xD0]
		mov edx, dword ptr ds : [0x982A7C] // Render 1
		mov eax, dword ptr ds : [esi + 0x48]
		mov ecx, dword ptr ds : [eax]
		push edx
		mov edx, dword ptr ds : [esi + 0x44]
		mov edx, dword ptr ds : [edx + 0x2A4]
		push edx
		push eax
		call dword ptr ds : [ecx + 0xD0]
		mov eax, dword ptr ds : [0x982BDC]
		mov ecx, dword ptr ds : [eax]
		push edi
		push 0x07
		push eax
		call dword ptr ds : [ecx + 0xE4]
		push esi
		mov ecx, ebp
		call sub_6D4EF0
		jmp VisualTreatmentSub2Exit
	}
}

DWORD EAX_Temp_VT;
DWORD EBX_Temp_VT;
DWORD ECX_Temp_VT;
DWORD EDX_Temp_VT;
DWORD ESI_Temp_VT;
DWORD EDI_Temp_VT;
DWORD ESP_Temp_VT;
DWORD sub_6DBEC0 = 0x6DBEC0;
DWORD sub_6DBFE0 = 0x6DBFE0;
DWORD sub_6D4620 = 0x6D4620;
DWORD sub_73D0C0 = 0x73D0C0;
DWORD sub_6D49A0 = 0x6D49A0;
DWORD sub_6D3850 = 0x6D3850;
DWORD sub_6D53D0 = 0x6D53D0;
DWORD VisualTreatmentMirrorEnablerCodeCaveExit = 0x6DE9FE;


void __declspec(naked) VisualTreatmentMirrorEnablerCodeCave()
{
	_asm
	{
		mov dword ptr ds : [EAX_Temp_VT] , eax
		mov dword ptr ds : [EBX_Temp_VT] , ebx
		mov dword ptr ds : [ECX_Temp_VT] , ecx
		mov dword ptr ds : [EDX_Temp_VT] , edx
		mov dword ptr ds : [ESI_Temp_VT] , esi
		mov dword ptr ds : [EDI_Temp_VT] , edi
		mov dword ptr ds : [ESP_Temp_VT] , esp
		mov ebx, 0x00000000
		mov esi, dword ptr ds : [0x9196B4]
		cmp dword ptr ds : [0x925E90] , 0x06 // checks game state
		jne ExitCode // jumps if not in-game

		call VisualTreatmentSub1
		mov byte ptr ds : [esp + 0x0C] , 0x00
		mov byte ptr ds : [esp + 0x34] , 0x00
		mov byte ptr ds : [esp + 0x3C] , 0x00
		mov byte ptr ds : [esp + 0x18] , 0x00
		mov byte ptr ds : [esp + 0x38] , 0x00
		cmp dword ptr ds : [0x901828] , 0x00 // Visual Treatment Bool
		je ExitCode
		mov byte ptr ds : [esp + 0x18] , 0x01
		mov ecx, dword ptr ds : [0x982AF0]
		cmp ecx, 0x00
		je ExitCode
		cmp byte ptr ds : [0x8F9B5C] , 0x00 // Visual Treatment Bool
		je ExitCode
		mov edx, 0x00
		push edx
		call VisualTreatmentSub2

	ExitCode :
		mov eax, dword ptr ds : [EAX_Temp_VT]
		mov ebx, dword ptr ds : [EBX_Temp_VT]
		mov ecx, dword ptr ds : [ECX_Temp_VT]
		mov edx, dword ptr ds : [EDX_Temp_VT]
		mov esi, dword ptr ds : [ESI_Temp_VT]
		mov edi, dword ptr ds : [EDI_Temp_VT]
		mov esp, dword ptr ds : [ESP_Temp_VT]
		mov ecx, dword ptr ds : [0x93DEBC]
		jmp VisualTreatmentMirrorEnablerCodeCaveExit
	}
}

DWORD sub_6E54E0 = 0x6E54E0;
DWORD ShadowMirrorEnablerCodeCaveExit = 0x6DE893;
DWORD EDX_Temp_SH;

void __declspec(naked) ShadowMirrorEnablerCodeCave()
{
	_asm
	{
		call sub_6E2F50
		cmp byte ptr ds : [RestoreShaders] , 0x00
		jle ExitCode // jumps if RestoreShaders are disabled
		cmp byte ptr ds : [0x901830] , 0x00
		jle ExitCode // jumps if shadows are disabled
		fld dword ptr ds : [0x8FAE68]
		fcomp dword ptr ds : [0x8FAE50]
		fnstsw ax
		test ah, 0x01
		jne ExitCode // jumps if shadows are invisible
		mov dword ptr ds : [EDX_Temp_SH] , edx
		lea edx, dword ptr ds : [esp + 0xE0]
		push edx
		push 0x919730
		call sub_6E54E0
		add esp, 0x08
		call sub_6E2F50
		mov edx, dword ptr ds : [EDX_Temp_SH]

	ExitCode :
		jmp ShadowMirrorEnablerCodeCaveExit
	}
}

DWORD loc_74E70F = 0x74E70F;
DWORD loc_74E7A3 = 0x74E7A3;
float CarModelOffset = -0.25f;

void __declspec(naked) CarModelRoadReflectionCodeCave()
{
	_asm
	{
		jne IsRoadReflection
		jmp ExitCode

	IsRoadReflection:
		cmp dword ptr ds : [0x925E90], 0x06 // checks game state
		jne IsRoadReflection_ExitCode // jumps if not in-game
		fld dword ptr ds : [edi + 0x38]
		fadd dword ptr ds : [CarModelOffset]
		fstp dword ptr ds : [edi + 0x38]

	IsRoadReflection_ExitCode:
		mov eax, dword ptr ds : [ebp + 0x28]
		test eax, eax
		jmp loc_74E7A3

	ExitCode:
		jmp loc_74E70F
	}
}

DWORD FECarModelFixCodeCaveExit = 0x6DEF42;

void __declspec(naked) FECarModelFixCodeCave()
{
	_asm
	{
		mov dword ptr ds : [0x8FAE6C], 0x03 // Invert Backface Culling For Flipped Car Model
		call sub_6E2F50
		mov dword ptr ds : [0x8FAE6C], 0x02 // Normalize Backface Culling For Everything Else
		jmp FECarModelFixCodeCaveExit
	}
}