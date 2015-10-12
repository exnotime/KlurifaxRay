#pragma once
#include "RenderObjects.h"
#include "Camera.h"
/*
This class is an interface used to override the way objects are rendered
*/
class IRenderProgram{
public:
	virtual void Draw(RenderObject* obj, Camera &cam)  = 0; // draw things to screen
	virtual void Init() = 0; // initialize shaders and other variables
	virtual void Apply() = 0; // Apply things that should be applyed before 
	const bool GetDefered(){ return m_Defered; }
protected:
	bool m_Defered;	//this is to tell the renderer wether it should finish the defered rendering stage before it should render this object
};