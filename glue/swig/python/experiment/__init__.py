#!/usr/bin/python

import sys
sys.path.append('/usr/local/glue/swig/python')
import SwiggableExperiment



class Output:
    def __init__(self, filename):
        self.backend = SwiggableExperiment.OutputGeneratorNew(filename)

    def advance(self, time):
        return SwiggableExperiment.OutputGeneratorTimedStep(self.backend, time)

    def compile(self):
        return SwiggableExperiment.OutputGeneratorInitiate(self.backend)

    def AddOutput(self, name, address):
        SwiggableExperiment.OutputGeneratorAddVariable(self.backend, name, address)

    def finish(self):
        SwiggableExperiment.OutputGeneratorFinish(self.backend)
