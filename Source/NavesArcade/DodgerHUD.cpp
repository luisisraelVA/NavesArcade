#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "NaveJugador.h"
#include "NaveFacade.h"
#include "NucleoEnergia.h"
#include "NavesArcadeGameMode.h"
#include "PortalSalto.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

ADodgerHUD::ADodgerHUD()
{
    static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
    if (FontObj.Succeeded()) FuentePrincipal = FontObj.Object;
}

void ADodgerHUD::BeginPlay() { Super::BeginPlay(); }

void ADodgerHUD::DrawHUD()
{
    Super::DrawHUD();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ANaveJugador* MiNave = Cast<ANaveJugador>(PlayerPawn);
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());

    if (!MiNave || !Canvas || !FuentePrincipal || !PC || !GM) return;

    UNaveFacade* Facade = MiNave->FindComponentByClass<UNaveFacade>();
    if (!Facade) return;

    float VidaReal = Facade->ObtenerVidaNave();
    int32 VidasRestantes = MiNave->GetVidas();
    int32 Puntos = MiNave->GetPuntuacion();
    float Combo = MiNave->GetCombo();
    int32 Recolectados = MiNave->GetNucleosRecolectados();
    float AnchoPantalla = Canvas->SizeX;
    float AltoPantalla = Canvas->SizeY;

    // Obtener nivel actual
    FString NombreMapa = GetWorld()->GetMapName();
    NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    int32 NivelActual = 0;
    if (NombreMapa.StartsWith("Nivel-"))
    {
        FString NumStr = NombreMapa.RightChop(6);
        NivelActual = FCString::Atoi(*NumStr);
    }


    if (VidasRestantes > 0 && VidaReal > 0)
    {
        // --- PANEL PRINCIPAL ---
        DrawRect(FLinearColor(0.0f, 0.0f, 0.05f, 0.7f), 20.0f, 20.0f, 400.0f, 160.0f);
        DrawText(FString::Printf(TEXT("VIDAS: %d"), VidasRestantes), FLinearColor::White, 40.f, 30.f, FuentePrincipal, 1.2f);
        DrawText(FString::Printf(TEXT("NIVEL: %d"), NivelActual), FLinearColor::White, 40.f, 55.f, FuentePrincipal, 1.2f);

        int32 NucleosReq = GM ? GM->GetNucleosRequeridos() : 3;
        DrawText(FString::Printf(TEXT("NUCLEOS: %d / %d"), Recolectados, NucleosReq), FLinearColor(0.0f, 1.0f, 1.0f), 40.f, 80.f, FuentePrincipal, 1.2f);

        DrawText(TEXT("INTEGRIDAD:"), FLinearColor::White, 40.f, 120.f, FuentePrincipal, 1.2f);
        FLinearColor ColorVida = VidaReal > 50 ? FLinearColor::Green : (VidaReal > 25 ? FLinearColor::Yellow : FLinearColor::Red);
        DrawRect(FLinearColor(0.1f, 0.0f, 0.0f, 0.8f), 190.0f, 125.0f, 200.0f, 15.0f);
        DrawRect(ColorVida, 190.0f, 125.0f, (VidaReal / 100.0f) * 200.0f, 15.0f);

        // --- PUNTUACIÓN Y COMBO ---
        DrawRect(FLinearColor(0.0f, 0.0f, 0.05f, 0.7f), AnchoPantalla - 320.0f, 20.0f, 300.0f, 100.0f);
        DrawText(FString::Printf(TEXT("SCORE: %d"), Puntos), FLinearColor::Yellow, AnchoPantalla - 300.0f, 30.0f, FuentePrincipal, 1.5f);
        FLinearColor ColorCombo = (Combo > 1.5f) ? FLinearColor(1.0f, 0.5f, 0.0f) : FLinearColor::White;
        DrawText(FString::Printf(TEXT("COMBO: x%.1f"), Combo), ColorCombo, AnchoPantalla - 300.0f, 75.0f, FuentePrincipal, 1.1f);

        // --- EFECTO VIDA BAJA ---
        if (VidaReal <= 25.0f)
        {
            float Alpha = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 5.0f)) * 0.3f;
            DrawRect(FLinearColor(1.0f, 0.0f, 0.0f, Alpha), 0.0f, 0.0f, AnchoPantalla, AltoPantalla);
        }

        // --- PORTAL ---
        if (Recolectados >= NucleosReq)
        {
            AActor* Portal = UGameplayStatics::GetActorOfClass(GetWorld(), APortalSalto::StaticClass());
            if (Portal)
            {
                FVector2D PosPantalla;
                if (PC->ProjectWorldLocationToScreen(Portal->GetActorLocation(), PosPantalla))
                {
                    float X = FMath::Clamp(PosPantalla.X, 50.0f, AnchoPantalla - 50.0f);
                    float Y = FMath::Clamp(PosPantalla.Y, 50.0f, AltoPantalla - 50.0f);
                    float Pulsacion = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 6.0f)) * 40.0f + 30.0f;
                    DrawLine(X - Pulsacion, Y - Pulsacion, X + Pulsacion, Y - Pulsacion, FLinearColor::Green, 3.0f);
                    DrawLine(X - Pulsacion, Y + Pulsacion, X + Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
                    DrawLine(X - Pulsacion, Y - Pulsacion, X - Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
                    DrawLine(X + Pulsacion, Y - Pulsacion, X + Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
                    DrawText(TEXT("PORTAL"), FLinearColor::Green, X - 40.0f, Y - Pulsacion - 20.0f, FuentePrincipal, 1.0f);
                }
            }
        }

        // --- NÚCLEOS ---
        TArray<AActor*> Nucleos;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), Nucleos);
        for (AActor* Nuc : Nucleos)
        {
            if (!Nuc) continue;
            FVector2D PosPantalla;
            if (PC->ProjectWorldLocationToScreen(Nuc->GetActorLocation(), PosPantalla))
            {
                float X = FMath::Clamp(PosPantalla.X, 50.0f, AnchoPantalla - 50.0f);
                float Y = FMath::Clamp(PosPantalla.Y, 50.0f, AltoPantalla - 50.0f);
                DrawLine(X - 15, Y - 15, X + 15, Y - 15, FLinearColor(0.0f, 1.0f, 1.0f), 2.0f);
                DrawLine(X - 15, Y + 15, X + 15, Y + 15, FLinearColor(0.0f, 1.0f, 1.0f), 2.0f);
                DrawLine(X - 15, Y - 15, X - 15, Y + 15, FLinearColor(0.0f, 1.0f, 1.0f), 2.0f);
                DrawLine(X + 15, Y - 15, X + 15, Y + 15, FLinearColor(0.0f, 1.0f, 1.0f), 2.0f);
                DrawText(TEXT("NUCLEO"), FLinearColor(0.0f, 1.0f, 1.0f), X - 25.0f, Y - 25.0f, FuentePrincipal, 0.8f);
            }
        }

        // --- OPTIMIZADO: LEER ENEMIGOS DIRECTOS DEL GAMEMODE ---
        DrawText(FString::Printf(TEXT("ENEMIGOS: %d"), GM->EnemigosActivos.Num()), FLinearColor::Yellow, 40.f, 190.f, FuentePrincipal, 1.2f);

        for (AActor* Enemigo : GM->EnemigosActivos)
        {
            if (!IsValid(Enemigo)) continue;

            float Distancia = FVector::Dist(MiNave->GetActorLocation(), Enemigo->GetActorLocation());
            FVector2D PosPantalla;
            bool bProyectado = PC->ProjectWorldLocationToScreen(Enemigo->GetActorLocation(), PosPantalla);
            float X, Y;
            if (bProyectado)
            {
                X = FMath::Clamp(PosPantalla.X, 30.0f, AnchoPantalla - 30.0f);
                Y = FMath::Clamp(PosPantalla.Y, 30.0f, AltoPantalla - 30.0f);
            }
            else
            {
                X = AnchoPantalla / 2.0f;
                Y = AltoPantalla / 2.0f;
            }

            DrawLine(X - 15, Y - 15, X + 15, Y - 15, FLinearColor::Red, 2.5f);
            DrawLine(X - 15, Y + 15, X + 15, Y + 15, FLinearColor::Red, 2.5f);
            DrawLine(X - 15, Y - 15, X - 15, Y + 15, FLinearColor::Red, 2.5f);
            DrawLine(X + 15, Y - 15, X + 15, Y + 15, FLinearColor::Red, 2.5f);

            FString NombreClase = Enemigo->GetClass()->GetName();
            NombreClase.RemoveFromEnd("_C");
            DrawText(NombreClase, FLinearColor::Red, X - 35.0f, Y - 35.0f, FuentePrincipal, 0.8f);

            FString DistText = FString::Printf(TEXT("%.0fm"), Distancia / 100.0f);
            DrawText(DistText, FLinearColor::Red, X - 20.0f, Y + 20.0f, FuentePrincipal, 0.8f);
        }
    }
    else if (VidaReal <= 0.0f && VidasRestantes > 0)
    {
        //DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.6f), 0.0f, 0.0f, AnchoPantalla, AltoPantalla);
        //DrawText(TEXT("RECONSTRUYENDO NAVE..."), FLinearColor::Yellow, (AnchoPantalla / 2.0f) - 250.0f, (AltoPantalla / 2.0f) - 50.0f, FuentePrincipal, 2.5f);
    }
    else if (VidasRestantes <= 0)
    {
        DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.9f), 0.0f, 0.0f, AnchoPantalla, AltoPantalla);
        float CX = AnchoPantalla / 2.0f;
        float CY = AltoPantalla / 2.0f;
        DrawText(TEXT("GAME OVER"), FLinearColor::Red, CX - 150.0f, CY - 50.0f, FuentePrincipal, 3.0f);
        DrawText(FString::Printf(TEXT("SCORE FINAL: %d"), Puntos), FLinearColor::Yellow, CX - 150.0f, CY + 20.0f, FuentePrincipal, 1.8f);
    }
}
