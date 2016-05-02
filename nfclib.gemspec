Gem::Specification.new do |s|
  s.name = 'nfclib'
  s.version = '0.0.136'
  s.date = '2014-08-05'
  s.summary ="NFC-Reader Library."
  s.description ="Executes command and gets results in c, everthing else ruby-yo"
  s.authors = ["Florian Stertz"]
  s.email = ['florian.stertz@gmail.com']
  s.files = Dir['ext/extconf.rb'] + Dir['lib/nfc.rb'] + ['nfclib.gemspec','README.md'] + Dir['ext/nfclib.c'] + Dir['ext/nfclib.h'] + Dir['ext/rathalos.c']
  s.platform = Gem::Platform::RUBY
  s.require_paths = ['lib']
  s.extensions = Dir['ext/extconf.rb']
  s.homepage = 'https://github.com/Triarier/lagiacrus'
  s.add_runtime_dependency 'ffi', '>=1.9.10', '~>1.9'
end
