#include "MiniginPCH.h"
#include "DiscTransformComponent.h"
#include <algorithm>
#include "GraphicsComponent2D.h"

int DiscTransformComponent::m_Counter{};

DiscTransformComponent::DiscTransformComponent(std::shared_ptr<std::vector<utils::Tile*>>& grid, std::function<std::pair<int, int>()> getQbertPos,
	std::function<void(bool)> setTP, std::function<bool()> hasLevelEnded, std::shared_ptr<RemainingDiscCommand> discCMD)
	: HexTransformComponent(grid),
	m_QBertPos{ getQbertPos },
	m_SetTeleport{ setTP },
	m_IsActive{ false },
	m_HasBeenUsed{ false },
	m_DiscOffsetX{ 50.f },
	m_DiscOffsetY{ 10.f },
	m_HasLevelEnded{ hasLevelEnded },
	m_DiscCMD{ discCMD },
	m_HasScoreChanged{ false }
{
	++m_Counter;

	InitPos();
}

void DiscTransformComponent::InitPos()
{
	m_Row = (rand() % 6 + 1);
	switch (m_Counter)
	{
	case 1:
		m_Col = -m_Row;
		auto leftPos = m_GridMap[std::make_pair(m_Row, m_Col)]->center;
		m_Transform.SetPosition(leftPos.x - m_OffsetX - m_DiscOffsetX, leftPos.y - m_OffsetY + m_DiscOffsetY, 0.f);
		break;
	case 2:
		m_Col = 0;
		auto rightPos = m_GridMap[std::make_pair(m_Row, m_Col)]->center;
		m_Transform.SetPosition(rightPos.x - m_OffsetX + m_DiscOffsetX, rightPos.y - m_OffsetY + m_DiscOffsetY, 0.f);
		break;
	}
}

DiscTransformComponent::~DiscTransformComponent()
{
	--m_Counter;
}

void DiscTransformComponent::Update(float, GameObject& obj)
{
	HandleTeleportActivity(obj);

	if (m_HasLevelEnded && !m_HasBeenUsed && !m_HasScoreChanged)
	{
		m_DiscCMD->Execute();
		m_HasScoreChanged = true;
	}
}

void DiscTransformComponent::HandleTeleportActivity(GameObject& obj)
{
	if (m_HasBeenUsed)
		return;

	if (m_QBertPos() == std::pair<int, int>(m_Row, m_Col))
	{
		// Activate QBert's teleport flag
		m_SetTeleport(true);
		m_IsActive = true;
	}
	else if (m_IsActive)
	{
		// First check if QBert is out of bounds which means he jumped on the Disc
		if (m_GridMap.find(std::pair<int, int>(m_QBertPos().first, m_QBertPos().second)) == m_GridMap.end())
		{
			m_HasBeenUsed = true;
			obj.GetComponent<GraphicsComponent2D>()->SetVisibility(false);
			return;
		}

		m_IsActive = false;
		// Deactivate QBert's teleport flag
		m_SetTeleport(false);
	}
}