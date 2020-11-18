#pragma once
#include<d3d12.h>
class Dx12_Shader
{
private:
	ID3DBlob* mVsBlob = nullptr;
	ID3DBlob* mPsBlob = nullptr;
	ID3DBlob* mGsBlob = nullptr;
public:
	Dx12_Shader(const wchar_t* vsFilename, const wchar_t* psFilename,const wchar_t* gsFilename = L"");
	~Dx12_Shader();
	ID3DBlob* GetVsBlob();
	ID3DBlob* GetPsBlob();
	ID3DBlob* GetGsBlob();
};

