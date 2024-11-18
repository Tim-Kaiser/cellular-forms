#include "ObjectLoader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

bool loadObject(const char* path, Object& obj)
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

    std::vector< size_t > vertexIndices, uvIndices, normalIndices;


    std::string line;

    while (std::getline(file, line)) 
    {
        if (line[0] == 'v' && line[1] == ' ') {
            glm::vec3 vertex{};
            line.erase(0, 1);

            char* parts[3];
            parts[0] = strtok(&line[0], " ");

            for (int i = 1; i < 3; i++)
            {
                parts[i] = strtok(NULL, " ");
            }

            vertex.x = atof(parts[0]);
            vertex.y = atof(parts[1]);
            vertex.z = atof(parts[2]);

            temp_vertices.emplace_back(vertex);
            num_vert++;
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            glm::vec2 uv{};
            line.erase(0, 2);

            char* parts[3];
            parts[0] = strtok(&line[0], " ");
            parts[1] = strtok(NULL, " ");

            uv.x = atof(parts[0]);
            uv.y = atof(parts[1]);
            temp_uvs.emplace_back(uv);
            num_uv++;

        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            glm::vec3 normal{};

            line.erase(0, 2);

            char* parts[3];
            parts[0] = strtok(&line[0], " ");

            for (int i = 1; i < 3; i++)
            {
                parts[i] = strtok(NULL, " ");
            }

            normal.x = atof(parts[0]);
            normal.y = atof(parts[1]);
            normal.z = atof(parts[2]);

            temp_normals.emplace_back(normal);
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

            vertexIndices.emplace_back(vertexIndex[0]);
            vertexIndices.emplace_back(vertexIndex[1]);
            vertexIndices.emplace_back(vertexIndex[2]);

            uvIndices.emplace_back(uvIndex[0]);
            uvIndices.emplace_back(uvIndex[1]);
            uvIndices.emplace_back(uvIndex[2]);

            normalIndices.emplace_back(normalIndex[0]);
            normalIndices.emplace_back(normalIndex[1]);
            normalIndices.emplace_back(normalIndex[2]);
        }
    }
    indexing(vertexIndices, uvIndices, normalIndices, temp_vertices, temp_uvs, temp_normals, obj);

    return true;
}

void indexing(std::vector<size_t>& vertexIndices, std::vector<size_t>& uvIndices, std::vector<size_t>& normalIndices, std::vector<glm::vec3>& temp_v, std::vector<glm::vec2>& temp_uv, std::vector<glm::vec3>& temp_n, Object& obj)
{
    obj.vertices.reserve(vertexIndices.size());
    obj.normals.reserve(normalIndices.size());
    obj.uvs.reserve(uvIndices.size());

    for (int i = 0; i < vertexIndices.size(); i++)
    {
        size_t vi = vertexIndices[i];
        glm::vec3 vertex = temp_v[vi - 1];
        obj.vertices.emplace_back((GLfloat)vertex.x);
        obj.vertices.emplace_back((GLfloat)vertex.y);
        obj.vertices.emplace_back((GLfloat)vertex.z);
    }
    for (int i = 0; i < uvIndices.size(); i++)
    {
        size_t uvi = uvIndices[i];
        glm::vec2 uv = temp_uv[uvi - 1];
        obj.uvs.emplace_back((GLfloat)uv.x);
        obj.uvs.emplace_back((GLfloat)uv.y);
    }
    for (int i = 0; i < normalIndices.size(); i++)
    {
        size_t ni = normalIndices[i];
        glm::vec3 normal = temp_n[ni - 1];
        obj.normals.emplace_back(normal.x);
        obj.normals.emplace_back(normal.y);
        obj.normals.emplace_back(normal.z);
    }
}

