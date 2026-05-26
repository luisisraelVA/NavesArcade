#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NaveJugador.generated.h"

class UInventoryComponent;
class UNaveFacade;
class AProyectil;

UCLASS()
class NAVESARCADE_API ANaveJugador : public APawn
{
    GENERATED_BODY()

public:
    ANaveJugador();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributos")
    float EnergiaActual;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UStaticMeshComponent* MallaNave;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
    class USpringArmComponent* BrazoCamara;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara")
    class UCameraComponent* CamaraNave;

    UPROPERTY(EditAnywhere, Category = "Combate")
    TSubclassOf<AProyectil> ClaseProyectil;

    void InicializarDisparo();

    float IntegridadEstructural;
    float VelocidadMovimiento;
    float VelocidadRotacion;
    int32 VidasActuales;

    int32 PuntuacionTotal;
    float MultiplicadorCombo;
    float TiempoTemblorCamara;

    int32 NucleosRecolectados;

    void RotarDerecha(float Valor);
    void RotarArriba(float Valor);
    void MoverAdelante(float Valor);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
    UInventoryComponent* Inventario;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes", meta = (AllowPrivateAccess = "true"))
    UNaveFacade* FachadaNave;
};