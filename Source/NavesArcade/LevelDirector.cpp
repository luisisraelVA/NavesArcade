#include "LevelDirector.h"
#include "LevelBuilder.h"

ALevelDirector::ALevelDirector()
{
    PrimaryActorTick.bCanEverTick = false;
    Builder = nullptr;
    Dificultad = EDificultad::Medio;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder)
{
    Builder = NuevoBuilder;
}

void ALevelDirector::SetDificultad(EDificultad NuevaDificultad)
{
    Dificultad = NuevaDificultad;
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

// Niveles 4,5,6: dificultad media-alta (independiente de la dificultad global)
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

// ========== NIVELES 7, 8, 9: DIFICULTAD "DIFÍCIL" ==========
void ALevelDirector::ConstruirNivel7()
{
    if (!Builder) return;
    // Alta densidad y velocidad
    Builder->SpawnAsteroides(380, 2200.0f);
}

void ALevelDirector::ConstruirNivel8()
{
    if (!Builder) return;
    // Mayor densidad que nivel 7
    Builder->SpawnAsteroides(420, 2500.0f);
}

void ALevelDirector::ConstruirNivel9()
{
    if (!Builder) return;
    // Tormenta extrema de asteroides (pero aún controlable)
    Builder->SpawnAsteroides(550, 2800.0f);
    Builder->SpawnAsteroides(120, 3200.0f); // oleada extra muy rápida

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, TEXT("NIVEL 9: DIFICULTAD DIFÍCIL"));
    }
}

// ========== NIVELES 10, 11, 12: DIFICULTAD "SÚPER DIFÍCIL" ==========
void ALevelDirector::ConstruirNivel10()
{
    if (!Builder) return;
    // Densidad masiva, velocidad letal
    Builder->SpawnAsteroides(480, 3000.0f);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("NIVEL 10: SÚPER DIFÍCIL"));
}

void ALevelDirector::ConstruirNivel11()
{
    if (!Builder) return;
    // Casi saturación
    Builder->SpawnAsteroides(600, 3500.0f);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("NIVEL 11: SÚPER DIFÍCIL"));
}

void ALevelDirector::ConstruirNivel12()
{
    if (!Builder) return;
    // El jefe es la amenaza principal, pero asteroides de apoyo muy agresivos
    Builder->SpawnAsteroides(350, 2800.0f);
    Builder->SpawnAsteroides(80, 4000.0f); // ráfaga supersónica
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("NIVEL 12: SÚPER DIFÍCIL - JEFE FINAL"));
}