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
	static Geometry MakePlane() noexcept
	{
		return
		{
			{ // Vertices
				{ -1.0f, -1.0f, 0.0f },
				{ -1.0f,  1.0f, 0.0f },
				{  1.0f,  1.0f, 0.0f },
				{  1.0f, -1.0f, 0.0f }
			},
			{ // Indices
				0,1,2, 0,2,3
			}
		};
	}
	static Geometry MakeIcosphere() noexcept
	{
		return
		{
			{ // Vertices
				{ -0.7236f,   0.52572f,  0.44721   },
				{ -0.7236f,  -0.52572f,  0.44721f  },
				{ -0.89442f,  0.0f,     -0.44721f  },
				{ -0.27639f, -0.85064f, -0.44721f  },
				{  0.27639f, -0.85064f,  0.44721f  },
				{  0.7236f,  -0.52572f, -0.44721   },
				{  0.89442f,  0.0f,      0.44721f  },
				{  0.27639f,  0.85064f,  0.44721f  },
				{  0.7236f,   0.52572f, -0.44721f  },
				{ -0.27639f,  0.85064f, -0.44721f  },
				{  0.0f,      0.0f,      1.0f      },
				{  0.0f,      0.0f,     -1.0f      }
			},
			{ // Indices
				0,1,2,  2,1,3,  1,4,3,  3,4,5,
				4,6,5,  5,6,8,  6,7,8,  8,7,9,
				7,0,9,  0,2,9,  0,10,1, 1,10,4,
				4,10,6, 6,10,7, 7,10,0, 2,3,11,
				3,5,11,	5,8,11, 8,9,11, 9,2,11
			}
		};
	}
public:
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;
};