
#include "Shader.h"
#include "spdlog/spdlog.h"

Tyche::Shader::Shader() { }

void Tyche::Shader::init(const Tyche::ShaderCreateInfo &createInfo) {

    shaderProgram = glCreateProgram();

    if (createInfo.vertexSRC != nullptr) {
        addSubShader(createInfo.vertexSRC, ShaderType::VERTEX);
    }

    if (createInfo.fragmentSRC != nullptr) {
        addSubShader(createInfo.fragmentSRC, ShaderType::FRAGMENT);
    }


    glLinkProgram(shaderProgram);
}


Tyche::Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

Tyche::ShaderStatus Tyche::Shader::addSubShader(const char *src, Tyche::ShaderType type) {

    ShaderProgram shader;

    try {
        shader = loadShader(src, type);
    } catch (ShaderStatus error) {
        spdlog::error("Compilation Failure");
        return error;
    }

    glAttachShader(shaderProgram, shader);

    glDeleteShader(shader);

    return Tyche::ShaderStatus::SUCCESSFUL;
}

const char *Tyche::Shader::getCompilationErrorMessage() {
    return "Unknown";
}

Tyche::ShaderProgram Tyche::Shader::loadShader(const char *src, Tyche::ShaderType type) {

    unsigned int shader;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);


    //Check for errors
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        spdlog::error("Shader Compilation Error: {}", infoLog);
        throw ShaderStatus::COMPILATION_FAILED;
    }

    return shader;
}

void Tyche::Shader::bind() {
    glUseProgram(shaderProgram);
}

void Tyche::Shader::setInt(const char *name, int val) {
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniform1i(loc, val);
}

void Tyche::Shader::setMatrix4(const char *name, float *val) {
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, val);
}
