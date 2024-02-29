#include <string>

/// <summary>
/// サウンドユーティリティ
/// 基本的にファイル名がキーとなり、管理しています。
/// そのため、同時に同じ音を鳴らすことはできません。仕様です。
/// </summary>
namespace Sound
{
	/// <summary>
	/// 指定のサウンドを再生します
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="loop">ループするか</param>
	void Play(std::string fileName, bool loop = false);
	/// <summary>
	/// 指定のサウンドを停止します
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void Stop(std::string fileName);
	/// <summary>
	/// 指定のサウンドをフェードアウトさせます
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="second">フェードアウトにかかる時間（秒）</param>
	void FadeOut(std::string fileName,float second);
	/// <summary>
	/// 指定のサウンドをフェードインさせます
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="second">フェードインにかかる時間（秒）</param>
	/// <param name="maxVol">完了時の音量</param>
	/// <param name="loop">ループするか</param>
	void FadeIn(std::string fileName, float second, float maxVol = 1.0f, bool loop = false);
	/// <summary>
	/// サウンドユーティリティのシステム関数
	/// </summary>
	/// <param name="tick">ティック</param>
	void Update(float tick);
	/// <summary>
	/// 指定のサウンドの音量を調節します
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="volume">ボリューム</param>
	void SetVolume(std::string fileName, float volume);
	/// <summary>
	/// すべての音源を解放します
	/// </summary>
	void ReleaseAll();
}