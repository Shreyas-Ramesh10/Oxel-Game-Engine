#include "Shader.h"
#include <string>

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

//Reading from file.
//--------------------------------------------------------------------------------------------------------------------------------
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) 
{
	CompileShader(vertexCode, fragmentCode);
};

void Shader::CreateFromFile(const char* vertexFileLocation, const char* fragmentFileLocation)
{
	std::string VertexString = ReadFile(vertexFileLocation);
	std::string FragmentString = ReadFile(fragmentFileLocation);

	const char *vertexCode = VertexString.c_str();
	const char *fragmentCode = FragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
};

std::string Shader::ReadFile(const char* fileLocation) 
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Could not read file: %s. File does not exist.\n", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
};

//--------------------------------------------------------------------------------------------------------------------------------
//Reading from file code ends here.


void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) 
{
	//We are creating a shader program and glCreateProgram will return the ID of the shader program created and we are storing that in shader variable
	shaderID = glCreateProgram();

	//If the shader program is not created properly, it will return 0
	if (!shaderID) {
		printf("Error creating the shader program \n");
		return;
	};

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };// Error log for the shader program, will initiate with 0

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); //

	//If the shader program is not linked properly, it will return 0
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("ERROR LINKING PROGRAM: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("ERROR VALIDATING PROGRAM: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");//Will use the uniform location to pass the model matrix to the shader
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");//Binding to directinoalLight. struct's color
	uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.color");
};

GLuint Shader::GetProjectionLocation() 
{	
	return uniformProjection;
};
GLuint Shader::GetModelLocation() 
{	
	return uniformModel;
};
GLuint Shader::GetViewLocation()
{
	return uniformView;
};
GLuint Shader::GetAmbientColorLocation()
{
	return uniformAmbientColor;
};
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformAmbientIntensity;
};


void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("ERROR COMPILING %d SHADER: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
};

Shader::~Shader()
{
	ClearShader();
}
