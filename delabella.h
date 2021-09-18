/*
DELABELLA - Delaunay triangulation library
Copyright (C) 2018 GUMIX - Marcin Sokalski
*/

#ifndef DELABELLA_H
#define DELABELLA_H

// returns: positive value: number of triangle indices, negative: number of line segment indices (degenerated input)
//          triangle indices in abc array are always returned in clockwise order
// DEPRECIATED. move to new API either extern "C" or IDelaBella (C++)
int DelaBella(int points, const double* xy/*[points][2]*/, int* abc/*[2*points-5][3]*/, int (*errlog)(const char* fmt,...) = printf);

struct DelaBella_Vertex
{
	int i; // index of original point
	double x, y; // coordinates (input copy)
	DelaBella_Vertex* next; // next silhouette vertex
};

struct DelaBella_Triangle
{
	DelaBella_Vertex* v[3]; // 3 vertices spanning this triangle
	DelaBella_Triangle* f[3]; // 3 adjacent faces, f[i] is at the edge opposite to vertex v[i]
	DelaBella_Triangle* next; // next triangle (of delaunay set or hull set)
};

#ifdef __cplusplus
struct IDelaBella
{
	static IDelaBella* Create();
	virtual ~IDelaBella() = default;

	virtual void Destroy() = 0;

	virtual void SetErrLog(int(*proc)(void* stream, const char* fmt, ...), void* stream) = 0;

	// return 0: no output 
	// negative: all points are colinear, output hull vertices form colinear segment list, no triangles on output
	// positive: output hull vertices form counter-clockwise ordered segment contour, delaunay and hull triangles are available
	// if 'y' pointer is null, y coords are treated to be located immediately after every x
	// if advance_bytes is less than 2*sizeof coordinate type, it is treated as 2*sizeof coordinate type  
	virtual int Triangulate(int points, const float* x, const float* y = 0, int advance_bytes = 0) = 0;
	virtual int Triangulate(int points, const double* x, const double* y = 0, int advance_bytes = 0) = 0;

	// num of points passed to last call to Triangulate()
	virtual int GetNumInputPoints() const = 0;

	// num of verts returned from last call to Triangulate()
	virtual int GetNumOutputVerts() const = 0;

	virtual const DelaBella_Triangle* GetFirstDelaunayTriangle() const = 0; // valid only if Triangulate() > 0
	virtual const DelaBella_Triangle* GetFirstHullTriangle() const = 0; // valid only if Triangulate() > 0
	virtual const DelaBella_Vertex*   GetFirstHullVertex() const = 0; // if Triangulate() < 0 it is list, otherwise closed contour! 
};
#else
void* DelaBella_Create();
void  DelaBella_Destroy(void* db);
void  DelaBella_SetErrLog(void* db, int(*proc)(void* stream, const char* fmt, ...), void* stream);
int   DelaBella_TriangulateFloat(void* db, int points, float* x, float* y = 0, int advance_bytes = 0);
int   DelaBella_TriangulateDouble(void* db, int points, double* x, double* y = 0, int advance_bytes = 0);
int   DelaBella_GetNumInputPoints(void* db);
int   DelaBella_GetNumOutputVerts(void* db);
const DelaBella_Triangle* GetFirstDelaunayTriangle(void* db);
const DelaBella_Triangle* GetFirstHullTriangle(void* db);
const DelaBella_Vertex*   GetFirstHullVertex(void* db);
#endif

#endif
