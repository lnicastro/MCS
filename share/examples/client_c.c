#include <stdio.h>
#include <mcs_c.h>

int main(int argc, char* argv[])
{
  int i;
  IFD_OBJP cli = new_Client(0, "./", "localhost", 6523, 0, 10000);

  Client_login(cli, "mcstest", "mcstest", "test");
  Client_exec(cli, "CID", 0);
  printf("Client identifier is: %d\n",
	 Data_ival( Record_field( Client_aux(cli), 0) ));

  Client_exec(cli, "CID -help", 0);
  if (ifd_got_error()) {
    printf("ERROR: %s\n", ifd_last_error());
    ifd_reset_error();
  } else {
    for (i=0; i<Record_count( Client_out(cli) ); i++)
      printf ("%s\n",
	      Data_sval( Record_field( Client_out(cli), i ) ) );
  }

  Client_exec(cli, "qry SELECT * FROM mcstest", 0);
  if (ifd_got_error()) {
    printf("ERROR: %s\n", ifd_last_error());
    ifd_reset_error();
  } else {
    printf("Number of rows: %d\n", RecordSet_nRows(cli));

    for (i=0; i<RecordSet_nFields(cli); i++)
      printf ("%s\t",
  	    Data_name( Record_field( RecordSet_metarec(cli), i ) ) );
    printf("\n");

    while (! RecordSet_eof(cli)) {
      for (i=0; i<RecordSet_nFields(cli); i++)
        printf ("%s\t",
	        Data_sval( Record_field( RecordSet_rec(cli), i ) ) );
      printf("\n");

      RecordSet_setNext(cli);
    }
  }

  if (ifd_got_error()) {
    printf("ERROR: %s\n", ifd_last_error());
    ifd_reset_error();
  }

  Client_exec(cli, "BYE", 0);
  del_Client(cli);

  return 0;
}
