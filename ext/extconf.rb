require 'mkmf'

spec = eval(File.read(File.dirname(__FILE__) + '/../nfclib.gemspec'))
PKG_NAME="nfclib"

$CFLAGS = ' -Wall -pedantic -O3 -c -fPIC ' + $CFLAGS
$LDFLAGS = ' -pthread ' + $LDFLAGS
$LIBPATH.push(RbConfig::CONFIG['libdir'])


create_header()
create_makefile(PKG_NAME)
