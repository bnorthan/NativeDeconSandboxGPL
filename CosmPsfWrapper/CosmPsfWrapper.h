#pragma once

//#ifdef _WIN64
__declspec(dllexport)
//#endif
//extern "C" 
int CosmPsf
(float * array, int* size, float* spacing,
	double emissionWavelength,
	double numericalAperture,
	double designImmersionOilRefractiveIndex,
	double designSpecimenLayerRefractiveIndex,
	double actualImmersionOilRefractiveIndex,
	double actualSpecimenLayerRefractiveIndex,
	double actualPointSourceDepthInSpecimenLayer,
	int type,
	int model);
