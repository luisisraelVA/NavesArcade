#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"
#include "DodgerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.h"
#include "AsteroideExplosivo.h"
#include "AsteroideFractal.h"
#include "NucleoEnergia.h"
#include "PortalSalto.h"
#include "Blueprint/UserWidget.h" // Necesario para la Interfaz

ANavesArcadeGameMode::ANavesArcadeGameMode()
{
    DefaultPawnClass = ANaveJugador::StaticClass();
    HUDClass = ADodgerHUD::StaticClass();
    NivelActual = 1;
    NucleosRequeridos = 3;
}

void ANavesArcadeGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 1. Obtenemos el nombre del mapa y lo limpiamos de prefijos (ej. UEDPIE_0_MapaMenu)
    FString NombreMapa = GetWorld()->GetMapName();
    NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    // =========================================================
        // RUTA A: ESTAMOS EN EL MENÚ PRINCIPAL
        // =========================================================
    if (NombreMapa.Contains("MapaMenu")) // <-- ¡Comprueba que el nombre del mapa coincida!
    {
        if (ClaseMenuWidget)
        {
            UUserWidget* MenuActual = CreateWidget<UUserWidget>(GetWorld(), ClaseMenuWidget);
            if (MenuActual)
            {
                MenuActual->AddToViewport();

                APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
                if (PC)
                {
                    // --- ESTA ES LA LÍNEA CRUCIAL PARA BORRAR EL SCORE ---
                    if (PC->GetHUD())
                    {
                        PC->GetHUD()->bShowHUD = false; // Desactivar el dibujo nativo del HUD
                    }
                    // ---------------------------------------------------

                    PC->bShowMouseCursor = true;
                    FInputModeUIOnly ModoInput;
                    ModoInput.SetWidgetToFocus(MenuActual->TakeWidget());
                    PC->SetInputMode(ModoInput);
                }
            }
        }

        return; // ¡No olvides este return! Detiene el código aquí y no crea Builder/Director.
    }

    // =========================================================
    // RUTA B: ESTAMOS JUGANDO UN NIVEL NORMAL
    // =========================================================

    // Ocultamos el mouse y devolvemos el control a la nave
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }

    // Instanciamos la arquitectura Builder/Director
    FVector PosicionBuilder = FVector(1500.0f, 0.0f, 0.0f);
    InstanciaBuilder = GetWorld()->SpawnActor<ALevelBuilder>(ALevelBuilder::StaticClass(), PosicionBuilder, FRotator::ZeroRotator);
    InstanciaDirector = GetWorld()->SpawnActor<ALevelDirector>(ALevelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (InstanciaBuilder && InstanciaDirector)
    {
        InstanciaDirector->SetBuilder(InstanciaBuilder);
        InstanciaDirector->SetDificultad(DificultadActual);

        // Detectamos en qué nivel estamos
        if (NombreMapa.StartsWith("Nivel-"))
        {
            FString NumStr = NombreMapa.RightChop(6);
            NivelActual = FCString::Atoi(*NumStr);
        }
        else
        {
            NivelActual = 1;
        }

        ActualizarRequerimientoPorNivel();
        CargarRecetaNivel(NivelActual);
    }
}

void ANavesArcadeGameMode::ActualizarRequerimientoPorNivel()
{
    if (NivelActual == 12)
    {
        NucleosRequeridos = 1;
    }
    else if (NivelActual <= 2) NucleosRequeridos = 3;
    else if (NivelActual <= 4) NucleosRequeridos = 4;
    else if (NivelActual <= 6) NucleosRequeridos = 5;
    else if (NivelActual <= 9) NucleosRequeridos = 6;
    else NucleosRequeridos = 7;
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
    if (!InstanciaBuilder || !InstanciaDirector) return;

    if (NumeroNivel == 1 || NumeroNivel == 7) InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
    else if (NumeroNivel == 2 || NumeroNivel == 8) InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
    else if (NumeroNivel == 3 || NumeroNivel == 9) InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());
    else InstanciaBuilder->SetClaseAsteroide(AAsteroideFractal::StaticClass());

    switch (NumeroNivel)
    {
    case 1: InstanciaDirector->ConstruirNivel1(); break;
    case 2: InstanciaDirector->ConstruirNivel2(); break;
    case 3: InstanciaDirector->ConstruirNivel3(); break;
    case 4: InstanciaDirector->ConstruirNivel4(); break;
    case 5: InstanciaDirector->ConstruirNivel5(); break;
    case 6: InstanciaDirector->ConstruirNivel6(); break;
    case 7: InstanciaDirector->ConstruirNivel7(); break;
    case 8: InstanciaDirector->ConstruirNivel8(); break;
    case 9: InstanciaDirector->ConstruirNivel9(); break;
    case 10: InstanciaDirector->ConstruirNivel10(); break;
    case 11: InstanciaDirector->ConstruirNivel11(); break;
    case 12: InstanciaDirector->ConstruirNivel12(); break;
    default: InstanciaDirector->ConstruirNivel1(); break;
    }
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
    int32 ProximoNivel = NivelActual + 1;
    if (ProximoNivel > NUMERO_TOTAL_NIVELES) ProximoNivel = 1;

    FString NombreCorto = FString::Printf(TEXT("Nivel-%02d"), ProximoNivel);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Loading %s..."), *NombreCorto));

    UGameplayStatics::OpenLevel(GetWorld(), FName(*NombreCorto));
}