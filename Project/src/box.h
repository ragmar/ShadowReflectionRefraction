#pragma once
#include "point.h"
#include <algorithm>

class box
{
public:
	point min;
	point max;
	box();
	~box();
	void inicializar(point vertice);
	point Medio();
	float normalizar();
	void asignar_menor(point p);
	void asignar_mayor(point p);
	void Pintar();

};