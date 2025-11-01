#include "engine/graphics/mesh.hpp"
#include <iostream>

using namespace engine::graphics;
using namespace engine::core;   


void Mesh::setupPositionAttribute(GLuint location) 
{
    glVertexAttribPointer(location, 3,
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(location);
}


void Mesh::setupColorAttribute(GLuint location)
{
    glVertexAttribPointer(location, 4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, m_color));
    glEnableVertexAttribArray(location);
}

void Mesh::setupTexCoordsAttribute(GLuint location)
{
    glVertexAttribPointer(location, 2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, m_texCoords));
    glEnableVertexAttribArray(location);
}

void Mesh::setupNormalAttribute(GLuint location) 
{
    glVertexAttribPointer(location, 3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, m_normal));
    glEnableVertexAttribArray(location);
}

Mesh::Mesh(const std::vector<Vertex>& vertexs, 
           const std::vector<GLuint>& indexs, 
           const std::vector<Texture*>& textures)
    : m_vertexs(vertexs)
    , m_indexs(indexs)
    , m_textures(textures)
    , m_attributes(VertexAttributes::POSITION)
{
    setup();
}

Mesh::Mesh(const std::vector<Vertex>& vertexs, 
           const std::vector<GLuint>& indexs, 
           const std::vector<Texture*>& textures,
           const VertexAttributes attributes)
    : m_vertexs(vertexs)
    , m_indexs(indexs)
    , m_textures(textures)
    , m_attributes(attributes)
{
    setup();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw(const Shader& shader)
{
    shader.use();
    for (GLuint i = 0; i < m_textures.size(); ++i) {
        m_textures[i]->bind(GL_TEXTURE0 + i);
    }

    glBindVertexArray(m_VAO);
    if (m_indexs.size() != 0)
        glDrawElements(GL_TRIANGLES, m_indexs.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, m_vertexs.size());
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setup() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertexs.size() * sizeof(Vertex),
                 m_vertexs.data(),
                 GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_indexs.size() * sizeof(GLuint),
                 m_indexs.data(),
                 GL_STATIC_DRAW);
    
    GLuint currentLocation = 0;

    if (hasPosition())
        setupPositionAttribute(currentLocation++);
    if (hasColor())
        setupColorAttribute(currentLocation++);
    if (hasTexCoords()) 
        setupTexCoordsAttribute(currentLocation++);
    if (hasNormal()) 
        setupNormalAttribute(currentLocation++);

    glBindVertexArray(0);
}

GLuint Mesh::VAO() const 
{ 
    return m_VAO; 
}

bool Mesh::hasPosition() const
{
    return m_attributes & VertexAttributes::POSITION;
}

bool Mesh::hasColor() const
{
    return m_attributes & VertexAttributes::COLOR;
}

bool Mesh::hasTexCoords() const
{
    return m_attributes & VertexAttributes::TEXCOORDS;
}

bool Mesh::hasNormal() const 
{
    return m_attributes & VertexAttributes::NORMAL;
}

size_t Mesh::vertexCount() const
{
    return m_vertexs.size();
}

size_t Mesh::indexCount() const 
{ 
    return m_indexs.size(); 
}

size_t Mesh::textureCount() const 
{ 
    return m_textures.size(); 
}