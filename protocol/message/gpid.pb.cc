// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: gpid.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "gpid.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace gproto {
namespace gather {

namespace {

const ::google::protobuf::EnumDescriptor* PID_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_gpid_2eproto() {
  protobuf_AddDesc_gpid_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "gpid.proto");
  GOOGLE_CHECK(file != NULL);
  PID_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_gpid_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_gpid_2eproto() {
}

void protobuf_AddDesc_gpid_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\ngpid.proto\022\rgproto.gather*P\n\003PID\022\027\n\022CS"
    "ID_C2G_HeartBeat\020\220N\022\027\n\022CSID_G2C_HeartBea"
    "t\020\221N\022\027\n\022CSID_C2G_StartGame\020\364N", 109);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "gpid.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_gpid_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_gpid_2eproto {
  StaticDescriptorInitializer_gpid_2eproto() {
    protobuf_AddDesc_gpid_2eproto();
  }
} static_descriptor_initializer_gpid_2eproto_;
const ::google::protobuf::EnumDescriptor* PID_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PID_descriptor_;
}
bool PID_IsValid(int value) {
  switch(value) {
    case 10000:
    case 10001:
    case 10100:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace gather
}  // namespace gproto

// @@protoc_insertion_point(global_scope)
