#include "MiniginPCH.h"
#include "InputManager.h"
#include <iostream>
#include <SDL_events.h>

UINT dae::InputManager::m_ID = 0;

bool dae::InputManager::ProcessInput()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	for (auto& controllerCommand : m_ControllerCommands)
	{
		if (IsPressed(controllerCommand.first.second, controllerCommand.second.second) && controllerCommand.second.first)
		{
			controllerCommand.second.first->Execute();
			return true;
		}
	}
	
	return ProcessKeyboardInput();
}

bool dae::InputManager::ProcessKeyboardInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

bool dae::InputManager::IsPressed(ControllerButton button, WORD keyStroke)
{
	for (const auto& controller : m_Controllers)
	{
		if (controller->IsButtonPressed(button, keyStroke))
		{
			return true;
		}
	}
	return false;
}

bool dae::InputManager::IsPressed(ControllerButton button, UINT& id)
{
	for (const auto& controller : m_Controllers)
	{
		if (controller->IsButtonPressed(button))
		{
			id = controller->GetID() + 1;
			return true;
		}
	}
	return false;
}

void dae::InputManager::AddController()
{
	if (m_Controllers.size() >= XUSER_MAX_COUNT)
		return;
	m_Controllers.push_back(std::make_unique<Controller>(m_ID));
	++m_ID;
}

std::vector<std::unique_ptr<dae::Controller>>& dae::InputManager::GetControllers()
{
	return m_Controllers;
}