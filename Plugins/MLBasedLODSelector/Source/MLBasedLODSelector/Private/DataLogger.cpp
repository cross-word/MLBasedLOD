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
    // CSV: SceneID, Distance, ScreenBound, NumTriangle, NumMatrial, MemoryUsage, ms, Lod
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

    FFileHelper::SaveStringToFile(
        LogLine,
        *LogFilePath,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        EFileWrite::FILEWRITE_Append
    );
}