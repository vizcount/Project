#include "content_decoder.h"
#include <stdlib.h>
#include "zlib.h"


gzip_decoder::gzip_decoder(size_t uncompress_buff_len)
:uncompress_buff_len_(uncompress_buff_len),
uncompress_buff_(NULL)
{
    uncompress_buff_ = (unsigned char*)malloc(uncompress_buff_len_);
}

gzip_decoder::~gzip_decoder()
{
    if (uncompress_buff_)
    {
        free(uncompress_buff_);
    }
}

bool gzip_decoder::ungzip(unsigned char* gzdata, size_t gzdata_len, std::string& out_data)
{
    int err;
    unsigned long out_count = 0;
    z_stream d_stream = {0}; /* decompression stream */

    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in = gzdata;
    d_stream.avail_in = gzdata_len;
    d_stream.avail_out = uncompress_buff_len_;
    d_stream.next_out = uncompress_buff_;

    if(inflateInit2(&d_stream, 47) != Z_OK)
        return false;

    out_data.clear();

    while (d_stream.total_in < gzdata_len)
    {
        if((err = inflate(&d_stream, Z_SYNC_FLUSH)) == Z_STREAM_END)
        {
            out_data.append((const char*)uncompress_buff_, d_stream.total_out - out_count);
            err = inflateEnd(&d_stream);
            break;
        }

        if(err == Z_OK)
        {
            out_data.append((const char*)uncompress_buff_, d_stream.total_out - out_count);
            out_count = d_stream.total_out;
            d_stream.avail_out = uncompress_buff_len_;
            d_stream.next_out = uncompress_buff_;
        }
        else
        {
            goto unzip_exit;
        }
    }

unzip_exit:
    return err == Z_OK;
}
