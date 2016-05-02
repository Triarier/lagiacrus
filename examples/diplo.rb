#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
reader = NFC::Reader.new
reader.poll do |result|
  data = { 'serialnr' => reader.serialnr, 'info' => result.hex }
  p result
  p result.hex
end  
