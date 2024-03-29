#pragma once

// Constants
int RoadResX = 320;
int RoadResY = 240;
int VehicleRes = 128;
int MirrorResX = 256;
int MirrorResY = 256;
int BlurResX = 320;
int BlurResY = 240;

// Resolution
bool HDReflections;
bool OldGPUCompatibility;
float VehicleScale;
float RoadScale;
float MirrorScale;

// General
bool ImproveReflectionLOD;
bool RemoveParticleEffects;
bool AspectRatioFix;
bool FrontEndReflectionBlur;
bool ForceEnableMirror;
bool RestoreEnhancedContrast;
bool RestoreSkybox;
bool RestoreHeadlights;
bool ExtendRenderDistance;
float VehicleReflectionBrightness;

// Extra
bool ImproveMotionBlur;
float MotionBlurStrength;
bool ExpandMemoryPools;
bool RealisticChrome;