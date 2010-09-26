#!/usr/bin/perl -w
#!/usr/bin/perl -w -d:ptkdb
#


use SwiggableExperiment;


package Experiment;


sub version
{
    # $Format: "    my $version=\"${package}-${label}\";"$
    my $version="experiment-alpha";

    return $version;
}


package Experiment::Glue;


sub backend
{
    my $self = shift;

    return $self->{backend};
}


package Experiment::Event::Output;


BEGIN { our @ISA = qw(Experiment::Glue); }


sub add
{
    my $self = shift;

    my $options = shift;

    my $backend = $self->backend();

    #t if I am correct we also have to set the iTable of the SpikeGen
    #t overhere, not sure.

    my $name
	= $options->{service_request}->{component_name}
	    . "__"
		. $options->{service_request}->{field};

    $name =~ s/\//____/g;
#     $name =~ s/\>/__/g;
#     $name =~ s/\-//g;

    my $result = $backend->OutputGeneratorAddVariable($name, $options->{address});

    return $result;
}


# sub advance
# {
#     my $self = shift;

#     # event output is not dependent on advancing time

#     return undef;
# }


sub connect
{
    my $self = shift;

    my $peer = shift;

    my $scheduler = $self->{scheduler};

    my $event_system = $scheduler->lookup_object('event_system');

    my $backend = $self->backend();

    return undef;
}


sub finish
{
    my $self = shift;

    # close files, free memory

    my $backend = $self->backend();

    $backend->OutputGeneratorFinish();
}


sub get_driver
{
    my $self = shift;

    my $result
	= {
# 	   data => $self->{backend}->output_generator_get_driver_data(),
# 	   method => $self->{backend}->output_generator_get_driver_method(),
# 	   data => $self->{backend},
# 	   method => \&SwiggableHeccer::OutputGeneratorAnnotatedStep,
	   data => -1,
	   method => -1,
	  };

    return $result;
}


sub get_time_step
{
    my $self = shift;

    # an event output object does not have a time step

    return undef;
}


sub initiate
{
    my $self = shift;

    # create files

    my $backend = $self->backend();

    $backend->OutputGeneratorInitiate();
}


sub new
{
    my $package = shift;

    my $options = shift;

    # construct object

    my $self = { %$options, };

    bless $self, $package;

    if (!defined $self->{filename})
    {
	$self->{filename} = "/tmp/EventOutputGenerator";
    }

    # construct backend

    $self->{backend} = SwiggableExperiment::OutputGeneratorNew($self->{filename});

    if (!defined $self->{backend})
    {
	return undef;
    }

    # set the base value to zero, such that only values different from zero are recorded

    $self->{backend}->swig_dBase_set(0);

#     # lookup the event_distributor service

#     my $event_source = $options->{event_source};

#     if ($event_source)
#     {
# 	my $scheduler = $self->{scheduler};

# 	my $ssp_distributor = $scheduler->lookup_object($event_source);

# 	if (!$ssp_distributor)
# 	{
# 	    die "$0: " . __PACKAGE__ . " cannot construct, $event_source not found as an ssp service";
# 	}

# 	my $distributor = $ssp_distributor->backend();

# 	if (!$distributor)
# 	{
# 	    die "$0: " . __PACKAGE__ . " cannot construct, $event_source has no low-level backend";
# 	}

# 	if (!$distributor->add_output($self))
# 	{
# 	    die "$0: " . __PACKAGE__ . " cannot construct";
# 	}
#     }

    # return result

    return $self;
}


sub pause
{
    my $self = shift;

    # flush files

    my $backend = $self->backend();

    $backend->OutputGeneratorFlush();
}


sub report
{
    my $self = shift;

    #t nothing I guess ?
}


sub step
{
    my $self = shift;

    my $scheduler = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $result;

    if ($self->{output_mode}) # eq 'steps')
    {
	$result = $backend->OutputGeneratorAnnotatedStep("$options->{steps}");
    }
    else
    {
	$result = $backend->OutputGeneratorTimedStep($options->{time});
    }

    return $result;
}


package Experiment::Output;


BEGIN { our @ISA = qw(Experiment::Glue); }


sub add
{
    my $self = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $name
	= $options->{service_request}->{component_name}
	    . "__"
		. $options->{service_request}->{field};

    $name =~ s/\//____/g;
#     $name =~ s/\>/__/g;
#     $name =~ s/\-//g;

    my $result = $backend->OutputGeneratorAddVariable($name, $options->{address});

    return $result;
}


# sub advance
# {
#     my $self = shift;

#     my $scheduler = shift;

#     my $time = shift;

#     # call the appropriate method

#     my $backend = $self->backend();

#     my $result = $backend->OutputGeneratorAnnotatedStep($time);

#     # return result

#     return $result;
# }


sub connect
{
    my $self = shift;

    my $backend = $self->backend();

    return undef;
}


sub finish
{
    my $self = shift;

    # close files, free memory

    my $backend = $self->backend();

    $backend->OutputGeneratorFinish();
}


sub get_driver
{
    my $self = shift;

    my $result
	= {
	   data => $self->{backend}->output_generator_get_driver_data(),
	   method => $self->{backend}->output_generator_get_driver_method(),
# 	   data => $self->{backend},
# 	   method => \&SwiggableExperiment::OutputGeneratorAnnotatedStep,
	  };

    return $result;
}


sub get_time_step
{
    my $self = shift;

    # an output object does not have a time step

    return undef;
}


sub initiate
{
    my $self = shift;

    # create files

    my $backend = $self->backend();

    $backend->OutputGeneratorInitiate();
}


sub new
{
    my $package = shift;

    my $options = shift;

    my $self = { %$options, };

    bless $self, $package;

    if (!defined $self->{filename})
    {
	$self->{filename} = "/tmp/OutputGenerator";
    }

    $self->{backend} = SwiggableExperiment::OutputGeneratorNew($self->{filename});

    if (!defined $self->{backend})
    {
	return undef;
    }

    if ($self->{resolution})
    {
	if ($self->{output_mode} ne 'steps')
	{
	    die "$0: output generator options contain a resolution specification, but is not running in 'steps' output_mode.";
	}

	$self->{backend}->swig_iResolutionStep_set(0);

	$self->{backend}->swig_iResolution_set($self->{resolution});
    }

    if ($self->{format})
    {
	$self->set_format($self->{format});
    }

    return $self;
}


sub pause
{
    my $self = shift;

    # flush files

    my $backend = $self->backend();

    $backend->OutputGeneratorFlush();
}


sub report
{
    my $self = shift;

    #t nothing I guess ?
}


sub set_format
{
    my $self = shift;

    my $format = shift;

    my $backend = $self->backend();

    my $result;

    $backend->OutputGeneratorSetFormat($format);

    return $result;
}


sub step
{
    my $self = shift;

    my $scheduler = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $result;

    if ($self->{output_mode}) # eq 'steps')
    {
	$result = $backend->OutputGeneratorAnnotatedStep("$options->{steps}");
    }
    else
    {
	$result = $backend->OutputGeneratorTimedStep($options->{time});
    }

    return $result;
}


package Experiment::PerfectClamp;


BEGIN { our @ISA = qw(Experiment::Glue); }


sub add
{
    my $self = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $name
	= $options->{service_request}->{component_name}
	    . "__"
		. $options->{service_request}->{field};

    $name =~ s/\//____/g;
#     $name =~ s/\>/__/g;
#     $name =~ s/\-//g;

    my $result = $backend->PerfectClampAddVariable($options->{address});

    return $result;
}


# sub advance
# {
#     my $self = shift;

#     #t call the appropriate method or something

#     return undef;
# }


sub connect
{
    my $self = shift;

    my $backend = $self->backend();

    return undef;
}


sub finish
{
    my $self = shift;

    # close files, free memory

    my $backend = $self->backend();

    $backend->PerfectClampFinish();
}


sub get_driver
{
    my $self = shift;

    my $result
	= {
	   data => $self->{backend}->perfect_clamp_get_driver_data(),
	   method => $self->{backend}->perfect_clamp_get_driver_method(),
# 	   data => $self->{backend},
# 	   method => \&SwiggableExperiment::PerfectClampSingleStep,
	  };

    return $result;
}


sub get_time_step
{
    my $self = shift;

    # a perfect clamp object does not have a time step

    return undef;
}


sub initiate
{
    my $self = shift;

    #t perhaps need to set the command voltage here ?
}


sub new
{
    my $package = shift;

    my $options = shift;

    my $self = { %$options, };

    bless $self, $package;

    if (!defined $self->{name})
    {
	$self->{name} = "a pc";
    }

    $self->{backend} = SwiggableExperiment::PerfectClampNew($self->{name});

    if (!defined $self->{backend})
    {
	return undef;
    }

    # make distinction between command_filename and command voltage option

    if (defined $options->{command})
    {
	my $backend = $self->backend();

	$backend->PerfectClampSetFields($options->{command}, undef);
    }
    elsif (defined $options->{filename})
    {
	my $backend = $self->backend();

	#! the command voltage is ignored in this case, use an
	#! unreasonable value to make result invalid if it would be used
	#! (due to a bug).

	$backend->PerfectClampSetFields(-10000, $options->{filename});
    }
    else
    {
	return "Experiment::PerfectClamp constructor: cannot construct a perfect clamp without command voltage and without a filename";
    }

    return $self;
}


sub report
{
    my $self = shift;

    #t nothing I guess ?
}


sub step
{
    my $self = shift;

    my $scheduler = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $result = $backend->PerfectClampSingleStep($options->{steps});

    return $result;
}


package Experiment::PulseGen;


BEGIN { our @ISA = qw(Experiment::Glue); }


sub add
{
    my $self = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $name
	= $options->{service_request}->{component_name}
	    . "__"
		. $options->{service_request}->{field};

    $name =~ s/\//____/g;

    my $result = $backend->PulseGenAddVariable($options->{address});

    return $result;
}


sub connect
{
    my $self = shift;

    return '';
}


sub finish
{
    my $self = shift;

    # close files, free memory

    my $backend = $self->backend();

    $backend->PulseGenFinish();
}


sub get_driver
{
    my $self = shift;

    my $result
	= {
	   data => $self->{backend}->pulse_gen_get_driver_data(),
	   method => $self->{backend}->pulse_gen_get_driver_method(),
# 	   data => $self->{backend},
# 	   method => \&SwiggableExperiment::PerfectClampSingleStep,
	  };

    return $result;
}


sub get_time_step
{
    my $self = shift;


    return undef;
}


sub initiate
{
    my $self = shift;


}


sub new
{
    my $package = shift;

    my $options = shift;

    my $self = { %$options, };

    bless $self, $package;

    if (!defined $self->{name})
    {
	$self->{name} = "a pulsegen";
    }

    $self->{backend} = SwiggableExperiment::PulseGenNew($self->{name});

    if (!defined $self->{backend})
    {
	return undef;
    }

    #
    # Here we check for all of our needed variables, we die if even 
    # one is missing since we need it to continue creating the object.
    #
    if (!defined $options->{width1})
    {	
	return "Experiment::PulseGen constructor: width1 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{level1})
    {
	return "Experiment::PulseGen constructor: level1 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{delay1})
    {
	return "Experiment::PulseGen constructor: delay1 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{level2})
    {
	return "Experiment::PulseGen constructor: level2 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{width2})
    {
	return "Experiment::PulseGen constructor: width2 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{delay2})
    {
	return "Experiment::PulseGen constructor: delay2 is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{baselevel})
    {
	return "Experiment::PulseGen constructor: baselevel is not defined, cannot construct pulsegen object";
    }
    elsif(!defined $options->{triggermode})
    {
	return "Experiment::PulseGen constructor: triggermode is not defined, cannot construct pulsegen object";
    }
    else
    {
	my $backend = $self->backend();

	$backend->PulseGenSetFields($options->{level1},
					$options->{width1},
					$options->{delay1},
					$options->{level2},
					$options->{width2},
					$options->{delay2},
					$options->{baselevel},
					$options->{triggermode});
    }

    return $self;
}


sub report
{
    my $self = shift;

    #t nothing I guess ?
}


sub step
{
    my $self = shift;

    my $scheduler = shift;

    my $options = shift;

    my $backend = $self->backend();

    my $result = $backend->PulseGenSingleStep($options->{steps});

    return $result;
}


1;


