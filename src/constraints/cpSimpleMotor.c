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

#include "../chipmunk.h"
#include "util.h"

static void
preStep(cpSimpleMotor *joint, cpFloat dt, cpFloat dt_inv)
{
	cpBody *a = joint->constraint.a;
	cpBody *b = joint->constraint.b;
	
	// calculate moment of inertia coefficient.
	joint->iSum = 1.0f/(a->moment_inv + b->moment_inv);
	
	// compute max impulse
	joint->jMax = J_MAX(joint, dt);

	// apply joint torque
	a->ang_vel -= joint->jAcc*a->moment_inv;
	b->ang_vel += joint->jAcc*b->moment_inv;
}

static void
applyImpulse(cpSimpleMotor *joint)
{
	cpBody *a = joint->constraint.a;
	cpBody *b = joint->constraint.b;
	
	// compute relative rotational velocity
	cpFloat wr = b->ang_vel - a->ang_vel + joint->rate;
	
	// compute normal impulse	
	cpFloat j = -wr*joint->iSum;
	cpFloat jOld = joint->jAcc;
	joint->jAcc = cpfclamp(jOld + j, -joint->jMax, joint->jMax);
	j = joint->jAcc - jOld;
	
	// apply impulse
	a->ang_vel -= j*a->moment_inv;
	b->ang_vel += j*b->moment_inv;
}

static cpFloat
getImpulse(cpSimpleMotor *joint)
{
	return cpfabs(joint->jAcc);
}

const cpConstraintClass cpSimpleMotorClass = {
	(cpConstraintPreStepFunction)preStep,
	(cpConstraintApplyImpulseFunction)applyImpulse,
	(cpConstraintGetImpulseFunction)getImpulse,
};

cpSimpleMotor *
cpSimpleMotorAlloc(void)
{
	return (cpSimpleMotor *)malloc(sizeof(cpSimpleMotor));
}

cpSimpleMotor *
cpSimpleMotorInit(cpSimpleMotor *joint, cpBody *a, cpBody *b, cpFloat rate)
{
	cpConstraintInit((cpConstraint *)joint, &cpSimpleMotorClass, a, b);
	
	joint->rate = rate;
	
	joint->jAcc = 0.0f;
	
	return joint;
}

cpConstraint *
cpSimpleMotorNew(cpBody *a, cpBody *b, cpFloat rate)
{
	return (cpConstraint *)cpSimpleMotorInit(cpSimpleMotorAlloc(), a, b, rate);
}
