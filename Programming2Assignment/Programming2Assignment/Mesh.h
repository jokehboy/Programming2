#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;

};

class Mesh
{

public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<struct Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<struct Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		this->SetupMesh();


	}

	void Draw(Shader shader)
	{
		GLuint numOfDiffuse = 1;
		GLuint numOfSpecular = 1;

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			
			glActiveTexture(GL_TEXTURE0 + i);

			stringstream ss;

			string number;
			string name = this->textures[i].type;

			if (name == "texture_diffuse")
			{
				ss << numOfDiffuse++;
			}
			else if (name == "texture_specular")
			{
				ss << numOfSpecular++;
			}

			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		glUniform1f(glGetUniformLocation(shader.Program, "material.shine"), 16.0f);
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}


private:
	GLuint VAO, VBO, EBO;

	void SetupMesh()
	{
		//Setup vertex array obj and buffers
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);


		//Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		//Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

		//Vertex texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));


	}
};

