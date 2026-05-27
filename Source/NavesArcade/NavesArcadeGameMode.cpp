#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"
#include "DodgerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "AsteroideBase.h"
#include "AsteroideDinamico.h"
#include "AsteroideErratico.h"
#include "AsteroideExplosivo.h"
#include "NucleoEnergia.h"
#include "PortalSalto.h"
#include "LevelBuilder.h"
#include "LevelDirector.h"
#include "FaseUnoFab.h"
#include "FaseFinalFab.h"
#include "FaseAvanzadaFab.h"
#include "AsteroideFractal.h"


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

    // 1. Inicializar las fábricas de fases abstractas
    FabricaFaseUno = NewObject<UFaseUnoFab>(this);
    FabricaFaseFinal = NewObject<UFaseFinalFab>(this);
    FabricaFaseAvanzada = NewObject<UFaseAvanzadaFab>(this);

    // 2. Spawnear las instancias del Builder y del Director de Niveles
    FVector PosicionBuilder = FVector(1500.0f, 0.0f, 0.0f);
    InstanciaBuilder = GetWorld()->SpawnActor<ALevelBuilder>(ALevelBuilder::StaticClass(), PosicionBuilder, FRotator::ZeroRotator);
    InstanciaDirector = GetWorld()->SpawnActor<ALevelDirector>(ALevelDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (InstanciaBuilder && InstanciaDirector)
    {
        InstanciaDirector->SetBuilder(InstanciaBuilder);
        InstanciaDirector->SetDificultad(DificultadActual);

        // --- EXTRACCIÓN DINÁMICA DEL NIVEL ---
        // Extrae automáticamente el número final del mapa (Ej: "Nivel-07" -> 7)
        FString NombreMapa = GetWorld()->GetMapName();
        NombreMapa.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

        if (NombreMapa.StartsWith("Nivel-"))
        {
            FString NumStr = NombreMapa.Right(2);
            NivelActual = FCString::Atoi(*NumStr);
        }
        else
        {
            NivelActual = 1;
        }

        // 3. Configurar reglas y cargar la receta correspondiente
        ActualizarRequerimientoPorNivel();
        CargarRecetaNivel(NivelActual);
    }
}

void ANavesArcadeGameMode::ActualizarRequerimientoPorNivel()
{
    if (NivelActual <= 2)      NucleosRequeridos = 3;
    else if (NivelActual <= 4) NucleosRequeridos = 4;
    else if (NivelActual <= 6) NucleosRequeridos = 5;
    else                       NucleosRequeridos = 6; // Exigimos 6 núcleos para los niveles difíciles 7, 8 y 9
}

void ANavesArcadeGameMode::CargarRecetaNivel(int32 NumeroNivel)
{
    if (!InstanciaBuilder || !InstanciaDirector) return;


    // Asignación de fábricas según el bloque de niveles (Patrón Abstract Factory mejorado)
    if (NumeroNivel <= 3)
        InstanciaBuilder->SetFabrica(FabricaFaseUno);
    else if (NumeroNivel <= 6)
        InstanciaBuilder->SetFabrica(FabricaFaseFinal);
    else
        InstanciaBuilder->SetFabrica(FabricaFaseAvanzada); // ? Para Niveles 7, 8 y 9 usar la del Dron Híbrido Observer
    // Configuración de tipos de asteroides por nivel
   

    if (NumeroNivel == 1)
    {
        InstanciaBuilder->SetClaseAsteroide(AAsteroideDinamico::StaticClass());
    }
    else if (NumeroNivel == 2)
    {
        InstanciaBuilder->SetClaseAsteroide(AAsteroideErratico::StaticClass());
    }
    else if (NumeroNivel == 8)
    {
        // Nivel especial fractal
        InstanciaBuilder->SetClaseAsteroide(AAsteroideFractal::StaticClass());
    }
    else
    {
        InstanciaBuilder->SetClaseAsteroide(AAsteroideExplosivo::StaticClass());
    }

    // Ejecutar la construcción en el Director (Casos independientes del 1 al 9)
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
    default: InstanciaDirector->ConstruirNivel1(); break;
    }
}

void ANavesArcadeGameMode::AvanzarSiguienteNivel()
{
    // --- CAMBIO DE NIVEL AUTOMÁTICO ---
    int32 ProximoNivel = NivelActual + 1;

    // Si pasamos el clímax del nivel 9, reiniciamos el ciclo al nivel 1
    if (ProximoNivel > 9)
    {
        ProximoNivel = 1;
    }

    // Formatea de forma segura el string (ej: si es 7 genera "Nivel-07")
    FString NombreSiguienteNivel = FString::Printf(TEXT("Nivel-%02d"), ProximoNivel);
    FName NivelSiguiente = FName(*NombreSiguienteNivel);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Cargando %s..."), *NombreSiguienteNivel));

    UGameplayStatics::OpenLevel(GetWorld(), NivelSiguiente);
}

void ANavesArcadeGameMode::LimpiarMapa()
{
    TArray<AActor*> Asteroides;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAsteroideBase::StaticClass(), Asteroides);
    for (AActor* Asteroide : Asteroides) if (Asteroide) Asteroide->Destroy();

    TArray<AActor*> Energias;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), Energias);
    for (AActor* Energia : Energias) if (Energia) Energia->Destroy();

    TArray<AActor*> Portales;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortalSalto::StaticClass(), Portales);
    for (AActor* Portal : Portales) if (Portal) Portal->Destroy();
}