/*
	Copyright (c) 2009 David Bucciarelli (davibu@interfree.it)

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// NOTE: workaround for an Apple OpenCL compiler bug
#ifdef __APPLE__
#define OCL_CONSTANT_BUFFER __global
#else
#define OCL_CONSTANT_BUFFER __constant
#endif

//------------------------------------------------------------------------------
// vec.h

typedef struct {
	float x, y, z; // position, also color (r,g,b)
} Vec;

#define vinit(v, a, b, c) { (v).x = a; (v).y = b; (v).z = c; }
#define vassign(a, b) vinit(a, (b).x, (b).y, (b).z)
#define vclr(v) vinit(v, 0.f, 0.f, 0.f)
#define vadd(v, a, b) vinit(v, (a).x + (b).x, (a).y + (b).y, (a).z + (b).z)
#define vsub(v, a, b) vinit(v, (a).x - (b).x, (a).y - (b).y, (a).z - (b).z)
#define vsadd(v, a, b) { float k = (a); vinit(v, (b).x + k, (b).y + k, (b).z + k) }
#define vssub(v, a, b) { float k = (a); vinit(v, (b).x - k, (b).y - k, (b).z - k) }
#define vmul(v, a, b) vinit(v, (a).x * (b).x, (a).y * (b).y, (a).z * (b).z)
#define vsmul(v, a, b) { float k = (a); vinit(v, k * (b).x, k * (b).y, k * (b).z) }
#define vdot(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)
#define vnorm(v) { float l = 1.f / sqrt(vdot(v, v)); vsmul(v, l, v); }
#define vxcross(v, a, b) vinit(v, (a).y * (b).z - (a).z * (b).y, (a).z * (b).x - (a).x * (b).z, (a).x * (b).y - (a).y * (b).x)
#define vfilter(v) ((v).x > (v).y && (v).x > (v).z ? (v).x : (v).y > (v).z ? (v).y : (v).z)
#define viszero(v) (((v).x == 0.f) && ((v).x == 0.f) && ((v).z == 0.f))

#define toInt(x) ((int)(pow(clamp(x, 0.f, 1.f), 1.f / 2.2f) * 255.f + .5f))

//------------------------------------------------------------------------------
// camera.h
typedef struct {
	/* User defined values */
	Vec orig, target;
	/* Calculated values */
	Vec dir, x, y;
} Camera;

//------------------------------------------------------------------------------
// geom.h

#define EPSILON 0.01f
#define FLOAT_PI 3.14159265358979323846f

typedef struct {
	Vec o, d;
	//Vector invDir;
	//int sign[3];

	//Ray(Vector o, Vector d) : origin(o), direction(d) {
	//	this->invDir = Vector(1 / direction.x, 1 / direction.y, 1 / direction.z);
	//	sign[0] = (invDir.x < 0);
	//	sign[1] = (invDir.y < 0);
	//	sign[2] = (invDir.z < 0);
	//}
} Ray;

#define rinit(r, a, b) { vassign((r).o, a); vassign((r).d, b); }
#define rassign(a, b) { vassign((a).o, (b).o); vassign((a).d, (b).d); }

enum TypePrimitive {
	SPHERE, CUBE
};

enum Refl {
	DIFF, SPEC, REFR
}; /* material types, used in radiance() */

typedef struct {
	float rad; /* radius */
	Vec p, e, c; /* position, emission, color */
	enum Refl refl; /* reflection type (DIFFuse, SPECular, REFRactive) */
	enum TypePrimitive type;
	Vec dimensions;
} Sphere; // Nom est juste pour eviter de tout changer

//typedef struct Sphere {
//	float rad; /* radius */
//};

//struct Cube : Primitive {
//	Vec dimensions
//};

//------------------------------------------------------------------------------
// simplernd.h

static float GetRandom(unsigned int *seed0, unsigned int *seed1) {
	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

	unsigned int ires = ((*seed0) << 16) + (*seed1);

	/* Convert to float */
	union {
		float f;
		unsigned int ui;
	} res;
	res.ui = (ires & 0x007fffff) | 0x40000000;

	return (res.f - 2.f) / 2.f;
}

//------------------------------------------------------------------------------

static float SphereIntersect(
	OCL_CONSTANT_BUFFER const Sphere *s,
	const Ray *r) { /* returns distance, 0 if nohit */
	if (s->type == SPHERE) {
		Vec op; /* Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 */
		vsub(op, s->p, r->o);

		float b = vdot(op, r->d);
		float det = b * b - vdot(op, op) + s->rad * s->rad;
		if (det < 0.f)
			return 0.f;
		else
			det = sqrt(det);

		float t = b - det;
		if (t > EPSILON)
			return t;
		else {
			t = b + det;

			if (t > EPSILON)
				return t;
			else
				return 0.f;
		}
	} else if (s->type == CUBE) {
		Vec bounds[2];
		bounds[0] = s->p;
		vinit(bounds[1], s->p.x + s->dimensions.x, s->p.y + s->dimensions.y, s->p.z + s->dimensions.z)

		Vec invDir;
		vinit(invDir, 1 / r->d.x, 1 / r->d.y, 1 / r->d.z);
		int sign[3];
		sign[0] = (invDir.x < 0);
		sign[1] = (invDir.y < 0);
		sign[2] = (invDir.z < 0);


		float tmin = (bounds[sign[0]].x - r->o.x) * invDir.x;
		float tmax = (bounds[1 - sign[0]].x - r->o.x) * invDir.x;
		float tymin = (bounds[sign[1]].y - r->o.y) * invDir.y;
		float tymax = (bounds[1 - sign[1]].y - r->o.y) * invDir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return 0;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (bounds[sign[2]].z - r->o.z) * invDir.z;
		float tzmax = (bounds[1 - sign[2]].z - r->o.z) * invDir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return 0;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		float distance = tmin;
		if (distance < 0) {
			distance = tmax;
			if (distance < 0) return 0;
		}

		return distance;
	}
}

static void UniformSampleSphere(const float u1, const float u2, Vec *v) {
	const float zz = 1.f - 2.f * u1;
	const float r = sqrt(max(0.f, 1.f - zz * zz));
	const float phi = 2.f * FLOAT_PI * u2;
	const float xx = r * cos(phi);
	const float yy = r * sin(phi);

	vinit(*v, xx, yy, zz);
}

static int Intersect(
	OCL_CONSTANT_BUFFER const Sphere* spheres,
	const unsigned int sphereCount,
	const Ray* r,
	float* t,
	unsigned int* id) {
	float inf = (*t) = 1e20f;

	unsigned int i = sphereCount;
	for (; i--;) {
		const float d = SphereIntersect(&spheres[i], r);
		if ((d != 0.f) && (d < *t)) {
			*t = d;
			*id = i;
		}
	}

	return (*t < inf);
}

static int IntersectP(
	OCL_CONSTANT_BUFFER const Sphere* spheres,
	const unsigned int sphereCount,
	const Ray* r,
	const float maxt) {
	unsigned int i = sphereCount;
	for (; i--;) {
		const float d = SphereIntersect(&spheres[i], r);
		if ((d != 0.f) && (d < maxt))
			return 1;
	}

	return 0;
}

static void SampleLights(
	OCL_CONSTANT_BUFFER const Sphere *spheres,
	const unsigned int sphereCount,
	unsigned int *seed0, unsigned int *seed1,
	const Vec *hitPoint,
	const Vec *normal,
	Vec *result) {
	vclr(*result);

	/* For each light */
	unsigned int i;
	for (i = 0; i < sphereCount; i++) {
		OCL_CONSTANT_BUFFER const Sphere *light = &spheres[i];
		if (!viszero(light->e)) {
			/* It is a light source */
			Ray shadowRay;
			shadowRay.o = *hitPoint;

			/* Choose a point over the light source */
			Vec unitSpherePoint;
			UniformSampleSphere(GetRandom(seed0, seed1), GetRandom(seed0, seed1), &unitSpherePoint);
			Vec spherePoint;
			vsmul(spherePoint, light->rad, unitSpherePoint);
			vadd(spherePoint, spherePoint, light->p);

			/* Build the shadow ray direction */
			vsub(shadowRay.d, spherePoint, *hitPoint);
			const float len = sqrt(vdot(shadowRay.d, shadowRay.d));
			vsmul(shadowRay.d, 1.f / len, shadowRay.d);

			float wo = vdot(shadowRay.d, unitSpherePoint);
			if (wo > 0.f) {
				/* It is on the other half of the sphere */
				continue;
			} else
				wo = -wo;

			/* Check if the light is visible */
			const float wi = vdot(shadowRay.d, *normal);
			if ((wi > 0.f) && (!IntersectP(spheres, sphereCount, &shadowRay, len - EPSILON))) {
				Vec c; vassign(c, light->e);
				const float s = (4.f * FLOAT_PI * light->rad * light->rad) * wi * wo / (len *len);
				vsmul(c, s, c);
				vadd(*result, *result, c);
			}
		}
	}
}

static void Radiance(
	OCL_CONSTANT_BUFFER const Sphere *spheres,
	const unsigned int sphereCount,
	const Ray *startRay,
	unsigned int *seed0, unsigned int *seed1,
	Vec *result) {
	Ray currentRay; rassign(currentRay, *startRay);
	Vec rad; vinit(rad, 0.f, 0.f, 0.f);
	Vec throughput; vinit(throughput, 1.f, 1.f, 1.f);

	unsigned int depth = 0;
	int specularBounce = 1;
	for (;; ++depth) {
		// Removed Russian Roulette in order to improve execution on SIMT
		if (depth > 6) {
			*result = rad;
			return;
		}

		float t; /* distance to intersection */
		unsigned int id = 0; /* id of intersected object */
		if (!Intersect(spheres, sphereCount, &currentRay, &t, &id)) {
			*result = rad; /* if miss, return */
			return;
		}

		OCL_CONSTANT_BUFFER const Sphere *obj = &spheres[id]; /* the hit object */

		Vec hitPoint;
		vsmul(hitPoint, t, currentRay.d);
		vadd(hitPoint, currentRay.o, hitPoint);

		Vec normal;
		vsub(normal, hitPoint, obj->p);
		vnorm(normal);

		const float dp = vdot(normal, currentRay.d);
		Vec nl;
		// SIMT optimization
		const float invSignDP = -1.f * sign(dp);
		vsmul(nl, invSignDP, normal);

		/* Add emitted light */
		Vec eCol; vassign(eCol, obj->e);
		if (!viszero(eCol)) {
			if (specularBounce) {
				vsmul(eCol, fabs(dp), eCol);
				vmul(eCol, throughput, eCol);
				vadd(rad, rad, eCol);
			}

			*result = rad;
			return;
		}

		if (obj->refl == DIFF) { /* Ideal DIFFUSE reflection */
			specularBounce = 0;
			vmul(throughput, throughput, obj->c);

			/* Direct lighting component */

			Vec Ld;
			SampleLights(spheres, sphereCount, seed0, seed1, &hitPoint, &nl, &Ld);
			vmul(Ld, throughput, Ld);
			vadd(rad, rad, Ld);

			// Check if we have to stop

			/* Diffuse component */

			float r1 = 2.f * FLOAT_PI * GetRandom(seed0, seed1);
			float r2 = GetRandom(seed0, seed1);
			float r2s = sqrt(r2);

			Vec w; vassign(w, nl);

			Vec u, a;
			if (fabs(w.x) > .1f) {
					vinit(a, 0.f, 1.f, 0.f);
			} else {
					vinit(a, 1.f, 0.f, 0.f);
			}
			vxcross(u, a, w);
			vnorm(u);

			Vec v;
			vxcross(v, w, u);

			Vec newDir;
			vsmul(u, cos(r1) * r2s, u);
			vsmul(v, sin(r1) * r2s, v);
			vadd(newDir, u, v);
			vsmul(w, sqrt(1 - r2), w);
			vadd(newDir, newDir, w);

			currentRay.o = hitPoint;
			currentRay.d = newDir;
			continue;
		} else if (obj->refl == SPEC) { /* Ideal SPECULAR reflection */
			specularBounce = 1;

			Vec newDir;
			vsmul(newDir,  2.f * vdot(normal, currentRay.d), normal);
			vsub(newDir, currentRay.d, newDir);

			vmul(throughput, throughput, obj->c);

			rinit(currentRay, hitPoint, newDir);
			continue;
		} else {
			specularBounce = 1;

			Vec newDir;
			vsmul(newDir,  2.f * vdot(normal, currentRay.d), normal);
			vsub(newDir, currentRay.d, newDir);

			Ray reflRay; rinit(reflRay, hitPoint, newDir); /* Ideal dielectric REFRACTION */
			int into = (vdot(normal, nl) > 0); /* Ray from outside going in? */

			float nc = 1.f;
			float nt = 1.5f;
			float nnt = into ? nc / nt : nt / nc;
			float ddn = vdot(currentRay.d, nl);
			float cos2t = 1.f - nnt * nnt * (1.f - ddn * ddn);

			if (cos2t < 0.f)  { /* Total internal reflection */
				vmul(throughput, throughput, obj->c);

				rassign(currentRay, reflRay);
				continue;
			}

			float kk = (into ? 1 : -1) * (ddn * nnt + sqrt(cos2t));
			Vec nkk;
			vsmul(nkk, kk, normal);
			Vec transDir;
			vsmul(transDir, nnt, currentRay.d);
			vsub(transDir, transDir, nkk);
			vnorm(transDir);

			float a = nt - nc;
			float b = nt + nc;
			float R0 = a * a / (b * b);
			float c = 1 - (into ? -ddn : vdot(transDir, normal));

			float Re = R0 + (1 - R0) * c * c * c * c*c;
			float Tr = 1.f - Re;
			float P = .25f + .5f * Re;
			float RP = Re / P;
			float TP = Tr / (1.f - P);

			if (GetRandom(seed0, seed1) < P) { /* R.R. */
				vsmul(throughput, RP, throughput);
				vmul(throughput, throughput, obj->c);

				rassign(currentRay, reflRay);
				continue;
			} else {
				vsmul(throughput, TP, throughput);
				vmul(throughput, throughput, obj->c);

				rinit(currentRay, hitPoint, transDir);
				continue;
			}
		}
	}
}

static void GenerateCameraRay(OCL_CONSTANT_BUFFER const Camera *camera,
		unsigned int *seed0, unsigned int *seed1,
		const int width, const int height, const int x, const int y, Ray *ray) {
	const float invWidth = 1.f / width;
	const float invHeight = 1.f / height;
	const float r1 = GetRandom(seed0, seed1) - .5f;
	const float r2 = GetRandom(seed0, seed1) - .5f;
	const float kcx = (x + r1) * invWidth - .5f;
	const float kcy = (y + r2) * invHeight - .5f;

	Vec rdir;
	vinit(rdir,
			camera->x.x * kcx + camera->y.x * kcy + camera->dir.x,
			camera->x.y * kcx + camera->y.y * kcy + camera->dir.y,
			camera->x.z * kcx + camera->y.z * kcy + camera->dir.z);

	Vec rorig;
	vsmul(rorig, 0.1f, rdir);
	vadd(rorig, rorig, camera->orig)

	vnorm(rdir);
	rinit(*ray, rorig, rdir);
}

__kernel void RadianceGPU(
    __global Vec *colors, __global unsigned int *seedsInput,
	OCL_CONSTANT_BUFFER const Sphere *sphere, OCL_CONSTANT_BUFFER const Camera *camera,
	const unsigned int sphereCount,
	const unsigned int width, const unsigned int height,
	const unsigned int currentSample,
	__global int *pixels,
	const unsigned int workOffset,
	const unsigned int workAmount) {
	const int gid = get_global_id(0);
	// Check if we have to do something
	if (gid >= workAmount)
		return;

	const int scrX = (workOffset + gid) % width;
	const int scrY = (workOffset + gid) / width;

	/* LordCRC: move seed to local store */
	unsigned int seed0 = seedsInput[2 * gid];
	unsigned int seed1 = seedsInput[2 * gid + 1];

	Ray ray;
	GenerateCameraRay(camera, &seed0, &seed1, width, height, scrX, scrY, &ray);

	Vec r;
	Radiance(sphere, sphereCount, &ray, &seed0, &seed1, &r);

	if (currentSample == 0) {
		// Jens's patch for MacOS
		vassign(colors[gid], r);
	} else {
		const float k1 = currentSample;
		const float k2 = 1.f / (currentSample + 1.f);
		colors[gid].x = (colors[gid].x * k1  + r.x) * k2;
		colors[gid].y = (colors[gid].y * k1  + r.y) * k2;
		colors[gid].z = (colors[gid].z * k1  + r.z) * k2;
	}

	pixels[gid] = toInt(colors[gid].x) |
			(toInt(colors[gid].y) << 8) |
			(toInt(colors[gid].z) << 16);

	seedsInput[2 * gid] = seed0;
	seedsInput[2 * gid + 1] = seed1;
}
