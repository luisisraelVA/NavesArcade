#include "DodgerHUD.h"
#include "Engine/Canvas.h"
#include "NaveJugador.h"
#include "NaveFacade.h"
#include "NucleoEnergia.h"
#include "NaveAcechadora.h"
#include "DronCentinela.h"
#include "PortalSalto.h" 
#include "Kismet/GameplayStatics.h"

ADodgerHUD::ADodgerHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (FontObj.Succeeded()) FuentePrincipal = FontObj.Object;
}

void ADodgerHUD::BeginPlay() { Super::BeginPlay(); }

void ADodgerHUD::DrawHUD()
{
	Super::DrawHUD();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ANaveJugador* MiNave = Cast<ANaveJugador>(PlayerPawn);

	if (MiNave && Canvas && FuentePrincipal)
	{
		UNaveFacade* Facade = MiNave->FindComponentByClass<UNaveFacade>();
		if (Facade)
		{
			float VidaReal = Facade->ObtenerVidaNave();
			int32 VidasRestantes = MiNave->GetVidas();
			int32 Puntos = MiNave->GetPuntuacion();
			float Combo = MiNave->GetCombo();
			int32 Recolectados = MiNave->GetNucleosRecolectados();

			if (VidasRestantes > 0)
			{
				if (VidaReal > 0)
				{
					// --- PANEL DE ESTADO ESTÁTICO ---
					DrawRect(FLinearColor(0.0f, 0.0f, 0.05f, 0.7f), 20.0f, 20.0f, 400.0f, 160.0f);
					DrawText(FString::Printf(TEXT("VIDAS DE CLON: %d"), VidasRestantes), FLinearColor::White, 40.f, 30.f, FuentePrincipal, 1.2f);

					// MEJORA: Interfaz adaptada a los objetivos de la misión
					FLinearColor ColorMision = (Recolectados >= 3) ? FLinearColor::Green : FLinearColor(0.0f, 1.0f, 1.0f, 1.0f);
					DrawText(FString::Printf(TEXT("OBJETIVO: NUCLEOS %d / 3"), Recolectados), ColorMision, 40.f, 75.f, FuentePrincipal, 1.2f);

					DrawText(TEXT("INTEGRIDAD:"), FLinearColor::White, 40.f, 120.f, FuentePrincipal, 1.2f);
					FLinearColor ColorVida = FLinearColor::Green;
					if (VidaReal <= 50.0f) ColorVida = FLinearColor::Yellow;
					if (VidaReal <= 25.0f) ColorVida = FLinearColor::Red;
					DrawRect(FLinearColor(0.1f, 0.0f, 0.0f, 0.8f), 190.0f, 125.0f, 200.0f, 15.0f);
					DrawRect(ColorVida, 190.0f, 125.0f, (VidaReal / 100.0f) * 200.0f, 15.0f);

					float AnchoPantalla = Canvas->SizeX;
					DrawRect(FLinearColor(0.0f, 0.0f, 0.05f, 0.7f), AnchoPantalla - 320.0f, 20.0f, 300.0f, 100.0f);
					DrawText(FString::Printf(TEXT("SCORE: %d"), Puntos), FLinearColor::Yellow, AnchoPantalla - 300.0f, 30.0f, FuentePrincipal, 1.5f);
					FLinearColor ColorCombo = (Combo > 1.5f) ? FLinearColor(1.0f, 0.5f, 0.0f, 1.0f) : FLinearColor::White;
					DrawText(FString::Printf(TEXT("COMBO: x%.1f"), Combo), ColorCombo, AnchoPantalla - 300.0f, 75.0f, FuentePrincipal, 1.1f);

					if (VidaReal <= 25.0f)
					{
						float Alpha = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 5.0f)) * 0.3f;
						DrawRect(FLinearColor(1.0f, 0.0f, 0.0f, Alpha), 0.0f, 0.0f, Canvas->SizeX, Canvas->SizeY);
					}

					// --- RADAR DE SEGUIMIENTO EN PANTALLA ---

					// 1. RASTREAR EL PORTAL DE SALTO (SOLO SI YA SE AFIRMARON LOS 3 NÚCLEOS)
					if (Recolectados >= 3)
					{
						AActor* Portal = UGameplayStatics::GetActorOfClass(GetWorld(), APortalSalto::StaticClass());
						if (Portal)
						{
							FVector PosPantallaPortal = Canvas->Project(Portal->GetActorLocation());
							if (PosPantallaPortal.Z > 0.0f)
							{
								float X = PosPantallaPortal.X;
								float Y = PosPantallaPortal.Y;
								// Marcador gigante Verde parpadeante para escapar
								float Pulsacion = FMath::Abs(FMath::Sin(GetWorld()->GetTimeSeconds() * 6.0f)) * 30.0f + 20.0f;
								DrawLine(X - Pulsacion, Y - Pulsacion, X + Pulsacion, Y - Pulsacion, FLinearColor::Green, 3.0f);
								DrawLine(X - Pulsacion, Y + Pulsacion, X + Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
								DrawLine(X - Pulsacion, Y - Pulsacion, X - Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
								DrawLine(X + Pulsacion, Y - Pulsacion, X + Pulsacion, Y + Pulsacion, FLinearColor::Green, 3.0f);
								DrawText(TEXT("¡PORTAL DE ESCAPE ABIERTO!"), FLinearColor::Green, X - 80.0f, Y - Pulsacion - 25.0f, FuentePrincipal, 1.1f);
							}
						}
					}

					// 2. RASTREADOR DE NÚCLEOS DE ENERGÍA DE LA MISIÓN (CIAN)
					TArray<AActor*> NucleosEnEscena;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANucleoEnergia::StaticClass(), NucleosEnEscena);

					AActor* NucleoMasCercano = nullptr;
					float DistanciaMinimaEnergia = 999999.0f;

					for (AActor* Nucleo : NucleosEnEscena)
					{
						float Dist = FVector::Dist(MiNave->GetActorLocation(), Nucleo->GetActorLocation());
						if (Dist < DistanciaMinimaEnergia)
						{
							DistanciaMinimaEnergia = Dist;
							NucleoMasCercano = Nucleo;
						}
					}

					if (NucleoMasCercano)
					{
						// NUEVO: Buscamos SOLO la malla 3D, ignorando esferas de colisión
						UStaticMeshComponent* MallaCristal = NucleoMasCercano->FindComponentByClass<UStaticMeshComponent>();
						FVector CentroVisual = MallaCristal ? MallaCristal->Bounds.Origin : NucleoMasCercano->GetActorLocation();

						FVector PosicionPantalla = Canvas->Project(CentroVisual);
						if (PosicionPantalla.Z > 0.0f)
						{
							float X = PosicionPantalla.X; float Y = PosicionPantalla.Y;
							DrawLine(X - 20, Y - 20, X + 20, Y - 20, FLinearColor(0.f, 1.f, 1.f, 1.f), 2.0f);
							DrawLine(X - 20, Y + 20, X + 20, Y + 20, FLinearColor(0.f, 1.f, 1.f, 1.f), 2.0f);
							DrawLine(X - 20, Y - 20, X - 20, Y + 20, FLinearColor(0.f, 1.f, 1.f, 1.f), 2.0f);
							DrawLine(X + 20, Y - 20, X + 20, Y + 20, FLinearColor(0.f, 1.f, 1.f, 1.f), 2.0f);
							DrawText(FString::Printf(TEXT("OBJETIVO A: %.0fm"), DistanciaMinimaEnergia / 100.0f), FLinearColor(0.f, 1.f, 1.f, 1.f), X - 40.0f, Y - 45.0f, FuentePrincipal, 0.9f);
						}
					}

					// 3. RASTREADOR DE NAVES ENEMIGAS CERCANAS (ROJO)
					TArray<AActor*> EnemigosEnEscena;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANaveAcechadora::StaticClass(), EnemigosEnEscena);
					TArray<AActor*> DronesEnEscena;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADronCentinela::StaticClass(), DronesEnEscena);
					EnemigosEnEscena.Append(DronesEnEscena);

					AActor* EnemigoMasCercano = nullptr;
					float DistanciaMinimaEnemigo = 999999.0f;

					for (AActor* Enemigo : EnemigosEnEscena)
					{
						float Dist = FVector::Dist(MiNave->GetActorLocation(), Enemigo->GetActorLocation());
						if (Dist < DistanciaMinimaEnemigo)
						{
							DistanciaMinimaEnemigo = Dist;
							EnemigoMasCercano = Enemigo;
						}
					}

					if (EnemigoMasCercano)
					{
						// NUEVO: Buscamos SOLO la malla 3D, ignorando el radar gigante de 8000m
						UStaticMeshComponent* MallaEnemigo = EnemigoMasCercano->FindComponentByClass<UStaticMeshComponent>();
						FVector CentroVisual = MallaEnemigo ? MallaEnemigo->Bounds.Origin : EnemigoMasCercano->GetActorLocation();

						FVector PosicionPantallaEnemigo = Canvas->Project(CentroVisual);
						if (PosicionPantallaEnemigo.Z > 0.0f)
						{
							float X = PosicionPantallaEnemigo.X; float Y = PosicionPantallaEnemigo.Y;
							DrawLine(X, Y - 25, X + 25, Y, FLinearColor::Red, 2.0f);
							DrawLine(X + 25, Y, X, Y + 25, FLinearColor::Red, 2.0f);
							DrawLine(X, Y + 25, X - 25, Y, FLinearColor::Red, 2.0f);
							DrawLine(X - 25, Y, X, Y - 25, FLinearColor::Red, 2.0f);
							DrawText(FString::Printf(TEXT("PELIGRO A: %.0fm"), DistanciaMinimaEnemigo / 100.0f), FLinearColor::Red, X - 40.0f, Y - 50.0f, FuentePrincipal, 0.9f);
						}
					}
				}
				else
				{
					DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.6f), 0.0f, 0.0f, Canvas->SizeX, Canvas->SizeY);
					DrawText(TEXT("RECONSTRUYENDO NAVE..."), FLinearColor::Yellow, (Canvas->SizeX / 2.0f) - 250.0f, (Canvas->SizeY / 2.0f) - 50.0f, FuentePrincipal, 2.5f);
				}
			}
			else
			{
				DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.9f), 0.0f, 0.0f, Canvas->SizeX, Canvas->SizeY);
				float CX = (Canvas->SizeX / 2.0f); float CY = (Canvas->SizeY / 2.0f);
				DrawText(TEXT("SISTEMAS CRITICOS DESTRUIDOS"), FLinearColor::Red, CX - 350.0f, CY - 80.0f, FuentePrincipal, 2.5f);
				DrawText(FString::Printf(TEXT("SCORE FINAL: %d"), Puntos), FLinearColor::Yellow, CX - 150.0f, CY + 20.0f, FuentePrincipal, 1.8f);
			}
		}
	}
}