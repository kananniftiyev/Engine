#pragma once

class IRenderer
{
public:
	virtual ~IRenderer() = 0;

	virtual void Initialize() = 0;

	virtual void CreateBuffer() = 0;

	virtual void CreateShader() = 0;

	virtual void Frame() = 0;
private:

};