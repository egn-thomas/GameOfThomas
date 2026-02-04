#include "CameraShake.hpp"
#include <cmath>

/**
 * @brief Constructeur de CameraShake
 */
CameraShake::CameraShake()
    : shakeDuration(0.f), totalDuration(0.f), shakeIntensity(0.f),
      shakeFrequency(15.f), elapsedTime(0.f), shakeDelay(0.f)
{
}

/**
 * @brief Déclenche un tremblement de caméra
 * @param duration Durée du tremblement en secondes
 * @param intensity Intensité du tremblement en pixels
 * @param frequency Fréquence du tremblement en Hz
 * @param delay Délai avant que le tremblement commence en secondes
 */
void CameraShake::trigger(float duration, float intensity, float frequency, float delay)
{
    // Si un délai est spécifié, on ajoute la durée au délai pour la durée totale
    if (delay > 0.f)
    {
        shakeDuration = duration + delay;
        shakeDelay = delay;
    }
    else
    {
        shakeDuration = duration;
        shakeDelay = 0.f;
    }
    totalDuration = shakeDuration;
    shakeIntensity = intensity;
    shakeFrequency = frequency;
    elapsedTime = 0.f;
}

/**
 * @brief Met à jour le tremblement
 * @param deltaTime Temps écoulé depuis la dernière frame
 * @return L'offset de position à appliquer à la caméra
 */
sf::Vector2f CameraShake::update(float deltaTime)
{
    if (shakeDuration <= 0.f)
        return sf::Vector2f(0.f, 0.f);

    shakeDuration -= deltaTime;
    elapsedTime += deltaTime;

    // Si nous sommes dans le délai, ne pas appliquer de tremblement
    if (elapsedTime < shakeDelay)
        return sf::Vector2f(0.f, 0.f);

    // Calcul du décalage en fonction d'une onde sinusoïdale
    float progress = (totalDuration - shakeDuration) / totalDuration;
    // Atténuation progressive du tremblement
    float attenuation = 1.f - progress;
    
    float offset_x = std::sin(elapsedTime * shakeFrequency * 2.f * 3.14159f) * shakeIntensity * attenuation;
    float offset_y = std::cos(elapsedTime * shakeFrequency * 2.f * 3.14159f) * shakeIntensity * attenuation;

    return sf::Vector2f(offset_x, offset_y);
}

/**
 * @brief Vérifie si un tremblement est actuellement actif
 */
bool CameraShake::isShaking() const
{
    return shakeDuration > 0.f;
}

/**
 * @brief Arrête immédiatement le tremblement
 */
void CameraShake::stop()
{
    shakeDuration = 0.f;
    shakeDelay = 0.f;
}
