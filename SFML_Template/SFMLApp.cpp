#include "StdAfx.h"
#include "SFMLApp.h"
#include <algorithm>


SFMLApp::SFMLApp(void)
	:m_running(true),
	m_mainWindow()
{
}


SFMLApp::~SFMLApp(void)
{
}

int SFMLApp::OnExecute(void)
{
	//main program function
	if(!OnInit())
		return -1;
	sf::Event Event;
	while(m_running)
	{
		while(m_mainWindow.pollEvent(Event))
		{
			OnEvent(Event);
		}

		OnUpdate();
		OnRender();
	}

	OnCleanup();

	return 0;
}

void SFMLApp::registerState(GameStateBase* gameState, std::string stateID)
{
	m_registeredGameStates[stateID] = gameState;
}

void SFMLApp::unregisterState(std::string stateID)
{
	std::map<std::string, GameStateBase*>::iterator stateIt(m_registeredGameStates.find(stateID));
	if(stateIt != m_registeredGameStates.end())
	{
		delete stateIt->second;
		m_registeredGameStates.erase(stateIt);
	}
	else
	{
		printf("STATE %s NOT FOUND", stateID);
	}
}

void SFMLApp::changeState(std::string stateID)
{
	if(m_states.size() > 0)
	{
		m_states.back()->OnCleanup();
		m_states.pop_back();
	}
	m_states.push_back(getState(stateID));
	m_states.back()->OnAwake();
}

void SFMLApp::pushState(std::string stateID)
{
	if(m_states.size() > 0)
		m_states.back()->OnSuspend();
	m_states.push_back(getState(stateID));
	m_states.back()->OnAwake();
}

void SFMLApp::popState(void)
{
	if(m_states.size() > 0)
	{
		m_states.back()->OnCleanup();
		m_states.pop_back();
	}
	if(m_states.size() > 0)
	m_states.back()->OnResume();
}

void SFMLApp::removeState(std::string stateID)
{
	if(m_states.size() == 0)
		return;
	std::vector<GameStateBase*>::iterator stateIt(std::find(m_states.begin(), m_states.end(), getState(stateID)));
	if(stateIt != m_states.end())
	{
		(*stateIt)->OnCleanup();
		m_states.erase(stateIt);
	}
}

GameStateBase* SFMLApp::getState(std::string stateID)
{
	std::map<std::string, GameStateBase*>::iterator stateIt(m_registeredGameStates.find(stateID));
	if(stateIt != m_registeredGameStates.end())
		return stateIt->second;
	else
	{
		printf("STATE %s NOT FOUND", stateID);
		return NULL;
	}
}

void SFMLApp::decipherMessage(SFMLStateMessage* message)
{
	switch(message->getType())
	{
	case None:
		{
		break;
		}
	case Close:
		{
		OnClose();
		break;
		}
	case Resize:
		{
		SFMLStateMessage_Resize* msg = dynamic_cast<SFMLStateMessage_Resize*>(message);
		OnResize(msg->m_width, msg->m_height);
		break;
		}
	case ChangeState:
		{
		SFMLStateMessage_ChangeState* msg = dynamic_cast<SFMLStateMessage_ChangeState*>(message);
		changeState(msg->m_stateID);
		break;
		}
	case PushState:
		{
		SFMLStateMessage_PushState* msg = dynamic_cast<SFMLStateMessage_PushState*>(message);
		pushState(msg->m_stateID);
		break;
		}
	case PopState:
		{
		SFMLStateMessage_PopState* msg = dynamic_cast<SFMLStateMessage_PopState*>(message);
		popState();
		break;
		}
	case RemoveState:
		{
		SFMLStateMessage_RemoveState* msg = dynamic_cast<SFMLStateMessage_RemoveState*>(message);
		removeState(msg->m_stateID);
		break;
		}
	}
}