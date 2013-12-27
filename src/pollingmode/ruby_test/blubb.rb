require 'ffi'

module Blubb
  extend FFI::Library
  ffi_lib ::File.dirname(__FILE__) + '/blubb.so'
  attach_function :dumm, [:int], :int
  attach_function :dummstr, [:string], :string
end
    puts Blubb.dumm(2)
    p Blubb.dummstr("DADAS")
