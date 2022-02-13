#pragma once

// Constants
int RoadResX = 320;
int RoadResY = 240;
int VehicleRes = 256;
int MirrorResX = 512;
int MirrorResY = 256;
int PIPRes = 256;
int CubemapRes = 256;

// Resolution
bool HDReflections;
bool OldGPUCompatibility;
float VehicleScale;
float RoadScale;
float MirrorScale;
float PIPScale;

// General
bool ImproveReflectionLOD;
bool RestoreDetails;
int ReflectionContrast;
int ForceEnvironmentMap;
bool AlignmentFix;
bool AspectRatioFix;
bool CubemapFix;
bool RestoreSkybox;
bool EnableMirror;
bool RealFrontEndReflections;
float VehicleReflectionBrightness;

// Extra
int ReplaceVisualTreatmentIG;
int ReplaceVisualTreatmentFE;
bool TrafficSignFix;
bool PauseBlur;
bool RealisticChrome;