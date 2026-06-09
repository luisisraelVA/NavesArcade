#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPrincipal.generated.h"

// Declaración adelantada (Forward Declaration) para mejorar el tiempo de compilación.
// Le decimos al compilador "confía en mí, estas clases existen", sin necesidad de incluir sus librerías pesadas aquí.
class UButton;

UCLASS()
class NAVESARCADE_API UMenuPrincipal : public UUserWidget
{
    GENERATED_BODY()

protected:
    // La versión de C++ del evento "Construct" de Blueprints. 
    // Se ejecuta en el instante en que el menú se crea en la pantalla.
    virtual void NativeConstruct() override;

    // --- ENLACE CON EL DISEÑO VISUAL ---
    // La etiqueta "BindWidget" exige que en tu Widget Blueprint exista un botón llamado EXACTAMENTE "BotonIniciar".
    UPROPERTY(meta = (BindWidget))
    UButton* BotonIniciar;

    UPROPERTY(meta = (BindWidget))
    UButton* BotonSalir;

private:
    // --- FUNCIONES DE LOS BOTONES ---
    // Deben llevar UFUNCTION() obligatoriamente para poder ser enlazadas a los eventos de clic del motor.
    UFUNCTION()
    void AlHacerClicIniciar();

    UFUNCTION()
    void AlHacerClicSalir();
};