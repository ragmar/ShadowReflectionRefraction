#pragma once
#include "MapaAltura.h"

MapaAltura::MapaAltura()
{
}

MapaAltura::~MapaAltura()
{
}


void MapaAltura::limpiar() {
	for(int i=0;i<TAMANO;i++)
		for(int j=0;j<TAMANO;j++)
			Matriz[i][j]=0;

	rango_x=LIM_SUP_X+LIM_INF_X;
	rango_z=LIM_SUP_Z+LIM_INF_Z;
}
void MapaAltura::AgregarObjeto(point limInf,point limSup){
	//algoritmo del punto, cheuqear altura para ver si entra en el arreglo
	if(limInf.y>LIM_INF_Y&&limInf.y<LIM_SUP_Y)
	{
		float infx=(limInf.x+LIM_INF_X)*(TAMANO-1)/rango_x;
		float infz=(limInf.z+LIM_INF_Z)*(TAMANO-1)/rango_z;
		float supx=(limSup.x+LIM_INF_X)*(TAMANO-1)/rango_x;
		float supz=(limSup.z+LIM_INF_Z)*(TAMANO-1)/rango_z;
		if(infx>=0&&infz>=0&&supx<TAMANO&&supz<TAMANO){
			bresenham(infx,infz,supx,infz);
			bresenham(infx,supz,supx,supz);
			bresenham(infx,infz,infx,supz);
			bresenham(supx,infz,supx,supz);
			Matriz[(int)infx][(int)infz]=1;
			Matriz[(int)infx][(int)supz]=1;
			Matriz[(int)supx][(int)infz]=1;
			Matriz[(int)supx][(int)supz]=1;
		}
	}
}
	
void MapaAltura::bresenham(float x0,float z0, float x1,float z1)
{
		float dx=abs(x1-x0),dz=abs(z1-z0);
		float x,z;
		int matX,matZ;
		x=x0;z=z0;
		matX=(int)x; matZ=(int)z;
		if(matX-1>=0)
				Matriz[(int)x-1][(int)z]=1;
			if(matZ-1>=0)
				Matriz[(int)x][(int)z-1]=1;
			Matriz[(int)x][(int)z]=1;
		if(matX+1<TAMANO)
				Matriz[(int)x+1][(int)z]=1;
			if(matZ+1<TAMANO)
				Matriz[(int)x][(int)z+1]=1;
		if(dx>=dz){
			float d=dx+2*dz;
			float IncE=-2*dz;
			float IncNE=-2*(dz-dx);

			while(x<x1){
				x=x+0.01f;
				if(d>=0){
					d=d+IncE;
				}else{
					z=z+0.01f;
					d=d+IncNE;
				}
				if(matX-1>=0)
					Matriz[(int)x-1][(int)z]=1;
				if(matZ-1>=0)
					Matriz[(int)x][(int)z-1]=1;
				Matriz[(int)x][(int)z]=1;
				if(matX+1<TAMANO)
					Matriz[(int)x+1][(int)z]=1;
				if(matZ+1<TAMANO)
					Matriz[(int)x][(int)z+1]=1;
			}
		}else{
			float d=dz+2*dx;
			float IncE=-2*dx;
			float IncNE=-2*(dx-dz);
			while(z<z1){
				z=z+0.01f;
				if(d>=0)
					d=d+IncE;
				else{
					x=x+0.01f;
					d=d+IncNE;
				}
				if(matX-1>=0)
					Matriz[(int)x-1][(int)z]=1;
				if(matZ-1>=0)
					Matriz[(int)x][(int)z-1]=1;
				Matriz[(int)x][(int)z]=1;
				if(matX+1<TAMANO)
					Matriz[(int)x+1][(int)z]=1;
				if(matZ+1<TAMANO)
					Matriz[(int)x][(int)z]=1;
			}
		}
}

bool MapaAltura::Colision(float ejeX,float ejeZ){
		float infx,infz;
		infx=(ejeX+LIM_INF_X)*(TAMANO-1)/rango_x;
		infz=(ejeZ+LIM_INF_Z)*(TAMANO-1)/rango_z;
		int x,z;
		x=(int)infx;
		z=(int)infz;

		if(x<0||x>=TAMANO||z<0||z>=TAMANO)
			return false;
		if(Matriz[x][z]!=1)
			return true;
		if(Matriz[x][z]==1)
			return false;

		return false;
}
