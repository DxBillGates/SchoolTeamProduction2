#include"Application.h"
#include<Windows.h>
//DEBUG_MODE�Ȃ�R���\�[�����\���ł���main�֐����G���g���[�|�C���g
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
//RELEASE_MODE�Ȃ�WinMain�֐����G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	Application app;
	if (!app.Run())
	{
		return -1;
	}
	return 0;
}