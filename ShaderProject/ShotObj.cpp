#include "ShotObj.h"
#include "CameraBase.h"

#define ADDSHOT_NULL_MIN (1129893.0f)
// 色名配列
const std::string CShot::ColorName[COLOR_MAX] =
{
    "AQUA",
    "BLUE",
    "GREEN",
    "ORANGE",
    "PURPLE",
    "RED",
    "WHITE",
    "YELLOW",
};
// サイズ名配列
const std::string CShot::SizeName[SHOT_SIZE_MAX] =
{
    "01.png",
    "02.png",
    "03.png",
    "04.png",
    "05.png",
    "06.png",
    "07.png",
    "08.png",
};
// 当たり判定サイズ配列
const float CShot::ShotSize[SHOT_SIZE_MAX] =
{
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
};
// 削除距離
float CShot::ShotDeleteDepth = 35;

CShot::CShot()
{
    m_tag = "ShotFromEnemy";
    m_isKillShot = false;
    UseCollision();
}

CShot::~CShot()
{
    // ここでアイテム作成
    if (m_tag == "ShotFromPlayer") { return; }
}

void CShot::Update()
{
    if (m_shotDataReserve.size() != 0 && m_shotDataReserve.front()->frame == m_frame)
    {
        auto reserve = m_shotDataReserve.front();
        auto data = reserve->data;
        m_shotDataReserve.pop_front();
        if (data.speed >= ADDSHOT_NULL_MIN * SHOT_SPEED_COEF) { data.speed = m_shotData.speed; }
        if (data.angle >= DirectX::XMConvertToRadians(ADDSHOT_NULL_MIN)) { data.angle = m_shotData.angle; }
        m_shotData = data;
        m_speed.x = cosf(m_shotData.angle) * m_shotData.speed;
        m_speed.z = sinf(m_shotData.angle) * m_shotData.speed;
        delete reserve;
    }
    if (m_shotData.addAngle != 0 || m_shotData.addSpeed != 0)
    {
        m_shotData.angle += m_shotData.addAngle;
        m_shotData.speed += m_shotData.addSpeed;
        m_speed.x = cosf(m_shotData.angle) * m_shotData.speed;
        m_speed.z = sinf(m_shotData.angle) * m_shotData.speed;
    }
    DirectXUtil::Increment(&m_pos, m_speed);
    if (m_pos.z < CameraBase::GetPrimary()->GetPos().z || CameraBase::GetPrimary()->GetPos().z + ShotDeleteDepth < m_pos.z)
    {
        Destroy();
    }
}

void CShot::AddShotData(int frame, float speed, float degAngle, float addSpeed, float addAngle)
{
    // 弾データを生成し、追加。
    TShotDataReserve* tmp = new TShotDataReserve;
    tmp->frame = frame;
    tmp->data.angle = DirectX::XMConvertToRadians(degAngle);
    tmp->data.speed = speed * SHOT_SPEED_COEF;
    tmp->data.addSpeed = addSpeed * SHOT_SPEED_COEF;
    tmp->data.addAngle = DirectX::XMConvertToRadians(addAngle);
    m_shotDataReserve.push_back(tmp);
    // フレーム数を基準にソート
    m_shotDataReserve.sort(
        [](TShotDataReserve* l, TShotDataReserve* r)->bool
        {
            if (l == nullptr && r == nullptr) return false;
            if (l == nullptr) return true;
            if (r == nullptr) return false;
            return l->frame < r->frame;
        }
    );
}

void CShot::OnCollision(CObject* _obj)
{
    // 単純削除の場合はアイテムドロップ状態を解除して削除。
    if (_obj->GetTagName() == "Player" && m_tag == "ShotFromEnemy" ||
        _obj->GetTagName() == "Enemy" && m_tag == "ShotFromPlayer" ||
        _obj->GetTagName() == "Wall")
    {
        m_isDropItem = false;
        Destroy();
    }
    // 弾消しにぶつかったらアイテムドロップ状態で削除。
    if (_obj->GetTagName() == "EnemyShotDestroy" && m_tag == "ShotFromEnemy")
    {
        Destroy();
    }
}

void CShot::FromPlayer(bool fromPlayer)
{
    // 敵味方、どちら再度の弾かを指定。
    if (fromPlayer)
    {
        m_tag = "ShotFromPlayer";
    }
    else
    {
        m_tag = "ShotFromEnemy";
    }
}



CShot* CShot::Create(CObject* parent, DirectX::XMFLOAT2 pos, float speed, float degAngle, std::string color, float addSpeed, float addAngle)
{
    // 弾を生成し、渡されたデータをもとにデータをセット。
    auto shot = new CShot();
    SetInit(shot, parent, pos, speed, degAngle, color, addSpeed, addAngle);
    return shot;
}

void CShot::SetInit(CShot* shot, CObject* parent, DirectX::XMFLOAT2 pos, float speed, float degAngle, std::string color, float addSpeed, float addAngle)
{
    // 渡されたデータをもとにデータをセット
    shot->m_pos.x = pos.x;
    shot->m_pos.y = 1;
    shot->m_pos.z = pos.y;
    shot->m_shotData.speed = speed * SHOT_SPEED_COEF;
    shot->m_shotData.angle = DirectX::XMConvertToRadians(degAngle);
    shot->m_speed = DirectX::XMFLOAT3(cosf(shot->m_shotData.angle) * shot->m_shotData.speed, 0, sinf(shot->m_shotData.angle) * shot->m_shotData.speed);
    shot->m_shotData.addSpeed = addSpeed * SHOT_SPEED_COEF;
    shot->m_shotData.addAngle = DirectX::XMConvertToRadians(addAngle);
    shot->LoadTexture("Assets/Texture/Bullet/" + color);
    // サイズ設定用計算
    auto len = color.size();    // "XXXX01.png"
    int size = color[len - 5] - '1';
    shot->m_colliderScale = ShotSize[size];
    if (parent)
        parent->AddChild(shot);

}

void CShot::SetDeleteDepth(float newDepth)
{
    // 削除距離を指定
    ShotDeleteDepth = newDepth;
}
