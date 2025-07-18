#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexSrc, const char* fragmentSrc);
    void use() const;
    void setMat4(const std::string &name, const float* value) const;
    void deleteProgram();
};

#endif // SHADER_H
