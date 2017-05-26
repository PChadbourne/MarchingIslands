// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#pragma once

/*
 Implementation of SimplexNoise courtesy of devdad
 */


class MARCHINGISLANDS_API SimplexNoise
{
private:

	static unsigned char perm[];
	static float  grad(int hash, float x, float y, float z);

public:

	SimplexNoise();
	SimplexNoise(int32 seed);

	static void setNoiseSeed(const int32& newSeed);
	static float SimplexNoise3D(float x, float y, float z);
	static float SimplexNoiseScaled3D(float x, float y, float z, float s);
	static float SimplexNoiseInRange3D(float x, float y, float z, float rangeMin, float rangeMax);
};
