#include <acknex.h>
#include <default.c>

#include "..\\Source\\bmap.h"

int main ()
{
    wait(3);
    
    int format = 8888;
    BMAP* b = bmap_createblack(100, 200, format);
    
    bmap_lock(b, 0);
    
    // create red to green gradient from left to right
    // and alpha gradient from translucent to solid from up to bottom
    
    int ix, iy;
    for (ix = 0; ix < b->width; ix++)
    {
        for (iy = 0; iy < b->height; iy++)
        {
            double fx = (double)ix / (double)(b->width-1);
            double fy = (double)iy / (double)(b->height-1);
            
            VECTOR color;
            vec_lerp(&color, COLOR_RED, COLOR_GREEN, fx);
            
            var pixel = pixel_for_vec(&color, 100 * fy, format);
            
            pixel_to_bmap(b, ix, iy, pixel);
        }
    }
    
    bmap_unlock(b);
    
    // save to file
    bmap_savetga(b, "b.tga");
    
    // load from file to compare
    BMAP* bf = bmap_create("b.tga");
    
    video_set(32+b->width+16+bf->width+32, 32+maxv(b->height, bf->height)+32, 0, 2);
    
    while (1)
    {
        draw_text("engine", 32, 32, COLOR_WHITE);
        draw_quad(b, vector(32, 48, 0), NULL, NULL, NULL, NULL, 100, 0);
        
        draw_text("file", 32 + b->width + 16, 32, COLOR_WHITE);
        draw_quad(bf, vector(32 + b->width + 16, 48, 0), NULL, NULL, NULL, NULL, 100, 0);
        
        wait(1);
    }
}