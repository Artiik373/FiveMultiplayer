// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/ctocpp/post_data_element_ctocpp.h"


// STATIC METHODS - Body may be edited by hand.

CefRefPtr<CefPostDataElement> CefPostDataElement::Create() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_post_data_element_t* _retval = cef_post_data_element_create();

  // Return type: refptr_same
  return CefPostDataElementCToCpp::Wrap(_retval);
}


// VIRTUAL METHODS - Body may be edited by hand.

bool CefPostDataElementCToCpp::IsReadOnly() {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_read_only))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_read_only(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefPostDataElementCToCpp::SetToEmpty() {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_to_empty))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_to_empty(_struct);
}

void CefPostDataElementCToCpp::SetToFile(const CefString& fileName) {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_to_file))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: fileName; type: string_byref_const
  DCHECK(!fileName.empty());
  if (fileName.empty())
    return;

  // Execute
  _struct->set_to_file(_struct,
      fileName.GetStruct());
}

void CefPostDataElementCToCpp::SetToBytes(size_t size, const void* bytes) {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_to_bytes))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: bytes; type: simple_byaddr
  DCHECK(bytes);
  if (!bytes)
    return;

  // Execute
  _struct->set_to_bytes(_struct,
      size,
      bytes);
}

CefPostDataElement::Type CefPostDataElementCToCpp::GetType() {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_type))
    return PDE_TYPE_EMPTY;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_postdataelement_type_t _retval = _struct->get_type(_struct);

  // Return type: simple
  return _retval;
}

CefString CefPostDataElementCToCpp::GetFile() {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_file))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_file(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

size_t CefPostDataElementCToCpp::GetBytesCount() {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_bytes_count))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  size_t _retval = _struct->get_bytes_count(_struct);

  // Return type: simple
  return _retval;
}

size_t CefPostDataElementCToCpp::GetBytes(size_t size, void* bytes) {
  cef_post_data_element_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_bytes))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: bytes; type: simple_byaddr
  DCHECK(bytes);
  if (!bytes)
    return 0;

  // Execute
  size_t _retval = _struct->get_bytes(_struct,
      size,
      bytes);

  // Return type: simple
  return _retval;
}


// CONSTRUCTOR - Do not edit by hand.

CefPostDataElementCToCpp::CefPostDataElementCToCpp() {
}

template<> cef_post_data_element_t* CefCToCpp<CefPostDataElementCToCpp,
    CefPostDataElement, cef_post_data_element_t>::UnwrapDerived(
    CefWrapperType type, CefPostDataElement* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefPostDataElementCToCpp,
    CefPostDataElement, cef_post_data_element_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefPostDataElementCToCpp,
    CefPostDataElement, cef_post_data_element_t>::kWrapperType =
    WT_POST_DATA_ELEMENT;
