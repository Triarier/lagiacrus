require 'ffi'

module Blubb
  extend FFI::Library
  ffi_lib "/
    attach_function :getpid, [], :uint
    end

    puts GetPid.getpid
