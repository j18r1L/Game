#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Shader.h"
#include "Mesh.h"

GLuint TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
private:
    // Текстуры
    std::vector<Texture> textures_loaded;
    // Мешы
    std::vector<Mesh> meshes;
    // Папка
    std::string directory;
    // Количество мешей
    GLuint meshes_length;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);



public:
    Model(std::string path)
    {
        loadModel(path);
        std::cout << "Кол-во загруженных текстур: " << textures_loaded.size() << std::endl;
    }
    void getMeshesLenght(GLuint& lenght)
    {
        lenght = meshes_length;
    }
    Mesh* getMesh(GLuint i)
    {
        return &meshes[i];
    }
    Texture* getTexture(GLuint i)
    {
        return &textures_loaded[i];
    }
    //void Draw(Shader shader);
    //void Draw(Shader shader, int i);

};

#endif // MODEL_H
