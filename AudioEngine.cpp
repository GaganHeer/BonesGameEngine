#include "AudioEngine.h"

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
	result = sys->initialize(512, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, FMOD_INIT_NORMAL, 0);
	ErrorHandler(result, "Error in init FMOD", "Successfully init FMOD obj");

	result = sys->loadBankFile("Audio//Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	ErrorHandler(result, "Couldn't load bank file", "Successfully loaded bank file");
}

void AudioEngine::sfx(const char* GUID) {
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
	//result = inst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	//ErrorHandler(result, "Failed to stop?");
	inst->release();


	//int length;
	//desc->getLength(&length);
	//cout << length << " :LENGTH";
	//DWORD dw = (DWORD)length;
	//Sleep(dw);
	//sys->release();
}

void AudioEngine::update() {
	result = sys->update();
}