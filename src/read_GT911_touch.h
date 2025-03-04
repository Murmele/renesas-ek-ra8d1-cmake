#ifndef READ_GT911_TOUCH_H_
#define READ_GT911_TOUCH_H_

typedef enum {
    TOUCH_STATE_RELEASED = 0,
    TOUCH_STATE_PRESSED
} touch_state_t;

typedef struct {
    int32_t x;
    int32_t y;
} coord_t;

typedef struct {
    coord_t point;
    touch_state_t state;
} touch_data_t;

void touchpad_read(touch_data_t * data);

#endif /* READ_GT911_TOUCH_H_ */
