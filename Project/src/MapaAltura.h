#pragma once

#include <iostream>
#include "point.h"
using namespace std;

#define TAMANO 1500
#define LIM_SUP_Z 796.0f
#define LIM_SUP_X 796.0f
#define LIM_INF_Z 796.0f
#define LIM_INF_X 796.0f
#define LIM_INF_Y -6.0f
#define LIM_SUP_Y 5.0f

class MapaAltura
{
	public:
		int i;
		float rango_x,rango_z;
		int aux;
		short Pos_jugadores[8][2];
		bool Matriz[TAMANO][TAMANO];
		MapaAltura();
		~MapaAltura();
		void limpiar();
		void bresenham(float x0,float z0, float x1,float z1);
		void AgregarObjeto(point limInf,point limSup);
		bool Colision(float ejeX, float ejeZ);
};
