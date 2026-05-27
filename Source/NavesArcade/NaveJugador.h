#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

class UInventoryComponent;
class UNaveFacade;
class AProyectil;
class UWeaponSystem;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;

UCLASS()
class NAVESARCADE_API ANaveJugador : public APawn
{
    GENERATED_BODY()

public:
    ANaveJugador();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributos")
    float EnergiaActual;

    void RecibirDano(float CantidadDano);
    void RecolectarEnergia(float Cantidad);
    void ReiniciarNivel();
    void Reaparecer();

    void SumarPuntos(int32 PuntosBase);

    int32 GetPuntuacion() const { return PuntuacionTotal; }
    float GetCombo() const { return MultiplicadorCombo; }
    float GetIntegridadEstructural() const { return IntegridadEstructural; }
    int32 GetVidas() const { return VidasActuales; }
    int32 GetNucleosRecolectados() const { return NucleosRecolectados; }

protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    UStaticMeshComponent* MallaNave;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
    USpringArmComponent* BrazoCamara;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
    UCameraComponent* CamaraNave;

    UPROPERTY(EditAnywhere, Category = "Combate")
    TSubclassOf<AProyectil> ClaseProyectil;

    void InicializarDisparo();

    void MoverAdelante(float Valor);
    void RotarDerecha(float Valor);
    void RotarArriba(float Valor);

    float IntegridadEstructural;
    float VelocidadMovimiento;
    float VelocidadRotacion;

    int32 VidasActuales;
    int32 PuntuacionTotal;
    float MultiplicadorCombo;
    float TiempoTemblorCamara;
    int32 NucleosRecolectados;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
    UInventoryComponent* Inventario;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
    UNaveFacade* FachadaNave;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
    UWeaponSystem* SistemaArmas;
};