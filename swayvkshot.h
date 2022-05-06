#ifndef __SCREEN_COPY_DMABUF_H
#define __SCREEN_COPY_DMABUF_H

struct grim_box {
        int x, y;
        int width, height;
};
bool parse_box(struct grim_box *box, const char *str);
int run_screenshot(int x1, int y1, int width1 ,int height1,char flag) ;

#endif
