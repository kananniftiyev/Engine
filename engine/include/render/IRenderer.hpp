#pragma once

class IRenderer
{
public:
	virtual ~IRenderer() = 0;

	virtual void Start() = 0;
	virtual void Frame() = 0;

	virtual void SetVsync(bool bVsync) = 0;

private:

};
