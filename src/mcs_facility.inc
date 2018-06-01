      CHARACTER*200 ifd_last_error
      INTEGER*4 ifd_got_error
      INTEGER*8 ifd_null
      INTEGER TINY ,SMALL ,MEDIUM ,INT ,BIGINT ,FLOAT ,DOUBL
     .E ,STRING ,TIME ,TINY_BLOB ,BLOB ,POINTER

      PARAMETER(TINY = 0,SMALL = 1,MEDIUM = 2,INT = 3,BIGINT
     . = 4,FLOAT = 5,DOUBLE = 6,STRING = 7,TIME = 8,TINY_BLOB = 9,
     .BLOB = 10,POINTER = 11)
      INTEGER*8 new_Data
      INTEGER*8 copy_Data
      CHARACTER*200 Data_name
      INTEGER*4 Data_type
      INTEGER*4 Data_maxLength
      INTEGER*4 Data_length
      INTEGER*4 Data_isUnsigned
      INTEGER*4 Data_isNull
      INTEGER*4 Data_ival
      INTEGER*8 Data_uival
      INTEGER*8 Data_lval
      INTEGER*8 Data_ulval
      REAL*4 Data_fval
      REAL*8 Data_dval
      CHARACTER*200 Data_sval
      INTEGER*8 Data_tval
      CHARACTER*200 Data_print
      INTEGER*4 Data_getTag
      INTEGER*8 new_Record
      INTEGER*8 Record_count
      INTEGER*8 Record_pop
      INTEGER*8 Record_field
      INTEGER*4 Record_posWhoseNameIs
      INTEGER*4 RecordSet_setFirst
      INTEGER*4 RecordSet_setLast
      INTEGER*4 RecordSet_setNext
      INTEGER*4 RecordSet_setPrev
      INTEGER*4 RecordSet_setWhereS
      INTEGER*4 RecordSet_setWhereI
      INTEGER*4 RecordSet_setPos
      INTEGER*4 RecordSet_know_nRows
      INTEGER*4 RecordSet_eof
      INTEGER*4 RecordSet_alwaysSameStructure
      INTEGER*8 RecordSet_nRows
      INTEGER*8 RecordSet_nFields
      INTEGER*8 RecordSet_pos
      INTEGER*8 RecordSet_rec
      INTEGER*8 RecordSet_metarec
      INTEGER*8 new_DBConn
      INTEGER*4 DBConn_isOpen
      INTEGER*8 DBConn_newDBConn
      INTEGER*8 new_Query
      INTEGER*8 Query_nAffectedRows
      INTEGER*8 Query_param
      INTEGER*8 Query_lookup
      INTEGER*4 Query_gotRecordSet
      INTEGER*8 new_Table
      INTEGER*8 Table_newRec
      INTEGER*8 new_Client
      INTEGER*4 Client_login
      INTEGER*4 Client_exec
      INTEGER*8 Client_code
      INTEGER*8 Client_msg
      INTEGER*8 Client_out
      INTEGER*8 Client_recv
      INTEGER*8 Client_aux
      INTEGER*8 new_Conf
      CHARACTER*200 Conf_sval
      INTEGER*4 Conf_ival
