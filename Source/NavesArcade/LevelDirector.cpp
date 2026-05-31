#include "LevelDirector.h"
#include "LevelBuilder.h"
#include "Engine/Engine.h" // <-- CORREGIDO: Inclusión necesaria para usar GEngine sin errores

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

// ========== NIVELES 7, 8, 9: DIFICULTAD "DIFÍCIL" (CONTROLADA) ==========
void ALevelDirector::ConstruirNivel7()
{
    if (!Builder) return;
    Builder->SpawnAsteroides(280, 1600.0f); // Bajado para dar una curva suave
}

void ALevelDirector::ConstruirNivel8()
{
    if (!Builder) return;
    Builder->SpawnAsteroides(320, 1900.0f); // Desafiante, pero manejable con fractales
}

void ALevelDirector::ConstruirNivel9()
{
    if (!Builder) return;
    // CORREGIDO: Un solo llamado para evitar que el timer se quede buclado en memoria
    Builder->SpawnAsteroides(380, 2200.0f);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Orange, TEXT("NIVEL 9: EL GRAN CAMINO AL INFIERNO"));
    }
}

// ========== NIVELES 10, 11, 12: DIFICULTAD "IMPOSIBLE" (EL VERDADERO RETO) ==========
void ALevelDirector::ConstruirNivel10()
{
    if (!Builder) return;
    // Saturación masiva de Asteroides Fractales (Se dividen en el aire)
    Builder->SpawnAsteroides(600, 3500.0f);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NIVEL 10: ¡DIFICULTAD IMPOSIBLE - TORMENTA FRACTAL!"));
}

void ALevelDirector::ConstruirNivel11()
{
    if (!Builder) return;
    // Velocidad absurda con Asteroides Fractales
    Builder->SpawnAsteroides(700, 4200.0f);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NIVEL 11: ¡DIFICULTAD IMPOSIBLE - VELOCIDAD CRÍTICA!"));
}

void ALevelDirector::ConstruirNivel12()
{
    if (!Builder) return;
    // El fin del mundo: Asteroides explosivos a velocidad supersónica (Te quitan 50 de vida por impacto)
    Builder->SpawnAsteroides(750, 4200.0f);

    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NIVEL 12: ¡ENFRENTA AL DIOS DE LA NODRIZA!"));
}