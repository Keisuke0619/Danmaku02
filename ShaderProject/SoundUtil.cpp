#include "SoundUtil.h"
#include "audio.h"
#include <map>
#include <list>
namespace Sound
{
	std::map<std::string, Audio*> g_soundInstance;
	std::list<Audio*> g_updateList;
	void CheckAndLoad(std::string fileName);
}

void Sound::Play(std::string fileName, bool loop)
{
	CheckAndLoad(fileName);
	g_soundInstance[fileName]->Play(loop);
}

void Sound::Stop(std::string fileName)
{
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->Stop();
}

void Sound::FadeOut(std::string fileName, float second)
{
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_soundInstance[fileName]->FadeOut(second);
		g_updateList.push_back(g_soundInstance[fileName]);
	}
}

void Sound::FadeIn(std::string fileName, float second, float maxVol, bool loop)
{
	CheckAndLoad(fileName);
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_soundInstance[fileName]->FadeIn(second, maxVol, loop);
		g_updateList.push_back(g_soundInstance[fileName]);
	}

}

void Sound::Update(float tick)
{
	for (auto itr = g_updateList.begin(); itr != g_updateList.end();)
	{
		if ((*itr)->Update(tick))
		{
			itr = g_updateList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Sound::SetVolume(std::string fileName, float volume)
{
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->SetVolume(volume);
}

void Sound::ReleaseAll()
{
	
	for (auto itr = g_soundInstance.begin(); itr != g_soundInstance.end(); itr++)
	{
		auto ptr = itr->second;
		ptr->Uninit();
		delete itr->second;
	}
	g_soundInstance.clear();
}

void Sound::CheckAndLoad(std::string fileName)
{
	if (g_soundInstance.count(fileName) == 0)
	{
		auto ins = g_soundInstance[fileName] = new Audio();
		std::string path = "Assets/Audio/" + fileName;
		ins->Load(path.c_str());
	}
}
