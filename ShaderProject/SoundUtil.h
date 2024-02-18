#include <string>
namespace Sound
{
	void Play(std::string fileName, bool loop = false);
	void Stop(std::string fileName);
	void FadeOut(std::string fileName,float second);
	void FadeIn(std::string fileName, float second, bool loop = false);
	void Update(float tick);
	void ReleaseAll();
}