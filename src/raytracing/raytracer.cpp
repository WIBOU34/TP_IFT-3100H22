// IFT3100H22_Raytracer/raytracer.cpp
// Exemple d'un raytracer minimaliste.
// Inspiré de smallpt.
// make : g++ -std=c++11 raytracer.cpp -o raytracer
// run  : ./raytracer image_width image_height ray_per_pixel

#include "raytracer.h"
#include <glm/geometric.hpp>
#include "ofMain.h"
// traitement de la couleur d'un pixel
int Raytracer::format_color_component(double value) {
	// appliquer la correction gamma
	value = pow(clamp(value), gamma_correction);

	// convertir la valeur dans l'espace de couleur
	value = value * 255 + 0.5;

	// conversion numérique de réel vers entier
	return static_cast<int>(value);
}

// fonction qui valide s'il y a intersection entre un rayon et les géométries de la scène
// retourne l'intersection la plus près de la caméra (distance et index de l'élément)
bool Raytracer::raycast(const Ray& ray, double& distance, int& id) {
	// variable temporaire pour la distance d'une intersection entre un rayon et une sphère
	double d;

	// initialiser la distance à une valeur suffisamment éloignée pour qu'on la considère comme l'infinie
	const double infinity = distance = 1e20;

	// nombre d'éléments dans la scène
	const int n = static_cast<int>(Raytracer::scene.size());

	// parcourir tous les éléments de la scène
	for (int index = 0; index < n; ++index) {
		// test d'intersection entre le rayon et la géométrie à cet index
		d = Raytracer::scene[index]->intersect(ray);

		// valider s'il y a eu intersection et si la distance est inférieure aux autres intersections
		if (d != 0 && d < distance) {
			// nouvelle valeur courante de la distance et l'index de l'intersection la plus rapprochée de la caméra
			distance = d;
			id = index;
		}
	}

	// il y a eu intersection si la distance est plus petite que l'infini
	if (distance < infinity)
		return true;
	else
		return false;
}

// fonction récursive qui calcule la radiance
Vector Raytracer::compute_radiance(const Ray& ray, int depth) {
	// valeur de la radiance
	Vector radiance;

	// distance de l'intersection
	double distance;

	// identifiant de la géométrie en intersection
	int id = 0;

	// valider s'il n'y a pas intersection
	if (!raycast(ray, distance, id))
		return Vector(); // couleur par défault (noir)

	// référence sur une géométrie en intersection avec un rayon
	const Primitive& obj = *Raytracer::scene[id];

	// calculer les coordonnées du point d'intersection
	const Vector x = ray.origin + ray.direction * distance;

	// calculer la normale au point d'intersection
	const Vector n = (x - obj.position).normalize();

	// ajustement de la direction de la normale
	const Vector na = n.dot(ray.direction) < 0 ? n : n * -1;

	// isoler la composante de couleur la plus puissante
	Vector f = obj.color;
	double threshold = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

	// valider si la limite du nombre de récursions est atteinte
	if (++depth > max_depth) {
		// test de probabilité
		//if (doubleRand(0.0, 1.0) < threshold)
		//	f = f * (1 / threshold);
		//else
		return obj.emission;
	}

	if (obj.material == SurfaceType::diffuse) {
		// matériau avec réflexion diffuse

		const double r1 = 2 * M_PI * doubleRand(0.0, 1.0);
		const double r2 = doubleRand(0.0, 1.0);
		const double r2s = sqrt(r2);

		const Vector w = na;
		const Vector u = ((fabs(w.x) > 0.1 ? Vector(0, 1) : Vector(1)).cross(w)).normalize();
		const Vector v = w.cross(u);
		const Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();

		radiance = obj.emission + f.multiply(compute_radiance(Ray(x, d), depth));

		return radiance;
	} else if (obj.material == SurfaceType::specular) {
		// matériau avec réflexion spéculaire

		//Vector temp = glm::reflect<Vector>(ray.direction, n);
		glm::dvec3 temp = glm::reflect<3, double, glm::qualifier::defaultp>(glm::dvec3(ray.direction.x, ray.direction.y, ray.direction.z), glm::dvec3(n.x, n.y, n.z));
		radiance = obj.emission + f.multiply(compute_radiance(Ray(x, Vector(temp.x, temp.y, temp.z)), depth));
		//radiance = obj.emission + f.multiply(compute_radiance(Ray(x, ray.direction - n * 2.0 * n.dot(ray.direction)), depth));

		return radiance;
	} else if (obj.material == SurfaceType::refraction) {

		//const bool into = n.dot(na) > 0;
		////const double ior = 1.5; // indice de réfraction du verre
		//const double ior = 1.5; // indice de réfraction du verre
		//const double nc = 1.0;
		//const double nt = ior;
		//const double nnt = into ? nc / nt : nt / nc;

		//glm::dvec3 temp = glm::refract<3, double, glm::qualifier::defaultp>(glm::dvec3(ray.direction.x, ray.direction.y, ray.direction.z), glm::dvec3(n.x, n.y, n.z), nnt);
		//Vector tdir = Vector(temp.x, temp.y, temp.z);
		////if (temp.x == 0 && temp.y == 0 && temp.z == 0) {
		//radiance = obj.emission + f.multiply(compute_radiance(Ray(x, tdir), depth));

		//return radiance;
		//} else {
			// effet de fresnel
		//	const double ddn = ray.direction.dot(na);
		//	const double a = nt - nc;
		//	const double b = nt + nc;
		//	const double r0 = a * a / (b * b);
		//	const double c = 1.0 - (into ? -ddn : tdir.dot(n));
		//	const double re = r0 + (1.0 - r0) * c * c * c * c * c;
		//	const double tr = 1 - re;
		//	const double p = 0.25 + 0.5 * re;
		//	const double rp = re / p;
		//	const double tp = tr / (1.0 - p);

		//	radiance = obj.emission + f.multiply(depth > 2 ? compute_radiance(Ray(x, tdir), depth) * tp : compute_radiance(Ray(x, tdir), depth) * tr);
		//}
		//radiance = obj.emission + f.multiply(depth > 2 ? compute_radiance(Ray(x, Vector(temp.x, temp.y, temp.z)), depth) : );
		// matériau avec réflexion réfraction

		Ray reflection_ray(x, ray.direction - n * 2.0 * n.dot(ray.direction));

		const bool into = n.dot(na) > 0;

		const double ior = 1.5; // indice de réfraction du verre
		const double nc = 1.0;
		const double nt = ior;
		const double nnt = into ? nc / nt : nt / nc;
		const double ddn = ray.direction.dot(na);
		double cos2t;

		glm::dvec3 temp = glm::refract<3, double, glm::qualifier::defaultp>(glm::dvec3(ray.direction.x, ray.direction.y, ray.direction.z), glm::dvec3(n.x, n.y, n.z), ior);

		if ((cos2t = 1.0 - nnt * nnt * (1.0 - ddn * ddn)) < 0.0) {
			radiance = obj.emission + f.multiply(compute_radiance(reflection_ray, depth));

			return radiance;
		}

		Vector tdir = (ray.direction * nnt - n * ((into ? 1.0 : -1.0) * (ddn * nnt + sqrt(cos2t)))).normalize();

		// effet de fresnel
		const double a = nt - nc;
		const double b = nt + nc;
		const double r0 = a * a / (b * b);
		const double c = 1.0 - (into ? -ddn : tdir.dot(n));
		const double re = r0 + (1.0 - r0) * c * c * c * c * c;
		const double tr = 1 - re;
		const double p = 0.25 + 0.5 * re;
		const double rp = re / p;
		const double tp = tr / (1.0 - p);

		radiance = obj.emission + f.multiply(depth > 2 ? (doubleRand(0.0, 1.0) < p ?
			compute_radiance(reflection_ray, depth) * rp : compute_radiance(Ray(x, tdir), depth) * tp) :
			compute_radiance(reflection_ray, depth) * re + compute_radiance(Ray(x, tdir), depth) * tr);

		return radiance;
	} else {
		return radiance;
	}
}

// fonction qui permet de sauvegarder des pixels dans un fichier image (format .ppm)
void Raytracer::save_image_file(int width, int height, int ray_per_pixel, const Vector* pixel) {
	// nom du fichier image de type .ppm (portable pixmap)
	std::stringstream ss;
	ss << "image" << width << "x" << height << "_" << ray_per_pixel << ".ppm";
	std::string filename = ss.str();

	// déclaration et ouverture du fichier en mode écriture
	std::ofstream file;
	file.open(filename, std::ios::out);

	// entête du ficher pour une image avec un espace de couleur RGB 24 bits (P3 pour pixmap)
	file << "P3\n";

	// largeur et hauteur de l'image sur la seconde ligne de l'entête
	file << width << ' ' << height << '\n';

	// valeur maximale de l'espace de couleur sur la troisième ligne de l'entête
	file << "255\n";

	// écriture des pixels dans le fichier image
	for (int index = 0; index < width * height; ++index) {
		// écrire la couleur du pixel dans le fichier image
		file << static_cast<std::uint32_t>(format_color_component(pixel[index].x)) << " ";
		file << static_cast<std::uint32_t>(format_color_component(pixel[index].y)) << " ";
		file << static_cast<std::uint32_t>(format_color_component(pixel[index].z)) << " ";
	}

	// fermeture du fichier
	file.close();
}

// fonction d'initialisation de la scène
void Raytracer::init() {
	std::cout << "scene setup" << std::endl;

	//constexpr double anchor = 1e5;
	//constexpr double wall_radius = anchor;

	//constexpr double box_size_x = 100.0;
	//constexpr double box_size_y = 81.6;
	//constexpr double box_size_z = 81.6;

	//constexpr double box_x_min = 0.0;
	//constexpr double box_x_max = box_size_x;
	//constexpr double box_y_min = 0.0;
	//constexpr double box_y_max = box_size_y;
	//constexpr double box_z_min = 0.0;
	//constexpr double box_z_max = box_size_z;

	//constexpr double box_center_x = (box_x_max - box_x_min) / 2.0;
	//constexpr double box_center_y = (box_y_max - box_y_min) / 2.0;
	//constexpr double box_center_z = (box_z_max - box_z_min) / 2.0;

	//// vider la scène de son contenu
	//Raytracer::scene.clear();

	//// génération du contenu de la scène
	//Raytracer::scene.insert(Raytracer::scene.begin(), {

	//	// approximation d'une boîte de Cornell avec des sphères surdimensionnées qui simulent des surfaces planes
	//	Sphere(wall_radius, Vector(box_center_x,  anchor,            box_size_z),   Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse),    // plancher
	//	Sphere(wall_radius, Vector(box_center_x, -anchor + box_size_y, box_size_z),   Vector(), Vector(0.75, 0.75, 0.75), SurfaceType::diffuse),    // plafond
	//	Sphere(wall_radius, Vector(anchor + 1,      box_center_y,      box_size_z),   Vector(), Vector(0.75, 0.25, 0.25), SurfaceType::diffuse),    // mur gauche
	//	Sphere(wall_radius, Vector(box_center_x,  box_center_y,      anchor),       Vector(), Vector(0.25, 0.75, 0.25), SurfaceType::diffuse),    // mur arrière
	//	Sphere(wall_radius, Vector(-anchor + 99,     box_center_y,      box_size_z),   Vector(), Vector(0.25, 0.25, 0.75), SurfaceType::diffuse),    // mur droit
	//	Sphere(wall_radius, Vector(box_center_x,  box_center_y,     -anchor + 170),   Vector(), Vector(0.0,  0.0,  0.0),  SurfaceType::diffuse),    // mur avant

	//	// ensemble des sphères situées à l'intérieur de la boîte de Cornell
	//	Sphere(22.5,        Vector(30,            30,                40),           Vector(), Vector(1.0, 1.0, 1.0),    SurfaceType::specular),   // sphère mirroir
	//	Sphere(17.5,        Vector(75,            40,                75),           Vector(), Vector(1.0, 1.0, 1.0),    SurfaceType::refraction), // sphère de verre

	//	Sphere(600,  Vector(box_center_x, 600.0 + box_size_z - 0.27, box_size_z), Vector(15, 15, 15), Vector(0.0, 0.0, 0.0), SurfaceType::diffuse) // sphère lumineuse
	//	});

	// allocation de la mémoire de l'image en fonction des paramètres du programme
	Raytracer::image.resize(Raytracer::image_width, Raytracer::image_height);

	std::cout << "image resize to " << Raytracer::image.width << "x" << Raytracer::image.height << " (" << Raytracer::image.count << " pixels, " << Raytracer::image.size << " MB)" << std::endl;

	// calibration de la caméra
	Raytracer::camera = CameraRaytracing(camera_position, camera_orientation);
	//Raytracer::camera.position = camera_position;
	//Raytracer::camera.orientation = camera_orientation;
	Raytracer::camera.viewport_width = Raytracer::image.width;
	Raytracer::camera.viewport_height = Raytracer::image.height;
	Raytracer::camera.fov = camera_fov;
	Raytracer::camera.calibrate();
}

// fonction appelée quand le rendu de la scène est terminée
void Raytracer::post_render() {
	// écrire les pixels dans un fichier image
	save_image_file(Raytracer::image.width, Raytracer::image.height, Raytracer::ray_per_pixel, Raytracer::image.pixel);

	std::cout << "raytracer task is done" << std::endl;
}

HANDLE imageMutex;
// fonction de rendu de la scène
void Raytracer::render() {
	std::cout << "render start" << std::endl;

	Raytracer::progression = 0.0f;

	imageMutex = CreateMutex(NULL, FALSE, NULL);
	// itération sur les rangées de pixels
	std::vector<HANDLE> vecThreads = std::vector<HANDLE>(Raytracer::N_THREADS);
	for (int iThread = 0; iThread < Raytracer::N_THREADS; ++iThread) {
		vecThreads.at(iThread) = (HANDLE)_beginthread(calcRayons, 0, (void*)iThread);
	}
	WaitForMultipleObjects(N_THREADS, &vecThreads.at(0), TRUE, INFINITE);
	CloseHandle(imageMutex);

	std::cout << "\npixels hauteur completes: " << progression << "/" << image_height << std::endl;

	std::cout << "\nrender done" << std::endl;
}

void Raytracer::calcRayons(void* param) {
	//ParamThread paramThread = (ParamThread)param;
	// initialiser la radiance
	const int iThread = (int)param;

	unsigned short x = 0;

	int index, y, s, sx, sy = 0;

	double r1, r2 = 0.0;
	double dx, dy = 0.0;

	Vector radiance;

	Vector distance;
	const int nbrElementsCompleter = (std::ceil((double)Raytracer::image_height / Raytracer::N_THREADS));
	const int nbrMax = (nbrElementsCompleter * (iThread + 1) >= image_height) ? image_height : (nbrElementsCompleter * (iThread + 1));
	//printf("thread %d completant les colonnes %d a %d \n", iThread, nbrElementsCompleter * iThread, nbrMax);
	//std::cout << "thread " << iThread << " completant pixel " << nbrElementsCompleter * iThread << " a " << nbrElementsCompleter * (iThread + 1) << std::endl;
	// itération sur les rangées de pixels
	for (y = nbrElementsCompleter * iThread; y < nbrMax; ++y) {
		progression++;
		// calculer et afficher le pourcentage de progression du rendu dans la console
		fprintf(stderr, "\rraytracing (%d rays per pixel) : %4.1f %%", ray_per_pixel, progression * 100.0f / image_height);
		// itération sur les colonnes de pixels
		for (x = 0; x < image_width; ++x) {
			// déterminer l'index du pixel
			index = (image_height - y - 1) * image_width + x;

			// itération sur les rangées du bloc de 2x2 échantillons
			for (sy = 0; sy < 2; ++sy) {
				// itération sur les colonnes du bloc de 2x2 échantillons
				for (sx = 0; sx < 2; ++sx) {
					// initialiser la radiance
					radiance = Vector();

					// itération des sur les rayons par pixel
					for (s = 0; s < ray_per_pixel; ++s) {
						// filtre de la tente
						r1 = 2.0 * doubleRand(0.0, 1.0);
						dx = r1 < 1.0 ? sqrt(r1) - 1.0 : 1.0 - sqrt(2.0 - r1);

						r2 = 2.0 * doubleRand(0.0, 1.0);
						dy = r2 < 1.0 ? sqrt(r2) - 1.0 : 1.0 - sqrt(2.0 - r2);

						// calculer la distance de l'échantillon
						distance = camera.axis_x * (((sx + 0.5 + dx) / 2.0 + x) / image_width - 0.5) +
							camera.axis_y * (((sy + 0.5 + dy) / 2.0 + y) / image_height - 0.5) + camera.axis_z;

						// appel récursif du calcul de la radiance
						radiance = radiance + compute_radiance(Ray(camera.position + distance * 140, distance.normalize()), 0) * (1.0 / ray_per_pixel);
					}
					WaitForSingleObject(imageMutex, INFINITE);
					Raytracer::image.pixel[index] = Raytracer::image.pixel[index] + Vector(clamp(radiance.x), clamp(radiance.y), clamp(radiance.z)) * 0.25;
					ReleaseMutex(imageMutex);
				}
			}
		}
	}
	_endthread();
}

double Raytracer::doubleRand(const double& min, const double& max) {
	static thread_local std::mt19937 generator;
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(generator);
}

// fonction de mise à jour
void Raytracer::update() {
	auto t1 = std::chrono::high_resolution_clock::now();

	// rendu de la scène
	render();

	auto t2 = std::chrono::high_resolution_clock::now();

	// temps de calcul
	auto render_time = t2 - t1;

	std::cout << "render time : " << std::chrono::duration_cast<std::chrono::milliseconds>(render_time).count() / 1000.0f << "s" << std::endl;
}

// fonction principale du programme
// 1 arg: image_width
// 2 arg: image_width, image_height
// 3 arg: image_width, image_height, ray_per_pixel
int Raytracer::raytracerExecute(const int& imageWidth, const int& imageHeight, const int& rayPerPixel, const int& nbrThreads) {
	std::cout << "raytracer start" << std::endl;

	std::cout << "parse program arguments:" << std::endl;

	if (nbrThreads <= 0) {
		Raytracer::N_THREADS = 1;
	} else {
		Raytracer::N_THREADS = nbrThreads;
	}

	std::cout << "nbrThreads:\t" << Raytracer::N_THREADS << std::endl;

	std::cout << "arg[0]:\t" << imageWidth << std::endl;
	std::cout << "arg[1]:\t" << imageHeight << std::endl;
	std::cout << "arg[2]:\t" << rayPerPixel << std::endl;

	Raytracer::image_width = imageWidth;
	Raytracer::image_height = imageHeight;
	Raytracer::ray_per_pixel = rayPerPixel;

	std::cout << "program init done" << std::endl;

	//Cube cube = Cube(Vector(0,0,0), Vector(5,5,5), Vector(0.5,0.5,0.5), Vector(0.5,0.5,0.5), SurfaceType::diffuse);
	//Ray ray = Ray(Vector(5,5,5), Vector(9,9,9));
	//cube.intersect(ray);
	// initialisation de la scène
	init();

	// faire un rendu de la scène par lancer de rayons
	update();

	// procédure post-rendu (sauvegarde de l'image et désallocation de la mémoire)
	post_render();

	return 0;
}

//void Raytracer::setupCamera(const Vector& camPosition, const Vector& camOrientation, const double& fov, const Vector& axisX, const Vector& axisY, const Vector& axisZ) {
//	this->camera_position = camPosition;
//	this->camera_orientation = camOrientation;
//	this->camera_fov = fov;
//	Raytracer::camera.axis_x = axisX;
//	Raytracer::camera.axis_y = axisY;
//	Raytracer::camera.axis_z = axisZ;
//
//}

int Raytracer::image_width = 0;
int Raytracer::image_height = 0;
int Raytracer::ray_per_pixel = 0;
int Raytracer::pixel_count = 0;
float Raytracer::progression = 0;
unsigned int Raytracer::N_THREADS = 0;
ImageRaytracing Raytracer::image = ImageRaytracing();
CameraRaytracing Raytracer::camera = CameraRaytracing(Vector(), Vector());
std::vector<Primitive*> Raytracer::scene = std::vector<Primitive*>();
//HANDLE Raytracer::imageMutex = CreateMutex(NULL, FALSE, NULL);

