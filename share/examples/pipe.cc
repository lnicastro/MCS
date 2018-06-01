#include <unistd.h>

#include <mcs.hh>
using namespace mcs;

/*
  This example shows how to use the Pipe class to perform communication
  between threads.

  Two threads will be used here: the first runs an external program and
  captures its standard output, the second produces some data. Both thread's
  outputs will be read from a Pipe object.
*/



/*
  This class executes the specified command as a separate thread and send the
  standard output to the main program through a Pipe object.
 */
class SubProc1 : public Thread
{
private:
  //Command to be executed
  string cmd;

public:
  //Pipe object to send data to the main program
  Pipe pipe;

  //Constructor, the parameter is the command to be executed
  SubProc1(string cmd) : Thread()
    { this->cmd = cmd; }

  //Thread body
  void run() {
    string s;

    //Create a named pipe, that is a FIFO file
    pipe.createNamed();

    //Prepare the command line
    s = cmd + " > " + pipe.filename();

    //Execute the command
    system(s.c_str());
  }
};


/*
  This class behave as a data producer (actually data are just random numbers)
  whose output is sent to the main program through a Pipe object.
 */
class SubProc2 : public Thread
{
public:
  //Pipe object to send data to the main program
  Pipe pipe;

  //Thread body
  void run() {
    int i;
    int write_fd;
    char buf[100];

    //Create a pipe
    pipe.create();

    //Retrieve the file descriptor to write on the pipe
    write_fd = pipe.openWrite();

    //Generates random numbers and write them on the pipe
    for (i=0; i<10; i++) {
      sprintf(buf, "%d\n", rand());
      write(write_fd, buf, strlen(buf));
    }

    //Close write side of the pipe
    pipe.closeWrite();

// Error generation to be captured by the main
    throw MCS_ERROR(MSG_PIPE_YET_OPENED);
  }
};



/*
  Main program.
 */
int main()
{
  try {
    string cmd = "ls *.cc";  //Command to be executed
    SubProc1 sub1(cmd);      //First thread object
    SubProc2 sub2;           //Second thread object

    int read_fd;
    int n;
    char buf[100];

    //Start both thread
    sub1.start();
    sub2.start();


    cout << "Data coming from first thread:" << endl;
    read_fd = sub1.pipe.openRead();
    while ((n = read(read_fd, buf, 100)))
      cout.write(buf, n);
    sub1.pipe.closeRead();


    cout << "Data coming from second thread:" << endl;
    read_fd = sub2.pipe.openRead();
    while ((n = read(read_fd, buf, 100)))
      cout.write(buf, n);
    sub2.pipe.closeRead();


    //Ensure both threads are terminated
    while (sub1.state() != MCS_STATE_END) sleep_ms(10);
    while (sub2.state() != MCS_STATE_END) sleep_ms(10);

    //Check for errors occcurred during thread execution
    if (sub1.error()) throw *sub1.error();
    if (sub2.error()) throw *sub2.error();

  }
  catch(Event e) {
    cerr << e.where() << ": " << e.msg() << endl;
    exit(1);
  }
}
