#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "NucleoEnergia.h"

ADodgerHUD::ADodgerHUD()
{
    static ConstructorHelpers::FObjectFinder<UFont> FontAsset(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
    if (FontAsset.Succeeded())
    {
        FuentePrincipal = FontAsset.Object;
    }
}

void ADodgerHUD::BeginPlay()
{
    Super::BeginPlay();
}

void ADodgerHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!GetWorld() || !Canvas) return;

    FString NombreMapa = GetWorld()->GetMapName();
    NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    if (NombreMapa.Contains("MapaMenu"))
    {
        return;
    }

    ANaveJugador* Jugador = Cast<ANaveJugador>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    ANavesArcadeGameMode* GameMode = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());

    if (!Jugador || !GameMode) return;

    UFont* FuenteUsar = FuentePrincipal ? FuentePrincipal : GEngine->GetMediumFont();
    float ScreenX = Canvas->SizeX;
    float ScreenY = Canvas->SizeY;
    float VidaActual = Jugador->GetIntegridadEstructural();
    int32 VidasRestantes = Jugador->GetVidas();

    // =====================================================================
    // PANTALLAS CINEMATOGRAFICAS DE MUERTE Y GAME OVER
    // =====================================================================
    if (VidaActual <= 0.0f)
    {
        DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.85f), 0.0f, 0.0f, ScreenX, ScreenY);

        float CentroX = ScreenX / 2.0f;
        float CentroY = ScreenY / 2.0f;

        if (VidasRestantes > 0)
        {
            DrawText(TEXT("NAVE DESTRUIDA"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), CentroX - 150.0f, CentroY - 50.0f, FuenteUsar, 2.0f);
            DrawText(FString::Printf(TEXT("REENSAMBLANDO... VIDAS RESTANTES: %d"), VidasRestantes), FLinearColor::White, CentroX - 220.0f, CentroY + 20.0f, FuenteUsar, 1.2f);
        }
        else
        {
            DrawText(TEXT("GAME OVER"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), CentroX - 130.0f, CentroY - 80.0f, FuenteUsar, 2.5f);
            DrawText(FString::Printf(TEXT("SCORE FINAL: %06d"), Jugador->GetPuntuacion()), FLinearColor(1.0f, 0.85f, 0.0f, 1.0f), CentroX - 160.0f, CentroY + 10.0f, FuenteUsar, 1.8f);
            DrawText(TEXT("REINICIANDO SIMULACION..."), FLinearColor(0.5f, 0.5f, 0.5f, 1.0f), CentroX - 120.0f, CentroY + 70.0f, FuenteUsar, 1.0f);
        }

        return;
    }

    // =====================================================================
    // SISTEMA DE RADAR HOLOGRAFICO (CUADROS Y NOMBRES DE ENEMIGOS)
    // =====================================================================
    for (AActor* Enemigo : GameMode->EnemigosActivos)
    {
        if (Enemigo && !Enemigo->IsActorBeingDestroyed())
        {
            FVector PosicionPantalla = Project(Enemigo->GetActorLocation());

            if (PosicionPantalla.Z > 0.0f)
            {
                float TamanoCaja = 50.0f;
                float X = PosicionPantalla.X - (TamanoCaja / 2.0f);
                float Y = PosicionPantalla.Y - (TamanoCaja / 2.0f);
                FLinearColor ColorEnemigo = FLinearColor(1.0f, 0.1f, 0.1f, 0.7f);

                DrawLine(X, Y, X + TamanoCaja, Y, ColorEnemigo, 1.5f);
                DrawLine(X, Y + TamanoCaja, X + TamanoCaja, Y + TamanoCaja, ColorEnemigo, 1.5f);
                DrawLine(X, Y, X, Y + TamanoCaja, ColorEnemigo, 1.5f);
                DrawLine(X + TamanoCaja, Y, X + TamanoCaja, Y + TamanoCaja, ColorEnemigo, 1.5f);

                FString NombreEnemigo = Enemigo->GetClass()->GetName();
                NombreEnemigo.RemoveFromStart(TEXT("A"));

                DrawText(NombreEnemigo, FLinearColor(1.0f, 0.5f, 0.5f, 1.0f), X - 10.0f, Y - 20.0f, FuenteUsar, 0.8f);

                float Distancia = FVector::Dist(Jugador->GetActorLocation(), Enemigo->GetActorLocation()) / 100.0f;
                DrawText(FString::Printf(TEXT("%.0fm"), Distancia), ColorEnemigo, X + 5.0f, Y + TamanoCaja + 5.0f, FuenteUsar, 0.8f);
            }
        }
    }

    // =====================================================================
    // SISTEMA DE RADAR HOLOGRAFICO (NÚCLEOS DE ENERGÍA)
    // =====================================================================
    if (GetWorld()->GetTimeSeconds() - TiempoUltimoCacheo > 0.5f)
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), NucleosCacheados);
        TiempoUltimoCacheo = GetWorld()->GetTimeSeconds();
    }

    for (AActor* Nucleo : NucleosCacheados)
    {
        if (Nucleo && !Nucleo->IsActorBeingDestroyed())
        {
            FVector PosicionPantalla = Project(Nucleo->GetActorLocation());

            if (PosicionPantalla.Z > 0.0f)
            {
                float TamanoCaja = 30.0f;
                float X = PosicionPantalla.X - (TamanoCaja / 2.0f);
                float Y = PosicionPantalla.Y - (TamanoCaja / 2.0f);
                FLinearColor ColorNucleo = FLinearColor(0.0f, 0.8f, 1.0f, 0.8f);

                // Cuadro del radar
                DrawLine(X, Y, X + TamanoCaja, Y, ColorNucleo, 1.5f);
                DrawLine(X, Y + TamanoCaja, X + TamanoCaja, Y + TamanoCaja, ColorNucleo, 1.5f);
                DrawLine(X, Y, X, Y + TamanoCaja, ColorNucleo, 1.5f);
                DrawLine(X + TamanoCaja, Y, X + TamanoCaja, Y + TamanoCaja, ColorNucleo, 1.5f);

                // --- MODIFICACIÓN: Texto indicador encima del cuadro del núcleo ---
                DrawText(TEXT("NUCLEO"), ColorNucleo, X - 15.0f, Y - 20.0f, FuenteUsar, 0.9f);
            }
        }
    }

    // =====================================================================
    // INTERFAZ: PANEL IZQUIERDO (Vida, Enemigos, Sector)
    // =====================================================================
    float PosX_Izq = 40.0f;
    float PosY_Izq = 60.0f;

    DrawRect(FLinearColor(0.05f, 0.05f, 0.05f, 1.0f), PosX_Izq, PosY_Izq, 350.0f, 18.0f);
    float AnchoActualBarra = (FMath::Clamp(VidaActual, 0.0f, 100.0f) / 100.0f) * 350.0f;
    FLinearColor ColorBarra = (VidaActual > 30.0f) ? FLinearColor(0.0f, 1.0f, 0.4f, 1.0f) : FLinearColor(1.0f, 0.0f, 0.1f, 1.0f);
    DrawRect(ColorBarra, PosX_Izq, PosY_Izq, AnchoActualBarra, 18.0f);

    DrawText(FString::Printf(TEXT("INTEGRIDAD: %.0f%%"), VidaActual), FLinearColor::White, PosX_Izq, PosY_Izq - 25.0f, FuenteUsar, 1.1f);
    DrawText(FString::Printf(TEXT("VIDAS RESTANTES: %d"), VidasRestantes), FLinearColor(0.0f, 0.8f, 1.0f, 1.0f), PosX_Izq, PosY_Izq + 30.0f, FuenteUsar, 1.0f);

    int32 EnemigosRestantes = GameMode->EnemigosActivos.Num();
    DrawText(FString::Printf(TEXT("ENEMIGOS RESTANTES: %d"), EnemigosRestantes), FLinearColor(1.0f, 0.4f, 0.4f, 1.0f), PosX_Izq, PosY_Izq + 60.0f, FuenteUsar, 1.0f);
    DrawText(FString::Printf(TEXT("SECTOR ACTUAL: %d"), GameMode->GetNivelActual()), FLinearColor(0.8f, 0.2f, 1.0f, 1.0f), PosX_Izq, PosY_Izq + 90.0f, FuenteUsar, 1.0f);

    // =====================================================================
    // INTERFAZ: PANEL DERECHO (Puntaje con fondo solido)
    // =====================================================================
    float PosXDerecha = ScreenX - 350.0f;

    DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), PosXDerecha - 20.0f, 25.0f, 350.0f, 85.0f);
    DrawText(FString::Printf(TEXT("SCORE: %06d"), Jugador->GetPuntuacion()), FLinearColor(1.0f, 0.85f, 0.0f, 1.0f), PosXDerecha, 35.0f, FuenteUsar, 1.4f);

    FLinearColor ColorCombo = (Jugador->GetCombo() > 1.0f) ? FLinearColor(1.0f, 0.0f, 1.0f, 1.0f) : FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);
    DrawText(FString::Printf(TEXT("MULTIPLIER: x%.1f"), Jugador->GetCombo()), ColorCombo, PosXDerecha, 70.0f, FuenteUsar, 1.1f);

    // =====================================================================
    // INTERFAZ: PANEL INFERIOR IZQUIERDO (Inventario/Nucleos)
    // =====================================================================
    float PosYInferior = ScreenY - 80.0f;
    int32 NucRecolectados = Jugador->GetNucleosRecolectados();
    int32 NucRequeridos = GameMode->GetNucleosRequeridos();
    FLinearColor ColorNucleos = (NucRecolectados >= NucRequeridos) ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor::White;

    DrawText(FString::Printf(TEXT("NUCLEOS RECOLECTADOS: %d / %d"), NucRecolectados, NucRequeridos), ColorNucleos, 40.0f, PosYInferior, FuenteUsar, 1.2f);

    // =====================================================================
    // AVISOS Y ALERTAS (Centro de la pantalla arriba)
    // =====================================================================
    float CentroPantallaX = ScreenX / 2.0f;

    if (VidaActual <= 30.0f)
    {
        float AlphaAlerta = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 6.0f));
        DrawText(TEXT("¡ALERTA CRITICA!"), FLinearColor(1.0f, 0.0f, 0.0f, AlphaAlerta), CentroPantallaX - 120.0f, 50.0f, FuenteUsar, 1.5f);
    }

    if (NucRecolectados >= NucRequeridos)
    {
        float AlphaPortal = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 4.0f));
        DrawText(TEXT("¡PORTAL ABIERTO - ESCAPA!"), FLinearColor(0.0f, 1.0f, 0.0f, AlphaPortal), CentroPantallaX - 180.0f, 90.0f, FuenteUsar, 1.5f);
    }


    if (UGameplayStatics::IsGamePaused(GetWorld()))
    {
        DrawText(TEXT("PAUSA - PRESIONA P PARA SEGUIR | M PARA VOLVER AL MENU"),
            FLinearColor::Yellow,
            ScreenX / 2.0f - 300.0f,
            ScreenY / 2.0f,
            FuenteUsar, 1.2f);
    }
}