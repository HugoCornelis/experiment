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

			       {
				arguments => [
					     ],
				command => 'tests/python/perfectclamp_3.py',
				command_tests => [
						  {
						   description => "Will perfectclamp hold a voltage variable at -0.06 ?",

						   read => "0.0 0.111000
0.5 -0.060000
1.0 -0.060000
1.5 -0.060000
2.0 -0.060000
2.5 -0.060000
3.0 -0.060000
3.5 -0.060000
4.0 -0.060000
4.5 -0.060000
5.0 -0.060000
5.5 -0.060000
6.0 -0.060000
6.5 -0.060000
7.0 -0.060000
7.5 -0.060000
8.0 -0.060000
8.5 -0.060000
9.0 -0.060000
9.5 -0.060000
Done!
",

						  },
						 ],

				description => "Steps through 20 steps to see if perfectclamp can hold voltage",

			       },


			      ],
       description => "Tests for the PerfectClamp python bindings.",
       name => 'python_perfectclamp.t',
      };


return $test;


