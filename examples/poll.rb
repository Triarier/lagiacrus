#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

reader = NFC::Reader.new
reader.command_print('s')
reader.command_print('wb 07afff0f0a')
