#ifndef _OBJECTTYPES_H_
#define _OBJECTTYPES_H_

#define MAX_VERTICES 15000
#define MAX_POLYGONS 15000

#define RADIANS_TO_DEGREES 57.2957795
#define NUDGE 0.5

// Our vertex type
typedef struct
{
	float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct
{
	int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct
{
	float u,v;
}mapcoord_type;

// The object type
typedef struct 
{
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES]; 
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type, *obj_type_ptr;

typedef struct 
{
	float index[3][3];
}matrix3x3_type;

//Coordinates in  heightmap.
struct position
{
	float x;
	float z;
	position() :x(0), z(0) {}
	position(float x, float z) :x(x), z(z) {}
};

//Edge cost
typedef struct
{
	int waypointID_From;
	int waypointID_To;
	int cost;
}edge_type;

//Scene
#define SCENARIO_WALL_QUANTITY		95

#endif //_OBJECTTYPES_H_