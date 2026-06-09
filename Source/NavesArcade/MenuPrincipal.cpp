#include "MenuPrincipal.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuPrincipal::NativeConstruct()
{
    // Siempre llamamos a la versión del padre primero por seguridad de memoria
    Super::NativeConstruct();

    // --- SUSCRIPCIÓN A LOS EVENTOS DE CLIC ---
    // Verificamos que el botón no sea nulo (es decir, que sí lo hayas creado en el editor visual)
    if (BotonIniciar)
    {
        // Añadimos un delegado dinámico: "Cuando te hagan clic, ejecuta mi función AlHacerClicIniciar"
        BotonIniciar->OnClicked.AddDynamic(this, &UMenuPrincipal::AlHacerClicIniciar);
    }

    if (BotonSalir)
    {
        BotonSalir->OnClicked.AddDynamic(this, &UMenuPrincipal::AlHacerClicSalir);
    }
}

void UMenuPrincipal::AlHacerClicIniciar()
{
    // Reemplaza "TuNombreDeMapaReal" con el nombre exacto de tu archivo
    FName NombreDelMapa = FName("Nivel-01");

    // Cambiamos 'this' por 'GetWorld()'
    UGameplayStatics::OpenLevel(GetWorld(), NombreDelMapa);
}

void UMenuPrincipal::AlHacerClicSalir()
{
    // 1. Obtenemos al jugador actual (índice 0 es el jugador local)
    APlayerController* ControladorJugador = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    // 2. Si el controlador es válido, ordenamos al sistema operativo que cierre la aplicación de forma limpia
    if (ControladorJugador)
    {
        UKismetSystemLibrary::QuitGame(GetWorld(), ControladorJugador, EQuitPreference::Quit, true);
    }
}