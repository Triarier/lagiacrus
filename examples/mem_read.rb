#!/usr/bin/ruby
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

reader = NFC::Reader.new
reader.command('s') 
reader.command_print('r 08')
reader.command_print('r 09') 
reader.command_print('r 0a') 
reader.command_print('r 0b') 
reader.command_print('r 0c') 
reader.command_print('r 0d') 
reader.command_print('r 0e') 
reader.command_print('r 0f') 
reader.command_print('r 10') 
reader.command_print('r 11') 
reader.command_print('r 12') 
reader.command_print('r 13') 
reader.command_print('r 14') 
reader.command_print('r 15') 
reader.command_print('r 16') 
reader.command_print('r 17') 
