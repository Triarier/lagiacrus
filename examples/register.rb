#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

reader = NFC::Reader.new
reader.poll do |result|
  p result
  client = Riddl::Client.new( 'http://leonardo.wst.univie.ac.at:9309/register')
  client.post([Riddl::Parameter::Simple.new( "id" , '4'), Riddl::Parameter::Simple.new( "name" , 'aspirin'),Riddl::Parameter::Simple.new( 'tag', result.hex)])
end  
