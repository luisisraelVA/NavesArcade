#include "LevelDirector.h"
#include "LevelBuilder.h" // Aseguramos que conozca al Builder

ALevelDirector::ALevelDirector()
{
	PrimaryActorTick.bCanEverTick = false;
	Builder = nullptr;
}

void ALevelDirector::SetBuilder(ALevelBuilder* NuevoBuilder)
{
	Builder = NuevoBuilder;
}

void ALevelDirector::ConstruirNivel1()
{
	if (!Builder) return;

	// Ahora solo damos la orden inicial. 
	// El Builder se encarga de iniciar el Mundo Abierto y la Fase 1 automáticamente.
	Builder->SpawnAsteroides(100, 600.0f);
}

void ALevelDirector::ConstruirNivel2()
{
	if (!Builder) return;

	// Aumentamos velocidad y densidad de rocas para niveles superiores
	Builder->SpawnAsteroides(150, 850.0f);
}

void ALevelDirector::ConstruirNivel3()
{
	if (!Builder) return;

	// Nivel Hardcore
	Builder->SpawnAsteroides(200, 1200.0f);
}

// Repetimos la lógica de máxima dificultad para los niveles infinitos
void ALevelDirector::ConstruirNivel4() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel5() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel6() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel7() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel8() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel9() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel10() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel11() { ConstruirNivel3(); }
void ALevelDirector::ConstruirNivel12() { ConstruirNivel3(); }