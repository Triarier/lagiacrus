#!/usr/bin/ruby
require File.expand_path(File.dirname(__FILE__) + '/../lib/nfc')
require 'riddl/client'
require 'json'


reader = NFC::Reader.new
#reader.command 'x'
#reader.command 'b'
reader.poll do |result|
  data = { 'serialnr' => reader.serialnr, 'info' => 'bla' }
  p result
  p result.hex
  client = Riddl::Client.new( 'http://leonardo.wst.univie.ac.at:9309/triggers')
  client.post Riddl::Parameter::Complex.new "nfc_tag", "application/json", JSON.generate(data)
end  
