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