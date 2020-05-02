#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "Window.h"
#include "Shader.h"
#include "VideoSettings.h"
#include "Model.h"

class Renderer
{
private:
    // Вектор всех шейдеров
    std::vector<Shader> m_Shaders;

public:
    // Класс окна
    Window m_Window;
    // Класс видео настроек
    VideoSettings m_VideoSettings;
    
    // Создаем новое окно и контекст opengl 
    Renderer(GLuint in_major, GLuint in_minor, VideoSettings in_VideoSettings);
    
    // Устанавливаем вход с клавиатуры 
    void SetKeyCallback(GLFWkeyfun key_callback);

    // Добавить шейдер
    void addShader(std::string in_VertexPath, std::string in_FragmetPath, std::string in_GeometryPath = "");
    void addShader(Shader shader);
    // Вернуть шейдер
    Shader* getShader(GLuint id);

    // Рендер сцены в in_framebuffer
    void Draw(GLuint shader_id, Model* model) const;

    void Draw(GLuint shader_id, Model* model, GLuint i) const;




    // Устанавливаем юниформы
    void SetMat3Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::mat3> > *in_mat3Uniforms);
    void SetMat4Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::mat4> > *in_mat4Uniforms);
    void SetVec2Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec2> > *in_vec2Uniforms);
    void SetVec3Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec3> > *in_vec3Uniforms);
    void SetVec4Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec4> > *in_vec4Uniforms);
    void SetIntUniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<GLint> > *in_intUniforms);
    void SetFloatUniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<GLfloat> > *in_floatUniforms);

};

#endif // RENDERER_H
