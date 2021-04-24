#pragma once
#include <queue>
#include "AudioService.h"
#include <mutex>

struct AudioRequest
{
	bool isSound;
	const char* fileName;
	int volume;
};

class SimpleSDL2AudioService : public AudioService
{
public:
	SimpleSDL2AudioService();
	
	void InitAudio();

	virtual ~SimpleSDL2AudioService();
	
	virtual void PlaySound(const char* filename, int volume) override;
	virtual void PlayMusic(const char* filename, int volume) override;

	// Clean up Simple-SDL2-Audio
	void EndAudio();

	// Pause or Unpause running audio
	virtual void PauseAudio() override;
	virtual void UnpauseAudio() override;

	virtual void Update() override;
private:
	static std::deque<AudioRequest> m_EventQueue;
	static std::mutex m_Mutex;
	static std::condition_variable m_CV;
	std::thread m_AudioThread;
	static bool m_IsDone;
};