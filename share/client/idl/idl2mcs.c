#include <incl.h>
#include <stdlib.h>
#include <idl_export.h>

#define MSG_LEN 80
static char msg[MSG_LEN];
static const char* wrongnum = "Wrong number of parameters, should be %d, got %d";
static const char* wrongpar = "Wrong type for parameter %d, should be %s";


static IDL_VPTR idl_ifd_last_error(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static char* ret;
  static IDL_VPTR p[0];
  static int i;

  if (argc != 0) {
    snprintf(msg, MSG_LEN, wrongnum, 0);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<0; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }


  ret = ifd_last_error();

  for (i=0; i<0; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_STRING;
  idlret.flags = IDL_V_DYNAMIC;
  idlret.flags2 = IDL_V_DYNAMIC;
  IDL_StrStore(&(idlret.value.str), ret);
  return &idlret;
}


static IDL_VPTR idl_ifd_got_error(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static int ret;
  static IDL_VPTR p[0];
  static int i;

  if (argc != 0) {
    snprintf(msg, MSG_LEN, wrongnum, 0);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<0; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }


  ret = ifd_got_error();

  for (i=0; i<0; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_LONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.l =   ret;
  return &idlret;
}


static void idl_ifd_reset_error(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[0];
  static int i;

  if (argc != 0) {
    snprintf(msg, MSG_LEN, wrongnum, 0);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<0; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }


  ifd_reset_error();

  for (i=0; i<0; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_ifd_null(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[0];
  static int i;

  if (argc != 0) {
    snprintf(msg, MSG_LEN, wrongnum, 0);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<0; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }


  ret = ifd_null();

  for (i=0; i<0; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Types_TINY(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 0};
  return &idlret;
}

static IDL_VPTR idl_Types_SMALL(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 1};
  return &idlret;
}

static IDL_VPTR idl_Types_MEDIUM(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 2};
  return &idlret;
}

static IDL_VPTR idl_Types_INT(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 3};
  return &idlret;
}

static IDL_VPTR idl_Types_BIGINT(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 4};
  return &idlret;
}

static IDL_VPTR idl_Types_FLOAT(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 5};
  return &idlret;
}

static IDL_VPTR idl_Types_DOUBLE(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 6};
  return &idlret;
}

static IDL_VPTR idl_Types_STRING(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 7};
  return &idlret;
}

static IDL_VPTR idl_Types_TIME(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 8};
  return &idlret;
}

static IDL_VPTR idl_Types_TINY_BLOB(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 9};
  return &idlret;
}

static IDL_VPTR idl_Types_BLOB(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 10};
  return &idlret;
}

static IDL_VPTR idl_Types_POINTER(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret = { IDL_TYP_UINT, 0, 0, 11};
  return &idlret;
}

static IDL_VPTR idl_new_Data(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[4];
  static int i;

  if (argc != 4) {
    snprintf(msg, MSG_LEN, wrongnum, 4);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<4; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_UINT ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_UINT));
  p[2] = (argv[2]->type == IDL_TYP_UINT ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_UINT));
  p[3] = (argv[3]->type == IDL_TYP_INT ? argv[3] : IDL_BasicTypeConversion(1, &(argv[3]), IDL_TYP_INT));

  ret = new_Data((void*) p[0]->value.ptrint,
            (Types) p[1]->value.ui,
            (unsigned short int) p[2]->value.ui,
            (short) p[3]->value.i);

  for (i=0; i<4; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_copy_Data(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = copy_Data((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Data(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Data((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Data_name(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static char* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_name((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_STRING;
  idlret.flags = IDL_V_DYNAMIC;
  idlret.flags2 = IDL_V_DYNAMIC;
  IDL_StrStore(&(idlret.value.str), ret);
  return &idlret;
}


static IDL_VPTR idl_Data_type(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static Types ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_type((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_UINT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ui =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_maxLength(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned short int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_maxLength((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_UINT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ui =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_length(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned short int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_length((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_UINT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ui =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_isUnsigned(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_isUnsigned((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_isNull(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_isNull((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_ival(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_ival((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_LONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.l =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_uival(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_uival((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_lval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static long long int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_lval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_LONG64;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.l64 =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_ulval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned long long int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_ulval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG64;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul64 =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_fval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static float ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_fval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_FLOAT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.f =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_dval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static double ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_dval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_DOUBLE;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.d =   ret;
  return &idlret;
}


static IDL_VPTR idl_Data_sval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static char* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_sval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_STRING;
  idlret.flags = IDL_V_DYNAMIC;
  idlret.flags2 = IDL_V_DYNAMIC;
  IDL_StrStore(&(idlret.value.str), ret);
  return &idlret;
}


static IDL_VPTR idl_Data_tval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_tval((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static void idl_Data_setNull(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Data_setNull((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setival(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_LONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_LONG));

  Data_setival((void*) p[0]->value.ptrint,
            (int) p[1]->value.l);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setuival(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG));

  Data_setuival((void*) p[0]->value.ptrint,
            (unsigned int) p[1]->value.ul);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setlval(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_LONG64 ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_LONG64));

  Data_setlval((void*) p[0]->value.ptrint,
            (long long int) p[1]->value.l64);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setulval(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG64 ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG64));

  Data_setulval((void*) p[0]->value.ptrint,
            (unsigned long long int) p[1]->value.ul64);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setdval(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_DOUBLE ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_DOUBLE));

  Data_setdval((void*) p[0]->value.ptrint,
            (double) p[1]->value.d);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setsval(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  Data_setsval((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_setblob(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_PTR ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_PTR));
  p[2] = (argv[2]->type == IDL_TYP_ULONG ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_ULONG));

  Data_setblob((void*) p[0]->value.ptrint,
            (void*) p[1]->value.ptrint,
            (unsigned int) p[2]->value.ul);

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Data_settimeval(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG));

  Data_settimeval((void*) p[0]->value.ptrint,
            (unsigned int) p[1]->value.ul);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Data_print(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static char* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_print((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_STRING;
  idlret.flags = IDL_V_DYNAMIC;
  idlret.flags2 = IDL_V_DYNAMIC;
  IDL_StrStore(&(idlret.value.str), ret);
  return &idlret;
}


static void idl_Data_setTag(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_INT ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_INT));

  Data_setTag((void*) p[0]->value.ptrint,
            (short) p[1]->value.i);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Data_getTag(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Data_getTag((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_new_Record(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = new_Record((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Record(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Record((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Record_clear(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Record_clear((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Record_count(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Record_count((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static void idl_Record_addField(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_PTR ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_PTR));

  Record_addField((void*) p[0]->value.ptrint,
            (void*) p[1]->value.ptrint);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Record_pop(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_LONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_LONG));

  ret = Record_pop((void*) p[0]->value.ptrint,
            (int) p[1]->value.l);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Record_field(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_LONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_LONG));

  ret = Record_field((void*) p[0]->value.ptrint,
            (int) p[1]->value.l);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Record_posWhoseNameIs(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static int ret;
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  ret = Record_posWhoseNameIs((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_LONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.l =   ret;
  return &idlret;
}


static void idl_Record_setFieldMap(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  Record_setFieldMap((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_RecordSet_setFirst(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_setFirst((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setLast(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_setLast((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setNext(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_setNext((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setPrev(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_setPrev((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setWhereS(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));

  ret = RecordSet_setWhereS((void*) p[0]->value.ptrint,
            (unsigned int) p[1]->value.ul,
            (char*) IDL_STRING_STR(&(p[2]->value.str)));

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setWhereI(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG));
  p[2] = (argv[2]->type == IDL_TYP_LONG ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_LONG));

  ret = RecordSet_setWhereI((void*) p[0]->value.ptrint,
            (unsigned int) p[1]->value.ul,
            (int) p[2]->value.l);

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_setPos(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_ULONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_ULONG));

  ret = RecordSet_setPos((void*) p[0]->value.ptrint,
            (unsigned int) p[1]->value.ul);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static void idl_RecordSet_dump(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  RecordSet_dump((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_RecordSet_know_nRows(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_know_nRows((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_eof(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_eof((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_alwaysSameStructure(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_alwaysSameStructure((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_nRows(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_nRows((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_nFields(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_nFields((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_pos(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_pos((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_rec(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_rec((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_RecordSet_metarec(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = RecordSet_metarec((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_new_DBConn(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = new_DBConn((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_DBConn(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_DBConn((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_DBConn_connect(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[5];
  static int i;

  if (argc != 5) {
    snprintf(msg, MSG_LEN, wrongnum, 5);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<5; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_STRING ? argv[3] : IDL_CvtByte(1, &(argv[3])));
  p[4] = (argv[4]->type == IDL_TYP_STRING ? argv[4] : IDL_CvtByte(1, &(argv[4])));

  DBConn_connect((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (char*) IDL_STRING_STR(&(p[3]->value.str)),
            (char*) IDL_STRING_STR(&(p[4]->value.str)));

  for (i=0; i<5; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_DBConn_close(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  DBConn_close((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_DBConn_isOpen(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = DBConn_isOpen((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_DBConn_newDBConn(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = DBConn_newDBConn((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_new_Query(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_PTR ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_PTR));
  p[2] = (argv[2]->type == IDL_TYP_INT ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_INT));

  ret = new_Query((void*) p[0]->value.ptrint,
            (void*) p[1]->value.ptrint,
            (short) p[2]->value.i);

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Query(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Query((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Query_query(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_INT ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_INT));

  Query_query((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (short) p[2]->value.i);

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Query_prepare(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  Query_prepare((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Query_prepare_with_parameters(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[5];
  static int i;

  if (argc != 5) {
    snprintf(msg, MSG_LEN, wrongnum, 5);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<5; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_LONG ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_LONG));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_STRING ? argv[3] : IDL_CvtByte(1, &(argv[3])));
  p[4] = (argv[4]->type == IDL_TYP_STRING ? argv[4] : IDL_CvtByte(1, &(argv[4])));

  Query_prepare_with_parameters((void*) p[0]->value.ptrint,
            (int) p[1]->value.l,
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (char*) IDL_STRING_STR(&(p[3]->value.str)),
            (char*) IDL_STRING_STR(&(p[4]->value.str)));

  for (i=0; i<5; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Query_nAffectedRows(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static unsigned int ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Query_nAffectedRows((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_ULONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ul =   ret;
  return &idlret;
}


static IDL_VPTR idl_Query_param(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Query_param((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Query_lookup(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[4];
  static int i;

  if (argc != 4) {
    snprintf(msg, MSG_LEN, wrongnum, 4);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<4; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_STRING ? argv[3] : IDL_CvtByte(1, &(argv[3])));

  ret = Query_lookup((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (char*) IDL_STRING_STR(&(p[3]->value.str)));

  for (i=0; i<4; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Query_gotRecordSet(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Query_gotRecordSet((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static void idl_Query_execute(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_INT ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_INT));

  Query_execute((void*) p[0]->value.ptrint,
            (short) p[1]->value.i);

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Query_close(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Query_close((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_new_Table(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[4];
  static int i;

  if (argc != 4) {
    snprintf(msg, MSG_LEN, wrongnum, 4);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<4; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_PTR ? argv[1] : IDL_BasicTypeConversion(1, &(argv[1]), IDL_TYP_PTR));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_STRING ? argv[3] : IDL_CvtByte(1, &(argv[3])));

  ret = new_Table((void*) p[0]->value.ptrint,
            (void*) p[1]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (char*) IDL_STRING_STR(&(p[3]->value.str)));

  for (i=0; i<4; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Table(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Table((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Table_loadTable(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Table_loadTable((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Table_newRec(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Table_newRec((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_Table_insert(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Table_insert((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static void idl_Table_update(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  Table_update((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_new_Client(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[6];
  static int i;

  if (argc != 6) {
    snprintf(msg, MSG_LEN, wrongnum, 6);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<6; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_LONG ? argv[3] : IDL_BasicTypeConversion(1, &(argv[3]), IDL_TYP_LONG));
  p[4] = (argv[4]->type == IDL_TYP_LONG ? argv[4] : IDL_BasicTypeConversion(1, &(argv[4]), IDL_TYP_LONG));
  p[5] = (argv[5]->type == IDL_TYP_ULONG ? argv[5] : IDL_BasicTypeConversion(1, &(argv[5]), IDL_TYP_ULONG));

  ret = new_Client((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (int) p[3]->value.l,
            (int) p[4]->value.l,
            (unsigned int) p[5]->value.ul);

  for (i=0; i<6; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Client(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Client((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Client_login(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[4];
  static int i;

  if (argc != 4) {
    snprintf(msg, MSG_LEN, wrongnum, 4);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<4; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));
  p[3] = (argv[3]->type == IDL_TYP_STRING ? argv[3] : IDL_CvtByte(1, &(argv[3])));

  ret = Client_login((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)),
            (char*) IDL_STRING_STR(&(p[3]->value.str)));

  for (i=0; i<4; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_exec(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static short ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_PTR ? argv[2] : IDL_BasicTypeConversion(1, &(argv[2]), IDL_TYP_PTR));

  ret = Client_exec((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (void*) p[2]->value.ptrint);

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_INT;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.i =   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_code(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Client_code((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_msg(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Client_msg((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_out(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Client_out((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_recv(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Client_recv((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_Client_aux(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  ret = Client_aux((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static IDL_VPTR idl_new_Conf(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static void* ret;
  static IDL_VPTR p[2];
  static int i;

  if (argc != 2) {
    snprintf(msg, MSG_LEN, wrongnum, 2);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<2; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));

  ret = new_Conf((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)));

  for (i=0; i<2; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_PTR;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.ptrint =   (IDL_PTRINT)   ret;
  return &idlret;
}


static void idl_del_Conf(int argc, IDL_VPTR *argv) {
  static IDL_VPTR p[1];
  static int i;

  if (argc != 1) {
    snprintf(msg, MSG_LEN, wrongnum, 1);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<1; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));

  del_Conf((void*) p[0]->value.ptrint);

  for (i=0; i<1; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

}


static IDL_VPTR idl_Conf_sval(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static char* ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));

  ret = Conf_sval((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)));

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_STRING;
  idlret.flags = IDL_V_DYNAMIC;
  idlret.flags2 = IDL_V_DYNAMIC;
  IDL_StrStore(&(idlret.value.str), ret);
  return &idlret;
}


static IDL_VPTR idl_Conf_ival(int argc, IDL_VPTR *argv) {
  static IDL_VARIABLE idlret;
  static int ret;
  static IDL_VPTR p[3];
  static int i;

  if (argc != 3) {
    snprintf(msg, MSG_LEN, wrongnum, 3);
    IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
  }

  for (i=0; i<3; i++) 
    if (argv[i]->flags & (IDL_V_ARR | IDL_V_FILE | IDL_V_STRUCT)) {
      snprintf(msg, MSG_LEN, wrongpar, i, "");
      IDL_Message(IDL_M_GENERIC, IDL_MSG_LONGJMP, msg);
    }

  p[0] = (argv[0]->type == IDL_TYP_PTR ? argv[0] : IDL_BasicTypeConversion(1, &(argv[0]), IDL_TYP_PTR));
  p[1] = (argv[1]->type == IDL_TYP_STRING ? argv[1] : IDL_CvtByte(1, &(argv[1])));
  p[2] = (argv[2]->type == IDL_TYP_STRING ? argv[2] : IDL_CvtByte(1, &(argv[2])));

  ret = Conf_ival((void*) p[0]->value.ptrint,
            (char*) IDL_STRING_STR(&(p[1]->value.str)),
            (char*) IDL_STRING_STR(&(p[2]->value.str)));

  for (i=0; i<3; i++) 
    if (p[i] != argv[i])
      IDL_Deltmp(p[i]);

  idlret.type = IDL_TYP_LONG;
  idlret.flags = 0;
  idlret.flags2 = 0;
  idlret.value.l =   ret;
  return &idlret;
}


int IDL_Load(void) {
  static IDL_SYSFUN_DEF2 function_addr[] = {
{idl_Client_aux, "CLIENT_AUX", 1, 1, 0, 0}, 
{idl_Client_code, "CLIENT_CODE", 1, 1, 0, 0}, 
{idl_Client_exec, "CLIENT_EXEC", 3, 3, 0, 0}, 
{idl_Client_login, "CLIENT_LOGIN", 4, 4, 0, 0}, 
{idl_Client_msg, "CLIENT_MSG", 1, 1, 0, 0}, 
{idl_Client_out, "CLIENT_OUT", 1, 1, 0, 0}, 
{idl_Client_recv, "CLIENT_RECV", 1, 1, 0, 0}, 
{idl_Conf_ival, "CONF_IVAL", 3, 3, 0, 0}, 
{idl_Conf_sval, "CONF_SVAL", 3, 3, 0, 0}, 
{idl_copy_Data, "COPY_DATA", 1, 1, 0, 0}, 
{idl_Data_dval, "DATA_DVAL", 1, 1, 0, 0}, 
{idl_Data_fval, "DATA_FVAL", 1, 1, 0, 0}, 
{idl_Data_getTag, "DATA_GETTAG", 1, 1, 0, 0}, 
{idl_Data_isNull, "DATA_ISNULL", 1, 1, 0, 0}, 
{idl_Data_isUnsigned, "DATA_ISUNSIGNED", 1, 1, 0, 0}, 
{idl_Data_ival, "DATA_IVAL", 1, 1, 0, 0}, 
{idl_Data_length, "DATA_LENGTH", 1, 1, 0, 0}, 
{idl_Data_lval, "DATA_LVAL", 1, 1, 0, 0}, 
{idl_Data_maxLength, "DATA_MAXLENGTH", 1, 1, 0, 0}, 
{idl_Data_name, "DATA_NAME", 1, 1, 0, 0}, 
{idl_Data_print, "DATA_PRINT", 1, 1, 0, 0}, 
{idl_Data_sval, "DATA_SVAL", 1, 1, 0, 0}, 
{idl_Data_tval, "DATA_TVAL", 1, 1, 0, 0}, 
{idl_Data_type, "DATA_TYPE", 1, 1, 0, 0}, 
{idl_Data_uival, "DATA_UIVAL", 1, 1, 0, 0}, 
{idl_Data_ulval, "DATA_ULVAL", 1, 1, 0, 0}, 
{idl_DBConn_isOpen, "DBCONN_ISOPEN", 1, 1, 0, 0}, 
{idl_DBConn_newDBConn, "DBCONN_NEWDBCONN", 1, 1, 0, 0}, 
{idl_ifd_got_error, "IFD_GOT_ERROR", 0, 0, 0, 0}, 
{idl_ifd_last_error, "IFD_LAST_ERROR", 0, 0, 0, 0}, 
{idl_ifd_null, "IFD_NULL", 0, 0, 0, 0}, 
{idl_new_Client, "NEW_CLIENT", 6, 6, 0, 0}, 
{idl_new_Conf, "NEW_CONF", 2, 2, 0, 0}, 
{idl_new_Data, "NEW_DATA", 4, 4, 0, 0}, 
{idl_new_DBConn, "NEW_DBCONN", 1, 1, 0, 0}, 
{idl_new_Query, "NEW_QUERY", 3, 3, 0, 0}, 
{idl_new_Record, "NEW_RECORD", 1, 1, 0, 0}, 
{idl_new_Table, "NEW_TABLE", 4, 4, 0, 0}, 
{idl_Query_gotRecordSet, "QUERY_GOTRECORDSET", 1, 1, 0, 0}, 
{idl_Query_lookup, "QUERY_LOOKUP", 4, 4, 0, 0}, 
{idl_Query_nAffectedRows, "QUERY_NAFFECTEDROWS", 1, 1, 0, 0}, 
{idl_Query_param, "QUERY_PARAM", 1, 1, 0, 0}, 
{idl_RecordSet_alwaysSameStructure, "RECORDSET_ALWAYSSAMESTRUCTURE", 1, 1, 0, 0}, 
{idl_RecordSet_eof, "RECORDSET_EOF", 1, 1, 0, 0}, 
{idl_RecordSet_know_nRows, "RECORDSET_KNOW_NROWS", 1, 1, 0, 0}, 
{idl_RecordSet_metarec, "RECORDSET_METAREC", 1, 1, 0, 0}, 
{idl_RecordSet_nFields, "RECORDSET_NFIELDS", 1, 1, 0, 0}, 
{idl_RecordSet_nRows, "RECORDSET_NROWS", 1, 1, 0, 0}, 
{idl_RecordSet_pos, "RECORDSET_POS", 1, 1, 0, 0}, 
{idl_RecordSet_rec, "RECORDSET_REC", 1, 1, 0, 0}, 
{idl_RecordSet_setFirst, "RECORDSET_SETFIRST", 1, 1, 0, 0}, 
{idl_RecordSet_setLast, "RECORDSET_SETLAST", 1, 1, 0, 0}, 
{idl_RecordSet_setNext, "RECORDSET_SETNEXT", 1, 1, 0, 0}, 
{idl_RecordSet_setPos, "RECORDSET_SETPOS", 2, 2, 0, 0}, 
{idl_RecordSet_setPrev, "RECORDSET_SETPREV", 1, 1, 0, 0}, 
{idl_RecordSet_setWhereI, "RECORDSET_SETWHEREI", 3, 3, 0, 0}, 
{idl_RecordSet_setWhereS, "RECORDSET_SETWHERES", 3, 3, 0, 0}, 
{idl_Record_count, "RECORD_COUNT", 1, 1, 0, 0}, 
{idl_Record_field, "RECORD_FIELD", 2, 2, 0, 0}, 
{idl_Record_pop, "RECORD_POP", 2, 2, 0, 0}, 
{idl_Record_posWhoseNameIs, "RECORD_POSWHOSENAMEIS", 2, 2, 0, 0}, 
{idl_Table_newRec, "TABLE_NEWREC", 1, 1, 0, 0}, 
{idl_Types_BIGINT, "TYPES_BIGINT", 0, 0, 0, 0}, 
{idl_Types_BLOB, "TYPES_BLOB", 0, 0, 0, 0}, 
{idl_Types_DOUBLE, "TYPES_DOUBLE", 0, 0, 0, 0}, 
{idl_Types_FLOAT, "TYPES_FLOAT", 0, 0, 0, 0}, 
{idl_Types_INT, "TYPES_INT", 0, 0, 0, 0}, 
{idl_Types_MEDIUM, "TYPES_MEDIUM", 0, 0, 0, 0}, 
{idl_Types_POINTER, "TYPES_POINTER", 0, 0, 0, 0}, 
{idl_Types_SMALL, "TYPES_SMALL", 0, 0, 0, 0}, 
{idl_Types_STRING, "TYPES_STRING", 0, 0, 0, 0}, 
{idl_Types_TIME, "TYPES_TIME", 0, 0, 0, 0}, 
{idl_Types_TINY, "TYPES_TINY", 0, 0, 0, 0}, 
{idl_Types_TINY_BLOB, "TYPES_TINY_BLOB", 0, 0, 0, 0}, 
};

  static IDL_SYSFUN_DEF2 procedure_addr[] = {
{ (IDL_SYSRTN_GENERIC) idl_Data_setblob, "DATA_SETBLOB", 3, 3, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setdval, "DATA_SETDVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setival, "DATA_SETIVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setlval, "DATA_SETLVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setNull, "DATA_SETNULL", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setsval, "DATA_SETSVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setTag, "DATA_SETTAG", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_settimeval, "DATA_SETTIMEVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setuival, "DATA_SETUIVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Data_setulval, "DATA_SETULVAL", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_DBConn_close, "DBCONN_CLOSE", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_DBConn_connect, "DBCONN_CONNECT", 5, 5, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Client, "DEL_CLIENT", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Conf, "DEL_CONF", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Data, "DEL_DATA", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_DBConn, "DEL_DBCONN", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Query, "DEL_QUERY", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Record, "DEL_RECORD", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_del_Table, "DEL_TABLE", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_ifd_reset_error, "IFD_RESET_ERROR", 0, 0, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Query_close, "QUERY_CLOSE", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Query_execute, "QUERY_EXECUTE", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Query_prepare, "QUERY_PREPARE", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Query_prepare_with_parameters, "QUERY_PREPARE_WITH_PARAMETERS", 5, 5, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Query_query, "QUERY_QUERY", 3, 3, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_RecordSet_dump, "RECORDSET_DUMP", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Record_addField, "RECORD_ADDFIELD", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Record_clear, "RECORD_CLEAR", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Record_setFieldMap, "RECORD_SETFIELDMAP", 2, 2, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Table_insert, "TABLE_INSERT", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Table_loadTable, "TABLE_LOADTABLE", 1, 1, 0, 0}, 
{ (IDL_SYSRTN_GENERIC) idl_Table_update, "TABLE_UPDATE", 1, 1, 0, 0}, 
};

  return IDL_SysRtnAdd(function_addr, TRUE, IDL_CARRAY_ELTS(function_addr))
      && IDL_SysRtnAdd(procedure_addr, FALSE, IDL_CARRAY_ELTS(procedure_addr));
}
