/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "chipmunk.h"
#include "drawSpace.h"
#include "ChipmunkDemo.h"

cpSpace *space;
cpBody *staticBody;

static void
update(int ticks)
{
	int steps = 1;
	cpFloat dt = 1.0/60.0/(cpFloat)steps;
	
	for(int i=0; i<steps; i++){
		cpSpaceStep(space, dt);
		
		// Update the static body spin so that it looks like it's rotating.
		cpBodyUpdatePosition(staticBody, dt);
	}
}

static void
planetGravityVelocityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	cpVect p = body->p;
	cpVect g = cpvmult(p, -50000.0f/cpvdot(p, p));
	
	cpBodyUpdateVelocity(body, g, damping, dt);
}

static cpVect
rand_pos(cpFloat radius)
{
	cpVect v;
	do {
		v = cpv(frand()*(640 - 2*radius) - (320 - radius), frand()*(480 - 2*radius) - (240 - radius));
	} while(cpvlength(v) < 100.0f);
	
	return v;
}

static void
add_box()
{
	const cpFloat size = 10.0f;
	const cpFloat mass = 1.0f;
	
	cpVect verts[] = {
		cpv(-size,-size),
		cpv(-size, size),
		cpv( size, size),
		cpv( size,-size),
	};
	
	cpFloat radius = cpvlength(cpv(size, size));

	cpBody *body = cpBodyNew(mass, cpMomentForPoly(mass, 4, verts, cpvzero));
	body->velocity_func = planetGravityVelocityFunc;
	body->p = rand_pos(radius);
	body->v = cpvmult(cpv(2*frand() - 1, 2*frand() - 1), 200);
	cpSpaceAddBody(space, body);

	cpShape *shape = cpPolyShapeNew(body, 4, verts, cpvzero);
	shape->e = 0.0f; shape->u = 0.7f;
	cpSpaceAddShape(space, shape);
}

static cpSpace *
init(void)
{
	staticBody = cpBodyNew(INFINITY, INFINITY);
	staticBody->w = 0.2f;
	
	cpResetShapeIdCounter();
	
	space = cpSpaceNew();
	space->iterations = 20;
	
	for(int i=0; i<30; i++)
		add_box();
	
	cpShape *shape = cpCircleShapeNew(staticBody, 70.0f, cpvzero);
	shape->e = 1.0; shape->u = 1.0;
	cpSpaceAddStaticShape(space, shape);
	
	return space;
}

static void
destroy(void)
{
	cpBodyFree(staticBody);
	cpSpaceFreeChildren(space);
	cpSpaceFree(space);
}

const chipmunkDemo Planet = {
	"Planet",
	NULL,
	init,
	update,
	destroy,
};
