#ifndef EVENT_HPP
#define EVENT_HPP

class Event {
public:
    virtual ~Event() = default;
    virtual void execute() = 0;
};

#endif