#pragma once
#include <DirectXMath.h>

/// <summary>
/// DirectXMathの構造体計算ユーティリティです。
/// </summary>
namespace DirectXUtil
{
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT3 src);
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Decriment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);

	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT2 src);
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Decriment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);

	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Increment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);
	/// <summary>
	/// 数値を加算します。
	/// </summary>
	/// <param name="dest">加算先</param>
	/// <param name="src">加算する値</param>
	void Decriment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);

	/// <summary>
	/// 加算したものを返します。
	/// </summary>
	/// <param name="left">数値１</param>
	/// <param name="right">数値２</param>
	/// <returns>数値１と数値２を足し合わせた値</returns>
	DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);
	/// <summary>
	/// 左から右を引いたものを返します。
	/// </summary>
	/// <param name="left">引かれる数</param>
	/// <param name="right">引く数</param>
	/// <returns>左から右を引いた値</returns>
	DirectX::XMFLOAT2 Sub(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);
	/// <summary>
	/// 左に右のスケールをかけたものを返します。
	/// </summary>
	/// <param name="src">元の値</param>
	/// <param name="num">係数</param>
	/// <returns>かけ合わせた値</returns>
	DirectX::XMFLOAT2 Mul(DirectX::XMFLOAT2 src, float num);

	/// <summary>
	/// 加算したものを返します。
	/// </summary>
	/// <param name="left">数値１</param>
	/// <param name="right">数値２</param>
	/// <returns>数値１と数値２を足し合わせた値</returns>
	DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
	/// <summary>
	/// 左から右を引いたものを返します。
	/// </summary>
	/// <param name="left">引かれる数</param>
	/// <param name="right">引く数</param>
	/// <returns>左から右を引いた値</returns>
	DirectX::XMFLOAT3 Sub(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
	/// <summary>
	/// 左に右のスケールをかけたものを返します。
	/// </summary>
	/// <param name="src">元の値</param>
	/// <param name="num">係数</param>
	/// <returns>かけ合わせた値</returns>
	DirectX::XMFLOAT3 Mul(DirectX::XMFLOAT3 src, float num);

	/// <summary>
	/// 加算したものを返します。
	/// </summary>
	/// <param name="left">数値１</param>
	/// <param name="right">数値２</param>
	/// <returns>数値１と数値２を足し合わせた値</returns>
	DirectX::XMFLOAT4 Add(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);
	/// <summary>
	/// 左から右を引いたものを返します。
	/// </summary>
	/// <param name="left">引かれる数</param>
	/// <param name="right">引く数</param>
	/// <returns>左から右を引いた値</returns>
	DirectX::XMFLOAT4 Sub(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);
	/// <summary>
	/// 左に右のスケールをかけたものを返します。
	/// </summary>
	/// <param name="src">元の値</param>
	/// <param name="num">係数</param>
	/// <returns>かけ合わせた値</returns>
	DirectX::XMFLOAT4 Mul(DirectX::XMFLOAT4 src, float num);

	/// <summary>
	/// 距離を返します。
	/// </summary>
	/// <param name="one">ポイント１</param>
	/// <param name="another">ポイント２</param>
	/// <returns>距離</returns>
	float Dist(DirectX::XMFLOAT2 one, DirectX::XMFLOAT2 another);
}