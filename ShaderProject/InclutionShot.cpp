#include "InclutionShot.h"
#include "CameraBase.h"
#include "Util.h"
CInclusionShot::CInclusionShot()
	:CShot()
{

}

void CInclusionShot::Update()
{
    if (m_frame % 3 == 0)
    {
        auto shot = Create(nullptr, m_pos, 1.0f, m_frame * 7, "ORANGE07.png");
        shot->AddShotData(60, 1.0f, ADDSHOT_NULL, 0.07f, 0);
        shot->AddShotData(200, ADDSHOT_NULL, ADDSHOT_NULL, 0, 0);
    }
    DirectXUtil::Increment(&m_pos, m_speed);
    if (m_pos.z < CameraBase::GetPrimary()->GetPos().z || CameraBase::GetPrimary()->GetPos().z + ShotDeleteDepth < m_pos.z)
    {
        Destroy();
    }
}
