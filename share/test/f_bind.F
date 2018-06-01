#ifdef MACH_64
#define PPTR INTEGER*8
#else
#define PPTR INTEGER*4
#endif

      INCLUDE 'mcs_fortran.inc'



      PROGRAM F_TEST
      INCLUDE 'mcs_facility.inc'

      PPTR null
      PPTR p, dbq, vec, d, i, cli, nrows, rs, cnf
      INTEGER*4 jj, port, ssl
      INTEGER*8 ui
      CHARACTER*100 query, str, cmd
      INTEGER*4 ciao

C      null = ifd_null2()
      null = ifd_null()

      WRITE(*,*) "Testing class Data..."
      p = new_Data(null, FLOAT, 0, 0)

      CALL Data_setival(p, 6)
      WRITE(*,*) ifd_got_error(), Data_print(p)

      CALL Data_setsval(p, "5.7")
      WRITE(*,*) ifd_got_error(), Data_print(p)

      CALL Data_setsval(p, "ciao")
      WRITE(*,*) ifd_got_error(), Data_print(p)

      IF (ifd_got_error() .eq. 1) THEN
         WRITE(*,*) ifd_last_error()
      END IF

      CALL ifd_reset_error()
      CALL del_Data(p)



C     -------------------------------------------------
      WRITE(*,*) "\n\n\nTesting class DBConn...\n"
      CALL ifd_reset_error()
      p = new_DBConn(null)

      WRITE(*,*) "Connecting...\n"
      CALL DBConn_connect(p, "mcstest", "mcstest", "test", "")

      WRITE(*,*) "DB connection is open: ", DBConn_isOpen(p)

      IF (ifd_got_error() .eq. 1) THEN
         WRITE(*,*) ifd_last_error()
      END IF


C     -------------------------------------------------
      WRITE(*,*) "\n\n\nTesting class Query...\n"
      CALL ifd_reset_error()
      dbq = new_Query(null, p, 0)

      query = "SELECT * FROM mcstest"
      WRITE(*,*) "Preparing query: ", query
      CALL Query_prepare(dbq, query)

      WRITE(*,*) "Executing query...\n"
      CALL Query_execute(dbq, 1)

      IF (Query_gotRecordSet(dbq) .NE. 0) THEN
         WRITE(*,*) "Is a SELECT-like query: ", 1
      ELSE 
         WRITE(*,*) "Is a SELECT-like query: ", 0
      ENDIF

      IF (dbq .NE. 0) THEN
         WRITE(*,*) "Number of returned rows: ", RecordSet_nRows(dbq)
         WRITE(*,*) "Printing rows..."

C     //-------------------------------------------------
         WRITE(*,*) "\n\n\nTesting class DBVector..."
         dummy = RecordSet_setFirst(dbq)
         i = 0

         DO WHILE (RecordSet_eof(dbq) .eq. 0)
            vec = RecordSet_rec(dbq)

            IF (i .eq. 0) THEN
C           First row, print head 
               DO jj=0, Record_count(vec)-1
                  d = Record_field(vec, jj)
                  WRITE(*, '(A10, A4, $)') Data_name(d), "    "
               END DO
               WRITE(*,*) ""
            END IF

C           Print data
            DO jj=0, Record_count(vec)-1
               d = Record_field(vec, jj)
               WRITE(*, '(A15, A4, $)') Data_sval(d), "    "
            END DO
            WRITE(*,*) ""
            
            IF (ifd_got_error() .eq. 1) THEN
               WRITE(*,*) ifd_last_error()
               STOP
            END IF

            i = i+1
            dummy = RecordSet_setNext(dbq)
         END DO
      ELSE
         WRITE(*,*) "Number of affected rows: ", 
     .   Query_nAffectedRows(dbq)
      END IF

      CALL Query_close(dbq)


      IF (ifd_got_error() .eq. 1) THEN
         WRITE(*,*) ifd_last_error()
      END IF

      CALL ifd_reset_error()
      CALL del_Query(dbq)


      CALL ifd_reset_error()
      CALL del_DBConn(p)

      
      CALL ifd_reset_error()
      WRITE(*,*) "\n\n\nTesting class Client..."



      cnf = new_Conf(null, "mcs.conf");
      port = Conf_ival(cnf, "test", "PORT");
      ssl = Conf_ival(cnf, "test", "use_ssl");
      cli = new_Client(null, "./", "localhost", port, ssl, 60000)

      WRITE(*,*) "Login..."
      dummy = Client_login(cli, "mcstest", "mcstest", "test")

      query = "qry SELECT * FROM mcstest"
      WRITE(*,*) "Executing command: ", query
      dummy = Client_exec(cli, query, null)


      nrows = RecordSet_nRows(cli)
c      nrows = Data_ival(Record_pop(Client_recv(cli), 0))

      WRITE(*,*) "Number of rows: ", nrows


      WRITE(*,*) "Print all result from query...."
      DO ui=0, nrows-1
c         WRITE(cmd, *) "rec ", i
c         WRITE(*,*) "Executing command: ", cmd
c         dummy = Client_exec(cli, cmd, null)
         dummy = RecordSet_setPos(cli, ui)

         p = RecordSet_rec(cli)

         IF (ui .eq. 0) THEN
C           First row, print head 
            DO jj=0, Record_count(p)-1
               d = Record_field(p, jj)
               WRITE(*, '(A10, A4, $)') Data_name(d), "    "
            END DO
            WRITE(*,*) ""
         END IF

C        Print data
         DO jj=0, Record_count(p)-1
            d = Record_field(p, jj)
            WRITE(*, '(A15, A4, $)') Data_sval(d), "    "
         END DO
         WRITE(*,*) ""


         IF (ifd_got_error() .eq. 1) THEN
            WRITE(*,*) ifd_last_error()
            STOP
         END IF
      END DO

      IF (ifd_got_error() .eq. 1) THEN
         WRITE(*,*) ifd_last_error()
      END IF

      CALL ifd_reset_error()
      CALL del_Client(cli)
      END

