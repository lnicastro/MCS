#include <fstream>
#include <mcs.hh>

using namespace mcs;


/*
  This example shows how to use the Buffer class while manipulating data
  coming from memory buffers or I/O streams.
*/

int main(int argc, char *argv[]) {

  try {
    Buffer buf(AUTO_FREE); //Buffer will be automatically freed
    void* pointer;
    char tmp[10];

    //Open a file for reading (actually the source file itself)
    ifstream fin("buffer.cc");

    //Read entire file into memory
    buf << fin;
    fin.close();

    //Print entire content of the buffer
    buf >> cout;

    //Modify 5 bytes starting from the second
    buf(1, 5) << "-----";

    //Append three chars
    buf(3) << "abc";

    //Copy data from position 20 to 30 into another buffer
    buf(20, 10) >> tmp;

    //Print first 10 bytes
    buf(10) >> cerr;
    cerr << endl;

    //Get a pointer to the buffer
    pointer = buf;

    //Get a pointer to the 5-th byte
    pointer = buf[4];

    cout<<pointer;

    //Resize the buffer to twice its actual size
    buf.resize( 2 * buf.size() );
  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(1);
  }
}
