#include "DataLogger.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UDataLogger& UDataLogger::Get()
{
    static UDataLogger* Singleton = NewObject<UDataLogger>();
    Singleton->AddToRoot();
    return *Singleton;
}

void UDataLogger::StartLogging()
{
    if (bIsLogging)
        return;

    bIsLogging = true;

    // 로그 파일 경로 설정
    LogFilePath = FPaths::Combine(
        FPaths::ProjectSavedDir(),
        TEXT("MLLogs"),
        TEXT("ActorDataLog.csv")
    );

    // 폴더가 없으면 생성
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.CreateDirectoryTree(*FPaths::GetPath(LogFilePath));

    // CSV 헤더
    FString Header = TEXT("sceneID,Distance,ScreenBound,NumTriangle,NumMatrial,MemoryUsage,fps,LodBiasPred\n");
    FFileHelper::SaveStringToFile(Header, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

void UDataLogger::StopLogging()
{
    bIsLogging = false;
}

void UDataLogger::LogData(
    FString Scene_id,
    float Distance,
    float ScreenBound,
    float NumTriangle,
    float NumMatrial,
    float MemoryUsage,
    float fps,
    float LodBiasPred
) {
    // 잠시멈춤기능 현재는 안씀
    //if (!bIsLogging || !Actor)
    //{
    //   return;
    //}

    // CSV: DeltaTime, ActorName, Distance, CosAngle, BoundingRadius, CullLabel, LODBias
    FString LogLine = FString::Printf(
        TEXT("%s,%f,%f,%f,%f,%f,%f,%f\n"),
        *Scene_id,
        Distance,
        ScreenBound,
        NumTriangle,
        NumMatrial,
        MemoryUsage,
        fps,
        LodBiasPred
    );
    // 파일에 append
    FFileHelper::SaveStringToFile(
        LogLine,
        *LogFilePath,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        EFileWrite::FILEWRITE_Append
    );
}

/*
TODO : 저장할 때 파일명 겹치면 덮어씌지 않고 새로 만들어 저장하게
*/