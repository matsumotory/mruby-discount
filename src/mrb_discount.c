/*
** mrb_discount - markdown class for mruby using discount
**
** Copyright (c) mod_mruby developers 2012-
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

#include <stdio.h>
#include <string.h>
#include <mkdio.h>

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/data.h"
#include "mruby/string.h"


#define DONE mrb_gc_arena_restore(mrb, 0);
#define HEADER_SIZE 1024
#define FOOTER_SIZE 64

typedef struct {
    char *css;
    char *title;
} mrb_md_context;

static void mrb_md_context_free(mrb_state *mrb, void *p)
{
    mrb_md_context *ctx = p;
    mrb_free(mrb, ctx);
}

static const struct mrb_data_type mrb_md_context_type = {
    "mrb_md_context", mrb_md_context_free,
};

static mrb_md_context *mrb_md_get_context(mrb_state *mrb,  mrb_value self, char *ctx_flag)
{
    mrb_md_context *c;
    mrb_value context;

    context = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, ctx_flag));
    Data_Get_Struct(mrb, context, &mrb_md_context_type, c);
    if (!c)
        mrb_raise(mrb, E_RUNTIME_ERROR, "get mrb__context failed");

    return c;
}

mrb_value mrb_discount_init(mrb_state *mrb, mrb_value self)
{
    mrb_md_context *md_ctx = (mrb_md_context *)mrb_malloc(mrb, sizeof(mrb_md_context));
    mrb_value css_path;
    mrb_value title;

    mrb_get_args(mrb, "oo", &css_path, &title);

    md_ctx->css     = RSTRING_PTR(css_path);
    md_ctx->title   = RSTRING_PTR(title);

    mrb_iv_set(mrb
        , self
        , mrb_intern_cstr(mrb, "mrb_md_context")
        , mrb_obj_value(Data_Wrap_Struct(mrb
            , mrb->object_class
            , &mrb_md_context_type
            , (void *)md_ctx)
        )
    );

    return self;
}

mrb_value mrb_discount_header(mrb_state *mrb, mrb_value self)
{
    mrb_md_context *md_ctx = mrb_md_get_context(mrb, self, "mrb_md_context");

    char header[HEADER_SIZE];

    snprintf(header, HEADER_SIZE, 
        //"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        //"<!DOCTYPE html PUBLIC \n"
        //"              \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
        //"              \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
        "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
        "<head>\n"
        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
        "<meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n"
        "<title>%s</title>\n"
        "<link rel=\"stylesheet\" href=\"%s\" type=\"text/css\" />\n"
        "</head>\n"
        "<body>\n"
        , md_ctx->title
        , md_ctx->css
    );

    return mrb_str_new(mrb, header, (mrb_int)strlen(header));
}
 

mrb_value mrb_discount_footer(mrb_state *mrb, mrb_value self)
{
    mrb_md_context *md_ctx = mrb_md_get_context(mrb, self, "mrb_md_context");

    char footer[FOOTER_SIZE];

    snprintf(footer, FOOTER_SIZE, 
        "\n"
        "</body>\n"
        "</html>\n"
    );

    return mrb_str_new(mrb, footer, strlen(footer));
}

mrb_value mrb_discount_md2html(mrb_state *mrb, mrb_value self)
{
    MMIOT *md;
    mrb_value md_obj;
    int size;
    char *html;

    mrb_get_args(mrb, "o", &md_obj);

    md = mkd_string(RSTRING_PTR(md_obj), strlen(RSTRING_PTR(md_obj)), 0);
    int ret = mkd_compile(md, MKD_TOC|MKD_AUTOLINK);
    if ((size = mkd_document(md, &html)) == EOF)
        mrb_raise(mrb, E_RUNTIME_ERROR, "mkd_document() failed");

    //mkd_cleanup(md);
        
    return mrb_str_new(mrb, html, strlen(html));

}
 
mrb_value mrb_discount_to_html(mrb_state *mrb, mrb_value self)
{
    MMIOT *md;
    int size;
    char *html;

    md = mkd_string(RSTRING_PTR(self), strlen(RSTRING_PTR(self)), 0);
    int ret = mkd_compile(md, MKD_TOC|MKD_AUTOLINK);
    if ((size = mkd_document(md, &html)) == EOF)
        mrb_raise(mrb, E_RUNTIME_ERROR, "mkd_document() failed");

    //mkd_cleanup(md);
        
    return mrb_str_new(mrb, html, strlen(html));

}

void mrb_mruby_discount_gem_init(mrb_state *mrb)
{
    struct RClass *discount;

    discount = mrb_define_class(mrb, "Discount", mrb->object_class);

    mrb_define_method(mrb, discount,            "initialize",  mrb_discount_init,      ARGS_ANY());
    mrb_define_method(mrb, discount,            "header",      mrb_discount_header,    ARGS_NONE());
    mrb_define_method(mrb, discount,            "footer",      mrb_discount_footer,    ARGS_NONE());
    mrb_define_method(mrb, discount,            "md2html",     mrb_discount_md2html,   ARGS_ANY());
    mrb_define_method(mrb, mrb->string_class,   "to_html",     mrb_discount_to_html,   ARGS_NONE());
    DONE;
}

void mrb_mruby_discount_gem_final(mrb_state *mrb)
{
}

