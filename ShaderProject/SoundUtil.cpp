#include "SoundUtil.h"
#include "audio.h"
#include <map>
#include <list>
namespace Sound
{
	// �T�E���h�C���X�^���X��ێ��B
	std::map<std::string, Audio*> g_soundInstance;
	// �t�F�[�h�ȂǂŃA�b�v�f�[�g���K�v�ȃC���X�^���X��ێ�����
	std::list<Audio*> g_updateList;
	/// <summary>
	/// �d�����[�h���Ȃ��悤�Ɋm�F�����Ă���ǂݍ��݂܂��B<br/>
	/// �܂����[�h����Ă��Ȃ����̂� g_soundInstance �Ƀ��[�h���܂��B
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	void CheckAndLoad(std::string fileName);
}

void Sound::Play(std::string fileName, bool loop)
{
	// �C���X�^���X���`�F�b�N�E���[�h���APlay
	CheckAndLoad(fileName);	
	g_soundInstance[fileName]->Play(loop);
}

void Sound::Stop(std::string fileName)
{
	// �C���X�^���X���`�F�b�N���A���݂��Ȃ��Ȃ�I���B���݂���Ȃ�X�g�b�v�B
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->Stop();
}

void Sound::FadeOut(std::string fileName, float second)
{
	// �C���X�^���X�����݂��Ȃ��Ȃ�I���B
	if (g_soundInstance.count(fileName) == 0) { return; }
	// �t�F�[�h�A�E�g�����s
	g_soundInstance[fileName]->FadeOut(second);

	// �A�b�v�f�[�g���X�g�ɓo�^����Ă��Ȃ���Γo�^�B
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_updateList.push_back(g_soundInstance[fileName]);
	}
}

void Sound::FadeIn(std::string fileName, float second, float maxVol, bool loop)
{
	// �C���X�^���X���`�F�b�N���A���[�h
	CheckAndLoad(fileName);
	// �t�F�[�h�C�������s
	g_soundInstance[fileName]->FadeIn(second, maxVol, loop);

	// �A�b�v�f�[�g���X�g�ɓo�^����Ă��Ȃ���Γo�^�B
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_updateList.push_back(g_soundInstance[fileName]);
	}

}

void Sound::Update(float tick)
{
	// �A�b�v�f�[�g���X�g�����ɃA�b�v�f�[�g�B
	for (auto itr = g_updateList.begin(); itr != g_updateList.end();)
	{
		// Audio::Update�͏��������������ture��Ԃ��B
		// ����(true)��������A�b�v�f�[�g���X�g����폜�B�i�߂�l�Ɏ��̃C�e���[�^���Ԃ��Ă���j
		// ����ȊO�������玟�̂��߂ɃC�e���[�^�̍X�V
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
	// �C���X�^���X�̑��݂��m�F���A�֐����Ăяo���B
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->SetVolume(volume);
}

void Sound::ReleaseAll()
{
	// ���ׂẴC���X�^���X��Uninit���A�폜����B���̌�C���X�^���X�}�b�v�����ꂢ�����ς�B
	for (auto itr = g_soundInstance.begin(); itr != g_soundInstance.end(); itr++)
	{
		itr->second->Uninit();
		delete itr->second;
	}
	g_soundInstance.clear();
}

void Sound::CheckAndLoad(std::string fileName)
{
	// �t�@�C�������݂��Ă��邩�m�F���A�����Ȃ烍�[�h�B
	if (g_soundInstance.count(fileName) == 0)
	{
		auto ins = g_soundInstance[fileName] = new Audio();
		std::string path = "Assets/Audio/" + fileName;
		ins->Load(path.c_str());
	}
}
