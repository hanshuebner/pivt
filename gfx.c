#include "gfx.h"
#include "console.h"

extern unsigned char G_FONT_GLYPHS;
static unsigned char* FNT = &G_FONT_GLYPHS;


#define MIN( v1, v2 ) ( v1 < v2 ? v1 : v2 )
#define PFB( X, Y ) ( ctx.pfb + Y*ctx.Pitch + X )

typedef struct {
    unsigned int W;
    unsigned int H;
    unsigned int Pitch;
    unsigned int size;
    unsigned char* pfb;

    unsigned int term_w;
    unsigned int term_h;

    GFX_COL bg;
    GFX_COL fg;

} FRAMEBUFFER_CTX;


static FRAMEBUFFER_CTX ctx;



void gfx_set_env( void* p_framebuffer, unsigned int width, unsigned int height, unsigned int pitch, unsigned int size )
{
    ctx.pfb = p_framebuffer;
    ctx.W = width;
    ctx.H = height;
    ctx.Pitch = pitch;
    ctx.size = size;

    ctx.term_w = ctx.W / 8;
    ctx.term_h = ctx.H / 8;
}


void gfx_set_bg( GFX_COL col )
{
    ctx.bg = col;
}


void gfx_set_fg( GFX_COL col )
{
    ctx.fg = col;
}


void gfx_get_term_size( unsigned int* rows, unsigned int* cols )
{
    *rows = ctx.term_h;
    *cols = ctx.term_w;
}


void gfx_clear()
{
    unsigned char* pf = ctx.pfb;
    unsigned char* pfb_end = pf + ctx.size;
    while(pf++ < pfb_end)
        *pf = ctx.bg;
}


void gfx_fill_rect( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
    if( x >= ctx.W || y >= ctx.H )
        return;

    if( x+width > ctx.W )
        width = ctx.W-x;

    if( y+height > ctx.H )
        height = ctx.H-y;


    while( height-- )
    {
        unsigned char* pf = PFB(x,y);
        const unsigned char* const pfb_end = pf + width;

        while( pf < pfb_end )
            *pf++ = ctx.fg;

        ++y;
    }
}

void gfx_putc( unsigned int row, unsigned int col, unsigned char c )
{
    if( col >= ctx.term_w )
        return;

    if( row >= ctx.term_h )
        return;

    unsigned char* p_glyph = FNT + c*64;
    unsigned char* pf = PFB(col*8, row*8);

    unsigned int h=8;
    while(h--)
    {
        unsigned int w=8;

        while( w-- )
        {
            unsigned char gv = *p_glyph;
            *pf =  (gv & ctx.fg) | ( ~gv & ctx.bg );
            ++pf;
            ++p_glyph;
        }
        pf += ctx.Pitch-8;
    }
}
