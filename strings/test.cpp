// 使用boost库解码pdf的stream...endstream间的流数据
#include <iostream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
int main(int argc, char const *argv[])
{
    for (int i = 1; argv[i]; ++i)
    {
        // 加载已准备好的stream...endstream间的流数据
        boost::iostreams::file_source file(argv[i]);
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::zlib_decompressor());
        in.push(file);
        // 解码
    }
    return 0;
}
