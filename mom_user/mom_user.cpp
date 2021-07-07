//////////////////////////////////////////////////////////////////////////////
//
//  mom_user.cpp
//
//  Description:
//      Contains Unigraphics entry points for the application.
//
//////////////////////////////////////////////////////////////////////////////

// #define _CRT_SECURE_NO_DEPRECATE 1

//  Include files
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <uf.h>
#include <uf_exit.h>
#include <uf_ui.h>
#include <uf_mom.h>

#include <windows.h>

#include "mom_user.h"

//----------------------------------------------------------------------------

/*
 * Защита через номер тома:
 */
int sub_10001720() ;
/*
 * Exported Tcl procedures:
 */
static int sub_10001080
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
);

//----------------------------------------------------------------------------
//  Activation Methods
//----------------------------------------------------------------------------

//  Explicit Activation
//      This entry point is used to activate the application explicitly, as in
//      "File->Execute UG/Open->User Function..."
//----- (10001010) --------------------------------------------------------
extern "C" DllExport void entry_rtv_mom_cp_part_attr( char *parm, int *returnCode, int rlen )
{
  int errorCode ;
  UF_MOM_id_t mom ;
  void *interp = NULL ;

  if ( !UF_is_initialized() )
  {
    errorCode = UF_initialize();
    *returnCode = errorCode;
    if ( errorCode ) return ;
  }

  /* Get the TCL interpreter id from the ufusr param */
  UF_MOM_ask_interp_from_param ( parm, &interp ) ;
  /* Get the MOM id from the ufusr param */
  UF_MOM_ask_mom (parm, &mom) ;
  /*Создаем функции в интепретаторе*/
  UF_MOM_extend_xlator ( mom , "EXTN_rtv_mom_cp_part_attr", sub_10001080 ) ;

  *returnCode = 0;
  if ( errorCode == 1 )  errorCode = UF_terminate();
  *returnCode = errorCode;
}


//----------------------------------------------------------------------------
//  Utilities
//----------------------------------------------------------------------------

// Unload Handler
//     This function specifies when to unload your application from Unigraphics.
//     If your application registers a callback (from a MenuScript item or a
//     User Defined Object for example), this function MUST return
//     "UF_UNLOAD_UG_TERMINATE".
extern "C" int ufusr_ask_unload( void )
{
     /* unload immediately after application exits*/
     //return ( UF_UNLOAD_IMMEDIATELY );

     /*via the unload selection dialog... */
     //return ( UF_UNLOAD_SEL_DIALOG );
     /*when UG terminates...              */
     return ( UF_UNLOAD_UG_TERMINATE );
}


/*-----------------------------------------------------------------------
 * Function Name: sub_10001080
 *
 *
 -----------------------------------------------------------------------*/
static int sub_10001080
(
  void *client_data,    /* This is the mom object */
  void *interp,         /* Pointer to the TCL interpreter */
  int  argc,            /* Count of the numnber of arguments */
  char **argv           /* Array of arguments where the
                           argv[0] - The TCL extension */
)
{
  UF_MOM_id_t mom_id = ( UF_MOM_id_t ) client_data ;

  int     ii ;
  char    Dest[256] ;
  double  angle1 , angle2 ;
  double  x , y , z , i , j , k ;
  const char *kin_retract_distance ;
  double  retract_distance ;
  double  dx , dy , dz ;

  int  dword_10003334 = 0 ;

  if ( sub_10001720() == 0 )
  {
  const int Size = 1024 ;
    char  str[Size]="НА ЭТОМ РАБОЧЕМ МЕСТЕ ПОСТПРОЦЕССОР НЕ МОЖЕТ БЫТЬ ЗАПУЩЕН\n \
Москва, CSoft (ЗАО \"СиСофт\"),\nТелефон: +7 (495) 913-22-22, Факс: +7 (495) 913-22-21\n \
e-mail: jura@csoft.ru, g00@inbox.ru\nHTTP: http://www.csoft.ru\n\n(c) 2007.\n\n\nПоследняя редакция июль 2007\0";

    UF_UI_display_nonmodal_msg("Внимание!", str, UF_UI_MSG_POS_CURSOR);
    UF_terminate();
    return ( 0 ) ;
  }

    sprintf(Dest, "Number of args %ld", argc);
    if ( argc > 0 )
    {
      for (ii=0;ii<argc;ii++) sprintf(Dest, "Arg 0 %s", argv[ii]);
    }

    if (0==strcmp(argv[1],"PLANE"))
    {
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "0", &angle1);
      UF_MOM_ask_assoc_double_array(mom_id, "out_angle_pos", "1", &angle2);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      sprintf(Dest, "MOM_output_literal \"G0 X%.3lf Y%.3lf \"", x, y);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G0 Z%.3lf \"", z);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"ROT X%.3lf Z%.3lf \"", angle1, angle2);
      UF_MOM_execute_command(mom_id, Dest);
    }

    if (0==strcmp(argv[1],"PLANE_moving"))
    {
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      sprintf(Dest, "MOM_output_literal \"G0 X%.3lf Y%.3lf \"", x, y);
      UF_MOM_execute_command(mom_id, Dest);
      sprintf(Dest, "MOM_output_literal \"G0 Z%.3lf \"", z);
      UF_MOM_execute_command(mom_id, Dest);
    }

    if (0==strcmp(argv[1],"INPUT_RETRACT"))
    {
      int  ia4[2]={0,0},
           ip6[2] = {1,1},
           resp;
      double ra5[2] = { 100, 100 } ;
      char   menu[2][16]; //={":Отвод=",":Врезание="};
      strcpy(&menu[0][0], ":Отвод (мм)=");
      strcpy(&menu[1][0], ":Врезание (мм)=");
      //ra5[0] = *n0;
      //ra5[1] = *n1;

      UF_UI_close_listing_window();
      UF_UI_lock_ug_access(UF_UI_FROM_CUSTOM);
      resp = uc1608(":Введите значения отвода и врезания инструмента:", menu, 2, ia4, ra5, ip6);
        //*n0 = ra5[0];
        //*n1 = ra5[1];
      UF_MOM_set_double(mom_id, "kin_retract_distance", ra5[0]);
      UF_MOM_set_double(mom_id, "kin_reengage_distance", ra5[1]);
      UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);
    }

    if (0==strcmp(argv[1],"INPUT_RETRACT_DIST"))
    {
      uc1601(":Ввод дистанций отвода и врезания:", 0);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_mcs_goto", "2", &z);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "0", &i);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "1", &j);
      UF_MOM_ask_assoc_double_array(mom_id, "prev_tool_axis", "2", &k);
      UF_MOM_ask_string(mom_id, "kin_retract_distance", &kin_retract_distance);
      retract_distance=atof(kin_retract_distance);
      //UF_free(&kin_retract_distance);

      dx=i*retract_distance + x ;
      dy=j*retract_distance + y ;
      dz=k*retract_distance + z ;

      UF_MOM_set_assoc_double_array(mom_id, "pos", "0", dx);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "1", dy);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "2", dz);
    }

    if ( !strcmp(argv[1], "M128") )
    {
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "0", &x);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "1", &y);
      UF_MOM_ask_assoc_double_array(mom_id, "mcs_goto", "2", &z);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "0", x);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "1", y);
      UF_MOM_set_assoc_double_array(mom_id, "pos", "2", z);
    }

    if ( !strcmp(argv[1], "UNLOAD") ) dword_10003334 = 0;
    UF_MOM_set_string(mom_id, "var_unknow", Dest);
    UF_terminate();

  return ( 0 ) ;
}

//----- (sub_10001720) --------------------------------------------------------
int sub_10001720()
{
  char VolumeNameBuffer[MAX_PATH]; // [sp+444h] [bp-3E8h]@1
  DWORD VolumeSerialNumber; // [sp+0h] [bp-82Ch]@1
  DWORD MaximumComponentLength; // [sp+54h] [bp-7D8h]@1
  DWORD FileSystemFlags; // [sp+58h] [bp-7D4h]@1
  char FileSystemNameBuffer[MAX_PATH]; // [sp+5Ch] [bp-7D0h]@1

  int i ;
  static DWORD sp[]=
  {  0x64D2F3FF , 0x0BCEC3EDA , 0x1C6992FB , 0x9C21B9ED , 0x0C4DF5004 , 0x0C4AB85B2 , 0x0B8C42862 , 0x78760A9F ,
     0x0E4025F02 , 0x9CB135B7 , 0x20B8E9D8 , 0x8415CD3F , 0x2C9B838F , 0x48D6068B , 0x8029B64 , 0x484B6F4D ,
     0x0E02F571C , 0x0B46FDB91 , 0x3031E5A2 , 0x0DC4C88DF , 0x0B0D5EE51 , 0x842574A8 , 0x0A0B050B8 , 0x54FFBBDB ,
   0x0B0F795EC , 0x0C44CDE1 , 0x67D071E1 , 0x0F1DFCA82 , 0x2CE89011 , 0x0C075ED43 , 0x6893C023 , 0x0CC434590 ,
   0x149E72B1 , 0x0D457F66C ,
     1 } ;

  GetVolumeInformation(
    "C:\\",
    VolumeNameBuffer,
    sizeof(VolumeNameBuffer),
    &VolumeSerialNumber,
    &MaximumComponentLength,
    &FileSystemFlags,
    FileSystemNameBuffer,
    sizeof(FileSystemNameBuffer));

  i = 0;
  while ( VolumeSerialNumber != sp[i] )
  {
    ++i;
    if ( i >= 35 ) return 0;
  }
  return 1;
}