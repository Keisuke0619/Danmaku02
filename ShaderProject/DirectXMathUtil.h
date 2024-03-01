#pragma once
#include <DirectXMath.h>

/// <summary>
/// DirectXMath�̍\���̌v�Z���[�e�B���e�B�ł��B
/// </summary>
namespace DirectXUtil
{
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT3 src);
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Decriment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);

	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT2 src);
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Decriment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);

	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Increment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);
	/// <summary>
	/// ���l�����Z���܂��B
	/// </summary>
	/// <param name="dest">���Z��</param>
	/// <param name="src">���Z����l</param>
	void Decriment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);

	/// <summary>
	/// ���Z�������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">���l�P</param>
	/// <param name="right">���l�Q</param>
	/// <returns>���l�P�Ɛ��l�Q�𑫂����킹���l</returns>
	DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);
	/// <summary>
	/// ������E�����������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">������鐔</param>
	/// <param name="right">������</param>
	/// <returns>������E���������l</returns>
	DirectX::XMFLOAT2 Sub(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);
	/// <summary>
	/// ���ɉE�̃X�P�[�������������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="src">���̒l</param>
	/// <param name="num">�W��</param>
	/// <returns>�������킹���l</returns>
	DirectX::XMFLOAT2 Mul(DirectX::XMFLOAT2 src, float num);

	/// <summary>
	/// ���Z�������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">���l�P</param>
	/// <param name="right">���l�Q</param>
	/// <returns>���l�P�Ɛ��l�Q�𑫂����킹���l</returns>
	DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
	/// <summary>
	/// ������E�����������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">������鐔</param>
	/// <param name="right">������</param>
	/// <returns>������E���������l</returns>
	DirectX::XMFLOAT3 Sub(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
	/// <summary>
	/// ���ɉE�̃X�P�[�������������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="src">���̒l</param>
	/// <param name="num">�W��</param>
	/// <returns>�������킹���l</returns>
	DirectX::XMFLOAT3 Mul(DirectX::XMFLOAT3 src, float num);

	/// <summary>
	/// ���Z�������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">���l�P</param>
	/// <param name="right">���l�Q</param>
	/// <returns>���l�P�Ɛ��l�Q�𑫂����킹���l</returns>
	DirectX::XMFLOAT4 Add(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);
	/// <summary>
	/// ������E�����������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="left">������鐔</param>
	/// <param name="right">������</param>
	/// <returns>������E���������l</returns>
	DirectX::XMFLOAT4 Sub(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);
	/// <summary>
	/// ���ɉE�̃X�P�[�������������̂�Ԃ��܂��B
	/// </summary>
	/// <param name="src">���̒l</param>
	/// <param name="num">�W��</param>
	/// <returns>�������킹���l</returns>
	DirectX::XMFLOAT4 Mul(DirectX::XMFLOAT4 src, float num);

	/// <summary>
	/// ������Ԃ��܂��B
	/// </summary>
	/// <param name="one">�|�C���g�P</param>
	/// <param name="another">�|�C���g�Q</param>
	/// <returns>����</returns>
	float Dist(DirectX::XMFLOAT2 one, DirectX::XMFLOAT2 another);
}