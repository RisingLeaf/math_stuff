#include "WavefrontObj.h"

#include <cstring>
#include <unordered_map>


#include "../system/Log.h"


namespace {
}



bool WavefrontObj::LoadObj(const std::string &filename, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
{
	Log::Msg("OBJ INFO: LOADING OBJ");

	
	std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
	std::vector<float> temp_vertices;
	std::vector<float> temp_uvs;
	std::vector<float> temp_normals;

	FILE *file = fopen(filename.c_str(), "r");
	if (file == nullptr)
	{
		Log::Msg("OBJ ERROR: Impossible to open the file: " + filename);
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
    if (int res = fscanf(file, "%s", lineHeader); res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			float x = 0, y = 0, z = 0;
			fscanf(file, "%f %f %f\n", &x, &y, &z);
			temp_vertices.emplace_back(x);
			temp_vertices.emplace_back(y);
			temp_vertices.emplace_back(z);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			float x = 0, y = 0;
			fscanf(file, "%f %f\n", &x, &y);
			temp_uvs.emplace_back(x);
			temp_uvs.emplace_back(y);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			float x = 0, y = 0, z = 0;
			fscanf(file, "%f %f %f\n", &x, &y, &z);
			temp_normals.emplace_back(x);
			temp_normals.emplace_back(y);
			temp_normals.emplace_back(z);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
								 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				Log::Msg("OBJ ERROR: File can't be read by our simple parser :-( Try exporting with other options");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]-1);
			vertexIndices.push_back(vertexIndex[2]-1);
			vertexIndices.push_back(vertexIndex[1]-1);
			uvIndices.push_back(uvIndex[0]-1);
			uvIndices.push_back(uvIndex[2]-1);
			uvIndices.push_back(uvIndex[1]-1);
			normalIndices.push_back(normalIndex[0]-1);
			normalIndices.push_back(normalIndex[2]-1);
			normalIndices.push_back(normalIndex[1]-1);
		}
		else
		{
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}
	Log::Msg("OBJ INFO: Finish reading file. Ordering vertices ...");

	std::unordered_map<Vertex, uint32_t> used_vertices;
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		uint32_t vertexIndex = vertexIndices[i];
		uint32_t uvIndex     = uvIndices[i];
		uint32_t normalIndex = normalIndices[i];

		WavefrontObj::Vertex vertex{};

		vertex.Position = {
			temp_vertices[3 * vertexIndex + 0],
			temp_vertices[3 * vertexIndex + 1],
			temp_vertices[3 * vertexIndex + 2]
		};

		vertex.Normal = {
			temp_normals[3 * normalIndex + 0],
			temp_normals[3 * normalIndex + 1],
			temp_normals[3 * normalIndex + 2]
		};

		vertex.UV = {
			temp_uvs[2 * uvIndex + 0],
			temp_uvs[2 * uvIndex + 1]
		};

	  if (!used_vertices.contains(vertex))
	  {
		  used_vertices[vertex] = static_cast<uint32_t>(vertices.size());
		  vertices.push_back(vertex);
	  }

		indices.push_back(used_vertices[vertex]);
	}

	Log::Msg("OBJ INFO: Finish ordering vertices.");
	fclose(file);
	return true;
}




