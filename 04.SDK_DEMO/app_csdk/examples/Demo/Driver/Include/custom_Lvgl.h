#ifndef __LIB_LVGL_H1__
#define __LIB_LVGL_H1_

void LvSetStyle(lv_style_t* pstStyleObj, LV_STYLE_ATTR* pstStyleAttr);
void LvSetCtrlObj(lv_obj_t *pstObj, CTRL_OBJ_ATTR *pstCtrlObjAttr, lv_style_selector_t selector);

#endif