#include <iostream>
#include <complex>

using std::complex;

#include "psf/psf.h"
#include "psf/functor.h"
#include "psf/gibsonLaniPsfFunctor.h"

// JNI wrapper to call cosm psf generation
int CosmPSf
  (float * array, long* size, float* spacing,
		  double emissionWavelength,
		  double numericalAperture,
		  double designImmersionOilRefractiveIndex,
		  double designSpecimenLayerRefractiveIndex,
		  double actualImmersionOilRefractiveIndex,
		  double actualSpecimenLayerRefractiveIndex,
		  double actualPointSourceDepthInSpecimenLayer,
		  int type,
		  int model)
{
	// print psf parameters to command line:

	std::cout<<"EMW_302: "<<emissionWavelength<<"\n";
	std::cout<<"Numerical Aperture: "<<numericalAperture<<"\n";
	std::cout<<"Design RI (L : S):"<<designImmersionOilRefractiveIndex<<" : "<<designSpecimenLayerRefractiveIndex<<"\n";
	std::cout<<"Actual RI (L : S):"<<actualImmersionOilRefractiveIndex<<" : "<<actualSpecimenLayerRefractiveIndex<<"\n";
	std::cout<<"Depth In Specimen Layer: "<<actualPointSourceDepthInSpecimenLayer<<"\n";

	// calculate the size of the psf array
	long lengthOfPsfArray = size[0]*size[1]*size[2];
	
	std::cout<<"Computing psf using COSMOS...\n";

	Psf<float>::Model psfModel;

	if (model==0)
	{
		std::cout<<"gibson_lanni\n";
		psfModel=Psf<float>::MODEL_GIBSON_LANI;
	}
	else if (model==1)
	{
		psfModel=Psf<float>::MODEL_HAEBERLE;
	}

	PsfType psfType;

	if (type==0)
	{
		std::cout<<"widefield\n";
		psfType = OPTICAL_SECTIONING_WIDEFIELD;
	}
	else if (type==1)
	{
		std::cout<<"two photon\n";
		psfType = OPTICAL_SECTIONING_2_PHOTON;
	}
	else if (type==2)
	{
		std::cout<<"confocal disk\n";
		psfType = CONFOCAL_ROTATING_DISK_CIRCULAR_APERTURE;
	}
	else if (type==3)
	{
		std::cout<<"confocal line\n";
		psfType =CONFOCAL_ROTATING_DISK_LINE_APERTURE;
	}

	// declare the COSM psf
	cosm::Psf<float> psf(psfModel, psfType, Psf<float>::EVAL_INTERPOLATION, NULL);

	// toDo: interfaces for version 0.7 and 0.9 of COSM are a bit different.  Currently using 0.7 code.  But need to update to latest version.
	//       I fixed a couple of (possible) memory leaks in 0.7... need to communicate (verify?) this with COSM team

	// set the parameters of the COSM psf
	psf.parameters(
		size[0],        // size of the image in x and y
		size[2],         // size of the image in z
		spacing[0]*1e-6, //65e-6,      // pixel size in z and y of the image (mm)
		spacing[2]*1e-6,       // pixel size in z of the image (mm)
		actualPointSourceDepthInSpecimenLayer*1e-3,           // specimen thickness
		0.16,          // immersion thickness design
		0.16,          // immersion thickness actual
		0.17,          // coverglascmake set a path explicitlys thickness design
		0.17,          // coverglass thickness actual
		actualSpecimenLayerRefractiveIndex,           // specimen refractive index
		designImmersionOilRefractiveIndex,          // immersion refractive index design
		actualImmersionOilRefractiveIndex,          // immersion refractive index actual
		1.522,          // coverglass refractive index design
		1.522,          // coverglass refractive index actual
		160.0,          // tube length design
		160.0,          // tube length actual
		40.0,    //       // lateral magnification
		numericalAperture,           // numerical aperture (NA)
		emissionWavelength*1e-6, //
		1e-5

		// below parameters are for 0.9 version... commented out for now
		//,
     //0, //fsize
     //0, //distance
     //0, //magY,
     //0, //shear
     //0, //bias,
     //0, //ampRatio
 	//0 //rotation
		);

		//psf.evaluate(MAGNITUDE|REAL|IMAGINARY);
		psf.evaluate();

		//Array<float,3> psfArray = psf.psf();

		// copy the psf data into the java buffer
		for (int z=0;z<size[2];z++)
		{
			for (int y=0;y<size[1];y++)
			{
				for (int x=0;x<size[0];x++)
				{
					long index = z*size[0]*size[1]+y*size[0]+x;
					array[index]=psf.psf()(z,y,x);
				}
			}
		}

		return true;
}

