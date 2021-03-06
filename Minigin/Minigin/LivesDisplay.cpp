#include "MiniginPCH.h"
#include "LivesDisplay.h"

#include <algorithm>


#include "PlayerComponent.h"
#include "TextComponent.h"
#include "Transform.h"

float LivesDisplay::m_Y = 450.f;
std::vector<int> LivesDisplay::m_Lives;

LivesDisplay::LivesDisplay(UINT players)
	: m_Players{}
{
	m_Lives.clear();
	
	if (m_Lives.size() == players) return;
	for(UINT i{}; i < players; ++i)
	{
		m_Lives.push_back(3);
	}
}

void LivesDisplay::AddData(GameObject& obj)
{
	Transform pos{};
	pos.SetPosition(m_X, m_Y, 0);
	++m_Players;
	obj.GetComponent<TextComponent>()->SetText("Player " + std::to_string(m_Players)
		+ " Lives: " + std::to_string(m_StartLives));
	obj.GetComponent<TextComponent>()->SetPos(pos);
	obj.GetComponent<TextComponent>()->ObsUpdate();
	m_SetMethods.push_back(std::bind(&TextComponent::SetText, obj.GetComponent<TextComponent>(), std::placeholders::_1));
	m_UpdateMethods.push_back(std::bind(&TextComponent::ObsUpdate, obj.GetComponent<TextComponent>()));
	const float yOffset = 20.f;
	m_Y -= yOffset;
	const float yThreshold = 430.f;
	if (m_Y < yThreshold)
		m_Y = 450.f;
}

void LivesDisplay::OnNotify(const dae::GameObject& obj, Message message)
{
	UINT id = obj.GetComponent<PlayerComponent>()->GetID();
	switch (message)
	{
	case Message::PlayerDied:
		--m_Lives[id];
		m_SetMethods[id]("Player " + std::to_string(id + 1) + " Lives: " + std::to_string(m_Lives[id]));
		m_UpdateMethods[id]();
		if (std::find(m_Lives.begin(), m_Lives.end(), 0) != m_Lives.end())
		{
			m_Lives.clear();
			SceneManager::GetInstance().MarkForDeletion();
		}
		break;
	case Message::UpdateMsg:
		m_SetMethods[id]("Player " + std::to_string(id + 1) + " Lives: " + std::to_string(m_Lives[id]));
		m_UpdateMethods[id]();
		break;
	}
}