#include <Windows.h>
#include "dxcapi.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

DxcCreateInstanceProc gDxcCreateInstance = nullptr;

void task();

int main()
{
	HMODULE hModule = LoadLibrary("dxcompiler.dll");
	if(hModule == nullptr)
	{
		OutputDebugString("dxcompiler.dll�̓ǂݍ��݂Ɏ��s\n");
		return -1;
	}
	OutputDebugString("dxcompiler.dll�̓ǂݍ��݂ɐ���");

	gDxcCreateInstance = reinterpret_cast<decltype(gDxcCreateInstance)>(
		GetProcAddress(hModule, "DxcCreateInstance")
	);
	if(gDxcCreateInstance == nullptr)
	{
		OutputDebugString("DxcCreateInstance�̎擾�Ɏ��s\n");
		return -1;
	}
	OutputDebugString("DxcCreateInstance�̎擾�ɐ���\n");

	task();

	FreeLibrary(hModule);

	return 0;
}

void task()
{
	ComPtr<IDxcUtils> p_dxcutil;
	HRESULT hr = gDxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&p_dxcutil));
	if(FAILED(hr))
	{
		OutputDebugString("IDxcUtils�̃C���X�^���X�����Ɏ��s\n");
		return ;
	}
	OutputDebugString("IDxcUtils�̃C���X�^���X�����ɐ���\n");

	ComPtr<IDxcCompiler3> p_compiler;
	hr = gDxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&p_compiler));
	if(FAILED(hr))
	{
		OutputDebugString("IDxcCompiler3�̃C���X�^���X�����Ɏ��s\n");
		return ;
	}
	OutputDebugString("IDxcCompiler3�̃C���X�^���X�����ɐ���\n");

	UINT32 code_page = DXC_CP_UTF8;
	ComPtr<IDxcBlobEncoding> p_file_blob;
	hr = p_dxcutil->LoadFile(
		WORKING_DIR L"input.hlsl",
		&code_page,
		&p_file_blob
	);
	if(FAILED(hr))
	{
		OutputDebugString("�t�@�C���̓ǂݍ��݂Ɏ��s\n");
		return ;
	}
	OutputDebugString("�t�@�C���̓ǂݍ��݂ɐ���\n");

	DxcBuffer source;
	source.Ptr = p_file_blob->GetBufferPointer();
	source.Size = p_file_blob->GetBufferSize();
	source.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[]
	{
		L"-E", L"VS",
		L"-T", L"vs_6_4",
		L"-Od",
	};

	ComPtr<IDxcResult> p_result;
	hr = p_compiler->Compile(&source, arguments, _countof(arguments), nullptr, IID_PPV_ARGS(&p_result));
	if(FAILED(hr))
	{
		OutputDebugString("�R���p�C���Ɏ��s\n");
		return ;
	}

	if(FAILED(p_result->GetStatus(&hr)) || FAILED(hr))
	{
		OutputDebugString("�R���p�C���Ɏ��s\n");
		return ;
	}

	ComPtr<IDxcBlob> p_shader_blob;
	hr = p_result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&p_shader_blob), nullptr);
	if(FAILED(hr))
	{
		OutputDebugString("�V�F�[�_�[�I�u�W�F�N�g�̎擾�Ɏ��s\n");
		return ;
	}
	OutputDebugString("�V�F�[�_�[�I�u�W�F�N�g�̎擾�ɐ���\n");

	ComPtr<IDxcBlob> p_shader_hash_blob;
	hr = p_result->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&p_shader_hash_blob), nullptr);
	if(FAILED(hr))
	{
		OutputDebugString("�V�F�[�_�[�n�b�V���̎擾�Ɏ��s\n");
		return ;
	}
	OutputDebugString("�V�F�[�_�[�n�b�V���̎擾�ɐ���\n");
}
