#include "SoundUtil.h"
#include "audio.h"
#include <map>
#include <list>
namespace Sound
{
	// サウンドインスタンスを保持。
	std::map<std::string, Audio*> g_soundInstance;
	// フェードなどでアップデートが必要なインスタンスを保持する
	std::list<Audio*> g_updateList;
	/// <summary>
	/// 重複ロードしないように確認をしてから読み込みます。<br/>
	/// まだロードされていないものは g_soundInstance にロードします。
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void CheckAndLoad(std::string fileName);
}

void Sound::Play(std::string fileName, bool loop)
{
	// インスタンスをチェック・ロードし、Play
	CheckAndLoad(fileName);	
	g_soundInstance[fileName]->Play(loop);
}

void Sound::Stop(std::string fileName)
{
	// インスタンスをチェックし、存在しないなら終了。存在するならストップ。
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->Stop();
}

void Sound::FadeOut(std::string fileName, float second)
{
	// インスタンスが存在しないなら終了。
	if (g_soundInstance.count(fileName) == 0) { return; }
	// フェードアウトを実行
	g_soundInstance[fileName]->FadeOut(second);

	// アップデートリストに登録されていなければ登録。
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_updateList.push_back(g_soundInstance[fileName]);
	}
}

void Sound::FadeIn(std::string fileName, float second, float maxVol, bool loop)
{
	// インスタンスをチェックし、ロード
	CheckAndLoad(fileName);
	// フェードインを実行
	g_soundInstance[fileName]->FadeIn(second, maxVol, loop);

	// アップデートリストに登録されていなければ登録。
	if (std::find(g_updateList.begin(), g_updateList.end(), g_soundInstance[fileName]) == g_updateList.end())
	{
		g_updateList.push_back(g_soundInstance[fileName]);
	}

}

void Sound::Update(float tick)
{
	// アップデートリストを順にアップデート。
	for (auto itr = g_updateList.begin(); itr != g_updateList.end();)
	{
		// Audio::Updateは処理が完了するとtureを返す。
		// 完了(true)だったらアップデートリストから削除。（戻り値に次のイテレータが返ってくる）
		// それ以外だったら次のためにイテレータの更新
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
	// インスタンスの存在を確認し、関数を呼び出す。
	if (g_soundInstance.count(fileName) == 0) { return; }
	g_soundInstance[fileName]->SetVolume(volume);
}

void Sound::ReleaseAll()
{
	// すべてのインスタンスをUninitし、削除する。その後インスタンスマップもきれいさっぱり。
	for (auto itr = g_soundInstance.begin(); itr != g_soundInstance.end(); itr++)
	{
		itr->second->Uninit();
		delete itr->second;
	}
	g_soundInstance.clear();
}

void Sound::CheckAndLoad(std::string fileName)
{
	// ファイルが存在しているか確認し、無いならロード。
	if (g_soundInstance.count(fileName) == 0)
	{
		auto ins = g_soundInstance[fileName] = new Audio();
		std::string path = "Assets/Audio/" + fileName;
		ins->Load(path.c_str());
	}
}
