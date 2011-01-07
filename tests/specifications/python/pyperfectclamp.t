#!/usr/bin/perl -w
#

use strict;


my $test
    = {
       command_definitions => [
			       {
				arguments => [
					     ],
				command => 'tests/python/perfectclamp_import.py',
				command_tests => [
						  {
						   description => "Can we import the perfectclamp python module ?",

						   read => "Done!",

						  },
						 ],
				description => "Just performs a basic import",

			       },


			       {
				arguments => [
					     ],
				command => 'tests/python/perfectclamp_1.py',
				command_tests => [
						  {
						   description => "Can we create a basic perfectclamp data type ?",

						   read => "Name: pc object
Clamps Active: 1
Command: 0.10
Filename: My output file
Done!
",

						  },
						 ],

				description => "Creates a perfectclamp base type",

			       },


			      ],
       description => "Tests for the PerfectClamp python bindings.",
       name => 'python_perfectclamp.t',
      };


return $test;


