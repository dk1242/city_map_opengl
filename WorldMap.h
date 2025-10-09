#ifndef WORLDMAP_CLASS_H
#define WORLDMAP_CLASS_H

#include "Streets.h"
#include "Buildings.h"

class WorldMap {
public:
	double min_x = std::numeric_limits<double>::max();
	double max_x = std::numeric_limits<double>::lowest();
	double min_y = std::numeric_limits<double>::max();
	double max_y = std::numeric_limits<double>::lowest();

	std::map<long long, glm::dvec3>nodeVerticesMap;

	Streets* streetsObj;
	Buildings* buildingsObj;

	WorldMap(json& map_data);

	void Draw();
};

#endif