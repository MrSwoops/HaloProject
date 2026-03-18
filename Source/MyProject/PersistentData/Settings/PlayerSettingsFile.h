#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PlayerSettingsFile.generated.h"

class UFMODVCA;

UCLASS(BlueprintType)
class UPlayerSettingsFile : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UPlayerSettingsFile* GetPlayerSettings();

protected:
	virtual void LoadSettings(bool bForceReload = false) override;

#pragma region Audio
public:
	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetMasterVolume(const float InVolume);
	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetMusicVolume(const float InVolume);
	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetSFXVolume(const float InVolume);
	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetVoiceVolume(const float InVolume);
	UFUNCTION(BlueprintCallable, Category=Settings)
	void SetSubtitlesEnabled(const bool bIsOn);
	UFUNCTION(BlueprintCallable, Category=Settings)
	float GetMasterVolume() const {return MasterVolume;}
	UFUNCTION(BlueprintCallable, Category=Settings)
	float GetMusicVolume() const {return MusicVolume;}
	UFUNCTION(BlueprintCallable, Category=Settings)
	float GetSFXVolume() const {return SFXVolume;}
	UFUNCTION(BlueprintCallable, Category=Settings)
	float GetVoiceVolume() const {return VoiceVolume;}
	UFUNCTION(BlueprintCallable, Category=Settings)
	bool GetSubtitlesEnabled() const {return bSubtitlesEnabled;}
protected:
	UPROPERTY(Config)
	float MasterVolume = 8.0f;
	UPROPERTY(Config)
	float MusicVolume = 8.0f;
	UPROPERTY(Config)
	float SFXVolume = 8.0f;
	UPROPERTY(Config)
	float VoiceVolume = 8.0f;
	UPROPERTY(Config)
	bool bSubtitlesEnabled = true;
	
private:
	inline float GetVolumeDB(float InVolume);
	void RetrieveVCAs();
	UPROPERTY()
	UFMODVCA* MasterVca;
	UPROPERTY()
	UFMODVCA* MusicVca;
	UPROPERTY()
	UFMODVCA* SfxVca;
	UPROPERTY()
	UFMODVCA* VoiceVca;
#pragma endregion Audio
	
};
