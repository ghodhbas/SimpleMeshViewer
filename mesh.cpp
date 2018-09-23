#include "mesh.h"
#include "obj_loader.h"
#include <vector>

/*Constructor file*/
Mesh::Mesh(const std::string& fileName)
{
    IndexedModel model = OBJModel(fileName).ToIndexedModel();
    InitMesh(model);
}

/*Constructor verticies and indicies*/
Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;
    for(unsigned int i = 0; i < numVertices ; i++)
    {
        model.positions.push_back(*vertices[i].GetPos());
        model.texCoords.push_back(*vertices[i].GetTexCoord());
        model.normals.push_back(*vertices[i].GetNormal());

    }

    for(unsigned int i =0; i< numIndices;i++)
    {
        model.indices.push_back(indices[i]);
    }

    InitMesh(model);
}


void Mesh::InitMesh(const IndexedModel& model)
{
    m_drawCount = model.indices.size();

    //generate vertex array and bind it
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    /****************POSITIONS***************/
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);//give us blocks of data in gpu
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);//take data and put it on gpu


    //how to intrepret
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);



    /****** TEXTURES*************/
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXTCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);//take data and put it on gpu

    //how to intrepret
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /****** NORMALS*************/
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);//take data and put it on gpu

    //how to intrepret
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /****** INDICES*************/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);//take data and put it on gpu


    glBindVertexArray(0);
}

/*Destructor*/
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}


void Mesh::Draw()
{
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT,0);
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    glBindVertexArray(0);
}
