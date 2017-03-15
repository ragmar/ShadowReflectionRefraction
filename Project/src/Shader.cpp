#pragma once
#include "shader.h"

Shader::Shader()
{
	this->vShader = this->fShader = this->program = 0;
	this->vFile = this->fFile = 0;
	activo = false;
}

Shader::~Shader()
{
}

void Shader::createShader(char* vFileName, char* fFileName, string name)
{
	if (GLEW_ARB_vertex_program && GLEW_ARB_fragment_program && glewIsSupported("GL_VERSION_2_0")){
		cout<<"Se puede usar shaders por vertices y por fragmentos"<<endl;
		vShader=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		fShader=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		int verificacion;
		shaderName = name;
		vFile = vFileName;
		fFile = fFileName;
		const char* vcFile = readTextFile(vFile);
		const char* fcFile = readTextFile(fFile);
		glShaderSourceARB(vShader, 1, &vcFile, NULL);
		glShaderSourceARB(fShader, 1, &fcFile, NULL);
		glCompileShaderARB(vShader);
		glCompileShaderARB(fShader);
		glGetObjectParameterivARB(vShader, GL_OBJECT_COMPILE_STATUS_ARB, &verificacion); 
		if(verificacion) cout << "Vertex Shader compilado sin problemas." << endl;
		else cout << "Problemas compilando el Vertex Shader["<<name<<"]." << endl, checkError(vShader);
		
		glGetObjectParameterivARB(fShader, GL_OBJECT_COMPILE_STATUS_ARB, &verificacion); 
		if(verificacion) cout << "Fragment Shader compilado sin problemas." << endl;
		else cout << "Problemas compilando el Fragment Shader ["<<name<<"]." << endl, checkError(fShader);
		createProgram();
		loc = glGetUniformLocationARB(p,"env");
		loc2 = glGetUniformLocationARB(p,"fresnelBias");
		loc3 = glGetUniformLocationARB(p,"fresnelScale");
		loc4 = glGetUniformLocationARB(p,"fresnelPower");
		loc5 = glGetUniformLocationARB(p,"etaRatio");
		loc6 = glGetUniformLocationARB(p,"eyePos");
		glUniform1iARB(loc,0);
		glUniform1fARB(loc2,0.2);
		glUniform1fARB(loc3,0.5);
		glUniform1fARB(loc4,0.1);
		glUniform1fARB(loc5,1.0/1.5);
	}

}

void Shader::destroyShader()
{
	glDetachObjectARB(program,vShader);
	glDetachObjectARB(program,fShader);
	glDeleteObjectARB(program);
}

bool Shader::initShader()
{
	glewInit();
	GLenum err = glewInit();
    if (GLEW_OK != err)
	{
		cout<<"Glew no disponible "<<glewGetErrorString(err)<<endl;
	}
	else
	{
		cout<<"Funciona shaders con la version de Glew: "<<glewGetString(GLEW_VERSION)<<endl;
		return true;
	}
	return false;
 
}

void Shader::createProgram()
{
	program=glCreateProgramObjectARB();
	glAttachObjectARB(program,vShader);
	glAttachObjectARB(program,fShader);

	glLinkProgramARB(program);

	cout<<"   > Shader ready [ "<<shaderName<<" ]!"<<endl;

}

bool Shader::checkError(GLenum shaderType)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetObjectParameterivARB( shaderType, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength );

	enum errorARB typeObj;
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB( shaderType, infologLength, &charsWritten, infoLog);
		if (strcmp(infoLog,"")!=0) 
			cout<<"MSJ: "<<infoLog<<endl;
		free(infoLog);
		return true;
	}
	return false;
}

void Shader::useShader(bool usar)
{
	if(usar)
	{
		glUseProgramObjectARB(program);
	}
	else
	{
		glUseProgramObjectARB(0);
	}
}

char* Shader::readTextFile(char *archivo)
{
	FILE *file;
	char *shaderReaded = NULL;
	int byteCount = 0;

	fopen_s( &file, archivo, "rt" );
	if( file != NULL ){
		fseek(file, 0, SEEK_END);
		byteCount = ftell( file );
		rewind( file );

		if( byteCount ){
			shaderReaded = (char *) malloc( sizeof(char)*( byteCount+1 ) );
			byteCount = (int)fread( shaderReaded, sizeof(char), byteCount, file );
			shaderReaded[ byteCount ] = '\0';
		}
		fclose(file);
	}

	return shaderReaded;
}

/*void Shader::setUniformVec3( Vector3D <float> vector3D, char *name ){

	glUniform3fv( glGetUniformLocation( this->program, name ), 3, vector3D.vec3D ); 
	checkError( this->program, PROGRAM );

}*/

void Shader::setUniformInt( int val, char *name ){

	glUniform1i( glGetUniformLocation( this->program, name ), val );
}

void Shader::setEyePos(point a){
	float b[3];
	b[0]=a.x;
	b[1]=a.y;
	b[2]=a.z;
	glUniform3fvARB(loc6,1,b);
}
