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

// Niveles 4,5,6 más difíciles (independientemente de la dificultad global, se puede mezclar)
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

// Del nivel 7 al 12 (no usados) repiten nivel 3
void ALevelDirector::ConstruirNivel7() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel8() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel9() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel10() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel11() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel12() { ConstruirNivel3(); }