#pragma once
#pragma comment (lib,"lib/freeglut.lib")
#pragma comment (lib,"lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/freeglut.h"
#include <iostream>
#include <string>
#include "point.h"

using namespace std;

class Shader
{
	public:
		bool activo;
		GLhandleARB program;
		Shader();
		~Shader();
		void createShader(char* vFileName,char* fFileName, string name);
		void destroyShader();
		void useShader(bool usar);
		bool initShader();
		void setUniformInt( int val, char *name);
		void Shader::setEyePos(point a);

	private:
		string shaderName;
		GLhandleARB p,vShader,fShader;
		char* vFile;
		char* fFile;
		void createProgram();
		bool checkError(GLenum shaderType);
		char* readTextFile( char *archivo );
		int loc,loc2,loc3,loc4,loc5,loc6,locatrr;

};
