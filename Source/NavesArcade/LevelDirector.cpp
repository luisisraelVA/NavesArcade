#include "LevelDirector.h"
#include "LevelBuilder.h"
#include "EnemyFactory.h"
#include "Kismet/GameplayStatics.h"
#include "NavesArcadeGameMode.h"
#include "Engine/World.h"

ALevelDirector::ALevelDirector() {
    PrimaryActorTick.bCanEverTick = false;
    Builder = nullptr;
    Dificultad = EDificultad::Medio;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder) {
    Builder = NuevoBuilder;
    if (Builder) Builder->OnGenerarFase.BindUObject(this, &ALevelDirector::ManejarGeneracionFase);
}

void ALevelDirector::SetDificultad(EDificultad NuevaDificultad) { Dificultad = NuevaDificultad; }

void ALevelDirector::ManejarGeneracionFase(int32 FaseActual)
{
    if (!Builder || !GetWorld()) return;
    Builder->ResetEnemigos();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (!PlayerPawn || !GM) return;

    FVector PosicionJugador = PlayerPawn->GetActorLocation();
    FVector Forward = PlayerPawn->GetActorForwardVector();
    float Distancia = 8000.0f;
    int32 NivelActual = GM->GetNivelActual();
    int32 NumEnemigos = 8 + (FaseActual * 2) + NivelActual;

    // --- LÓGICA NIVEL 12 (BOSS) ---
    if (NivelActual == 12)
    {
        if (!Builder->GetJefeAparecido())
        {
            Builder->SetJefeAparecido(true);
            UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, PosicionJugador + (Forward * Distancia));
            // Escoltas esparcidos
            for (int32 i = 0; i < 10; i++)
                UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Hunter, PosicionJugador + (Forward * Distancia) + FVector(FMath::RandRange(-6000, 6000), FMath::RandRange(-6000, 6000), 0));
            NumEnemigos = 11;
        }
        else NumEnemigos = 15; // Post-jefe
    }

    // --- GENERACIÓN DE ENEMIGOS SEGÚN NIVEL ---
    for (int32 i = 0; i < NumEnemigos; i++)
    {
        EEnemyType Tipo;
        int32 Rand = FMath::RandRange(0, 100);

        if (NivelActual <= 3) // 1 al 3: Centinelas y Suicidas
            Tipo = (Rand < 50) ? EEnemyType::Sentry : EEnemyType::Suicide;

        else if (NivelActual <= 6) // 4 al 6: Acechadoras, Suicidas y Centinelas
        {
            if (Rand < 33) Tipo = EEnemyType::Hunter;
            else if (Rand < 66) Tipo = EEnemyType::Suicide;
            else Tipo = EEnemyType::Sentry;
        }

        else if (NivelActual <= 9) // 7 al 9: Elite y Suicidas
            Tipo = (Rand < 50) ? EEnemyType::Elite : EEnemyType::Suicide;

        else // 10 al 11: Híbrida, Elite y Suicidas
        {
            if (Rand < 33) Tipo = EEnemyType::Hibrido;
            else if (Rand < 66) Tipo = EEnemyType::Elite;
            else Tipo = EEnemyType::Suicide;
        }

        // Spawn con posición aleatoria
        FVector Pos = PosicionJugador + (Forward * Distancia) + FVector(FMath::RandRange(-3000, 3000), FMath::RandRange(-2000, 2000), 0);
        UEnemyFactory::SpawnEnemy(GetWorld(), Tipo, Pos);
    }

    Builder->RegistrarEnemigos(NumEnemigos);
}
void ALevelDirector::ConfigurarDificultad(float& Velocidad, int32& Cantidad)
{
    switch (Dificultad) {
    case EDificultad::Facil:   Velocidad *= 0.8f; Cantidad = (int32)(Cantidad * 0.75f); break;
    case EDificultad::Dificil: Velocidad *= 1.25f; Cantidad = (int32)(Cantidad * 1.25f); break;
    default: break;
    }
}

void ALevelDirector::ConstruirNivel1() { float V = 600; int32 C = 80; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel2() { float V = 850; int32 C = 120; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel3() { float V = 1100; int32 C = 180; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel4() { float V = 1400; int32 C = 220; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel5() { float V = 1600; int32 C = 260; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel6() { float V = 1800; int32 C = 300; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel7() { float V = 2000; int32 C = 340; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel8() { float V = 2200; int32 C = 380; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel9() { float V = 2400; int32 C = 420; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel10() { float V = 2600; int32 C = 460; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel11() { float V = 2800; int32 C = 500; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }
void ALevelDirector::ConstruirNivel12() { float V = 3000; int32 C = 550; ConfigurarDificultad(V, C); Builder->SpawnAsteroides(C, V); }