      CHARACTER*(*) FUNCTION ifd_last_error()
      CALL wf77_ifd_last_error(ifd_last_error, LEN(ifd_last_
     .error))
      RETURN
      END



      INTEGER*4 FUNCTION ifd_got_error()
      CALL wf77_ifd_got_error(ifd_got_error)
      RETURN
      END



      SUBROUTINE ifd_reset_error()
      CALL wf77_ifd_reset_error()
      RETURN
      END



      INTEGER*8 FUNCTION ifd_null()
      CALL wf77_ifd_null(ifd_null)
      RETURN
      END



      INTEGER*8 FUNCTION new_Data(dummy, type, maxLength, is
     .unsigned)
      INTEGER*8 dummy
      INTEGER*4 type
      INTEGER*4 maxLength
      INTEGER*4 isunsigned
      CALL wf77_new_Data(dummy, type, maxLength, isunsigned,
     . new_Data)
      RETURN
      END



      INTEGER*8 FUNCTION copy_Data(obj)
      INTEGER*8 obj
      CALL wf77_copy_Data(obj, copy_Data)
      RETURN
      END



      SUBROUTINE del_Data(obj)
      INTEGER*8 obj
      CALL wf77_del_Data(obj)
      RETURN
      END



      CHARACTER*(*) FUNCTION Data_name(obj)
      INTEGER*8 obj
      CALL wf77_Data_name(obj, Data_name, LEN(Data_name))
      RETURN
      END



      INTEGER*4 FUNCTION Data_type(obj)
      INTEGER*8 obj
      CALL wf77_Data_type(obj, Data_type)
      RETURN
      END



      INTEGER*4 FUNCTION Data_maxLength(obj)
      INTEGER*8 obj
      CALL wf77_Data_maxLength(obj, Data_maxLength)
      RETURN
      END



      INTEGER*4 FUNCTION Data_length(obj)
      INTEGER*8 obj
      CALL wf77_Data_length(obj, Data_length)
      RETURN
      END



      INTEGER*4 FUNCTION Data_isUnsigned(obj)
      INTEGER*8 obj
      CALL wf77_Data_isUnsigned(obj, Data_isUnsigned)
      RETURN
      END



      INTEGER*4 FUNCTION Data_isNull(obj)
      INTEGER*8 obj
      CALL wf77_Data_isNull(obj, Data_isNull)
      RETURN
      END



      INTEGER*4 FUNCTION Data_ival(obj)
      INTEGER*8 obj
      CALL wf77_Data_ival(obj, Data_ival)
      RETURN
      END



      INTEGER*8 FUNCTION Data_uival(obj)
      INTEGER*8 obj
      CALL wf77_Data_uival(obj, Data_uival)
      RETURN
      END



      INTEGER*8 FUNCTION Data_lval(obj)
      INTEGER*8 obj
      CALL wf77_Data_lval(obj, Data_lval)
      RETURN
      END



      INTEGER*8 FUNCTION Data_ulval(obj)
      INTEGER*8 obj
      CALL wf77_Data_ulval(obj, Data_ulval)
      RETURN
      END



      REAL*4 FUNCTION Data_fval(obj)
      INTEGER*8 obj
      CALL wf77_Data_fval(obj, Data_fval)
      RETURN
      END



      REAL*8 FUNCTION Data_dval(obj)
      INTEGER*8 obj
      CALL wf77_Data_dval(obj, Data_dval)
      RETURN
      END



      CHARACTER*(*) FUNCTION Data_sval(obj)
      INTEGER*8 obj
      CALL wf77_Data_sval(obj, Data_sval, LEN(Data_sval))
      RETURN
      END



      INTEGER*8 FUNCTION Data_tval(obj)
      INTEGER*8 obj
      CALL wf77_Data_tval(obj, Data_tval)
      RETURN
      END



      SUBROUTINE Data_setNull(obj)
      INTEGER*8 obj
      CALL wf77_Data_setNull(obj)
      RETURN
      END



      SUBROUTINE Data_setival(obj, val)
      INTEGER*8 obj
      INTEGER*4 val
      CALL wf77_Data_setival(obj, val)
      RETURN
      END



      SUBROUTINE Data_setuival(obj, val)
      INTEGER*8 obj
      INTEGER*8 val
      CALL wf77_Data_setuival(obj, val)
      RETURN
      END



      SUBROUTINE Data_setlval(obj, val)
      INTEGER*8 obj
      INTEGER*8 val
      CALL wf77_Data_setlval(obj, val)
      RETURN
      END



      SUBROUTINE Data_setulval(obj, val)
      INTEGER*8 obj
      INTEGER*8 val
      CALL wf77_Data_setulval(obj, val)
      RETURN
      END



      SUBROUTINE Data_setdval(obj, val)
      INTEGER*8 obj
      REAL*8 val
      CALL wf77_Data_setdval(obj, val)
      RETURN
      END



      SUBROUTINE Data_setsval(obj, val)
      INTEGER*8 obj
      CHARACTER*(*) val
      CALL wf77_Data_setsval(obj, val, LEN(val))
      RETURN
      END



      SUBROUTINE Data_setblob(obj, lbuf, size)
      INTEGER*8 obj
      INTEGER*8 lbuf
      INTEGER*8 size
      CALL wf77_Data_setblob(obj, lbuf, size)
      RETURN
      END



      SUBROUTINE Data_settimeval(obj, val)
      INTEGER*8 obj
      INTEGER*8 val
      CALL wf77_Data_settimeval(obj, val)
      RETURN
      END



      CHARACTER*(*) FUNCTION Data_print(obj)
      INTEGER*8 obj
      CALL wf77_Data_print(obj, Data_print, LEN(Data_print))
      RETURN
      END



      SUBROUTINE Data_setTag(obj, tag)
      INTEGER*8 obj
      INTEGER*4 tag
      CALL wf77_Data_setTag(obj, tag)
      RETURN
      END



      INTEGER*4 FUNCTION Data_getTag(obj)
      INTEGER*8 obj
      CALL wf77_Data_getTag(obj, Data_getTag)
      RETURN
      END



      INTEGER*8 FUNCTION new_Record(dummy)
      INTEGER*8 dummy
      CALL wf77_new_Record(dummy, new_Record)
      RETURN
      END



      SUBROUTINE del_Record(obj)
      INTEGER*8 obj
      CALL wf77_del_Record(obj)
      RETURN
      END



      SUBROUTINE Record_clear(obj)
      INTEGER*8 obj
      CALL wf77_Record_clear(obj)
      RETURN
      END



      INTEGER*8 FUNCTION Record_count(obj)
      INTEGER*8 obj
      CALL wf77_Record_count(obj, Record_count)
      RETURN
      END



      SUBROUTINE Record_addField(obj, d)
      INTEGER*8 obj
      INTEGER*8 d
      CALL wf77_Record_addField(obj, d)
      RETURN
      END



      INTEGER*8 FUNCTION Record_pop(obj, x)
      INTEGER*8 obj
      INTEGER*4 x
      CALL wf77_Record_pop(obj, x, Record_pop)
      RETURN
      END



      INTEGER*8 FUNCTION Record_field(obj, x)
      INTEGER*8 obj
      INTEGER*4 x
      CALL wf77_Record_field(obj, x, Record_field)
      RETURN
      END



      INTEGER*4 FUNCTION Record_posWhoseNameIs(obj, name)
      INTEGER*8 obj
      CHARACTER*(*) name
      CALL wf77_Record_posWhoseNameIs(obj, name, LEN(name), 
     .Record_posWhoseNameIs)
      RETURN
      END



      SUBROUTINE Record_setFieldMap(obj, s)
      INTEGER*8 obj
      CHARACTER*(*) s
      CALL wf77_Record_setFieldMap(obj, s, LEN(s))
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setFirst(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_setFirst(obj, RecordSet_setFirst)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setLast(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_setLast(obj, RecordSet_setLast)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setNext(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_setNext(obj, RecordSet_setNext)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setPrev(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_setPrev(obj, RecordSet_setPrev)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setWhereS(obj, field, whe
     .re)
      INTEGER*8 obj
      INTEGER*8 field
      CHARACTER*(*) where
      CALL wf77_RecordSet_setWhereS(obj, field, where, LEN(w
     .here), RecordSet_setWhereS)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setWhereI(obj, field, whe
     .re)
      INTEGER*8 obj
      INTEGER*8 field
      INTEGER*4 where
      CALL wf77_RecordSet_setWhereI(obj, field, where, Recor
     .dSet_setWhereI)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_setPos(obj, newpos)
      INTEGER*8 obj
      INTEGER*8 newpos
      CALL wf77_RecordSet_setPos(obj, newpos, RecordSet_setP
     .os)
      RETURN
      END



      SUBROUTINE RecordSet_dump(obj, fn)
      INTEGER*8 obj
      CHARACTER*(*) fn
      CALL wf77_RecordSet_dump(obj, fn, LEN(fn))
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_know_nRows(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_know_nRows(obj, RecordSet_know_nRo
     .ws)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_eof(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_eof(obj, RecordSet_eof)
      RETURN
      END



      INTEGER*4 FUNCTION RecordSet_alwaysSameStructure(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_alwaysSameStructure(obj, RecordSet
     ._alwaysSameStructure)
      RETURN
      END



      INTEGER*8 FUNCTION RecordSet_nRows(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_nRows(obj, RecordSet_nRows)
      RETURN
      END



      INTEGER*8 FUNCTION RecordSet_nFields(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_nFields(obj, RecordSet_nFields)
      RETURN
      END



      INTEGER*8 FUNCTION RecordSet_pos(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_pos(obj, RecordSet_pos)
      RETURN
      END



      INTEGER*8 FUNCTION RecordSet_rec(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_rec(obj, RecordSet_rec)
      RETURN
      END



      INTEGER*8 FUNCTION RecordSet_metarec(obj)
      INTEGER*8 obj
      CALL wf77_RecordSet_metarec(obj, RecordSet_metarec)
      RETURN
      END



      INTEGER*8 FUNCTION new_DBConn(dummy)
      INTEGER*8 dummy
      CALL wf77_new_DBConn(dummy, new_DBConn)
      RETURN
      END



      SUBROUTINE del_DBConn(obj)
      INTEGER*8 obj
      CALL wf77_del_DBConn(obj)
      RETURN
      END



      SUBROUTINE DBConn_connect(obj, user, pass, db, host)
      INTEGER*8 obj
      CHARACTER*(*) user
      CHARACTER*(*) pass
      CHARACTER*(*) db
      CHARACTER*(*) host
      CALL wf77_DBConn_connect(obj, user, LEN(user), pass, L
     .EN(pass), db, LEN(db), host, LEN(host))
      RETURN
      END



      SUBROUTINE DBConn_close(obj)
      INTEGER*8 obj
      CALL wf77_DBConn_close(obj)
      RETURN
      END



      INTEGER*4 FUNCTION DBConn_isOpen(obj)
      INTEGER*8 obj
      CALL wf77_DBConn_isOpen(obj, DBConn_isOpen)
      RETURN
      END



      INTEGER*8 FUNCTION DBConn_newDBConn(obj)
      INTEGER*8 obj
      CALL wf77_DBConn_newDBConn(obj, DBConn_newDBConn)
      RETURN
      END



      INTEGER*8 FUNCTION new_Query(dummy, lconn, handleNewDB
     .Conn)
      INTEGER*8 dummy
      INTEGER*8 lconn
      INTEGER*4 handleNewDBConn
      CALL wf77_new_Query(dummy, lconn, handleNewDBConn, new
     ._Query)
      RETURN
      END



      SUBROUTINE del_Query(obj)
      INTEGER*8 obj
      CALL wf77_del_Query(obj)
      RETURN
      END



      SUBROUTINE Query_query(obj, query, StoreResult)
      INTEGER*8 obj
      CHARACTER*(*) query
      INTEGER*4 StoreResult
      CALL wf77_Query_query(obj, query, LEN(query), StoreRes
     .ult)
      RETURN
      END



      SUBROUTINE Query_prepare(obj, query)
      INTEGER*8 obj
      CHARACTER*(*) query
      CALL wf77_Query_prepare(obj, query, LEN(query))
      RETURN
      END



      SUBROUTINE Query_prepare_with_parameters(obj, op, fiel
     .ds, table, where)
      INTEGER*8 obj
      INTEGER*4 op
      CHARACTER*(*) fields
      CHARACTER*(*) table
      CHARACTER*(*) where
      CALL wf77_Query_prepare_with_parameters(obj, op, field
     .s, LEN(fields), table, LEN(table), where, LEN(where))
      RETURN
      END



      INTEGER*8 FUNCTION Query_nAffectedRows(obj)
      INTEGER*8 obj
      CALL wf77_Query_nAffectedRows(obj, Query_nAffectedRows
     .)
      RETURN
      END



      INTEGER*8 FUNCTION Query_param(obj)
      INTEGER*8 obj
      CALL wf77_Query_param(obj, Query_param)
      RETURN
      END



      INTEGER*8 FUNCTION Query_lookup(obj, field, table, whe
     .re)
      INTEGER*8 obj
      CHARACTER*(*) field
      CHARACTER*(*) table
      CHARACTER*(*) where
      CALL wf77_Query_lookup(obj, field, LEN(field), table, 
     .LEN(table), where, LEN(where), Query_lookup)
      RETURN
      END



      INTEGER*4 FUNCTION Query_gotRecordSet(obj)
      INTEGER*8 obj
      CALL wf77_Query_gotRecordSet(obj, Query_gotRecordSet)
      RETURN
      END



      SUBROUTINE Query_execute(obj, StoreResult)
      INTEGER*8 obj
      INTEGER*4 StoreResult
      CALL wf77_Query_execute(obj, StoreResult)
      RETURN
      END



      SUBROUTINE Query_close(obj)
      INTEGER*8 obj
      CALL wf77_Query_close(obj)
      RETURN
      END



      INTEGER*8 FUNCTION new_Table(dummy, lconn, table, fiel
     .dkey)
      INTEGER*8 dummy
      INTEGER*8 lconn
      CHARACTER*(*) table
      CHARACTER*(*) fieldkey
      CALL wf77_new_Table(dummy, lconn, table, LEN(table), f
     .ieldkey, LEN(fieldkey), new_Table)
      RETURN
      END



      SUBROUTINE del_Table(obj)
      INTEGER*8 obj
      CALL wf77_del_Table(obj)
      RETURN
      END



      SUBROUTINE Table_loadTable(obj)
      INTEGER*8 obj
      CALL wf77_Table_loadTable(obj)
      RETURN
      END



      INTEGER*8 FUNCTION Table_newRec(obj)
      INTEGER*8 obj
      CALL wf77_Table_newRec(obj, Table_newRec)
      RETURN
      END



      SUBROUTINE Table_insert(obj)
      INTEGER*8 obj
      CALL wf77_Table_insert(obj)
      RETURN
      END



      SUBROUTINE Table_update(obj)
      INTEGER*8 obj
      CALL wf77_Table_update(obj)
      RETURN
      END



      INTEGER*8 FUNCTION new_Client(dummy, path, server, por
     .t, ssl, timeout)
      INTEGER*8 dummy
      CHARACTER*(*) path
      CHARACTER*(*) server
      INTEGER*4 port
      INTEGER*4 ssl
      INTEGER*8 timeout
      CALL wf77_new_Client(dummy, path, LEN(path), server, L
     .EN(server), port, ssl, timeout, new_Client)
      RETURN
      END



      SUBROUTINE del_Client(obj)
      INTEGER*8 obj
      CALL wf77_del_Client(obj)
      RETURN
      END



      INTEGER*4 FUNCTION Client_login(obj, user, pass, db)
      INTEGER*8 obj
      CHARACTER*(*) user
      CHARACTER*(*) pass
      CHARACTER*(*) db
      CALL wf77_Client_login(obj, user, LEN(user), pass, LEN
     .(pass), db, LEN(db), Client_login)
      RETURN
      END



      INTEGER*4 FUNCTION Client_exec(obj, cmd, data)
      INTEGER*8 obj
      CHARACTER*(*) cmd
      INTEGER*8 data
      CALL wf77_Client_exec(obj, cmd, LEN(cmd), data, Client
     ._exec)
      RETURN
      END



      INTEGER*8 FUNCTION Client_code(obj)
      INTEGER*8 obj
      CALL wf77_Client_code(obj, Client_code)
      RETURN
      END



      INTEGER*8 FUNCTION Client_msg(obj)
      INTEGER*8 obj
      CALL wf77_Client_msg(obj, Client_msg)
      RETURN
      END



      INTEGER*8 FUNCTION Client_out(obj)
      INTEGER*8 obj
      CALL wf77_Client_out(obj, Client_out)
      RETURN
      END



      INTEGER*8 FUNCTION Client_recv(obj)
      INTEGER*8 obj
      CALL wf77_Client_recv(obj, Client_recv)
      RETURN
      END



      INTEGER*8 FUNCTION Client_aux(obj)
      INTEGER*8 obj
      CALL wf77_Client_aux(obj, Client_aux)
      RETURN
      END



      INTEGER*8 FUNCTION new_Conf(dummy, filename)
      INTEGER*8 dummy
      CHARACTER*(*) filename
      CALL wf77_new_Conf(dummy, filename, LEN(filename), new
     ._Conf)
      RETURN
      END



      SUBROUTINE del_Conf(obj)
      INTEGER*8 obj
      CALL wf77_del_Conf(obj)
      RETURN
      END



      CHARACTER*(*) FUNCTION Conf_sval(obj, section, key)
      INTEGER*8 obj
      CHARACTER*(*) section
      CHARACTER*(*) key
      CALL wf77_Conf_sval(obj, section, LEN(section), key, L
     .EN(key), Conf_sval, LEN(Conf_sval))
      RETURN
      END



      INTEGER*4 FUNCTION Conf_ival(obj, section, key)
      INTEGER*8 obj
      CHARACTER*(*) section
      CHARACTER*(*) key
      CALL wf77_Conf_ival(obj, section, LEN(section), key, L
     .EN(key), Conf_ival)
      RETURN
      END



