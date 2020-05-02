#include "Renderer.h"

Renderer::Renderer(GLuint major, GLuint minor, VideoSettings in_VideoSettings): m_VideoSettings(in_VideoSettings)
{
    // Инициализируем GLFW 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Создаем новое окно
    GLint width = 800, height = 600;
    bool FullScreen = false;
    m_VideoSettings.getWidthHeight(&width, &height);
    m_VideoSettings.getFullScreen(&FullScreen);
    m_Window = Window(width, height, FullScreen);
    // Устанавливаем текущий контекст в созданное окно
    m_Window.MakeContextCurrent();
    // Устанавливаем функцию обработки нажатий клавиш
    //m_window.SetKeyCallback(key_callback);
    
    // Инициализируем GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }
    glGetError();

    // Вроде как при ретине изменяется размер экрана, так что убеждаемся, что он будет равен width и height
    glfwGetFramebufferSize(m_Window.getWindow(), &width, &height);
    glViewport(0, 0, width, height);
    m_VideoSettings.setWidthHeight(&width, &height);

    // Включаем параметры отрисовки
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClearColor(1.0f, 1.0, 1.0, 1.0f);
    //glfwSetInputMode(m_Window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(m_Window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Renderer::Draw(GLuint shader_id, Model* model) const
{
    GLuint mesh_lenght = 0;
    model->getMeshesLenght(mesh_lenght);
    for (GLuint i = 0; i < mesh_lenght; i++)
    {
        GLuint diffuseNr  = 1;
        GLuint specularNr = 1;
        GLuint normalNr   = 1;
        GLuint heightNr   = 1;
        for(GLuint j = 0; j < model->getMesh(i)->textures.size(); j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            std::string number;
            std::string name = model->getMesh(i)->textures[j].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++);
            else if(name == "texture_height")
                number = std::to_string(heightNr++);

            glUniform1i(glGetUniformLocation(m_Shaders[shader_id].ID, (name + number).c_str()), j);
            glBindTexture(GL_TEXTURE_2D, model->getMesh(i)->textures[j].id);
        }

        glBindVertexArray(model->getMesh(i)->VAO);
        glDrawElements(GL_TRIANGLES, model->getMesh(i)->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }
}

void Renderer::Draw(GLuint shader_id, Model* model, GLuint i) const
{
    GLuint diffuseNr  = 1;
    GLuint specularNr = 1;
    GLuint normalNr   = 1;
    GLuint heightNr   = 1;
    for(GLuint j = 0; j < model->getMesh(i)->textures.size(); j++)
    {
        glActiveTexture(GL_TEXTURE0 + j);
        std::string number;
        std::string name = model->getMesh(i)->textures[j].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_height")
            number = std::to_string(heightNr++);

        glUniform1i(glGetUniformLocation(m_Shaders[shader_id].ID, (name + number).c_str()), j);
        glBindTexture(GL_TEXTURE_2D, model->getMesh(i)->textures[j].id);
    }

    glBindVertexArray(model->getMesh(i)->VAO);
    glDrawElements(GL_TRIANGLES, model->getMesh(i)->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

}

void Renderer::addShader(std::string VertexPath, std::string FragmetPath, std::string GeometryPath)
{
    // Создаем новый объект шейдера
    Shader shader(VertexPath, FragmetPath, GeometryPath);
    // Заносим в вектор шейдеров
    m_Shaders.push_back(shader);
}

void Renderer::addShader(Shader shader)
{
    // Заносим в вектор шейдеров
    m_Shaders.push_back(shader);
}

Shader* Renderer::getShader(GLuint id)
{
    // WARNING: опасный подход, так как если сделать push_back, то указатель будет инвалидный
    // Предпологается что сначала мы загрузим все шейдеры, а потом уже будем их использовать
    return &m_Shaders[id];
}

void SetMat3Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::mat3> > *in_mat3Uniforms)
{
    GLint location_size = in_mat3Uniforms->first.size();
    GLint values_size = in_mat3Uniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetMat3(in_mat3Uniforms->first[i], in_mat3Uniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Mat3" << std::endl;
}
void SetMat4Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::mat4> > *in_mat4Uniforms)
{
    GLint location_size = in_mat4Uniforms->first.size();
    GLint values_size = in_mat4Uniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetMat4(in_mat4Uniforms->first[i], in_mat4Uniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Mat4" << std::endl;
}
void SetVec2Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec2> > *in_vec2Uniforms)
{
    GLint location_size = in_vec2Uniforms->first.size();
    GLint values_size = in_vec2Uniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetVec2(in_vec2Uniforms->first[i], in_vec2Uniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Vec2" << std::endl;
}
void SetVec3Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec3> > *in_vec3Uniforms)
{
    GLint location_size = in_vec3Uniforms->first.size();
    GLint values_size = in_vec3Uniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetVec3(in_vec3Uniforms->first[i], in_vec3Uniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Vec3" << std::endl;
}
void SetVec4Uniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<glm::vec4> > *in_vec4Uniforms)
{
    GLint location_size = in_vec4Uniforms->first.size();
    GLint values_size = in_vec4Uniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetVec4(in_vec4Uniforms->first[i], in_vec4Uniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Vec4" << std::endl;
}
void SetIntUniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<GLint> > *in_intUniforms)
{
    GLint location_size = in_intUniforms->first.size();
    GLint values_size = in_intUniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetInt(in_intUniforms->first[i], in_intUniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Int" << std::endl;
}
void SetFloatUniforms(Shader *in_shader, std::pair<std::vector<std::string>, std::vector<GLfloat> > *in_floatUniforms)
{
    GLint location_size = in_floatUniforms->first.size();
    GLint values_size = in_floatUniforms->first.size();
    if (location_size == values_size)
    {
        in_shader->Use();
        for (int i = 0; i < location_size; i++)
        {
            in_shader->SetFloat(in_floatUniforms->first[i], in_floatUniforms->second[i]);
        }
    }
    else
        std::cout << "location size doesnt math with values size Float" << std::endl;
}

