#include "Loader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

bool loadModel(const char* path, Model& model)
{

    std::ifstream file(path);
    if (file.fail()) {
        return false;
    }

    int num_vert = 0;
    int num_uv = 0;
    int num_normals = 0;

    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    std::vector< int > vertexIndices, uvIndices, normalIndices;


    std::string line;

    while (std::getline(file, line)) 
    {
        if (line[0] == 'v' && line[1] == ' ') {
            glm::vec3 vertex;
            line.erase(0, 1);

            std::stringstream ss(line);
            for (int i = 0; i < 3; i++)
            {
                ss >> vertex[i];
            }
            temp_vertices.push_back(vertex);
            num_vert++;
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            glm::vec2 uv;
            line.erase(0, 2);

            std::stringstream ss(line);
            for (int i = 0; i < 2; i++)
            {
                ss >> uv[i];
            }
            temp_uvs.push_back(uv);
            num_uv++;

        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            glm::vec3 normal;

            line.erase(0, 2);

            std::stringstream ss(line);
            for (int i = 0; i < 3; i++)
            {
                ss >> normal[i];
            }
            temp_normals.push_back(normal);
            num_normals++;

        }
        else if (line[0] == 'f')
        {
            std::string vertices[3];
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            line.erase(0, 1);

            char* parts[3];

            parts[0] = strtok(&line[0], " ");
            for (int i = 1; i < 3; i++)
            {
                parts[i] = strtok(NULL, " ");
            }

            vertexIndex[0] = atoi(strtok(parts[0], "/"));
            uvIndex[0] = atoi(strtok(NULL, "/"));
            normalIndex[0] = atoi(strtok(NULL, "/"));

            vertexIndex[1] = atoi(strtok(parts[1], "/"));
            uvIndex[1] = atoi(strtok(NULL, "/"));
            normalIndex[1] = atoi(strtok(NULL, "/"));

            vertexIndex[2] = atoi(strtok(parts[2], "/"));
            uvIndex[2] = atoi(strtok(NULL, "/"));
            normalIndex[2] = atoi(strtok(NULL, "/"));

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);

            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    indexing(vertexIndices, uvIndices, normalIndices, temp_vertices, temp_uvs, temp_normals, model);

    return true;
}

void indexing(std::vector<int>& vertexIndices, std::vector<int>& uvIndices, std::vector<int>& normalIndices, std::vector<glm::vec3> temp_v, std::vector<glm::vec2> temp_uv, std::vector<glm::vec3> temp_n, Model& model)
{
    for (int i = 0; i < vertexIndices.size(); i++)
    {
        int vi = vertexIndices[i];
        glm::vec3 vertex = temp_v[vi - 1];
        model.vertices.push_back((GLfloat)vertex.x);
        model.vertices.push_back((GLfloat)vertex.y);
        model.vertices.push_back((GLfloat)vertex.z);
    }
    for (int i = 0; i < uvIndices.size(); i++)
    {
        int uvi = uvIndices[i];
        glm::vec2 uv = temp_uv[uvi - 1];
        model.uvs.push_back((GLfloat)uv.x);
        model.uvs.push_back((GLfloat)uv.y);
    }
    for (int i = 0; i < normalIndices.size(); i++)
    {
        int ni = normalIndices[i];
        glm::vec3 normal = temp_n[ni - 1];
        model.normals.push_back(normal.x);
        model.normals.push_back(normal.y);
        model.normals.push_back(normal.z);
    }
}

