Gem::Specification.new do |s|
  s.name = 'nfclib'
  s.version = '0.0.125'
  s.date = '2014-08-05'
  s.summary ="NFC-Reader Library."
  s.description ="Executes command and gets results in c, everthing else ruby-yo"
  s.authors = ["Florian Stertz"]
  s.email = ['florian.stertz@gmail.com']
  s.files = Dir['lib/**/*'] + ['nfclib.gemspec','README.md'] + Dir['ext/**/*']
  s.platform = Gem::Platform::RUBY
  s.require_paths = ['lib']
  s.homepage = 'https://github.com/Triarier/lagiacrus'
end
