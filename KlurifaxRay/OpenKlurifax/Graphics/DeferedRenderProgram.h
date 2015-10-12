#include "IRenderProgram.h"
#include "ShaderProgram.h"
/*
This class renders a model to the defered context before the ligting stage
*/
class DeferedRenderProgram : public IRenderProgram
{
public:
	DeferedRenderProgram();
	~DeferedRenderProgram();
	void Draw(RenderObject* obj, Camera &cam);
	void Init();
	void Apply();
private:
	ShaderProgram m_ShaderProgram;
};
