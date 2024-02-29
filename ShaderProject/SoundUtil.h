#include <string>

/// <summary>
/// �T�E���h���[�e�B���e�B
/// ��{�I�Ƀt�@�C�������L�[�ƂȂ�A�Ǘ����Ă��܂��B
/// ���̂��߁A�����ɓ�������炷���Ƃ͂ł��܂���B�d�l�ł��B
/// </summary>
namespace Sound
{
	/// <summary>
	/// �w��̃T�E���h���Đ����܂�
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="loop">���[�v���邩</param>
	void Play(std::string fileName, bool loop = false);
	/// <summary>
	/// �w��̃T�E���h���~���܂�
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	void Stop(std::string fileName);
	/// <summary>
	/// �w��̃T�E���h���t�F�[�h�A�E�g�����܂�
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="second">�t�F�[�h�A�E�g�ɂ����鎞�ԁi�b�j</param>
	void FadeOut(std::string fileName,float second);
	/// <summary>
	/// �w��̃T�E���h���t�F�[�h�C�������܂�
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="second">�t�F�[�h�C���ɂ����鎞�ԁi�b�j</param>
	/// <param name="maxVol">�������̉���</param>
	/// <param name="loop">���[�v���邩</param>
	void FadeIn(std::string fileName, float second, float maxVol = 1.0f, bool loop = false);
	/// <summary>
	/// �T�E���h���[�e�B���e�B�̃V�X�e���֐�
	/// </summary>
	/// <param name="tick">�e�B�b�N</param>
	void Update(float tick);
	/// <summary>
	/// �w��̃T�E���h�̉��ʂ𒲐߂��܂�
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="volume">�{�����[��</param>
	void SetVolume(std::string fileName, float volume);
	/// <summary>
	/// ���ׂẲ�����������܂�
	/// </summary>
	void ReleaseAll();
}