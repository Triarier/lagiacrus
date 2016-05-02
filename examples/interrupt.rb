#!/usr/bin/ruby
#require '../lib/nfc'
require 'nfc'
require 'riddl/client'
require 'json'
require 'pp'

def container(pos_arr,reader,data) #{{{

  res = reader.command("r %02X" % pos_arr[0])
  begin
    if(res.high)
      pos_arr[0]+= 2
      container(pos_arr,reader,data)
      pos_arr[0]+= 2
      res = reader.command("r %02X" % pos_arr[0])
    end
    data["data#{pos_arr[1]}"] = res.dec
    pos_arr[0] += 1
    res = reader.command("r %02X" % pos_arr[0])
    data["dosis#{pos_arr[1]}"]=res.dec
    pos_arr[0]+=1
    pos_arr[1]+=1
    res = reader.command("r %02X" % pos_arr[0])
  end while !(res.zero)

end
 #}}}

def read_tag_mem(reader) #{{{

s_nr = reader.command('b')
data = {:reader_id => s_nr.hex}
res = reader.command('s')
data['tag_id'] = res.dec
pos_arr = Array.new
# Erste Stelle sagt von welcher Addresse lesen beginnt. 2. Stelle dient dem Data-Hash
pos_arr << 8
pos_arr << 0
res = reader.command("r %02X" % pos_arr[0])
if(res.high)
  pos_arr[0]+=2
  container(pos_arr,reader,data)
else
  data["data#{pos_arr[1]}"] = res.dec
  pos_arr[0]+=1
  res = reader.command("r %02X" % pos_arr[0])
  data["dosis#{pos_arr[1]}"]=res.dec
end

data

end #}}} 

reader = NFC::Reader.new
p reader.command('s')
sleep(2)
x = JSON.parse(JSON.generate(read_tag_mem(reader)))
pp x
i=0
i.upto((x.size-3)/2) do
 pp x["data#{i}"].join
 pp x["dosis#{i}"].join
 i=i+1
end

client = Riddl::Client.new( 'http://leonardo.wst.univie.ac.at:9309/triggers')
client.post Riddl::Parameter::Complex.new "list", "application/json", JSON.generate(x)
