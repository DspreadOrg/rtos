/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev.h"
#include "../lvgl.h"

#include "LibApi.h"
#include "Key.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static int touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static uint32_t keypad_get_key(void);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_touchpad;
lv_indev_t * indev_mouse;
lv_indev_t * indev_keypad;
lv_indev_t * indev_encoder;
lv_indev_t * indev_button;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void LvGuiGroupSetObj(lv_obj_t* obj)
{
    static lv_group_t *g = NULL;

    if (g != NULL)
    {
        lv_group_remove_all_objs(g);
        lv_group_del(g);
    }
    g = lv_group_create();
    lv_group_add_obj(g, obj);
    lv_indev_set_group(indev_keypad, g);
}

void lv_port_indev_init(void)
{

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/

	/*------------------
	 * Touchpad
	 * -----------------*/
	static lv_indev_drv_t indev_tp;

	/*Initialize your touchpad if you have*/
	touchpad_init();

	/*Register a touchpad input device*/
	lv_indev_drv_init(&indev_tp);
	indev_tp.type = LV_INDEV_TYPE_POINTER;
	indev_tp.read_cb = touchpad_read;
	indev_touchpad = lv_indev_drv_register(&indev_tp);

	/*------------------
	 * Keypad
	 * -----------------*/
	static lv_indev_drv_t indev_kp;

	/*Initialize your keypad or keyboard if you have*/
	keypad_init();

	/*Register a keypad input device*/
	lv_indev_drv_init(&indev_kp);
	indev_kp.type = LV_INDEV_TYPE_KEYPAD;
	indev_kp.read_cb = keypad_read;
	indev_keypad = lv_indev_drv_register(&indev_kp);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    extern void TPInit(void);
    /*Your code comes here*/
    TPInit();
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if (touchpad_get_xy(&last_x, &last_y) == 0)
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

	//LV_LOG_INFO("[touchpad_read] x: %d, y: %d\n", last_x, last_y);

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}


typedef struct _TOUCH_POINT {
    unsigned short x;
    unsigned short y;
    unsigned short w;
} TP_TOUCH_POINT;

/*Get the x and y coordinates if the touchpad is pressed*/
static int touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    TP_TOUCH_POINT stTpPoint;

    /*Your code comes here*/
    extern unsigned char TPGetScanPoint(TP_TOUCH_POINT *p);

    if (TPGetScanPoint(&stTpPoint))
    {
        *x = stTpPoint.x;
        *y = stTpPoint.y;

        return 0;
    }

    return -1;
}


/*------------------
 * Keypad
 * -----------------*/

/*Initialize your keypad*/
static void keypad_init(void)
{
    /*Your code comes here*/
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    /*Get the current x and y coordinates*/
    //mouse_get_xy(&data->point.x, &data->point.y);

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = keypad_get_key();
	if (0 != act_key) {
		//LV_LOG_INFO("[touchpad_read] key: %d[%c]\n", act_key, (char)act_key);
	}
    if(act_key != 0) {
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch(act_key) {
			#if 0
            case 1:
                act_key = LV_KEY_NEXT;
                break;
            case 2:
                act_key = LV_KEY_PREV;
                break;
            case 3:
                act_key = LV_KEY_LEFT;
                break;
            case 4:
                act_key = LV_KEY_RIGHT;
                break;
            case 5:
                act_key = LV_KEY_ENTER;
                break;
			#endif
        }

        last_key = act_key;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
static uint32_t keypad_get_key(void)
{
    /*Your code comes here*/
    unsigned char uckey = Lib_KbGetKey();

    if (uckey == KB_NULL)
    {
        return 0;
    }

    return uckey;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
