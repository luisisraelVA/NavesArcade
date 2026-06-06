#include "LevelDirector.h"
#include "LevelBuilder.h"
#include "EnemyFactory.h"
#include "DronSuicida.h"
#include "NaveElite.h"
#include "Kismet/GameplayStatics.h"
#include "NavesArcadeGameMode.h"
#include "Engine/World.h"

ALevelDirector::ALevelDirector()
{
    PrimaryActorTick.bCanEverTick = false;
    Builder = nullptr;
    Dificultad = EDificultad::Medio;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder)
{
    Builder = NuevoBuilder;
    if (Builder)
    {
        Builder->OnGenerarFase.BindUObject(this, &ALevelDirector::ManejarGeneracionFase);
    }
}

void ALevelDirector::SetDificultad(EDificultad NuevaDificultad)
{
    Dificultad = NuevaDificultad;
}

void ALevelDirector::ManejarGeneracionFase(int32 FaseActual)
{
    if (!Builder || !GetWorld()) return;

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    ANavesArcadeGameMode* GM = Cast<ANavesArcadeGameMode>(GetWorld()->GetAuthGameMode());
    if (!GM) return;

    int32 NivelActual = GM->GetNivelActual();
    int32 NucleosRequeridos = GM->GetNucleosRequeridos();
    bool bEsUltimaFase = (FaseActual == NucleosRequeridos - 1);

    FVector UbicacionBase = PlayerPawn->GetActorLocation() + (PlayerPawn->GetActorForwardVector() * 8000.0f);
    UbicacionBase += FVector(0.0f, FMath::RandRange(-3000.0f, 3000.0f), FMath::RandRange(-2000.0f, 2000.0f));
    int32 NumEnemigos = 0;

    // ========== NIVEL 12: JEFE + ENEMIGOS ALREDEDOR DEL JUGADOR ==========
    if (NivelActual == 12)
    {
        // Primera fase: aparece el jefe lejos y los escoltas rodean al jugador
        if (!Builder->GetJefeAparecido())
        {
            Builder->SetJefeAparecido(true);
            // Jefe delante del jugador (posición original)
            UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, UbicacionBase + FVector(2000.0f, 0.0f, 0.0f));

            // Escoltas alrededor del jugador, no cerca del jefe
            int32 NumEscoltas = 7;
            for (int32 i = 0; i < NumEscoltas; i++)
            {
                float Angulo = FMath::RandRange(0.0f, 360.0f);
                float Distancia = FMath::RandRange(2500.0f, 4000.0f);
                FVector Offset = FVector(
                    FMath::Cos(FMath::DegreesToRadians(Angulo)) * Distancia,
                    FMath::Sin(FMath::DegreesToRadians(Angulo)) * Distancia,
                    FMath::RandRange(-500.0f, 500.0f)
                );
                FVector PosEnemigo = PlayerPawn->GetActorLocation() + Offset;
                UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Hunter, PosEnemigo);
            }
            Builder->RegistrarEnemigos(1 + NumEscoltas);
        }
        // Después de derrotar al jefe, siguen apareciendo enemigos alrededor del jugador
        else
        {
            // REUTILIZAR la variable NumEnemigos ya declarada arriba (no volver a declararla)
            NumEnemigos = 0;

            if (bEsUltimaFase)
            {
                NumEnemigos = 6 + FaseActual;
                for (int32 i = 0; i < NumEnemigos; i++)
                {
                    float Angulo = FMath::RandRange(0.0f, 360.0f);
                    float Distancia = FMath::RandRange(2000.0f, 3500.0f);
                    FVector Offset = FVector(
                        FMath::Cos(FMath::DegreesToRadians(Angulo)) * Distancia,
                        FMath::Sin(FMath::DegreesToRadians(Angulo)) * Distancia,
                        FMath::RandRange(-500.0f, 500.0f)
                    );
                    FVector Pos = PlayerPawn->GetActorLocation() + Offset;
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Elite, Pos);
                }
            }
            else
            {
                NumEnemigos = 3 + FaseActual + (NivelActual / 2);
                for (int32 i = 0; i < NumEnemigos; i++)
                {
                    float Angulo = FMath::RandRange(0.0f, 360.0f);
                    float Distancia = FMath::RandRange(2000.0f, 3500.0f);
                    FVector Offset = FVector(
                        FMath::Cos(FMath::DegreesToRadians(Angulo)) * Distancia,
                        FMath::Sin(FMath::DegreesToRadians(Angulo)) * Distancia,
                        FMath::RandRange(-500.0f, 500.0f)
                    );
                    FVector Pos = PlayerPawn->GetActorLocation() + Offset;
                    UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, Pos);
                }
            }
            Builder->RegistrarEnemigos(NumEnemigos);
        }
        return;
    }

    // ========== NIVELES 1 AL 11 ==========
    if (bEsUltimaFase)
    {
        Builder->SetJefeAparecido(true);

        if (NivelActual == 9)
        {
            int32 Escoltas = 12;
            NumEnemigos = 1 + Escoltas;
            UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Boss, UbicacionBase + FVector(2000.0f, 0.0f, 0.0f));
            for (int32 i = 0; i < Escoltas; i++)
            {
                FVector PosEnemigo = UbicacionBase + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-1800.f, 1800.f), FMath::RandRange(-1000.f, 1000.f));
                if (i % 2 == 0) UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Hunter, PosEnemigo);
                else UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, PosEnemigo);
            }
        }
        else if (NivelActual == 10)
        {
            int32 Escoltas = 3 + (NivelActual - 1);
            NumEnemigos = Escoltas + 5;
            for (int32 i = 0; i < NumEnemigos; i++)
            {
                FVector Pos = UbicacionBase + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-1800.f, 1800.f), FMath::RandRange(-1000.f, 1000.f));
                UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Suicide, Pos);
            }
        }
        else if (NivelActual == 11)
        {
            int32 Escoltas = 3 + (NivelActual - 1);
            NumEnemigos = Escoltas + 5;
            for (int32 i = 0; i < NumEnemigos; i++)
            {
                FVector Pos = UbicacionBase + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-1800.f, 1800.f), FMath::RandRange(-1000.f, 1000.f));
                UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Elite, Pos);
            }
        }
        else
        {
            NumEnemigos = 3 + (NivelActual - 1);
            for (int32 i = 0; i < NumEnemigos; i++)
            {
                FVector Pos = UbicacionBase + FVector(FMath::RandRange(-2000.f, 2000.f), FMath::RandRange(-1800.f, 1800.f), FMath::RandRange(-1000.f, 1000.f));
                if (i % 2 == 0) UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Hunter, Pos);
                else UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, Pos);
            }
        }
    }
    else
    {
        NumEnemigos = 3 + FaseActual + (NivelActual / 2);
        for (int32 i = 0; i < NumEnemigos; i++)
        {
            FVector Pos = UbicacionBase + FVector(FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-1500.f, 1500.f), FMath::RandRange(-800.f, 800.f));

            if (NivelActual == 10) UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Suicide, Pos);
            else if (NivelActual == 11) UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Elite, Pos);
            else UEnemyFactory::SpawnEnemy(GetWorld(), EEnemyType::Sentry, Pos);
        }
    }

    Builder->RegistrarEnemigos(NumEnemigos);
}

void ALevelDirector::ConstruirNivel1()
{
    if (!Builder) return;
    float VelocidadBase = 600.0f;
    int32 CantidadBase = 80;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 450.0f; CantidadBase = 60; break;
    case EDificultad::Medio:   VelocidadBase = 600.0f; CantidadBase = 80; break;
    case EDificultad::Dificil: VelocidadBase = 750.0f; CantidadBase = 100; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel2()
{
    if (!Builder) return;
    float VelocidadBase = 850.0f;
    int32 CantidadBase = 120;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 700.0f; CantidadBase = 90; break;
    case EDificultad::Medio:   VelocidadBase = 850.0f; CantidadBase = 120; break;
    case EDificultad::Dificil: VelocidadBase = 1000.0f; CantidadBase = 150; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel3()
{
    if (!Builder) return;
    float VelocidadBase = 1100.0f;
    int32 CantidadBase = 180;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 900.0f; CantidadBase = 140; break;
    case EDificultad::Medio:   VelocidadBase = 1100.0f; CantidadBase = 180; break;
    case EDificultad::Dificil: VelocidadBase = 1300.0f; CantidadBase = 220; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel4()
{
    if (!Builder) return;
    Builder->SpawnAsteroides(220, 1400.0f);
}

void ALevelDirector::ConstruirNivel5()
{
    if (!Builder) return;
    Builder->SpawnAsteroides(260, 1600.0f);
}

void ALevelDirector::ConstruirNivel6()
{
    if (!Builder) return;
    Builder->SpawnAsteroides(300, 1800.0f);
}

void ALevelDirector::ConstruirNivel7()
{
    if (!Builder) return;
    float VelocidadBase = 2000.0f;
    int32 CantidadBase = 340;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 1600.0f; CantidadBase = 260; break;
    case EDificultad::Medio:   VelocidadBase = 2000.0f; CantidadBase = 340; break;
    case EDificultad::Dificil: VelocidadBase = 2400.0f; CantidadBase = 400; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel8()
{
    if (!Builder) return;
    float VelocidadBase = 2200.0f;
    int32 CantidadBase = 380;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 1800.0f; CantidadBase = 300; break;
    case EDificultad::Medio:   VelocidadBase = 2200.0f; CantidadBase = 380; break;
    case EDificultad::Dificil: VelocidadBase = 2600.0f; CantidadBase = 450; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel9()
{
    if (!Builder) return;
    float VelocidadBase = 2400.0f;
    int32 CantidadBase = 420;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 2000.0f; CantidadBase = 350; break;
    case EDificultad::Medio:   VelocidadBase = 2400.0f; CantidadBase = 420; break;
    case EDificultad::Dificil: VelocidadBase = 2800.0f; CantidadBase = 500; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel10()
{
    if (!Builder) return;
    float VelocidadBase = 2600.0f;
    int32 CantidadBase = 460;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 2200.0f; CantidadBase = 400; break;
    case EDificultad::Medio:   VelocidadBase = 2600.0f; CantidadBase = 460; break;
    case EDificultad::Dificil: VelocidadBase = 3000.0f; CantidadBase = 550; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel11()
{
    if (!Builder) return;
    float VelocidadBase = 2800.0f;
    int32 CantidadBase = 500;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 2400.0f; CantidadBase = 450; break;
    case EDificultad::Medio:   VelocidadBase = 2800.0f; CantidadBase = 500; break;
    case EDificultad::Dificil: VelocidadBase = 3200.0f; CantidadBase = 600; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}

void ALevelDirector::ConstruirNivel12()
{
    if (!Builder) return;
    float VelocidadBase = 3000.0f;
    int32 CantidadBase = 550;
    switch (Dificultad)
    {
    case EDificultad::Facil:   VelocidadBase = 2600.0f; CantidadBase = 500; break;
    case EDificultad::Medio:   VelocidadBase = 3000.0f; CantidadBase = 550; break;
    case EDificultad::Dificil: VelocidadBase = 3500.0f; CantidadBase = 650; break;
    }
    Builder->SpawnAsteroides(CantidadBase, VelocidadBase);
}