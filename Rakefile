require 'rubygems'                                                                                       
require 'rake/clean'
require 'rake/testtask'
require 'rubygems/package_task'
require 'rdoc/rdoc'

spec = eval(File.read('nfclib.gemspec'))

task :default => [:compile]

#DESC "CONFIGURING LIBRARY"
#TASK :CONFIG DO
#  DIR.CHDIR('EXT')  
#  RUBY 'EXTCONF.RB'
#  DIR.CHDIR(FILE.DIRNAME(__FILE__))  
#END

desc "Compiling library"
task :compile  do
  Dir.chdir('ext')
  system "touch *.c"
  system "make"
  Dir.chdir(File.dirname(__FILE__))  
end

Gem::PackageTask.new(spec) do |pkg|
  pkg.need_zip = true
  pkg.need_tar = true
end

desc "Installing library"
task :install => [:compile,:gem] do
  system "sudo gem install pkg/nfclib-#{spec.version.to_s}.gem"
end

desc "Pushing library"
task :push => [:compile,:gem] do
  system "gem push pkg/nfclib-#{spec.version.to_s}.gem"
end

#desc "Generate Documentation"
#RDoc::Task.new do |rdoc|
#  rdoc.main ="README.rdoc"
#  rdoc.rdoc_files.include("ext/*.c","ext/*.h","lib/**/*")
#end
#
#desc "Generate RI"
#Rake::RDocTask.new(:ri) do |rd|
#  rd.rdoc_dir = "doc/ri"
#  rd.options << "--ri-system"
#  rd.rdoc_files.include("ext/*.c","ext/*.h","lib/**/*")
#end
#
#desc "Testing"
#Rake::TestTask.new do |t|
#  t.test_files = FileList['test/*_test.rb']
#  t.verbose = true
#end
