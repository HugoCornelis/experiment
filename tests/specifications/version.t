#!/usr/bin/perl -w
#

use strict;


my $test
    = {
       command_definitions => [
			       {
				arguments => [
					     ],
				command => 'tests/code/version',
				command_tests => [
						  {
						   # $Format: "description => \"Does the version information match with ${package}-${label} ?\","$
description => "Does the version information match with heccer-alpha ?",

						   # $Format: "read => \"${package}-${label}\","$
read => "heccer-alpha",

						   write => "version",
						  },
						 ],
				description => "check version information",
				disabled => "not implemented yet",
			       },
			      ],
       description => "run-time versioning",
       name => 'version.t',
      };


return $test;


