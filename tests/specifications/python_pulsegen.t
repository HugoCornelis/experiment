#!/usr/bin/perl -w
#

use strict;


my $test
    = {
       command_definitions => [
			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_import.py',
				command_tests => [
						  {
						   description => "Can we import the pulsegen python module ?",

						   read => "Done!",

						  },
						 ],
				description => "Just performs a basic import",

			       },


			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_1.py',
				command_tests => [
						  {
						   description => "Can we create a basic pulsegen data type ?",

						   read => "Name: Test name
Level1: 50.0
Width1: 3.0
Delay1: 5.0
Level2: -20.0
Width2: 5.0
Delay2: 8.0
BaseLevel: 10.0
TriggerMode: 0
Done!
",

						  },
						 ],
				description => "Creates a pulsegen base type",

			       },


			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_2.py',
				command_tests => [
						  {
						   description => "Can we create a Pulsegen class object ?",

						   read => "Name: Test name
Level1: 50.0
Width1: 3.0
Delay1: 5.0
Level2: -20.0
Width2: 5.0
Delay2: 8.0
BaseLevel: 10.0
TriggerMode: 0
Done!
",

						  },
						 ],
				description => "Creates a pulsegen object with basic test values",

			       },


			       {

				command => 'tests/python/pulsegen_5.py',
				command_tests => [
						  {
						   description => "Can set an output variable ?",

						   read => "Value in added variable is 10.000000
Done!
",

						  },
						 ],
				description => "Creates an output variable for the pulsegen output",

			       },

			      ],
       description => "Tests for the PulseGen python bindings.",
       name => 'python_pulsegen.t',
      };


return $test;


