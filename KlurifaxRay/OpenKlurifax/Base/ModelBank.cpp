#include "ModelBank.h"
#include "MaterialBank.h"
ModelBank::ModelBank(){

}

ModelBank::~ModelBank(){

}

ModelBank& ModelBank::GetInstance(){
	static ModelBank m_Bank;
	return m_Bank;
}

ModelHandle ModelBank::LoadModel(const char* filename){
	Model model;
	ModelHandle id = ++m_Numerator;
	Assimp::Importer loader;
	model.Name = filename;
	const aiScene* scene = loader.ReadFile(filename, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SplitLargeMeshes | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);
	if (scene){
		//load geometry
		if (scene->HasMeshes()){
			model.VertexHandle = static_cast<int>(m_Vertices.size());
			LoadMeshes(model, scene);
			g_MaterialBank.LoadMaterials(model, filename, scene);
		}
		model.Loaded = true;
		//find bbv for model
		Box bbv;
		bbv.MinBounds = vec3(0);
		bbv.MaxBounds = vec3(0);
		for (auto& it : model.Meshes){
			if (it.BoundingVolume.MinBounds.x < bbv.MinBounds.x)
				bbv.MinBounds.x = it.BoundingVolume.MinBounds.x;
			if (it.BoundingVolume.MinBounds.y < bbv.MinBounds.y)
				bbv.MinBounds.y = it.BoundingVolume.MinBounds.y;
			if (it.BoundingVolume.MinBounds.z < bbv.MinBounds.z)
				bbv.MinBounds.z = it.BoundingVolume.MinBounds.z;

			if (it.BoundingVolume.MaxBounds.x > bbv.MaxBounds.x)
				bbv.MaxBounds.x = it.BoundingVolume.MaxBounds.x;
			if (it.BoundingVolume.MaxBounds.y > bbv.MaxBounds.y)
				bbv.MaxBounds.y = it.BoundingVolume.MaxBounds.y;
			if (it.BoundingVolume.MaxBounds.z > bbv.MaxBounds.z)
				bbv.MaxBounds.z = it.BoundingVolume.MaxBounds.z;
		}
		model.BoundingBox = bbv;

		m_Models[id] = model;
		return id;
	}
	else {
		 //Logger::Log("Error loading model filename :" + string(filename), "Model", LogSeverity::ERROR_);
		return -1;
	}
}

void ModelBank::LoadMeshes(Model& model, const aiScene* scene){
	int size = 0;
	int indices = 0;
	int totalNumVertices = 0;
	Mesh modelMesh;
	
	for (unsigned int i = 0; i < scene->mNumMeshes; i++){
		const aiMesh* mesh = scene->mMeshes[i];
		modelMesh.VertexBufferOffset = size;
		unsigned int numVertices = 0;
		unsigned int numIndices = 0;
		vec3 minBounds, maxBounds;
		//foreach vertice
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i){
			VertexPosNormalTexTangent vertex;
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;
			//check bbv
			 if (vertex.Position.x < minBounds.x)
				 minBounds.x = vertex.Position.x;
			 if (vertex.Position.y < minBounds.y)
				 minBounds.y = vertex.Position.y;
			 if (vertex.Position.z < minBounds.z)
				 minBounds.z = vertex.Position.z;

			 if (vertex.Position.x > maxBounds.x)
				 maxBounds.x = vertex.Position.x;
			 if (vertex.Position.y > maxBounds.y)
				 maxBounds.y = vertex.Position.y;
			 if (vertex.Position.z > maxBounds.z)
				 maxBounds.z = vertex.Position.z;

			if (mesh->HasNormals()){
				vertex.Normal.x = mesh->mNormals[i].x;
				vertex.Normal.y = mesh->mNormals[i].y;
				vertex.Normal.z = mesh->mNormals[i].z;
			}
			if (mesh->HasTangentsAndBitangents()){
				vertex.Tangent.x = mesh->mTangents[i].x;
				vertex.Tangent.y = mesh->mTangents[i].y;
				vertex.Tangent.z = mesh->mTangents[i].z;
			}
			if (mesh->mTextureCoords[0] != NULL){
				const aiVector3D* vt = &mesh->mTextureCoords[0][i];
				vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
			}
			numVertices++;
			m_Vertices.push_back(vertex);
		}//end foreach vertice
		//Indices
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i){
			//index = (Num vertices from the already loaded models) + (Size of all the already loaded meshes + mesh->faceindices)
			m_Indices.push_back(model.VertexHandle + size + mesh->mFaces[i].mIndices[0]);
			m_Indices.push_back(model.VertexHandle + size + mesh->mFaces[i].mIndices[1]);
			m_Indices.push_back(model.VertexHandle + size + mesh->mFaces[i].mIndices[2]);
			numIndices += 3;
		}
		modelMesh.BoundingVolume.MinBounds = minBounds;
		modelMesh.BoundingVolume.MaxBounds = maxBounds;
		modelMesh.Material = mesh->mMaterialIndex;
		modelMesh.IndexBufferOffset = indices;
		size += numVertices;
		indices += numIndices;
		modelMesh.Size = numVertices;
		modelMesh.Indices = numIndices;
		model.Meshes.push_back(modelMesh);
	}//end foreach mesh
	model.NumVertices = size;
	model.NumIndices = indices;
}


ModelHandle ModelBank::AddModel(Model& TheModel){
	ModelHandle id = ++m_Numerator;
	m_Models[id] = TheModel;
	return id;
}

void ModelBank::DeleteModel(ModelHandle& handle){
	//TODO: Delete model data ,vertices and indices.
	//then update all the other models after it in the memory.
	// tbh its much easier and less cumnbersome to just delete all models and load them in again.
}

const Model& ModelBank::FetchModel(ModelHandle handle){
	return m_Models[handle];
}

void ModelBank::Clear(){
	glDeleteBuffers(1, &m_IndexBuffer);
	m_VertexBuffer.Release();
	m_Vertices.clear();
	m_Indices.clear();
	m_Numerator = 0;
}

void ModelBank::BuildBuffers(){
	//VertexBuffer
	m_VertexBuffer.Init(POS_NORMAL_TEX_TANGENT, m_Vertices.data(), static_cast<int>(m_Vertices.size()) * sizeof(VertexPosNormalTexTangent), 0);
	//IndexBuffer
	int h = 0;
	for (std::map<ModelHandle, Model>::iterator it = m_Models.begin(); it != m_Models.end(); ++it){
		it->second.IndexHandle = h;
		h += it->second.NumIndices;
	}
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
}

void ModelBank::ApplyBuffers(){
	m_VertexBuffer.Apply();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
}
VertexBuffer& ModelBank::GetVertexBuffer(){
	return m_VertexBuffer;
}

GLuint& ModelBank::GetIndexBuffer(){
	return m_IndexBuffer;
}

vector<unsigned int> ModelBank::GetIndices(){
	return m_Indices;
}

vector<VertexPosNormalTexTangent> ModelBank::GetVertices(){
	return m_Vertices;
}

ModelHandle ModelBank::AddModel(vector<VertexPosNormalTexTangent>& vertices, vector<unsigned int>& indices, Model& outModel){
	Model model;
	ModelHandle id = ++m_Numerator;

	model.VertexHandle = static_cast<int>(m_Vertices.size());
	model.NumVertices = (int)vertices.size();
	model.NumIndices = (int)indices.size();
	Mesh modelMesh;
	modelMesh.IndexBufferOffset = 0;
	modelMesh.VertexBufferOffset = 0;
	modelMesh.Indices = (int)indices.size();
	modelMesh.Size = (int)vertices.size();
	//copy vertices
	for (auto it : vertices){
		m_Vertices.push_back(it);
	}
	for (auto it : indices){
		m_Indices.push_back(model.VertexHandle + it);
	}
	model.Meshes.push_back(modelMesh);
	m_Models[id] = model;
	outModel = m_Models[id];
	return id;
}

void ModelBank::UpdateModel(ModelHandle& handle, Model& model){
	m_Models[handle] = model;
}

vector<Model> ModelBank::GetModels(){
	vector<Model> models;

	for (auto& it : m_Models){
		models.push_back(it.second);
	}

	return models;
}