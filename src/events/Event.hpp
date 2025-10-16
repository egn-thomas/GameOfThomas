#ifndef EVENT_HPP
#define EVENT_HPP

/**
 * Classe mère de tous les évenements
 */
class Event {
public:
    virtual ~Event() = default;
    virtual void execute() = 0;
};

#endif