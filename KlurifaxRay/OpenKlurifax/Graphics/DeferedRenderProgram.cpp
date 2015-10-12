#include "DeferedRenderProgram.h"
#include "../Base/MaterialBank.h"
#include "../Base/ModelBank.h"
#include "../Base/Input.h"
DeferedRenderProgram::DeferedRenderProgram(){
	m_Defered = true;
}

DeferedRenderProgram::~DeferedRenderProgram(){
}

void DeferedRenderProgram::Init(){
	m_ShaderProgram.LoadCompleteShaderProgramFromFile("Graphics/Shaders/DeferedGeometry.glsl",true);
}

void DeferedRenderProgram::Apply(){
	m_ShaderProgram.Apply();
}

void DeferedRenderProgram::Draw(RenderObject* obj, Camera &cam){
	ModelObject* object = static_cast<ModelObject*>(obj);
	Model model = g_ModelBank.FetchModel(object->Model);
	for (vector<Mesh>::iterator meshIterator = model.Meshes.begin(); meshIterator != model.Meshes.end(); ++meshIterator){
		unsigned int mat = model.MaterialBasePointer + meshIterator->Material;
		Material material = g_MaterialBank.GetMaterial(mat);

		//Diffuse albedo map
		if (material.GetDiffuseTexture() > 0){
			m_ShaderProgram.SetUniformValue("UseTexture", true);
			m_ShaderProgram.SetUniformTexture("tex1", *g_MaterialBank.FetchTexture(material.GetDiffuseTexture()));
		} else {
			m_ShaderProgram.SetUniformValue("UseTexture", false);
		}

		//normalmap
		if (material.GetNormalTexture() > 0){
			m_ShaderProgram.SetUniformValue("UseNormalMap", true);
			m_ShaderProgram.SetUniformTexture("NormalMap", *g_MaterialBank.FetchTexture(material.GetNormalTexture()));
		}
		else{
			m_ShaderProgram.SetUniformValue("UseNormalMap", false);
		}

		//roughnessmap
		if (material.GetGlossTexture() > 0){
			m_ShaderProgram.SetUniformValue("UseRoughnessMap", true);
			m_ShaderProgram.SetUniformTexture("RougnessMap", *g_MaterialBank.FetchTexture(material.GetGlossTexture()));
		}
		else{
			m_ShaderProgram.SetUniformValue("UseRoughnessMap", false);
		}
		//Reflection
		if (material.GetReflectionTexture() > 0){
			m_ShaderProgram.SetUniformValue("UseReflectionMap", true);
			m_ShaderProgram.SetUniformTexture("CubeTex", *g_MaterialBank.FetchTexture(material.GetReflectionTexture()));
		}
		else{
			m_ShaderProgram.SetUniformValue("UseReflectionMap", false);
		}
		static float g = 0;
		if (Input::GetInstance()->KeyDown(SDL_SCANCODE_1)){
			g += 0.001f;
		}
		//Material properties
		m_ShaderProgram.SetUniformValue("Material.Gloss", material.GetGlossFactor() + g);
		m_ShaderProgram.SetUniformValue("Material.BaseColor", material.GetBaseColor());
		m_ShaderProgram.SetUniformValue("Material.ReflectedColor", material.GetReflectiveColor());

		glm::mat4x4 wvp;
		wvp = cam.GetViewProj() * object->World;
		m_ShaderProgram.SetUniformValue("worldViewProj", wvp);
		m_ShaderProgram.SetUniformValue("CamPos", cam.GetPos());
		m_ShaderProgram.SetUniformValue("world", object->World);
		m_ShaderProgram.SetUniformValue("MaterialID", mat);
		glDrawElements(GL_TRIANGLES, meshIterator->Indices, GL_UNSIGNED_INT,
			(GLvoid*)(0 + ((model.IndexHandle + meshIterator->IndexBufferOffset) * sizeof(unsigned int))));
	}
}