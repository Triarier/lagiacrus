require 'ffi'

class NFC_READER < FFI::Struct
  layout  :fh, :int,
          :polling, :pthread_t,
          :read_puffer, [:u_int8_t, 100],
          :write_puffer, [:u_int8_t,100],
          :reddit, :pointer,
          :reddit_len, :int,
          :command, [:u_int8_t,100],
          :command_len, :int
  def initialize(tty = "/dev/ttyUSB0")
    NFC.nfc_reader_init(tty)
  end
end

module NFC
  extend FFI::Library
  ffi_lib ::File.dirname(__FILE__) + '/nfclib.so'
# Retun value must be set to the right type. Very important  
  attach_function :nfc_reader_init, [:string], :pointer
  attach_function :nfc_reader_poll, [:pointer], :int
  attach_function :nfc_reader_stop_poll, [:pointer], :int
  attach_function :nfc_reader_destroy, [:pointer], :int
  attach_function :nfc_reader_read, [:pointer], :void
  attach_function :nfc_reader_do, [:pointer], :void
  attach_function :byebye, [:string], :void 
  attach_function :ustrlen, [:string], :void
  attach_function :nfc_set_cmd, [:pointer, :string], :int;
  

end  

class Reader
  @blubb = NFC_READER
  @reader = 0
  def self.blubb
    @blubb
  end
  def initialize(tty = "/dev/ttyUSB0")
    @reader=NFC.nfc_reader_init(tty)

  end
  def self.reader 
    @reader
  end

  def on_tag(&bl)
  end

end


puts "Hallo,Welt"

p "Test 0"

p "Blubb"


p "Test1"

nfc_ptr = NFC.nfc_reader_init("/dev/ttyUSB0")

p "Init done"

p " Set Command to v"

NFC.nfc_set_cmd(nfc_ptr,"v")

p "Done"

p "Test2"

NFC.nfc_reader_do(nfc_ptr)


#nfc1 = NFC::Reader.new("/dev/ttyUSB0")
#nfc1.on_tag do |data|
#  puts "Hallo,Welt"
#end  
#
#nfc1.poll
