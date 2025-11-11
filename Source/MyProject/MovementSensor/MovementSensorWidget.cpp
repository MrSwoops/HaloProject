// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementSensorWidget.h"
#include "MovementSensorComponent.h"

#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "MyProject/GameplayCharacter.h"

void UMovementSensorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (IndicatorImage)
    {
        DotBrush.SetResourceObject(IndicatorImage);
        DotBrush.ImageSize = FVector2D(6.f, 6.f);
    }
}


int32 UMovementSensorWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                         int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 CurrentLayer = Super::NativePaint(
        Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled
    );

    if (!MovementSensor || !BackgroundImage)
        return CurrentLayer;

    AGameplayCharacter* Owner = Cast<AGameplayCharacter>(MovementSensor->GetOwner());
    if (!Owner)
        return CurrentLayer;
    
    const FVector PlayerLoc = Owner->GetActorLocation();
    const FRotator PlayerRot = Owner->Controller->GetControlRotation();
    const int32 PlayerTeam = Owner->Team;

    //------------------------------------------------------------
    // 1. Get BackgroundImage size and position relative to overlay
    //------------------------------------------------------------
    const FVector2D BGTopLeft = AllottedGeometry.AbsoluteToLocal(
    BackgroundImage->GetCachedGeometry().GetAbsolutePosition()
);
    const FVector2D BGSize = BackgroundImage->GetCachedGeometry().GetLocalSize();
    // Center of radar in widget local space
    const FVector2D ImageCenterInWidget = BGTopLeft + BGSize * 0.5f;

    // Use smaller dimension for circle radius
    const float RadarRadiusPixels = FMath::Min(BGSize.X, BGSize.Y) * 0.5f;

    // World units -> pixels scale
    const float Scale = RadarRadiusPixels / MovementSensor->ScanRadius;

    //------------------------------------------------------------
    // 2. Draw detected actors
    //------------------------------------------------------------
    const FSlateBrush* Brush = &DotBrush;
    const FVector2D DotHalfSize = Brush->ImageSize * 0.5f;

    for (const FDetectedActorInfo& Info : MovementSensor->GetDetectedActors())
    {
        if (!Info.Actor)
            continue;

        FVector ToTarget = Info.Location - PlayerLoc;
        ToTarget.Z = 0.f;

        // Rotate by negative player yaw (forward = up)
        const float YawRad = FMath::DegreesToRadians(-PlayerRot.Yaw);
        const float CosYaw = FMath::Cos(YawRad);
        const float SinYaw = FMath::Sin(YawRad);

        const float LocalX = ToTarget.X * CosYaw - ToTarget.Y * SinYaw;
        const float LocalY = ToTarget.X * SinYaw + ToTarget.Y * CosYaw;

        FVector2D DotPos(LocalY, -LocalX);
        DotPos *= Scale;

        // Clamp to radar circle
        if (DotPos.Size() > RadarRadiusPixels)
            DotPos = DotPos.GetSafeNormal() * RadarRadiusPixels;

        // Final dot position relative to BackgroundImage
        const FVector2D ScreenPos = ImageCenterInWidget + DotPos;

        const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(ScreenPos - DotHalfSize, Brush->ImageSize);

        FSlateDrawElement::MakeBox(
            OutDrawElements,
            ++CurrentLayer,
            Geo,
            Brush,
            ESlateDrawEffect::None,
            (PlayerTeam == Info.Actor->Team) ? FLinearColor::Yellow : FLinearColor::Red
        );
    }

// #if WITH_EDITOR
//     // Optional: debug radar circle
//     FSlateDrawElement::MakeCircle(
//         OutDrawElements,
//         ++CurrentLayer,
//         AllottedGeometry.ToPaintGeometry(
//             ImageCenterInWidget,
//             FVector2D(RadarRadiusPixels * 2.f, RadarRadiusPixels * 2.f)
//         ),
//         nullptr,
//         ESlateDrawEffect::None,
//         FLinearColor(0.f, 1.f, 0.f, 0.15f)
//     );
// #endif

    return CurrentLayer;
}
// {
//     int32 CurrentLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
//
//     if (!MovementSensor || !BackgroundImage)
//         return CurrentLayer;
//
//     AActor* Owner = MovementSensor->GetOwner();
//     if (!Owner)
//         return CurrentLayer;
//
//     const FVector PlayerLoc = Owner->GetActorLocation();
//     const FRotator PlayerRot = Owner->GetActorRotation();
//     const int32 PlayerTeam = Cast<AGameplayCharacter>(Owner)->Team;
//
//     // --- Get radar background image geometry ---
//     const FGeometry& BGGeo = BackgroundImage->GetCachedGeometry();
//
//     // Convert image center into widget local space
//     const FVector2D ImageCenterInWidget = AllottedGeometry.AbsoluteToLocal(
//         BGGeo.LocalToAbsolute(BGGeo.GetLocalSize() * 0.5f)
//     );
//
//     // Radar scale: world units -> pixels
//     const float RadarRadiusPixels = FMath::Min(BGGeo.GetLocalSize().X, BGGeo.GetLocalSize().Y) * 0.5f;
//     const float Scale = RadarRadiusPixels / MovementSensor->ScanRadius;
//
//     const FSlateBrush* Brush = &DotBrush;
//     // --- Draw each detected actor ---
//     for (const FDetectedActorInfo& Info : MovementSensor->DetectedActors)
//     {
//         if (!Info.Actor)
//             continue;
//
//         // Vector from player to target, ignore Z
//         FVector ToTarget = Info.Location - PlayerLoc;
//         ToTarget.Z = 0.f;
//
//         // Rotate by negative player yaw to align forward = up
//         float YawRad = FMath::DegreesToRadians(-PlayerRot.Yaw);
//         float CosYaw = FMath::Cos(YawRad);
//         float SinYaw = FMath::Sin(YawRad);
//
//         float LocalX = ToTarget.X * CosYaw - ToTarget.Y * SinYaw; // forward/back
//         float LocalY = ToTarget.X * SinYaw + ToTarget.Y * CosYaw; // right/left
//
//         FVector2D DotPos(LocalY, -LocalX);
//         DotPos *= Scale;
//
//         // Clamp to radar radius
//         if (DotPos.Size() > RadarRadiusPixels)
//             DotPos = DotPos.GetSafeNormal() * RadarRadiusPixels;
//
//         FVector2D ScreenPos = ImageCenterInWidget + DotPos;
//
//         FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(ScreenPos - Brush->ImageSize * 0.5f, Brush->ImageSize);
//
//         FSlateDrawElement::MakeBox(
//             OutDrawElements,
//             ++CurrentLayer,
//             Geo,
//             Brush,
//             ESlateDrawEffect::None,
//             (PlayerTeam == Info.Actor->Team) ? FLinearColor::Yellow : FLinearColor::Red
//         );
//     }
//
//     return CurrentLayer;
// }

void UMovementSensorWidget::Repaint()
{
	Invalidate(EInvalidateWidgetReason::Paint);
}

