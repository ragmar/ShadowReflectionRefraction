#pragma once
#pragma comment (lib,"lib/freeglut.lib")//incluyo al libreria de free glut
#pragma comment (lib,"lib/AntTweakBar.lib")
#pragma comment (lib,"lib/FreeImage.lib")
#pragma comment (lib,"lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/freeglut.h"
#include "../tools/AntTweakBar/AntTweakBar.h"
#include "../tools/FreeImage/FreeImage.h"
#include <string>

using namespace std;




class textura
{
	private:
		GLuint tex;
		string archivo;
	public:
		bool activa;
		textura();
		~textura();
		void Init(string fname);
		void cargarMapaCubico(char *px,char *nx,char *py,char *ny,char *pz,char *nz);
		void CargarTextura();
		void Bind();
		void BindCube(GLuint texind);
		void UnBindCube();
};
