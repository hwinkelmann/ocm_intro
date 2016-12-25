#ifndef _BOXES_H_
#define _BOXES_H_

struct boxes {
    void (*Initialize)(void);
    void (*Render)(void);
};

extern const struct boxes Boxes;
#endif