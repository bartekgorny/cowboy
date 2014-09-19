/*********************************************************************
*  @author Rafal Slota
*  @copyright (C): 2013-2014, Rafal Slota <roxeon@gmail.com>
*
*  Permission to use, copy, modify, and/or distribute this software for any
*  purpose with or without fee is hereby granted, provided that the above
*  copyright notice and this permission notice appear in all copies.
* 
*  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*********************************************************************/

#include <erl_nif.h>
#include <stdint.h>
#include <netinet/in.h>

#define BADARG enif_make_badarg(env)


static ERL_NIF_TERM websocket_unmask(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin;
    ERL_NIF_TERM unmasked;
    union {
        int32_t         num;
        unsigned char   bin[4];
    } mask_key;

    if(!enif_inspect_binary(env, argv[0], &bin))
        return BADARG;

    if(!enif_get_int(env, argv[1], &mask_key.num))
        return BADARG;

    mask_key.num = htonl(mask_key.num);

    char *tmp = (char *) enif_make_new_binary(env, bin.size, &unmasked);

    for(int i = 0; i < bin.size; ++i)
    {
        tmp[i] = bin.data[i] ^ mask_key.bin[i % 4];
    }

    return unmasked;
}


static ErlNifFunc nif_funcs[] =
{
    {"websocket_unmask", 2, websocket_unmask}
};


ERL_NIF_INIT(cowboy_websocket, nif_funcs, NULL,NULL,NULL,NULL);
