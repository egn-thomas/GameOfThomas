#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Classe responsable du tremblement de la caméra
 * Gère les tremblements déclenchés par des événements (dégâts, attaques, etc.)
 */
class CameraShake
{
private:
    float shakeDuration = 0.f;      // Durée restante du tremblement
    float totalDuration = 0.f;      // Durée totale du tremblement
    float shakeIntensity = 0.f;     // Intensité du tremblement
    float shakeFrequency = 15.f;    // Fréquence du tremblement (Hz)
    float elapsedTime = 0.f;        // Temps écoulé pour le calcul de la fréquence
    float shakeDelay = 0.f;         // Délai avant que le tremblement commence

public:
    CameraShake();
    ~CameraShake() = default;

    /**
     * @brief Déclenche un tremblement de caméra
     * @param duration Durée du tremblement en secondes
     * @param intensity Intensité du tremblement en pixels
     * @param frequency Fréquence du tremblement en Hz (par défaut 15)
     * @param delay Délai avant que le tremblement commence en secondes (par défaut 0)
     */
    void trigger(float duration, float intensity, float frequency = 15.f, float delay = 0.f);

    /**
     * @brief Met à jour le tremblement
     * @param deltaTime Temps écoulé depuis la dernière frame
     * @return L'offset de position à appliquer à la caméra
     */
    sf::Vector2f update(float deltaTime);

    /**
     * @brief Vérifie si un tremblement est actuellement actif
     */
    bool isShaking() const;

    /**
     * @brief Arrête immédiatement le tremblement
     */
    void stop();
};
