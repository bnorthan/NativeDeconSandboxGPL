%module CosmPsf_swig
%include arrays_java.i
%apply float[] {float *};
%apply int[] {int *};
%{
extern int CosmPsf(float * array, int* size, float* spacing,
		  double emissionWavelength,
		  double numericalAperture,
		  double designImmersionOilRefractiveIndex,
		  double designSpecimenLayerRefractiveIndex,
		  double actualImmersionOilRefractiveIndex,
		  double actualSpecimenLayerRefractiveIndex,
		  double actualPointSourceDepthInSpecimenLayer,
		  int type,
		  int model);
%}

extern int CosmPsf(float * array, int* size, float* spacing,
		  double emissionWavelength,
		  double numericalAperture,
		  double designImmersionOilRefractiveIndex,
		  double designSpecimenLayerRefractiveIndex,
		  double actualImmersionOilRefractiveIndex,
		  double actualSpecimenLayerRefractiveIndex,
		  double actualPointSourceDepthInSpecimenLayer,
		  int type,
		  int model);


