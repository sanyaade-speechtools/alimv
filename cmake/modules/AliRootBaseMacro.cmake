# -*- mode: cmake -*-

#_______________________________________________________________________________
Function (AddLibrary LIB SRCS DHDRS)

# Adds an library as a target

  Set(_path)
  List(LENGTH SRCS _len)
  If(_len GREATER 0)
    List(GET SRCS 0 _file)
    Get_filename_component(_path ${_file} PATH)
  Endif(_len GREATER 0)

  Set(LDEF)
  If(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}LinkDef.h)
    Set(LDEF "${LIB}LinkDef.h")
  Endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}LinkDef.h)

  If(_path)
    If(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/${LIB}LinkDef.h)
      Set(LDEF "${_path}/${LIB}LinkDef.h")
    Endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/${LIB}LinkDef.h)
  Endif(_path)

  Set(DICT)
  If(LDEF)
    Set(DICT "G__${LIB}.cxx")
    Set(ASRCS ${SRCS} ${DICT})
    Root_Generate_Dictionary("${DHDRS}" "${LDEF}" "${DICT}" "${INCLUDE_DIRECTORIES}")
  Else(LDEF)
    Message(STATUS "No ${LIB}LinkDef.h found... building lib with no ROOT dict")
    Set(ASRCS ${SRCS})
  Endif(LDEF)

  Add_Library(${LIB} SHARED ${ASRCS})
  Target_Link_Libraries(${LIB} ${ALIMV_LIBRARIES})
#   Set_Target_Properties(${LIB} PROPERTIES ${ALIMV_LIBRARY_PROPERTIES})
  
  Install(TARGETS ${LIB} DESTINATION ${ALI_MIX_PATH}/lib/tgt_${ALICE_TARGET}
    COMPONENT shared)

EndFunction (AddLibrary)


#_______________________________________________________________________________
Function (AddExecutable BIN SRCS LIBS)

# Adds an AliRoot executable as a target

  Add_Executable(${BIN} ${SRCS})
  Target_Link_Libraries(${BIN} ${ROOT_LIBRARIES} ${LIBS})
  Install(TARGETS ${BIN} DESTINATION ${ALIMV_INSTALL_DIR}/bin)

  If(ALICE_STATIC_BUILD)
    Add_Executable(${BIN}_a ${SRCS})
    Set(_ar_libs)
    Foreach(_lib ${LIBS})
      Set(_ar_libs ${_ar_libs} ${_lib}_a)
    EndForeach(_lib ${LIBS})
    Foreach(_lib ${LIBS})
      Set(_ar_libs ${_ar_libs} ${_lib}_a)
    EndForeach(_lib ${LIBS})
    Target_Link_Libraries(${BIN}_a ${ROOT_LIBRARIES} ${_ar_libs})
    Install(TARGETS ${BIN}_a DESTINATION ${ALIMV_INSTALL_DIR}/bin)
  EndIf(ALICE_STATIC_BUILD)

#   CheckViols(${BIN} "${SRCS}")

EndFunction (AddExecutable)

