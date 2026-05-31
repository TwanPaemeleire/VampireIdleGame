#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Bloodforge.h>
#include <WindowUtils.h>

int main(int, char* [])
{
	Bloodforge::Bloodforge& engine = Bloodforge::Bloodforge::GetInstance();
	engine.SetResourcesDirectory("Resources");

	Bloodforge::WindowUtils::SetWindowAlwaysOnTop(true);
	Bloodforge::WindowUtils::SetWindowBordered(false);
	Bloodforge::WindowUtils::SetWindowFullScreen(false);
	Bloodforge::WindowUtils::SetWindowSize(250, 250);
	Bloodforge::WindowUtils::SetWindowTitle("VampireIdleGame");

	engine.Run();
	return 0;
}