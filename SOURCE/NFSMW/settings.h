#pragma once

// Constants
int RoadResX = 320;
int RoadResY = 240;
int VehicleRes = 256;
int MirrorResX = 256;
int MirrorResY = 256;

// Resolution
bool HDReflections;
bool OldGPUCompatibility;
float VehicleScale;
float RoadScale;
float MirrorScale;

// General
bool ImproveReflectionLOD;
bool ForceEnableMirror;
bool RestoreVisualTreatment;
bool RestoreShaders;
bool CubemapBrightnessFix;
bool RestoreShadows;
int RestoreDetails;
int RestoreCars;
bool RestoreBackfaceCulling;
bool RestoreWaterReflections;
int RemoveLastGenEffects;
bool OptimizeRenderDistance;
float VehicleReflectionBrightness;

// Extra
bool ExpandMemoryPools;
bool DisableBackFaceCulling;