#include "ModelBuffer.h"
#include "../../Base/MaterialBank.h"
ModelBuffer::ModelBuffer(){
	
}

ModelBuffer::~ModelBuffer(){
	glDeleteBuffers(1,&m_Buffer);
}

void ModelBuffer::Init(){
	m_Indices.reserve(MaxNrIndices);
	m_Models.reserve(MaxNrOfModels);
	m_Vertices.reserve(MaxNrVertices);

	glGenBuffers(1, &m_Buffer);
}


void ModelBuffer::Build(){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	unsigned char* buffer = new unsigned char[ModelBufferSize];
	memcpy(buffer + ModelBufferPos, &m_Models[0], m_Models.size() * sizeof(ShaderModel));
	memcpy(buffer + IndicesBufferPos, &m_Indices[0], m_Indices.size() * sizeof(unsigned int));
	memcpy(buffer + VerticesBuffferPos, &m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
	glBufferData(GL_SHADER_STORAGE_BUFFER, ModelBufferSize, buffer, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	delete[] buffer;
	float buffersizeMB = (float)(ModelBufferSize) / (float)(1024 * 1024);
	printf("Built model buffer of size %f MB\n", buffersizeMB);
}

void ModelBuffer::Bind(GLuint program, GLuint blockBinding){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, blockBinding, m_Buffer, (GLintptr)0, (GLsizeiptr)ModelBufferSize);
	GLuint blockIndex = 0;
	blockIndex = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "ModelBuffer");
	glShaderStorageBlockBinding(program, blockIndex, blockBinding);
	printf("binded model buffer to bindingpoint %d with index %d\n", blockBinding,blockIndex);
}
GLuint ModelBuffer::GetBuffer(){
	return m_Buffer;
}

void ModelBuffer::AddModels(vector<Model>& models, vector<VertexPosNormalTexTangent>& vertices, vector<unsigned int>& indices){
	ShaderModel model;
	for (auto& it : models){
		if (it.Meshes.size() >= 20){
			printf("%s has more than 20 meshes which is the current maximum", it.Name.c_str() );
			continue;
		}
		//bounds
		model.ModelMaxBounds = vec4(it.BoundingBox.MaxBounds,1);
		model.ModelMinBounds = vec4(it.BoundingBox.MinBounds,1);
		//vertex info
		model.IndexVertexSizeISizeV = ivec4(it.IndexHandle,it.VertexHandle,it.NumIndices,it.NumVertices);
		model.MaterialSizeM = ivec4(it.MaterialBasePointer,it.Meshes.size(),0,0);
		//meshes
		for (int i = 0; i < it.Meshes.size(); ++i){
			model.Meshes[i] = ivec4(it.Meshes[i].VertexBufferOffset, it.Meshes[i].IndexBufferOffset,
				it.Meshes[i].Indices, it.Meshes[i].Material);
			//materialtex
			Material mat = g_MaterialBank.GetMaterial(it.MaterialBasePointer + it.Meshes[i].Material);
			model.MeshTextures[i].x = mat.GetDiffuseTexture(); //Diffusemap
			model.MeshTextures[i].y = mat.GetNormalTexture(); //normalmap
			//mesh bounds
			model.MeshMinBounds[i] = vec4(it.Meshes[i].BoundingVolume.MinBounds,1);
			model.MeshMaxBounds[i] = vec4(it.Meshes[i].BoundingVolume.MaxBounds, 1);
		}
		m_Models.push_back(model);
	}
	Vertex v;
	for (auto& it : vertices){
		v.Position = vec4(it.Position, 1.0f);
		v.Normal = vec4(it.Normal, 0.0f);
		v.Tangent = vec4(it.Tangent, 0.0f);
		v.TexCoord = vec4(it.TexCoord, 0, 0);
		m_Vertices.push_back(v);
	}

	for (auto& it : indices){
		m_Indices.push_back(it);
	}
}