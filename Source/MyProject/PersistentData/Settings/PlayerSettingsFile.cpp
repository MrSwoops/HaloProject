#include "PlayerSettingsFile.h"

#include "FMODBlueprintStatics.h"
#include "FMODVCA.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerSettingsFile* UPlayerSettingsFile::GetPlayerSettings()
{
	if (GEngine) return Cast<UPlayerSettingsFile>(GEngine->GetGameUserSettings());
	return nullptr;
}

void UPlayerSettingsFile::LoadSettings(bool bForceReload)
{
	Super::LoadSettings(bForceReload);

	RetrieveVCAs();
	SetMasterVolume(MasterVolume);
	SetMusicVolume(MusicVolume);
	SetSFXVolume(SFXVolume);
	SetVoiceVolume(VoiceVolume);
	SetSubtitlesEnabled(bSubtitlesEnabled);
}

void UPlayerSettingsFile::SetMasterVolume(const float InVolume)
{
	MasterVolume = InVolume;
	if (MasterVca) UFMODBlueprintStatics::VCASetVolume(MasterVca, GetVolumeDB(MasterVolume));
}
void UPlayerSettingsFile::SetMusicVolume(const float InVolume)
{
	MusicVolume = InVolume;
	if (MusicVca) UFMODBlueprintStatics::VCASetVolume(MusicVca, GetVolumeDB(MusicVolume));
}
void UPlayerSettingsFile::SetSFXVolume(const float InVolume)
{
	SFXVolume = InVolume;
	if (SfxVca) UFMODBlueprintStatics::VCASetVolume(SfxVca, GetVolumeDB(SFXVolume));
}
void UPlayerSettingsFile::SetVoiceVolume(const float InVolume)
{
	VoiceVolume = InVolume;
	if (VoiceVca) UFMODBlueprintStatics::VCASetVolume(VoiceVca, GetVolumeDB(VoiceVolume));
}
void UPlayerSettingsFile::SetSubtitlesEnabled(const bool bIsOn)
{
	bSubtitlesEnabled = bIsOn;
}

float UPlayerSettingsFile::GetVolumeDB(float InVolume)
{
	float Normalized = FMath::Clamp(InVolume / 10.0f, 0.0f, 1.0f);
	return FMath::Pow(Normalized, 2.0f);
}

void UPlayerSettingsFile::RetrieveVCAs()
{
	auto* masVCA = LoadObject<UFMODVCA>(nullptr, TEXT("/Game/FMOD/VCAs/Master_VCA.Master_VCA"));
	if (masVCA) MasterVca = masVCA;
	auto* musVCA = LoadObject<UFMODVCA>(nullptr, TEXT("/Game/FMOD/VCAs/Music_VCA.Music_VCA"));
	if (musVCA) MusicVca = musVCA;
	auto* soundVCA = LoadObject<UFMODVCA>(nullptr, TEXT("/Game/FMOD/VCAs/SFX_VCA.SFX_VCA"));
	if (soundVCA) SfxVca = soundVCA;
	auto* voiceVCA = LoadObject<UFMODVCA>(nullptr, TEXT("/Game/FMOD/VCAs/Voice_VCA.Voice_VCA"));
	if (voiceVCA) VoiceVca = voiceVCA;
}
