#pragma once

#include "box.h"

	box::box(){
		}
	box::~box(){
		}

	void box::inicializar(point vertice){
			min=vertice;
			max=vertice;
		}

	point box::Medio(){
			point middle;
			middle.x=(min.x+max.x)/2;
			middle.y=(min.y+max.y)/2;
			middle.z=(min.z+max.z)/2;

			return middle;

		}

	float box::normalizar(){
			float x,y,z;
			x=max.x-min.x;
			y=max.y-min.y;
			z=max.z-min.z;
			if(x<0) x=x*-1;
			if(y<0)	y=y*-1;
			if(z<0) z=z*-1;

			x = std::max(x,std::max(y,z));
			return x;
		};

	void box::asignar_menor(point p){
			if(min.x>p.x){
				min.x=p.x;
			}
			if(min.y>p.y){
				min.y=p.y;
			}
			if(min.z>p.z){
				min.z=p.z;
			}
		}

	void box::asignar_mayor(point p){
			if(max.x<p.x){
				max.x=p.x;
			}
			if(max.y<p.y){
				max.y=p.y;
			}
			if(max.z<p.z){
				max.z=p.z;
			}
		}
	void box::Pintar(){
			point P1,P2,P3,P4,P5,P6,P7,P8;
			P1=max; P8=min;

			P2.x=P1.x; P2.y=P1.y; P2.z=P8.z;
			P3.x=P8.x; P3.y=P1.y; P3.z=P8.z;
			P4.x=P8.x; P4.y=P1.y; P4.z=P1.z;

			P5.x=P8.x; P5.y=P8.y; P5.z=P1.z;
			P6.x=P1.x; P6.y=P8.y; P6.z=P1.z;
			P7.x=P1.x; P7.y=P8.y; P7.z=P8.z;

			glBegin(GL_LINES);
				P1.Pintar(); P2.Pintar();
				P5.Pintar(); P6.Pintar(); 
				P7.Pintar(); P8.Pintar();
				P3.Pintar(); P4.Pintar();
				P7.Pintar(); P6.Pintar();
				P5.Pintar(); P8.Pintar();
				P1.Pintar(); P4.Pintar();
				P2.Pintar(); P3.Pintar();
				P1.Pintar(); P6.Pintar();
				P2.Pintar(); P7.Pintar();
				P4.Pintar(); P5.Pintar();
				P3.Pintar(); P8.Pintar();

			glEnd();
		}