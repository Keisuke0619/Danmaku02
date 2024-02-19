#include <string>
namespace Sound
{
	void Play(std::string fileName, bool loop = false);
	void Stop(std::string fileName);
	void FadeOut(std::string fileName,float second);
	void FadeIn(std::string fileName, float second, float maxVol = 1.0f, bool loop = false);
	void Update(float tick);
	void SetVolume(std::string fileName, float volume);
	void ReleaseAll();
}