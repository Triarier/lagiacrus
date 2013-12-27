require 'ffi'

class NFC_READER < FFI::Struct
  layout  :fh, :int,
          :polling, :pthread_t,
          :read_puffer, :string,
          :write_puffer, :string,
          :reddit, :string,
          :reddit_len, :int,
          :command, :string
  def initialize(tty = "/dev/ttyUSB0")
    NFC.nfc_reader_init(tty)
  end
end

module NFC
  extend FFI::Library
  ffi_lib ::File.dirname(__FILE__) + '/nfclib.so'
# Retun value must be set to the right type. Very important  
  attach_function :nfc_reader_init, [:string], NFC_READER.ptr 
  attach_function :nfc_reader_poll, [:pointer], :int
  attach_function :nfc_reader_stop_poll, [:pointer], :int
  attach_function :nfc_reader_destroy, [:pointer], :int
  attach_function :nfc_reader_read, [:pointer], :void
  attach_function :nfc_reader_do, [:pointer], :void
  attach_function :byebye, [:string], :void 
  attach_function :ustrlen, [:string], :void
  attach_function :set_cmd, [:pointer,:string], :void
  
  class Reader
    def initialize(tty = "/dev/ttyUSB0")
      NFC.nfc_reader_init(tty)

    end

    def on_tag(&bl)
    end

  end

end  

puts "Hallo,Welt"

p "Test1"

NFC::Reader.new

p "Init done"

p nfc_ptr[:fh]

p "FH:"
p nfc_obj[:fh]

p "Test2"

nfc2 = NFC::Nfc_reader.new()



#nfc1 = NFC::Reader.new("/dev/ttyUSB0")
#nfc1.on_tag do |data|
#  puts "Hallo,Welt"
#end  
#
#nfc1.poll
