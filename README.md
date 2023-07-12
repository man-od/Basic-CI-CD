# Basic-CI-CD
Development of a simple CI/CD for the SimpleBashUtils project. Building, style, unit-testing, deployment, notify

## Deployment stage
- To accomplish this task, I moved the executable files to another virtual machine, which will play the role of production.

Run this stage manually, if all the previous stages have passed successfully
Writen a bash script which copies the files received after the building (artifacts) into the /usr/local/bin directory of the second virtual machine using ssh and scp

In case of an error, fail the pipeline
