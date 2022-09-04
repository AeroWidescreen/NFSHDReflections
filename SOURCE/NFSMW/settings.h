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
bool RestoreVisualTreatment;
bool RestoreShaders;
bool RestoreShadows;
bool RestoreDetails;
bool RestoreBackfaceCulling;
bool OptimizeRenderDistance;
float VehicleReflectionBrightness;

// Extra
bool ExpandMemoryPools;