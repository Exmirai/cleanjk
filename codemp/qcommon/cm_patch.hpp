/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2019, OpenJK contributors
Copyright (C) 2019 - 2020, CleanJoKe contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#pragma once

//#define	CULL_BBOX

/*

This file does not reference any globals, and has these entry points:

void CM_ClearLevelPatches( void );
patchCollide_t *CM_GeneratePatchCollide( int width, int height, const vec3_t *points );
void CM_TraceThroughPatchCollide( traceWork_t *tw, const patchCollide_t *pc );
bool CM_PositionTestInPatchCollide( traceWork_t *tw, const patchCollide_t *pc );
void CM_DrawDebugSurface( void (*drawPoly)(int color, int numPoints, flaot *points) );

Issues for collision against curved surfaces:

Surface edges need to be handled differently than surface planes

Plane expansion causes raw surfaces to expand past expanded bounding box

Position test of a volume against a surface is tricky.

Position test of a point against a surface is not well defined, because the surface has no volume.

Tracing leading edge points instead of volumes?
Position test by tracing corner to corner? (8*7 traces -- ouch)

coplanar edges
triangulated patches
degenerate patches

  endcaps
  degenerate

WARNING: this may misbehave with meshes that have rows or columns that only
degenerate a few triangles.  Completely degenerate rows and columns are handled
properly.
*/

#define	MAX_FACETS			1024
#define	MAX_GRID_SIZE      129
#define	MAX_PATCH_PLANES	2048
#define	SUBDIVIDE_DISTANCE 16 //4 // never more than this units away from curve
#define	PLANE_TRI_EPSILON  0.1
#define	WRAP_POINT_EPSILON 0.1



struct patchPlane_t {
	float	plane[4];
	int		signbits;		// signx + (signy<<1) + (signz<<2), used as lookup during collision
};

struct facet_t {
	int			surfacePlane;
	int			numBorders;		// 3 or four + 6 axial bevels + 4 or 3 * 4 edge bevels
	int			borderPlanes[4+6+16];
	int			borderInward[4+6+16];
	bool		borderNoAdjust[4+6+16];
};

struct patchCollide_t {
	vec3_t	bounds[2];
	int		numPlanes;			// surface planes plus edge planes
	patchPlane_t	*planes;
	int		numFacets;
	facet_t	*facets;
};

struct cPatch_t {
	int             checkcount; // to avoid repeated testings
	int             surfaceFlags;
	int             contents;
	patchCollide_t *pc;
};

struct cGrid_t {
	int			width;
	int			height;
	bool		wrapWidth;
	bool		wrapHeight;
	vec3_t	points[MAX_GRID_SIZE][MAX_GRID_SIZE];	// [width][height]
};



patchCollide_t *CM_GeneratePatchCollide( int width, int height, vec3_t *points );
