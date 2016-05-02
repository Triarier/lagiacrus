#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

reader = NFC::Reader.new
reader.command_print('s')
reader.command_print('wb 08ffffffff')
reader.command_print('wb 09ffffffff')
reader.command_print('wb 0a00000001')
reader.command_print('wb 0b00000001')
reader.command_print('wb 0c10000001')
reader.command_print('wb 0d00002001')
reader.command_print('wb 0effffffff')
reader.command_print('wb 0fffffffff')
reader.command_print('wb 1010000002')
reader.command_print('wb 1110000003')
reader.command_print('wb 1200000000')
reader.command_print('wb 1300000000')
reader.command_print('wb 1400000002')
reader.command_print('wb 1500000002')
reader.command_print('wb 1600000000')
reader.command_print('wb 1700000000')
