/*
Copyright (C) 2007 Peter Mackay and Chris Swindle.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef QUAKE_CLIPPING_HPP
#define QUAKE_CLIPPING_HPP

#include <cstddef>

/*
struct glvert_s
{
	SVector3 pos;
	//float x, y, z;
	float nx, ny, nz;
	int c;
	float u, v;
}
*/


typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec2_t[2];

typedef struct glvert_s
{
	vec2_t	st;
	vec3_t	xyz;
} glvert_t;



		// Calculates clipping planes from the GU view and projection matrices.
		void begin_frame();

		// Calculates clipping planes from the GU view and projection matrices.
		void begin_brush_model();

		// Resets the frustum to camera space.
		void end_brush_model();

		// Is in front of
		bool is_in_front(vec3_t v1, vec3_t v2, vec3_t v3);

		// Is clipping required?
		bool is_clipping_required(const struct glvert_s* vertices, std::size_t vertex_count);

		// Clips a polygon.
		void clip(
			const struct glvert_s* unclipped_vertices,
			std::size_t unclipped_vertex_count,
			const struct glvert_s** clipped_vertices,
			std::size_t* clipped_vertex_count);
		
#endif

