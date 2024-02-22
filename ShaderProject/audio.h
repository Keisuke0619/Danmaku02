// Audioコンポーネント [audio.h]
#pragma once

#include <xaudio2.h>

class Audio
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};

	float m_time;
	float m_fadeSecond;
	bool m_isFadeIn;
	float m_maxVolume;
public:
	static void InitMaster();
	static void UninitMaster();


	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void Stop();
	bool Update(float tick);
	void FadeOut(float seconds);
	void FadeIn(float seconds, float maxVol = 1.0f, bool loop = false);
	void SetVolume(float volume);
};
