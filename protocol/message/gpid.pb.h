// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gpid.proto

#ifndef PROTOBUF_gpid_2eproto__INCLUDED
#define PROTOBUF_gpid_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace gproto {
namespace gather {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_gpid_2eproto();
void protobuf_AssignDesc_gpid_2eproto();
void protobuf_ShutdownFile_gpid_2eproto();


enum PID {
  CSID_C2G_HeartBeat = 1000,
  CSID_G2C_HeartBeat = 1001,
  CSID_O2G_Deposit = 2000,
  CSID_O2G_AccountReg = 2001,
  CSID_O2G_RoleCreate = 2002,
  CSID_O2G_Login = 2003,
  CSID_O2G_BagExtend = 2004,
  CSID_O2G_BuyInstance = 2005,
  CSID_O2G_BuyCoin = 2006,
  CSID_O2G_Diamond = 2007,
  CSID_O2G_Coin = 2008,
  CSID_O2G_Item = 2009,
  CSID_O2G_Equip = 2010,
  CSID_O2G_Relive = 2011,
  CSID_O2G_Instance = 2012,
  CSID_O2G_Arena = 2013
};
bool PID_IsValid(int value);
const PID PID_MIN = CSID_C2G_HeartBeat;
const PID PID_MAX = CSID_O2G_Arena;
const int PID_ARRAYSIZE = PID_MAX + 1;

const ::google::protobuf::EnumDescriptor* PID_descriptor();
inline const ::std::string& PID_Name(PID value) {
  return ::google::protobuf::internal::NameOfEnum(
    PID_descriptor(), value);
}
inline bool PID_Parse(
    const ::std::string& name, PID* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PID>(
    PID_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace gather
}  // namespace gproto

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::gproto::gather::PID>() {
  return ::gproto::gather::PID_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_gpid_2eproto__INCLUDED