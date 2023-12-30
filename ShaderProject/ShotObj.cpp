#include "ShotObj.h"
#include "CameraBase.h"
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

CShot::CShot()
{
    m_tag = "ShotFromEnemy";
    m_isKillShot = false;
    UseCollision();
}

CShot::~CShot()
{
    // ‚±‚±‚ÅƒAƒCƒeƒ€ì¬
    if (m_tag == "ShotFromPlayer") { return; }
}

void CShot::Update()
{
    if (m_shotDataReserve.size() != 0 && m_shotDataReserve.front()->frame == m_frame)
    {
        auto reserve = m_shotDataReserve.front();
        auto data = reserve->data;
        m_shotDataReserve.pop_front();
        if (data.speed == ADDSHOT_NULL) { data.speed = m_shotData.speed; }
        if (data.speed == ADDSHOT_NULL) { data.speed = m_shotData.speed; }
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
    if (m_pos.z < CameraBase::GetPrimary()->GetPos().z || CameraBase::GetPrimary()->GetPos().z + 100 < m_pos.z)
    {
        Destroy();
    }
}

void CShot::AddShotData(int frame, float speed, float degAngle, float addSpeed, float addAngle)
{
    TShotDataReserve* tmp = new TShotDataReserve;
    tmp->frame = frame;
    tmp->data.angle = DirectX::XMConvertToRadians(degAngle);
    tmp->data.speed = speed * SHOT_SPEED_COEF;
    tmp->data.addSpeed = addSpeed * SHOT_SPEED_COEF;
    tmp->data.addAngle = DirectX::XMConvertToRadians(addAngle);
    m_shotDataReserve.push_back(tmp);
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
    if (_obj->GetTagName() == "Player" && m_tag == "ShotFromEnemy" ||
        _obj->GetTagName() == "Enemy" && m_tag == "ShotFromPlayer" ||
        _obj->GetTagName() == "Wall")
    {
        m_isDropItem = false;
        Destroy();
    }
}

void CShot::FromPlayer(bool fromPlayer)
{
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
    auto shot = new CShot();
    shot->m_pos.x = pos.x;
    shot->m_pos.y = 1;
    shot->m_pos.z = pos.y;
    shot->m_shotData.speed = speed * SHOT_SPEED_COEF;
    shot->m_shotData.angle = DirectX::XMConvertToRadians(degAngle);
    shot->m_speed = DirectX::XMFLOAT3(cosf(shot->m_shotData.angle) * shot->m_shotData.speed, 0, sinf(shot->m_shotData.angle) * shot->m_shotData.speed);
    shot->m_shotData.addSpeed = addSpeed * SHOT_SPEED_COEF;
    shot->m_shotData.addAngle = DirectX::XMConvertToRadians(addAngle);
    shot->LoadTexture("Assets/Texture/Bullet/" + color);
    if (parent)
        parent->AddChild(shot);
    return shot;
}
