// Copyright 2017 Patrick Chadbourne (PChadbourne). All Rights Reserved.

#pragma once

/**
 * 
 */

class MARCHINGISLANDS_API Octree
{
public:
	Octree();
	Octree(TArray<float> Grid, int Width);
	~Octree();

	Octree * Children[8];
	FVector Corners[8];
	float Values[8];
	void Split();
	bool Leaf;

	const float SPLIT_VALUE = 1.0f;

};
