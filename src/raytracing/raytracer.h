#pragma once
// IFT3100H22_Raytracer/raytracer.cpp
// Exemple d'un raytracer minimaliste.
// Inspiré de smallpt.
// make : g++ -std=c++11 raytracer.cpp -o raytracer
// run  : ./raytracer image_width image_height ray_per_pixel

// c
#include <cmath>    // sqrt pow fabs cos sin
#include "corecrt_math_defines.h"

// c++
#include <iostream> // std cout endl
#include <sstream>  // stringstream
#include <fstream>

// c++11
#include <vector>
#include <random>
#include <chrono>

#include <process.h>
#include <windows.h>

#include <limits>

#include <ofMath.h>

//namespace Raytracing {

// types de matériau
enum class SurfaceType { diffuse, specular, refraction };

// type vecteur 3D
struct Vector {
	double x, y, z;

	// construction
	constexpr Vector() : x(0.0), y(0.0), z(0.0) {}
	constexpr Vector(double x) : x(x), y(0.0), z(0.0) {}
	constexpr Vector(double x, double y) : x(x), y(y), z(0.0) {}
	constexpr Vector(double x, double y, double z) : x(x), y(y), z(z) {}

	// produit scalaire (dot product)
	double dot(const Vector& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	// produit vectoriel (cross product)
	Vector cross(const Vector& v) const {
		return Vector(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	// multiplication vectorielle
	Vector multiply(const Vector& v) const {
		return Vector(
			x * v.x,
			y * v.y,
			z * v.z);
	}

	// multiplication scalaire
	Vector operator*(double s) const {
		return Vector(
			x * s,
			y * s,
			z * s);
	}

	// addition vectorielle
	Vector operator+(const Vector& v) const {
		return Vector(
			x + v.x,
			y + v.y,
			z + v.z);
	}

	// soustraction vectorielle
	Vector operator-(const Vector& v) const {
		return Vector(
			x - v.x,
			y - v.y,
			z - v.z);
	}

	// normalisation
	Vector& normalize() {
		return *this = *this * (1.0 / sqrt(x * x + y * y + z * z));
	}

	//bool operator< (const Vector& other) const {
	//	return this->x < other.x
	//		|| this->y < other.y
	//		|| this->z < other.z;
	//}
	//bool operator> (const Vector& other) const {
	//	return this->x > other.x
	//		|| this->y > other.y
	//		|| this->z > other.z;
	//}

	double operator[] (const int& index) const {
		switch (index) {
		case 0:
			return this->x;
			break;
		case 1:
			return this->y;
			break;
		case 2:
			return this->z;
			break;
		}
		return 0;
	}
};

// structure d'un rayon sous forme de droite paramétrique
// équation d'un rayon : p(t) = o + td
struct RayCpu {
	Vector origin;
	Vector direction;
	Vector invDir;
	int sign[3];

	RayCpu(Vector o, Vector d) : origin(o), direction(d) {
		this->invDir = Vector(1 / direction.x, 1 / direction.y, 1 / direction.z);
		sign[0] = (invDir.x < 0);
		sign[1] = (invDir.y < 0);
		sign[2] = (invDir.z < 0);
	}
};

struct PrimitiveCpu {
public:
	Vector position;
	Vector emission; // couleur émissive du cube
	Vector color;    // couleur diffuse du cube
	SurfaceType material; // type de réflexion du cube
	virtual double intersect(const RayCpu& ray) const = 0;
};

struct CylinderCpu : public PrimitiveCpu {
	Vector dimensions;

	CylinderCpu(Vector position, Vector dimensions, Vector e, Vector c, SurfaceType m) {
		this->emission = e;
		this->color = c;
		this->material = m;
		this->position = position;
		this->dimensions = dimensions;
	}

	double intersect(const RayCpu& ray) const {

	}
};

struct CubeCpu : public  PrimitiveCpu {
	//Vector position; // point du cube bas gauche avant
	Vector dimensions; // width, height, depth
	//Vector emission; // couleur émissive du cube
	//Vector color;    // couleur diffuse du cube

	//SurfaceType material; // type de réflexion du cube

	//Vector pHautDroitAvant;
	//Vector pBasGaucheAvant;
	//Vector pHautGaucheAvant;
	//Vector pBasDroitAvant;
	//Vector pBasGaucheArriere;
	//Vector pHautDroitArriere;
	//Vector pBasDroitArriere;
	//Vector pHautGaucheArriere;
	Vector bounds[2];

	CubeCpu(Vector position, Vector dimensions, Vector e, Vector c, SurfaceType m) {
		this->emission = e;
		this->color = c;
		this->material = m;
		this->position = position;
		this->dimensions = dimensions;
		bounds[0] = position;
		bounds[1] = positionHautDroitArriere();

		//this->pHautDroitAvant = positionHautDroitAvant();
		//this->pBasGaucheAvant = positionBasGaucheAvant();
		//this->pHautGaucheAvant = positionHautGaucheAvant();
		//this->pBasDroitAvant = positionBasDroitAvant();

		//this->pBasGaucheArriere = positionBasGaucheArriere();
		//this->pHautDroitArriere = positionHautDroitArriere();
		//this->pBasDroitArriere = positionBasDroitArriere();
		//this->pHautGaucheArriere = positionHautGaucheArriere();
	}

	double intersect(const RayCpu& ray) const {
		double tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
		tmax = (bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
		tymin = (bounds[ray.sign[1]].y - ray.origin.y) * ray.invDir.y;
		tymax = (bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.invDir.y;

		if ((tmin > tymax) || (tymin > tmax))
			return 0;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[ray.sign[2]].z - ray.origin.z) * ray.invDir.z;
		tzmax = (bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.invDir.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return 0;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		double distance = tmin;

		if (distance < 0) {
			distance = tmax;
			if (distance < 0) return 0;
		}

		return distance;


		//const double Sx = ray.origin.x;
		//const double Rx = dimensions.x;
		//const double Dx = ray.direction.x;
		//const Vector pointA = position;
		//const Vector pointB = positionHautDroitAvant();
		//const Vector pointC = positionBasDroitAvant();
		//const Vector AB = Vector(pointB.x - pointA.x, pointB.y - pointA.y, pointB.z - pointA.z);
		//const Vector AC = Vector(pointC.x - pointA.x, pointC.y - pointA.y, pointC.z - pointA.z);
		//Vector normal = AB.cross(AC).normalize();

		//double prodScalaire = ray.direction.dot(normal);
		//if (prodScalaire >= 0) {
		//	return 0;
		//} else {
		//	double t = (Rx - Sx) / Dx;
		//}
	}

	double dist(const Vector& point1, const Vector& point2) const {
		return ofDist(point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
	}
	// Avant
	Vector positionBasGaucheAvant() const {
		return position;
	}
	Vector positionHautDroitAvant() const {
		return Vector(
			position.x + dimensions.x,
			position.y + dimensions.y,
			position.z);
	}
	Vector positionHautGaucheAvant() const {
		return Vector(
			position.x,
			position.y + dimensions.y,
			position.z);
	}
	Vector positionBasDroitAvant() const {
		return Vector(
			position.x + dimensions.x,
			position.y,
			position.z);
	}
	// Arriere
	Vector positionBasGaucheArriere() const {
		return Vector(
			position.x,
			position.y,
			position.z + dimensions.z);
	}
	Vector positionHautDroitArriere() const {
		return Vector(
			position.x + dimensions.x,
			position.y + dimensions.y,
			position.z + dimensions.z);
	}
	Vector positionBasDroitArriere() const {
		return Vector(
			position.x + dimensions.x,
			position.y,
			position.z + dimensions.z);
	}
	Vector positionHautGaucheArriere() const {
		return Vector(
			position.x,
			position.y + dimensions.y,
			position.z + dimensions.z);
	}

};

// type Sphere sous forme d'un point central et d'un rayon
struct SphereCpu : public PrimitiveCpu {
	double radius;   // rayon de la sphère
	//Vector position; // position du centre de la sphère
	//Vector emission; // couleur émissive de la sphère
	//Vector color;    // couleur diffuse de la sphère

	//SurfaceType material; // type de réflexion de la sphère

	// constructeur
	//Sphere(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r), position(p), emission(e), color(c), material(m) {}

	SphereCpu(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r) {
		this->emission = e;
		this->color = c;
		this->material = m;
		this->position = p;
	}

	// fonction d'intersection entre la sphère et un rayon
	double intersect(const RayCpu& ray) const {
		// distance de l'intersection la plus près si elle existe
		double distance;

		// seuil de tolérance numérique du test d'intersection
		double epsilon = 1e-4;

		// distance du point d'intersection
		double t;

		// vecteur entre le centre de la sphère et l'origine du rayon
		Vector delta = position - ray.origin;

		// calculer a
		double a = delta.dot(delta);

		// calculer b
		double b = delta.dot(ray.direction);

		// calculer c
		double c = radius * radius;

		// calculer le discriminant de l'équation quadratique
		double discriminant = b * b - a + c;

		// valider si le discriminant est négatif
		if (discriminant < 0) {
			// il n'y a pas d'intersection avec cette sphère
			return distance = 0;
		}

		// calculer la racine carrée du discriminant seulement si non négatif
		discriminant = sqrt(discriminant);

		// déterminer la distance de la première intersection
		t = b - discriminant;

		// valider si la distance de la première intersection est dans le seuil de tolérance
		if (t > epsilon)
			distance = t;
		else {
			// déterminer la distance de la première intersection
			t = b + discriminant;

			// valider si la distance de la seconde intersection est dans le seuil de tolérance
			if (t > epsilon)
				distance = t;
			else
				distance = 0;
		}

		// retourner la distance du point d'intersection
		return distance;
	}
};

// structure d'une caméra
struct CameraRaytracing {
	Vector position;
	Vector orientation;
	Vector target;

	Vector axis_x;
	Vector axis_y;
	Vector axis_z;

	Vector up;

	float viewport_width;
	float viewport_height;

	float fov;

	CameraRaytracing(Vector p, Vector o) : position(p), orientation(o) {}

	// fonction qui permet de calibrer la caméra en fonction la valeur courante de ses attributs
	void calibrate() {
		axis_z = orientation;
		axis_x = Vector(viewport_width * fov / viewport_height);
		axis_y = axis_x.cross(axis_z).normalize() * fov;
	}
};

// structure d'une image
struct ImageRaytracing {
	int width;  // largeur de l'image
	int height; // hauteur de l'image
	int count;  // nombre de pixels
	int index;  // index d'un des pixels

	double size; // poids en mémoire (en mégaoctets)

	// pointeur vers une séquence de pixels sous forme vectorielle
	Vector* pixel;

	// constructeurs
	ImageRaytracing() : width(1), height(1) {}
	ImageRaytracing(int w) : width(w), height(w) {}
	ImageRaytracing(int w, int h) : width(w), height(h) {}

	// fonction qui redimensionne l'image
	void resize(int w, int h) {
		if (w > 0 && h > 0) {
			width = w;
			height = h;

			// calculer le nombre de pixels dans l'image
			count = width * height;

			// calculer la taille de l'espace mémoire (en mégaoctets)
			size = sizeof(Vector) * count / pow(1024.0, 2);

			// allocation de la mémoire pour les pixels de l'image
			pixel = new Vector[count];
		}
	}

	~ImageRaytracing() {
		if (pixel != nullptr)
			delete[] pixel;
	}
};

class Raytracer {
public:
	// paramètres du programme
	static const int max_depth = 5;
	const double camera_fov = 0.5135;
	const double gamma_correction = 1 / 2.2;

	// variables du programme
	static int image_width;
	static int image_height;
	static int ray_per_pixel;
	static int pixel_count;

	// variables globales

	// déclaration du graphe de scène
	static std::vector<PrimitiveCpu*> scene;

	// orientation initiale de la caméra
	Vector camera_orientation = (Vector(0, -0.042612, -1).normalize());

	// position initiale de la caméra
	const Vector camera_position = Vector(50, 50, 300);

	// déclaration de la caméra utilisée pour rendre la scène
	static CameraRaytracing camera;

	// fonction principale du programme
	int raytracerExecute(const int& imageWidth, const int& imageHeight, const int& rayPerPixel, const int& nbrThreads);
	//int raytracerExecute(int argc, char* argv[]);
	//int main(int argc, char* argv[]);
	//void setupCamera(const Vector& camPosition, const Vector& camOrientation, const double& fov, const Vector& axisX, const Vector& axisY, const Vector& axisZ);

private:

	static unsigned int N_THREADS;
	static float progression;

	// déclaration d'une image
	static ImageRaytracing image;

	// fonction pour borner une valeur numérique entre 0 et 1
	static double clamp(double x) {
		return x < 0 ? 0 : x > 1 ? 1 : x;
	}

	// traitement de la couleur d'un pixel
	int format_color_component(double value);

	// fonction qui valide s'il y a intersection entre un rayon et les géométries de la scène
	// retourne l'intersection la plus près de la caméra (distance et index de l'élément)
	static bool raycast(const RayCpu& ray, double& distance, int& id);

	// fonction récursive qui calcule la radiance
	static Vector compute_radiance(const RayCpu& ray, int depth);

	// fonction qui permet de sauvegarder des pixels dans un fichier image (format .ppm)
	void save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel);

	// fonction d'initialisation de la scène
	void init();

	// fonction appelée quand le rendu de la scène est terminée
	void post_render();

	// fonction de rendu de la scène
	void render();

	// fonction de mise à jour
	void update();

	static void calcRayons(void* param);

	static double doubleRand(const double& min, const double& max);
};
//}
