#pragma once

#include <DirectXMath.h>
#include <vector>

class Geometry
{
public:
	Geometry(std::vector<DirectX::XMFLOAT3> vertices, std::vector<unsigned short> indices) noexcept(!IS_DEBUG)
		:
		vertices(vertices),
		indices(indices)
	{
		assert(vertices.size() >= 3);
		assert(indices.size() % 3 == 0 && "Index count must be a multiple of 3");
	}
public:
	static Geometry MakeCube() noexcept
	{
		return
		{
			{ // Vertices
				{-1.0f, 1.0f,-1.0f},	// 0
				{ 1.0f, 1.0f,-1.0f},	// 1
				{ 1.0f,-1.0f,-1.0f},	// 2
				{-1.0f,-1.0f,-1.0f},	// 3
				{-1.0f, 1.0f, 1.0f},	// 4
				{ 1.0f, 1.0f, 1.0f},	// 5
				{ 1.0f,-1.0f, 1.0f},	// 6
				{-1.0f,-1.0f, 1.0f}		// 7
			},
			{ // Indices
				0,2,1, 0,3,2,			// Front
				1,6,5, 1,2,6,			// Right
				5,7,4, 5,6,7,			// Back
				4,3,0, 4,7,3,			// Left
				4,1,5, 4,0,1,			// Top
				3,6,2, 3,7,6			// Bottom
			}
		};
	}
public:
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;
};