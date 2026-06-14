#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "NaveJugador.h"
#include "NavesArcadeGameMode.h"
#include "NucleoEnergia.h"
#include "NaveNodriza.h"

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

                DrawLine(X, Y, X + TamanoCaja, Y, ColorNucleo, 1.5f);
                DrawLine(X, Y + TamanoCaja, X + TamanoCaja, Y + TamanoCaja, ColorNucleo, 1.5f);
                DrawLine(X, Y, X, Y + TamanoCaja, ColorNucleo, 1.5f);
                DrawLine(X + TamanoCaja, Y, X + TamanoCaja, Y + TamanoCaja, ColorNucleo, 1.5f);
                DrawText(TEXT("NUCLEO"), ColorNucleo, X - 15.0f, Y - 20.0f, FuenteUsar, 0.9f);
            }
        }
    }

    // =====================================================================
    // INTERFAZ: PANELES
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

    float PosXDerecha = ScreenX - 350.0f;
    DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), PosXDerecha - 20.0f, 25.0f, 350.0f, 85.0f);
    DrawText(FString::Printf(TEXT("SCORE: %06d"), Jugador->GetPuntuacion()), FLinearColor(1.0f, 0.85f, 0.0f, 1.0f), PosXDerecha, 35.0f, FuenteUsar, 1.4f);

    FLinearColor ColorCombo = (Jugador->GetCombo() > 1.0f) ? FLinearColor(1.0f, 0.0f, 1.0f, 1.0f) : FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);
    DrawText(FString::Printf(TEXT("MULTIPLIER: x%.1f"), Jugador->GetCombo()), ColorCombo, PosXDerecha, 70.0f, FuenteUsar, 1.1f);

    float PosYInferior = ScreenY - 80.0f;
    int32 NucRecolectados = Jugador->GetNucleosRecolectados();
    int32 NucRequeridos = GameMode->GetNucleosRequeridos();
    FLinearColor ColorNucleos = (NucRecolectados >= NucRequeridos) ? FLinearColor(0.0f, 1.0f, 0.0f, 1.0f) : FLinearColor::White;
    DrawText(FString::Printf(TEXT("NUCLEOS RECOLECTADOS: %d / %d"), NucRecolectados, NucRequeridos), ColorNucleos, 40.0f, PosYInferior, FuenteUsar, 1.2f);

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
        DrawText(TEXT("PAUSA - PRESIONA P PARA SEGUIR | M PARA VOLVER AL MENU"), FLinearColor::Yellow, ScreenX / 2.0f - 300.0f, ScreenY / 2.0f, FuenteUsar, 1.2f);
    }

    // =====================================================================
        // CROSSHAIR DINÁMICO (MIRA INTELIGENTE)
        // =====================================================================
    if (!UGameplayStatics::IsGamePaused(GetWorld()))
    {
        // 1. Calculamos el punto exacto en el espacio 3D hacia donde disparará la nave
        // (8000 unidades hacia adelante desde la nariz de la nave)
        FVector PuntoApuntado = Jugador->GetActorLocation() + (Jugador->GetActorForwardVector() * 8000.0f);

        // 2. Convertimos ese punto 3D lejano a coordenadas 2D en tu pantalla
        FVector PosicionMira = Project(PuntoApuntado);

        // 3. Solo dibujamos la mira si ese punto está frente a la cámara (Z > 0)
        if (PosicionMira.Z > 0.0f)
        {
            float CentroX = PosicionMira.X;
            float CentroY = PosicionMira.Y;

            float L = 15.0f; // Tamaño de las líneas
            float G = 2.0f;  // Grosor

            FLinearColor ColorMira = FLinearColor(0.0f, 1.0f, 0.0f, 0.8f); // Verde brillante

            // Dibujamos las 4 líneas guiadas a la posición real
            DrawLine(CentroX - L - 5.0f, CentroY, CentroX - 5.0f, CentroY, ColorMira, G);
            DrawLine(CentroX + 5.0f, CentroY, CentroX + L + 5.0f, CentroY, ColorMira, G);
            DrawLine(CentroX, CentroY - L - 5.0f, CentroX, CentroY - 5.0f, ColorMira, G);
            DrawLine(CentroX, CentroY + 5.0f, CentroX, CentroY + L + 5.0f, ColorMira, G);

            // Punto rojo indicador de impacto directo
            DrawRect(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), CentroX - 1.5f, CentroY - 1.5f, 3.0f, 3.0f);
        }
    }
    // =====================================================================
      // BARRA DE VIDA DEL JEFE FINAL (NAVE NODRIZA) - NIVEL 12
      // =====================================================================
    if (GameMode->GetNivelActual() >= 12 && VidaActual > 0.0f)
    {
        // Buscamos a la Nodriza entre los enemigos activos
        for (AActor* Enemigo : GameMode->EnemigosActivos)
        {
            ANaveNodriza* Nodriza = Cast<ANaveNodriza>(Enemigo);
            if (Nodriza && !Nodriza->IsActorBeingDestroyed())
            {
                float VidaJefe = Nodriza->GetVidaActual();
                float MaxVida = Nodriza->GetVidaMaxima();

                if (VidaJefe > 0.0f)
                {
                    // ELIMINAMOS LA LÍNEA REPETIDA AQUÍ. Usamos el CentroPantallaX que ya existe.
                    float AnchoBarra = 700.0f;
                    float AltoBarra = 25.0f;
                    float PosX = CentroPantallaX - (AnchoBarra / 2.0f);
                    float PosY = 40.0f; // Arriba en la pantalla

                    // 1. Dibujamos el borde oscuro/marco
                    DrawRect(FLinearColor(0.05f, 0.0f, 0.0f, 0.9f), PosX - 4.0f, PosY - 4.0f, AnchoBarra + 8.0f, AltoBarra + 8.0f);

                    // 2. Calculamos el porcentaje y el color (Pasa de Morado a Rojo)
                    float Porcentaje = FMath::Clamp(VidaJefe / MaxVida, 0.0f, 1.0f);
                    FLinearColor ColorJefe = (Porcentaje > 0.5f) ? FLinearColor(0.6f, 0.0f, 1.0f, 1.0f) : FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

                    // 3. Dibujamos la barra de vida que se vacía
                    DrawRect(ColorJefe, PosX, PosY, AnchoBarra * Porcentaje, AltoBarra);

                    // 4. Textos épicos
                    DrawText(TEXT("NAVE NODRIZA - AMENAZA CLASE OMEGA"), FLinearColor::White, PosX, PosY - 25.0f, FuenteUsar, 1.3f);
                    DrawText(FString::Printf(TEXT("%.0f / %.0f"), VidaJefe, MaxVida), FLinearColor::White, CentroPantallaX - 50.0f, PosY + 2.0f, FuenteUsar, 1.0f);
                }
                break; // Una vez dibujado el jefe, dejamos de buscar
            }
        }
    }
}