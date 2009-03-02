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

extern cpSpace *space;
extern cpBody *staticBody;

static void
update(int ticks)
{
	int steps = 1;
	cpFloat dt = 1.0/60.0/(cpFloat)steps;
	
	for(int i=0; i<steps; i++){
		cpSpaceStep(space, dt);
	}
}

static cpBody *
add_bar(cpVect a, cpVect b, int group)
{
	cpVect center = cpvmult(cpvadd(a, b), 1.0f/2.0f);
	cpFloat length = cpvlength(cpvsub(b, a));
	cpFloat mass = length/160.0f;
	
	cpBody *body = cpBodyNew(mass, mass*length*length/12.0f);
	body->p = center;
	cpSpaceAddBody(space, body);
	
	cpShape *shape = cpSegmentShapeNew(body, cpvsub(a, center), cpvsub(b, center), 10.0f);
	shape->group = group;
	cpSpaceAddShape(space, shape);
	
	return body;
}

static cpSpace *
init(void)
{
	staticBody = cpBodyNew(INFINITY, INFINITY);
	
	space = cpSpaceNew();
	cpSpaceResizeActiveHash(space, 30.0, 999);
	cpSpaceResizeStaticHash(space, 200.0, 99);
	
	cpBody *body1  = add_bar(cpv(-240,  160), cpv(-160,   80), 1);
	cpBody *body2  = add_bar(cpv(-160,   80), cpv( -80,  160), 1);
	cpBody *body3  = add_bar(cpv(   0,  160), cpv(  80,    0), 0);
	cpBody *body4  = add_bar(cpv( 160,  160), cpv( 240,  160), 0);
	cpBody *body5  = add_bar(cpv(-240,    0), cpv(-160,  -80), 2);
	cpBody *body6  = add_bar(cpv(-160,  -80), cpv( -80,    0), 2);
	cpBody *body7  = add_bar(cpv( -80,    0), cpv(   0,    0), 2);
	cpBody *body8  = add_bar(cpv(   0,  -80), cpv(  80,  -80), 0);
	cpBody *body9  = add_bar(cpv( 240,   80), cpv( 160,    0), 3);
	cpBody *body10 = add_bar(cpv( 160,    0), cpv( 240,  -80), 3);
	cpBody *body11 = add_bar(cpv(-240,  -80), cpv(-160, -160), 4);
	cpBody *body12 = add_bar(cpv(-160, -160), cpv( -80, -160), 0);
	cpBody *body13 = add_bar(cpv(   0, -160), cpv(  80, -160), 0);
	cpBody *body14 = add_bar(cpv( 160, -160), cpv( 240, -160), 0);
	
	cpSpaceAddConstraint(space, cpPivotJointNew2( body1,  body2, cpv( 40,-40), cpv(-40,-40)));
	cpSpaceAddConstraint(space, cpPivotJointNew2( body5,  body6, cpv( 40,-40), cpv(-40,-40)));
	cpSpaceAddConstraint(space, cpPivotJointNew2( body6,  body7, cpv( 40, 40), cpv(-40,  0)));
	cpSpaceAddConstraint(space, cpPivotJointNew2( body9, body10, cpv(-40,-40), cpv(-40, 40)));
	cpSpaceAddConstraint(space, cpPivotJointNew2(body11, body12, cpv( 40,-40), cpv(-40,  0)));
	
	cpFloat stiff = 100.0f;
	cpFloat damp = 0.5f;
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body1, cpv(-320,  240), cpv(-40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body1, cpv(-320,   80), cpv(-40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body1, cpv(-160,  240), cpv(-40, 40), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body2, cpv(-160,  240), cpv( 40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body2, cpv(   0,  240), cpv( 40, 40), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body3, cpv(  80,  240), cpv(-40, 80), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body4, cpv(  80,  240), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body4, cpv( 320,  240), cpv( 40,  0), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body5, cpv(-320,   80), cpv(-40, 40), 0.0f, stiff, damp));
	
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody,  body9, cpv( 320,  80), cpv( 40, 40), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body10, cpv( 320,   0), cpv( 40,-40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body10, cpv( 320,-160), cpv( 40,-40), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body11, cpv(-320,-160), cpv(-40, 40), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body12, cpv(-240,-240), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body12, cpv(   0,-240), cpv( 40,  0), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body13, cpv(   0,-240), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body13, cpv(  80,-240), cpv( 40,  0), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body14, cpv(  80,-240), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body14, cpv( 240,-240), cpv( 40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(staticBody, body14, cpv( 320,-160), cpv( 40,  0), 0.0f, stiff, damp));

	cpSpaceAddConstraint(space, cpDampedSpringNew( body1,  body5, cpv( 40,-40), cpv(-40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body1,  body6, cpv( 40,-40), cpv( 40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body2,  body3, cpv( 40, 40), cpv(-40, 80), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body4, cpv(-40, 80), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body4, cpv( 40,-80), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body7, cpv( 40,-80), cpv( 40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body7, cpv(-40, 80), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body8, cpv( 40,-80), cpv( 40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body3,  body9, cpv( 40,-80), cpv(-40,-40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body4,  body9, cpv( 40,  0), cpv( 40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body5, body11, cpv(-40, 40), cpv(-40, 40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body5, body11, cpv( 40,-40), cpv( 40,-40), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body7,  body8, cpv( 40,  0), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body8, body12, cpv(-40,  0), cpv( 40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body8, body13, cpv(-40,  0), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body8, body13, cpv( 40,  0), cpv( 40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew( body8, body14, cpv( 40,  0), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(body10, body14, cpv( 40,-40), cpv(-40,  0), 0.0f, stiff, damp));
	cpSpaceAddConstraint(space, cpDampedSpringNew(body10, body14, cpv( 40,-40), cpv(-40,  0), 0.0f, stiff, damp));
	
	return space;
}

static void
destroy(void)
{
	cpBodyFree(staticBody);
	cpSpaceFreeChildren(space);
	cpSpaceFree(space);
}

const chipmunkDemo Springies = {
	"Springies",
	NULL,
	init,
	update,
	destroy,
};
