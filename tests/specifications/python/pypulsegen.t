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

				arguments => [
					     ],
				command => 'tests/python/pulsegen_3.py',
				command_tests => [
						  {
						   description => "Can we reset a pulsegen object from python ?",

						   read => "Output value before reset 50.0
Output value after reset 10.0
Done!
",

						  },
						 ],

				description => "Creates a pulsegen object, steps 12 steps then resets",

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


			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_freerun.py',
				command_tests => [
						  {
						   description => "Can a single pulsegen object output amplitude in free run mode ?",
						   read => (join '', `cat $::global_config->{core_directory}/tests/specifications/strings/pulsegen-freerun.txt`),
						   timeout => 40,
						  },
						 ],

				description => "pulsegen functionality, can we output a current in free run mode?",
			       },


			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_extgate.py',
				command_tests => [
						  {
						   description => " Can a single pulsegen object output amplitude in ext gate mode?",
						   read => (join '', `cat $::global_config->{core_directory}/tests/specifications/strings/pulsegen-extgate.txt`),
						   timeout => 40,
						  },
						 ],

				description => "pulsegen functionality, can we output a constant pulse with no trigger ?",
			       },
			       {
				arguments => [
					     ],
				command => 'tests/python/pulsegen_exttrig.py',
				command_tests => [
						  {
						   description => "Can a single pulsegen object output amplitude in ext gate mode ?",
						   read => (join '', `cat $::global_config->{core_directory}/tests/specifications/strings/pulsegen-exttrig.txt`),
						   timeout => 40,
						  },
						 ],

				description => "pulsegen functionality, can we output a constant pulse with no gate input ?",
			       },


			      ],
       description => "Tests for the PulseGen python bindings.",
       disabled => "the python modules have been disabled",
       name => 'python/pypulsegen.t',
      };


return $test;


