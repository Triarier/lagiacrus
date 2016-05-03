require 'ffi'

module NFC
  # FFI #{{{
    extend FFI::Library
    ffi_lib ::File.dirname(__FILE__) + '../ext/nfclib.so'
    callback :on_tag_callback, [:pointer], :void
    attach_function :nfc_reader_on_tag, [:pointer,:on_tag_callback], :pointer
    attach_function :nfc_reader_init, [:string], :pointer
    attach_function :nfc_reader_poll, [:pointer], :int
    attach_function :nfc_reader_stop_poll, [:pointer], :int
    attach_function :nfc_reader_destroy, [:pointer], :int
    attach_function :nfc_reader_read, [:pointer], :void
    attach_function :nfc_reader_do, [:pointer], :void
    attach_function :byebye, [:string], :void 
    attach_function :ustrlen, [:string], :void
    attach_function :nfc_reader_1_read, [:pointer], :pointer
    attach_function :nfc_set_cmd, [:pointer, :string], :int; 
  #}}}

  class Struct < FFI::Struct #{{{
    layout  :nfc_port, :string,
            :fh, :int,
            :polling, :pthread_t,
            :read_puffer, [:u_int8_t, 100],
            :write_puffer, [:u_int8_t,100],
            :reddit, :pointer,
            :reddit_len, :int,
            :tag_mem, [:pointer,36],
            :tag_mem_max_len, :int,
            :tag_mem_len, :int,
            :tag_mem_block_len, :int,
            :command, [:u_int8_t,100],
            :command_len, :int,
            :what_happens_len, :int,
            :what_happens_max_len, :int,
            :nfc_what_happens_if, :pointer
  end #}}}

  class Result #{{{ 
    def initialize(res)
      @res = res.unpack("C*") 
    end

    def hex
    #  @res.map{|e| e.to_s(16) }
      @res.map{|e| "%02x" % e }.join(' ')
    end
    def dec
      @res.join(' ')
    end
    def zero #{{{
      checker = true
      @res.each do |x|
        if x!=0
          checker = false
        end
      end
      checker
    end #}}}

    def high #{{{
      checker = true
      @res.each do |x|
        if x!=255
          checker = false
        end
      end
      checker
    end #}}}

    @res
  end #}}}

  class Reader
    attr_reader :reader
    attr_reader :serialnr 
    def initialize(tty = "/dev/ttyUSB0") #{{{
      @reader = NFC.nfc_reader_init(tty)
      @thread = nil
      @serialnr = nil
      command('x')

 #     begin
 #       @serialnr = command('b')
 #     end while @serialnr == nil
 #     @serialnr = @serialnr.unpack("C*").map{|e| "%02x" % e}.join
      Signal::trap("INT") do
        puts 'stopping...'
        stop_poll
        @thread.kill
      end
    end #}}} 

    def command( cmd) #{{{
      obj = Struct.new(@reader)
      temp = nil
      while temp.nil? do
        NFC.nfc_set_cmd(@reader,cmd)
        NFC.nfc_reader_do(@reader)
        if (obj[:reddit].read_string(obj[:reddit_len])[3..-3]== nil)
          temp = nil
        else
          temp=Result.new(obj[:reddit].read_string(obj[:reddit_len])[3..-3])
        end
      end
      temp
    end #}}}

    def command_print(cmd) #{{{
      obj = Struct.new(@reader)
      NFC.nfc_set_cmd(@reader,cmd)
      NFC.nfc_reader_do(@reader)
      if (obj[:reddit].read_string(obj[:reddit_len])[3..-3]== nil)
        temp = nil
      else
        temp=Result.new(obj[:reddit].read_string(obj[:reddit_len])[3..-3])
      end
      p temp
    end #}}} 
    
    def read_once #{{{
      NFC.nfc_reader_1_read(@reader)
    end #}}}
    def stop_poll #{{{
      NFC.nfc_reader_stop_poll(@reader)
    end #}}}
    def poll(&bl) #{{{
      pr = Proc.new do |obj|
        obj = Struct.new(@reader)
        bl.call(Result.new(obj[:reddit].read_string(obj[:reddit_len])[3..-3]))
      end
      NFC.nfc_reader_on_tag(@reader,pr)
      command('dp')
      NFC.nfc_reader_poll(@reader)
      @thread = Thread.new { Thread::stop }
      @thread.join
    end #}}}
  end
end  
