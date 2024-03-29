# Copyright (c) 1997-2009 Graham Barr <gbarr@pobox.com>. All rights reserved.
# This program is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
# Maintained since 2013 by Paul Evans <leonerd@leonerd.org.uk>

package List::Util;

use strict;
require Exporter;

our @ISA        = qw(Exporter);
our @EXPORT_OK  = qw(
  all any first min max minstr maxstr none notall product reduce sum sum0 shuffle
  pairmap pairgrep pairfirst pairs pairkeys pairvalues
);
our $VERSION    = "1.41";
our $XS_VERSION = $VERSION;
$VERSION    = eval $VERSION;

require XSLoader;
XSLoader::load('List::Util', $XS_VERSION);

sub import
{
  my $pkg = caller;

  # (RT88848) Touch the caller's $a and $b, to avoid the warning of
  #   Name "main::a" used only once: possible typo" warning
  no strict 'refs';
  ${"${pkg}::a"} = ${"${pkg}::a"};
  ${"${pkg}::b"} = ${"${pkg}::b"};

  goto &Exporter::import;
}

# For objects returned by pairs()
sub List::Util::_Pair::key   { shift->[0] }
sub List::Util::_Pair::value { shift->[1] }

1;

__END__

