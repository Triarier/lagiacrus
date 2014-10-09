#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

reader = NFC::Reader.new
reader.poll do |result|
  data = { 'serialnr' => reader.serialnr, 'info' => result.hex }
  p result
  p result.hex
  client = Riddl::Client.new( 'http://leonardo.wst.univie.ac.at:9309/triggers')
  client.post Riddl::Parameter::Complex.new "list", "application/json", JSON.generate(data)
end  
