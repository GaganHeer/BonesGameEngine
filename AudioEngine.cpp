#include "AudioEngine.h"

// {e4271e0b-f029-4234-8e24-058ea31e5847} Fight Music
// {502d9394-7c59-4191-b0c3-ba345f25da6d} Dungeon Music
// {afdb6b8e-6f22-4209-a949-1e85da29cfc2} death sound
// {21448bf9-0ba1-407b-93a7-5b2080f629c8} bone attack sound
// {3c16d103-ccc1-464c-8b49-77d1b99bddda} test looping sound
// {fec55164-3b98-462e-9377-2196815b9da1} enemy attack sound


void ErrorHandler(FMOD_RESULT result, const char* errorMsg, const char* successMsg = "Funk") {
	if (result != FMOD_OK) {
		cout << errorMsg << "\n";
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	cout << successMsg << "\n";
}

FMOD_RESULT result;
Studio::System* sys = NULL;
Studio::Bank* bank;

void AudioEngine::setup() {

	result = Studio::System::create(&sys); // Create the Studio System object.
	ErrorHandler(result, "Error in creating FMOD obj", "Successfully created FMOD obj");

	// Initialize FMOD Studio, which will also initialize FMOD Core
	result = sys->initialize(512, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, FMOD_LOOP_NORMAL, 0);
	ErrorHandler(result, "Error in init FMOD", "Successfully init FMOD obj");

	result = sys->loadBankFile("Audio//Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	ErrorHandler(result, "Couldn't load bank file", "Successfully loaded bank file");
}

Studio::EventInstance* play(const char* GUID) {
	Studio::EventDescription* desc;
	Studio::EventInstance* inst;

	result = sys->getEvent(GUID, &desc);
	ErrorHandler(result, "Couldn't find fmod GUID", "Successfully found fmod GUID");
	result = desc->createInstance(&inst);
	ErrorHandler(result, "Couldn't create inst", "Successfully created inst");

	result = inst->start();
	ErrorHandler(result, "failed to start audio file", "started audio file");

	sys->update();
	ErrorHandler(result, "Failed to update");
	inst->release();

	return(inst);
}

void AudioEngine::stopAudio(Studio::EventInstance* inst) {
	inst->stop(FMOD_STUDIO_STOP_IMMEDIATE);
}

void AudioEngine::testLooping() {
	play("{3c16d103-ccc1-464c-8b49-77d1b99bddda}");
}

Studio::EventInstance* AudioEngine::startDungeonBGM() {
	return(play("{502d9394-7c59-4191-b0c3-ba345f25da6d}"));
}

Studio::EventInstance* AudioEngine::startFightBGM() {
	return(play("{e4271e0b-f029-4234-8e24-058ea31e5847}"));
}

void AudioEngine::playerAtk() {
	play("{21448bf9-0ba1-407b-93a7-5b2080f629c8}");
}
void AudioEngine::enemyAtk() {
	play("{fec55164-3b98-462e-9377-2196815b9da1}");
}
void AudioEngine::enemyDeath() {
	play("{21448bf9-0ba1-407b-93a7-5b2080f629c8}");
}

void AudioEngine::update() {
	result = sys->update();
}